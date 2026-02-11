// Apply function names, parameter names, and types to GC retail binary
// @category CrashWOC
// @description Applies names from DWARF debug dump matching to GC retail DOL
// @author AI-assisted decompilation pipeline

import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.pcode.*;
import java.io.*;
import java.util.*;

public class ApplyGCNames extends GhidraScript {

    @Override
    public void run() throws Exception {
        // Locate map files
        String baseDir = System.getenv("GHIDRA_OUTPUT_DIR");
        String mapsDir;
        if (baseDir != null) {
            mapsDir = baseDir.replace("raw", "maps");
        } else {
            mapsDir = askString("Maps Directory",
                "Enter full path to maps directory (containing gc_name_map.json):");
        }

        // Load name map (prefer full map, fall back to base map)
        File nameMapFile = new File(mapsDir + File.separator + "gc_name_map_full.json");
        if (!nameMapFile.exists()) {
            nameMapFile = new File(mapsDir + File.separator + "gc_name_map.json");
        }
        if (!nameMapFile.exists()) {
            println("ERROR: Neither gc_name_map_full.json nor gc_name_map.json found at: " + mapsDir);
            return;
        }
        println("Using name map: " + nameMapFile.getName());
        String nameContent = new String(java.nio.file.Files.readAllBytes(nameMapFile.toPath()));
        Map<String, String> nameMap = parseSimpleJson(nameContent);
        println("Loaded " + nameMap.size() + " name mappings");

        // Load params map
        File paramsMapFile = new File(mapsDir + File.separator + "gc_params_map.json");
        Map<String, Map<String, Object>> paramsMap = new HashMap<>();
        if (paramsMapFile.exists()) {
            String paramsContent = new String(java.nio.file.Files.readAllBytes(paramsMapFile.toPath()));
            paramsMap = parseParamsJson(paramsContent);
            println("Loaded " + paramsMap.size() + " parameter mappings");
        } else {
            println("WARNING: gc_params_map.json not found, skipping parameter renaming");
        }

        AddressFactory af = currentProgram.getAddressFactory();
        FunctionManager fm = currentProgram.getFunctionManager();

        // Phase 1: Rename functions
        int renamed = 0;
        int skipped = 0;
        int notFound = 0;
        int errors = 0;

        for (Map.Entry<String, String> entry : nameMap.entrySet()) {
            String addrStr = entry.getKey().replace("0x", "");
            String newName = entry.getValue();

            try {
                Address addr = af.getDefaultAddressSpace().getAddress(addrStr);
                Function func = fm.getFunctionAt(addr);

                if (func == null) {
                    notFound++;
                    continue;
                }

                String currentName = func.getName();
                if (!currentName.startsWith("FUN_") && !currentName.startsWith("thunk_FUN_")) {
                    skipped++;
                    continue;
                }

                func.setName(newName, SourceType.IMPORTED);
                renamed++;
            } catch (Exception e) {
                errors++;
                println("Error renaming " + addrStr + " -> " + newName + ": " + e.getMessage());
            }
        }

        println("=== Phase 1: Function Renaming ===");
        println("Renamed:   " + renamed);
        println("Skipped:   " + skipped + " (already named)");
        println("Not found: " + notFound);
        println("Errors:    " + errors);

        // Phase 2: Apply parameter names using Decompiler
        if (!paramsMap.isEmpty()) {
            println("\n=== Phase 2: Parameter Renaming ===");
            int paramsApplied = 0;
            int paramsSkipped = 0;

            DecompInterface decomp = new DecompInterface();
            decomp.openProgram(currentProgram);

            for (Map.Entry<String, String> entry : nameMap.entrySet()) {
                String addrStr = entry.getKey().replace("0x", "");
                String funcName = entry.getValue();

                Map<String, Object> paramInfo = paramsMap.get(funcName);
                if (paramInfo == null) continue;

                List<Map<String, String>> params = (List<Map<String, String>>) paramInfo.get("params");
                if (params == null || params.isEmpty()) continue;

                try {
                    Address addr = af.getDefaultAddressSpace().getAddress(addrStr);
                    Function func = fm.getFunctionAt(addr);
                    if (func == null) continue;

                    // Rename function parameters
                    Parameter[] funcParams = func.getParameters();
                    int limit = Math.min(funcParams.length, params.size());

                    for (int i = 0; i < limit; i++) {
                        String paramName = params.get(i).get("name");
                        if (paramName != null && !paramName.isEmpty()) {
                            try {
                                funcParams[i].setName(paramName, SourceType.IMPORTED);
                                paramsApplied++;
                            } catch (Exception e) {
                                // Name conflict, try with suffix
                                try {
                                    funcParams[i].setName(paramName + "_" + i, SourceType.IMPORTED);
                                    paramsApplied++;
                                } catch (Exception e2) {
                                    // Skip this parameter
                                }
                            }
                        }
                    }

                    // Add comment with full DWARF signature for reference
                    StringBuilder sig = new StringBuilder();
                    sig.append("DWARF: ");
                    sig.append(funcName).append("(");
                    for (int i = 0; i < params.size(); i++) {
                        if (i > 0) sig.append(", ");
                        sig.append(params.get(i).get("type"));
                        sig.append(" ");
                        sig.append(params.get(i).get("name"));
                    }
                    sig.append(")");

                    // Add local variables as comment too
                    List<Map<String, String>> locals = (List<Map<String, String>>) paramInfo.get("locals");
                    if (locals != null && !locals.isEmpty()) {
                        sig.append("\nLocals: ");
                        int localLimit = Math.min(locals.size(), 20);
                        for (int i = 0; i < localLimit; i++) {
                            if (i > 0) sig.append(", ");
                            sig.append(locals.get(i).get("type"));
                            sig.append(" ");
                            sig.append(locals.get(i).get("name"));
                        }
                        if (locals.size() > 20) {
                            sig.append(" ... (+" + (locals.size() - 20) + " more)");
                        }
                    }

                    func.setComment(sig.toString());
                    paramsSkipped++;

                } catch (Exception e) {
                    println("Error applying params for " + funcName + ": " + e.getMessage());
                }
            }

            decomp.dispose();
            println("Parameters renamed: " + paramsApplied);
            println("Functions with comments: " + paramsSkipped);
        }

        println("\n=== Done ===");
        println("Total functions in binary: " + fm.getFunctionCount());
        println("Total named (non-FUN_): " + renamed + " + " + skipped);
    }

    private Map<String, String> parseSimpleJson(String json) {
        Map<String, String> map = new HashMap<>();
        json = json.trim();
        if (json.startsWith("{")) json = json.substring(1);
        if (json.endsWith("}")) json = json.substring(0, json.length() - 1);

        // Handle JSON with newlines
        String[] lines = json.split("\n");
        for (String line : lines) {
            line = line.trim();
            if (line.isEmpty() || line.equals("{") || line.equals("}")) continue;
            if (line.endsWith(",")) line = line.substring(0, line.length() - 1);

            int colonIdx = line.indexOf(":");
            if (colonIdx < 0) continue;

            String key = line.substring(0, colonIdx).trim().replace("\"", "");
            String val = line.substring(colonIdx + 1).trim().replace("\"", "");

            if (!key.isEmpty() && !val.isEmpty()) {
                map.put(key, val);
            }
        }
        return map;
    }

    @SuppressWarnings("unchecked")
    private Map<String, Map<String, Object>> parseParamsJson(String json) {
        // Minimal JSON parser for the params map structure
        // Format: {"FuncName": {"params": [{"name": "x", "type": "int"}], "locals": [...]}}
        Map<String, Map<String, Object>> result = new HashMap<>();

        // Find each top-level key (function name)
        int pos = json.indexOf("\"", 1); // skip opening {
        while (pos >= 0 && pos < json.length()) {
            // Find function name
            int nameEnd = json.indexOf("\"", pos + 1);
            if (nameEnd < 0) break;
            String funcName = json.substring(pos + 1, nameEnd);

            // Find the value object
            int objStart = json.indexOf("{", nameEnd);
            if (objStart < 0) break;

            // Find matching closing brace (track nesting)
            int depth = 1;
            int objEnd = objStart + 1;
            while (objEnd < json.length() && depth > 0) {
                char c = json.charAt(objEnd);
                if (c == '{' || c == '[') depth++;
                else if (c == '}' || c == ']') depth--;
                objEnd++;
            }

            String objStr = json.substring(objStart, objEnd);

            // Parse params array
            Map<String, Object> funcData = new HashMap<>();
            List<Map<String, String>> params = parseArrayOfObjects(objStr, "params");
            List<Map<String, String>> locals = parseArrayOfObjects(objStr, "locals");
            funcData.put("params", params);
            funcData.put("locals", locals);
            result.put(funcName, funcData);

            // Move to next top-level entry
            pos = json.indexOf("\"", objEnd);
            // Skip past potential comma
            if (pos >= 0) {
                // Make sure we're at a top-level key
                String between = json.substring(objEnd, pos);
                if (between.contains("}") && !between.contains("{")) {
                    break; // end of top-level object
                }
            }
        }
        return result;
    }

    private List<Map<String, String>> parseArrayOfObjects(String json, String key) {
        List<Map<String, String>> result = new ArrayList<>();
        int keyIdx = json.indexOf("\"" + key + "\"");
        if (keyIdx < 0) return result;

        int arrStart = json.indexOf("[", keyIdx);
        if (arrStart < 0) return result;

        int arrEnd = json.indexOf("]", arrStart);
        if (arrEnd < 0) return result;

        String arrStr = json.substring(arrStart + 1, arrEnd);

        // Parse each object in the array
        int objStart = arrStr.indexOf("{");
        while (objStart >= 0) {
            int objEnd = arrStr.indexOf("}", objStart);
            if (objEnd < 0) break;

            String obj = arrStr.substring(objStart + 1, objEnd);
            Map<String, String> item = new HashMap<>();

            // Parse key-value pairs
            String[] parts = obj.split(",");
            for (String part : parts) {
                int colon = part.indexOf(":");
                if (colon < 0) continue;
                String k = part.substring(0, colon).trim().replace("\"", "");
                String v = part.substring(colon + 1).trim().replace("\"", "");
                item.put(k, v);
            }
            if (!item.isEmpty()) result.add(item);

            objStart = arrStr.indexOf("{", objEnd);
        }
        return result;
    }
}
