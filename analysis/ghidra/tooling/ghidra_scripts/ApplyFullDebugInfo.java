// Apply full debug info: types, function signatures, and local variable comments
// @category CrashWOC
// @description Imports Nu2 types, applies function signatures and local variable info from debug maps
// @author AI-assisted decompilation pipeline

import ghidra.app.script.GhidraScript;
import ghidra.app.util.cparser.C.CParser;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.Function.FunctionUpdateType;
import ghidra.program.model.lang.Register;
import java.io.*;
import java.util.*;

public class ApplyFullDebugInfo extends GhidraScript {

    private DataTypeManager dtm;
    private Map<String, DataType> typeCache = new HashMap<>();

    @Override
    public void run() throws Exception {
        // Locate directories from env var
        String baseDir = System.getenv("GHIDRA_OUTPUT_DIR");
        String mapsDir;
        String toolingDir;
        if (baseDir != null) {
            mapsDir = baseDir.replace("raw", "maps");
            toolingDir = baseDir.replace("raw", "tooling");
        } else {
            mapsDir = askString("Maps Directory",
                "Enter full path to maps directory:");
            toolingDir = askString("Tooling Directory",
                "Enter full path to tooling directory:");
        }

        dtm = currentProgram.getDataTypeManager();
        AddressFactory af = currentProgram.getAddressFactory();
        FunctionManager fm = currentProgram.getFunctionManager();

        // ============================================================
        // Phase 1: Import types from nu2_types.h
        // ============================================================
        println("\n=== Phase 1: Import Types from nu2_types.h ===");

        File typesFile = new File(toolingDir + File.separator + "nu2_types.h");
        int typesImported = 0;
        if (typesFile.exists()) {
            println("Parsing: " + typesFile.getAbsolutePath());
            typesImported = importTypesWithForwardDecls(typesFile);
        } else {
            println("WARNING: nu2_types.h not found at: " + typesFile.getAbsolutePath());
        }

        // ============================================================
        // Phase 2: Apply function signatures from gc_signatures_map.json
        // ============================================================
        println("\n=== Phase 2: Apply Function Signatures ===");

        // Load name map to get name -> address mapping
        File nameMapFile = new File(mapsDir + File.separator + "gc_name_map_full.json");
        if (!nameMapFile.exists()) {
            nameMapFile = new File(mapsDir + File.separator + "gc_name_map.json");
        }
        if (!nameMapFile.exists()) {
            println("ERROR: Name map not found in: " + mapsDir);
            return;
        }
        String nameContent = new String(java.nio.file.Files.readAllBytes(nameMapFile.toPath()));
        Map<String, String> addrToName = parseSimpleJson(nameContent);
        println("Loaded " + addrToName.size() + " address->name mappings");

        // Build reverse map: name -> address
        Map<String, String> nameToAddr = new HashMap<>();
        for (Map.Entry<String, String> entry : addrToName.entrySet()) {
            nameToAddr.put(entry.getValue(), entry.getKey());
        }

        // Load signatures map
        File sigMapFile = new File(mapsDir + File.separator + "gc_signatures_map.json");
        if (!sigMapFile.exists()) {
            println("ERROR: gc_signatures_map.json not found at: " + mapsDir);
            return;
        }
        String sigContent = new String(java.nio.file.Files.readAllBytes(sigMapFile.toPath()));
        Map<String, Map<String, Object>> sigMap = parseNestedJson(sigContent);
        println("Loaded " + sigMap.size() + " function signatures");

        int sigApplied = 0;
        int retTypesApplied = 0;
        int paramNamesApplied = 0;
        int paramTypesApplied = 0;
        int sigErrors = 0;
        int sigNotFound = 0;

        for (Map.Entry<String, Map<String, Object>> entry : sigMap.entrySet()) {
            String funcName = entry.getKey();
            Map<String, Object> sigInfo = entry.getValue();

            if (monitor.isCancelled()) break;

            String addrStr = nameToAddr.get(funcName);
            if (addrStr == null) {
                sigNotFound++;
                continue;
            }

            try {
                Address addr = af.getDefaultAddressSpace().getAddress(addrStr.replace("0x", ""));
                Function func = fm.getFunctionAt(addr);
                if (func == null) {
                    sigNotFound++;
                    continue;
                }

                boolean anyChange = false;

                // Apply return type
                String retTypeStr = (String) sigInfo.get("return_type");
                if (retTypeStr != null && !retTypeStr.isEmpty()) {
                    DataType retType = resolveType(retTypeStr);
                    if (retType != null) {
                        try {
                            func.setReturnType(retType, SourceType.IMPORTED);
                            retTypesApplied++;
                            anyChange = true;
                        } catch (Exception e) {
                            // ignore return type errors
                        }
                    }
                }

                // Apply parameter names and types
                @SuppressWarnings("unchecked")
                List<Map<String, String>> params = (List<Map<String, String>>) sigInfo.get("params");
                if (params != null && !params.isEmpty()) {
                    Parameter[] funcParams = func.getParameters();

                    if (funcParams.length == 0 && params.size() > 0) {
                        // No existing params - build parameter list and update function
                        try {
                            List<Variable> newParams = new ArrayList<>();
                            for (int i = 0; i < params.size(); i++) {
                                Map<String, String> paramInfo = params.get(i);
                                String paramName = paramInfo.get("name");
                                String paramTypeStr = paramInfo.get("type");

                                if (paramName == null || paramName.isEmpty()) {
                                    paramName = "param_" + i;
                                }

                                DataType paramType = null;
                                if (paramTypeStr != null && !paramTypeStr.isEmpty()) {
                                    paramType = resolveType(paramTypeStr);
                                }
                                if (paramType == null) {
                                    paramType = Undefined4DataType.dataType;
                                }

                                ParameterImpl p = new ParameterImpl(
                                    paramName, paramType, currentProgram);
                                newParams.add(p);
                                paramNamesApplied++;
                                if (paramType != Undefined4DataType.dataType) {
                                    paramTypesApplied++;
                                }
                            }

                            DataType retType = func.getReturnType();
                            func.updateFunction(
                                func.getCallingConventionName(),
                                new ReturnParameterImpl(retType, currentProgram),
                                newParams,
                                FunctionUpdateType.DYNAMIC_STORAGE_ALL_PARAMS,
                                true,
                                SourceType.IMPORTED
                            );
                        } catch (Exception e) {
                            // Fall through to per-param approach below
                        }
                    } else {
                        // Existing params - rename/retype individually
                        int limit = Math.min(funcParams.length, params.size());
                        for (int i = 0; i < limit; i++) {
                            Map<String, String> paramInfo = params.get(i);
                            String paramName = paramInfo.get("name");
                            String paramTypeStr = paramInfo.get("type");

                            // Apply param name
                            if (paramName != null && !paramName.isEmpty()) {
                                try {
                                    funcParams[i].setName(paramName, SourceType.IMPORTED);
                                    paramNamesApplied++;
                                } catch (Exception e) {
                                    try {
                                        funcParams[i].setName(paramName + "_" + i, SourceType.IMPORTED);
                                        paramNamesApplied++;
                                    } catch (Exception e2) {
                                        // skip
                                    }
                                }
                            }

                            // Apply param type
                            if (paramTypeStr != null && !paramTypeStr.isEmpty()) {
                                DataType paramType = resolveType(paramTypeStr);
                                if (paramType != null) {
                                    try {
                                        funcParams[i].setDataType(paramType, SourceType.IMPORTED);
                                        paramTypesApplied++;
                                    } catch (Exception e) {
                                        // ignore type errors
                                    }
                                }
                            }
                        }
                    }
                    anyChange = true;
                }

                if (anyChange) {
                    sigApplied++;
                }
            } catch (Exception e) {
                sigErrors++;
            }
        }

        println("Signatures processed: " + sigApplied);
        println("Return types applied: " + retTypesApplied);
        println("Param names applied:  " + paramNamesApplied);
        println("Param types applied:  " + paramTypesApplied);
        println("Not found in binary:  " + sigNotFound);
        println("Errors:               " + sigErrors);

        // ============================================================
        // Phase 3: Apply local variable info as plate comments
        // ============================================================
        println("\n=== Phase 3: Apply Local Variable Comments ===");

        File localsMapFile = new File(mapsDir + File.separator + "gc_locals_map.json");
        int localsApplied = 0;
        int localsNotFound = 0;
        int localsErrors = 0;

        if (localsMapFile.exists()) {
            String localsContent = new String(java.nio.file.Files.readAllBytes(localsMapFile.toPath()));
            Map<String, Map<String, Object>> localsMap = parseNestedJson(localsContent);
            println("Loaded " + localsMap.size() + " functions with local variable info");

            for (Map.Entry<String, Map<String, Object>> entry : localsMap.entrySet()) {
                String funcName = entry.getKey();
                Map<String, Object> localInfo = entry.getValue();

                if (monitor.isCancelled()) break;

                String addrStr = nameToAddr.get(funcName);
                if (addrStr == null) {
                    localsNotFound++;
                    continue;
                }

                try {
                    Address addr = af.getDefaultAddressSpace().getAddress(addrStr.replace("0x", ""));
                    Function func = fm.getFunctionAt(addr);
                    if (func == null) {
                        localsNotFound++;
                        continue;
                    }

                    StringBuilder comment = new StringBuilder();

                    // Source file
                    String sourceFile = (String) localInfo.get("source_file");
                    if (sourceFile != null && !sourceFile.isEmpty()) {
                        comment.append("Source: ").append(sourceFile).append("\n");
                    }

                    // Parameters with registers
                    @SuppressWarnings("unchecked")
                    List<Map<String, String>> params = (List<Map<String, String>>) localInfo.get("params");
                    if (params != null && !params.isEmpty()) {
                        comment.append("Params: ");
                        for (int i = 0; i < params.size(); i++) {
                            if (i > 0) comment.append(", ");
                            Map<String, String> p = params.get(i);
                            comment.append(p.get("type")).append(" ").append(p.get("name"));
                            String reg = p.get("register");
                            if (reg != null && !reg.isEmpty()) {
                                comment.append(" (").append(reg).append(")");
                            }
                        }
                        comment.append("\n");
                    }

                    // Local variables with registers
                    @SuppressWarnings("unchecked")
                    List<Map<String, String>> locals = (List<Map<String, String>>) localInfo.get("locals");
                    if (locals != null && !locals.isEmpty()) {
                        comment.append("Locals: ");
                        for (int i = 0; i < locals.size(); i++) {
                            if (i > 0) comment.append(", ");
                            Map<String, String> l = locals.get(i);
                            comment.append(l.get("type")).append(" ").append(l.get("name"));
                            String reg = l.get("register");
                            if (reg != null && !reg.isEmpty()) {
                                comment.append(" (").append(reg).append(")");
                            }
                        }
                    }

                    String commentStr = comment.toString().trim();
                    if (!commentStr.isEmpty()) {
                        func.setComment(commentStr);
                        localsApplied++;
                    }
                } catch (Exception e) {
                    localsErrors++;
                }
            }
        } else {
            println("WARNING: gc_locals_map.json not found at: " + localsMapFile.getAbsolutePath());
        }

        println("Functions with local comments: " + localsApplied);
        println("Not found in binary:           " + localsNotFound);
        println("Errors:                        " + localsErrors);

        // ============================================================
        // Summary
        // ============================================================
        println("\n=== ApplyFullDebugInfo Summary ===");
        println("Types imported:         " + typesImported);
        println("Signatures applied:     " + sigApplied);
        println("  Return types set:     " + retTypesApplied);
        println("  Param names set:      " + paramNamesApplied);
        println("  Param types set:      " + paramTypesApplied);
        println("Local var comments set: " + localsApplied);
        println("Total functions:        " + fm.getFunctionCount());
    }

    // ================================================================
    // Type Resolution
    // ================================================================

    private DataType resolveType(String typeStr) {
        if (typeStr == null || typeStr.isEmpty()) return null;

        typeStr = typeStr.trim();

        // Check cache
        DataType cached = typeCache.get(typeStr);
        if (cached != null) return cached;

        DataType result = resolveTypeInternal(typeStr);
        if (result != null) {
            typeCache.put(typeStr, result);
        }
        return result;
    }

    private DataType resolveTypeInternal(String typeStr) {
        // Skip inline struct definitions (too complex for simple resolution)
        if (typeStr.contains("{ //") || typeStr.contains("// total size")) {
            // Complex inline struct - if it ends with * treat as void pointer
            if (typeStr.endsWith("*")) {
                return new PointerDataType(DataType.DEFAULT, dtm);
            }
            return null;
        }

        // Handle pointer types
        if (typeStr.endsWith("*")) {
            String baseStr = typeStr.substring(0, typeStr.length() - 1).trim();
            // Handle double/triple pointers recursively
            if (baseStr.endsWith("*")) {
                DataType innerPtr = resolveType(baseStr);
                if (innerPtr != null) {
                    return new PointerDataType(innerPtr, dtm);
                }
                return new PointerDataType(DataType.DEFAULT, dtm);
            }
            DataType baseType = resolveType(baseStr);
            if (baseType != null) {
                return new PointerDataType(baseType, dtm);
            }
            // Unknown base type - generic pointer
            return new PointerDataType(DataType.DEFAULT, dtm);
        }

        // Handle function pointer: type (*)(...)
        if (typeStr.contains("(*)")) {
            return new PointerDataType(DataType.DEFAULT, dtm);
        }

        // Handle arrays like "type[N]"
        if (typeStr.endsWith("]")) {
            int bracketStart = typeStr.lastIndexOf("[");
            if (bracketStart > 0) {
                return resolveType(typeStr.substring(0, bracketStart).trim());
            }
        }

        // Strip struct/enum/union prefix for lookup
        String lookupName = typeStr;
        if (lookupName.startsWith("struct ")) {
            lookupName = lookupName.substring(7).trim();
        }
        if (lookupName.startsWith("enum ")) {
            lookupName = lookupName.substring(5).trim();
        }
        if (lookupName.startsWith("union ")) {
            lookupName = lookupName.substring(6).trim();
        }

        // Primitive types
        switch (typeStr) {
            case "void": return VoidDataType.dataType;
            case "int": return IntegerDataType.dataType;
            case "signed int": return IntegerDataType.dataType;
            case "unsigned int": return UnsignedIntegerDataType.dataType;
            case "short": return ShortDataType.dataType;
            case "signed short": return ShortDataType.dataType;
            case "unsigned short": return UnsignedShortDataType.dataType;
            case "char": return CharDataType.dataType;
            case "signed char": return CharDataType.dataType;
            case "unsigned char": return UnsignedCharDataType.dataType;
            case "long": return LongDataType.dataType;
            case "signed long": return LongDataType.dataType;
            case "unsigned long": return UnsignedLongDataType.dataType;
            case "long long": return LongLongDataType.dataType;
            case "signed long long": return LongLongDataType.dataType;
            case "unsigned long long": return UnsignedLongLongDataType.dataType;
            case "float": return FloatDataType.dataType;
            case "double": return DoubleDataType.dataType;
            case "_Bool": return BooleanDataType.dataType;
            case "bool": return BooleanDataType.dataType;
        }

        // Search in DTM (includes types imported from nu2_types.h)
        DataType found = findDataType(lookupName);
        if (found != null) return found;

        // Also try with the original name (e.g. "struct foo")
        if (!lookupName.equals(typeStr)) {
            found = findDataType(typeStr);
            if (found != null) return found;
        }

        return null;
    }

    private DataType findDataType(String name) {
        Iterator<DataType> iter = dtm.getAllDataTypes();
        while (iter.hasNext()) {
            DataType dt = iter.next();
            if (dt.getName().equals(name)) {
                return dt;
            }
        }
        return null;
    }

    private int countDataTypes(DataTypeManager manager) {
        int count = 0;
        Iterator<DataType> iter = manager.getAllDataTypes();
        while (iter.hasNext()) {
            iter.next();
            count++;
        }
        return count;
    }

    /**
     * Import types from header file.
     * The header should be pre-sorted to resolve forward references.
     * Uses iterative retry: on "Undefined data type" errors, adds a
     * forward typedef declaration and retries (handles any remaining
     * external/missing types).
     */
    private int importTypesWithForwardDecls(File typesFile) {
        int totalImported = 0;
        try {
            String content = new String(java.nio.file.Files.readAllBytes(typesFile.toPath()));
            Set<String> forwardDecls = new HashSet<>();
            int maxAttempts = 30;
            int beforeAll = countDataTypes(dtm);

            for (int attempt = 0; attempt < maxAttempts; attempt++) {
                StringBuilder prefix = new StringBuilder();
                for (String name : forwardDecls) {
                    prefix.append("typedef int ").append(name).append(";\n");
                }
                String augmented = prefix.toString() + content;

                try {
                    CParser parser = new CParser(dtm);
                    parser.parse(new ByteArrayInputStream(augmented.getBytes()));

                    // CParser stores parsed types in its own result DTM
                    // We need to copy them into the program's DTM
                    DataTypeManager parsedDtm = parser.getDataTypeManager();
                    int added = 0;
                    if (parsedDtm != null) {
                        Iterator<DataType> iter = parsedDtm.getAllDataTypes();
                        while (iter.hasNext()) {
                            DataType dt = iter.next();
                            if (dt instanceof BuiltInDataType) continue;
                            try {
                                dtm.addDataType(dt, DataTypeConflictHandler.REPLACE_HANDLER);
                                added++;
                            } catch (Exception ex) {
                                // skip
                            }
                        }
                    }

                    int afterAll = countDataTypes(dtm);
                    totalImported = afterAll - beforeAll;
                    if (attempt > 0) {
                        println("CParser succeeded after " + (attempt + 1) + " attempts (" + forwardDecls.size() + " forward decls)");
                    }
                    // Verify types are accessible - check for a known type
                    DataType testType = findDataType("nuvec_s");
                    int dtmTotal = countDataTypes(dtm);
                    println("Types added from parsed DTM: " + added + ", DTM total: " + dtmTotal);
                    println("Verification: nuvec_s " + (testType != null ? "FOUND (" + testType.getLength() + " bytes)" : "NOT FOUND"));
                    return added > 0 ? added : totalImported;
                } catch (Exception e) {
                    String msg = e.getMessage();
                    // Handle "Undefined data type" by adding forward declaration
                    if (msg != null && msg.contains("Undefined data type \"")) {
                        int start = msg.indexOf("Undefined data type \"") + 21;
                        int endQuote = msg.indexOf("\"", start);
                        if (endQuote > start) {
                            String typeName = msg.substring(start, endQuote);
                            if (!forwardDecls.contains(typeName)) {
                                println("  Adding forward decl for: " + typeName);
                                forwardDecls.add(typeName);
                                continue;
                            }
                        }
                    }
                    // Non-recoverable or repeated error
                    println("CParser error: " + (msg != null && msg.length() > 100 ? msg.substring(0, 100) + "..." : msg));
                    break;
                }
            }

            int afterAll = countDataTypes(dtm);
            totalImported = afterAll - beforeAll;
            if (totalImported > 0) {
                println("Types imported (partial): " + totalImported);
            }
        } catch (Exception e) {
            println("ERROR importing types: " + e.getMessage());
        }
        return totalImported;
    }

    // ================================================================
    // JSON Parsing - robust approach handling complex nested values
    // ================================================================

    /**
     * Parse simple flat JSON: {"key1": "value1", "key2": "value2", ...}
     * Used for gc_name_map_full.json.
     */
    private Map<String, String> parseSimpleJson(String json) {
        Map<String, String> map = new HashMap<>();
        json = json.trim();
        if (json.startsWith("{")) json = json.substring(1);
        if (json.endsWith("}")) json = json.substring(0, json.length() - 1);

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

    /**
     * Parse nested JSON with complex string values that can contain braces/brackets.
     * Works for both gc_signatures_map.json and gc_locals_map.json.
     * Format: {"funcName": {"key": "value", "params": [{"name":"x","type":"int"}], ...}}
     *
     * Strategy: character-by-character scan with proper string/escape tracking.
     */
    @SuppressWarnings("unchecked")
    private Map<String, Map<String, Object>> parseNestedJson(String json) {
        Map<String, Map<String, Object>> result = new HashMap<>();
        int len = json.length();
        int pos = 0;

        // Skip to opening {
        pos = skipTo(json, pos, '{');
        if (pos >= len) return result;
        pos++; // skip {

        while (pos < len) {
            pos = skipWs(json, pos);
            if (pos >= len) break;
            if (json.charAt(pos) == '}') break;
            if (json.charAt(pos) == ',') { pos++; continue; }

            // Read function name key
            if (json.charAt(pos) != '"') { pos++; continue; }
            int[] keyResult = readQuotedString(json, pos);
            if (keyResult == null) break;
            String funcName = json.substring(pos + 1, keyResult[0]);
            pos = keyResult[1];

            // Skip to colon
            pos = skipTo(json, pos, ':');
            if (pos >= len) break;
            pos++;

            // Skip to opening { of value object
            pos = skipTo(json, pos, '{');
            if (pos >= len) break;

            // Find matching } for this value object
            int objStart = pos;
            int objEnd = findMatchingBrace(json, pos);
            if (objEnd < 0) break;

            String objStr = json.substring(objStart, objEnd + 1);
            Map<String, Object> funcData = parseFuncObject(objStr);
            result.put(funcName, funcData);

            pos = objEnd + 1;
        }

        return result;
    }

    /**
     * Parse a single function's data object.
     * Expected keys: return_type (string), source_file (string), params (array), locals (array)
     */
    @SuppressWarnings("unchecked")
    private Map<String, Object> parseFuncObject(String json) {
        Map<String, Object> data = new HashMap<>();
        int len = json.length();
        int pos = 1; // skip opening {

        while (pos < len) {
            pos = skipWs(json, pos);
            if (pos >= len) break;
            if (json.charAt(pos) == '}') break;
            if (json.charAt(pos) == ',') { pos++; continue; }

            // Read key
            if (json.charAt(pos) != '"') { pos++; continue; }
            int[] keyResult = readQuotedString(json, pos);
            if (keyResult == null) break;
            String key = json.substring(pos + 1, keyResult[0]);
            pos = keyResult[1];

            // Skip to colon
            pos = skipTo(json, pos, ':');
            if (pos >= len) break;
            pos++;
            pos = skipWs(json, pos);
            if (pos >= len) break;

            char nextChar = json.charAt(pos);
            if (nextChar == '"') {
                // String value
                int[] valResult = readQuotedString(json, pos);
                if (valResult == null) break;
                String value = json.substring(pos + 1, valResult[0]);
                data.put(key, value);
                pos = valResult[1];
            } else if (nextChar == '[') {
                // Array value - parse array of param/local objects
                int arrEnd = findMatchingBracket(json, pos);
                if (arrEnd < 0) break;
                String arrStr = json.substring(pos, arrEnd + 1);
                List<Map<String, String>> items = parseObjectArray(arrStr);
                data.put(key, items);
                pos = arrEnd + 1;
            } else if (nextChar == '{') {
                // Nested object - skip it
                int objEnd = findMatchingBrace(json, pos);
                if (objEnd < 0) break;
                pos = objEnd + 1;
            } else {
                // Number, boolean, null - read until comma or }
                int valEnd = pos;
                while (valEnd < len && json.charAt(valEnd) != ',' && json.charAt(valEnd) != '}') {
                    valEnd++;
                }
                String value = json.substring(pos, valEnd).trim();
                data.put(key, value);
                pos = valEnd;
            }
        }

        // Ensure expected keys exist
        if (!data.containsKey("return_type")) data.put("return_type", "");
        if (!data.containsKey("source_file")) data.put("source_file", "");
        if (!data.containsKey("params")) data.put("params", new ArrayList<>());
        if (!data.containsKey("locals")) data.put("locals", new ArrayList<>());

        return data;
    }

    /**
     * Parse a JSON array of objects, each with string key-value pairs.
     * Handles values that contain special characters including braces.
     */
    private List<Map<String, String>> parseObjectArray(String json) {
        List<Map<String, String>> result = new ArrayList<>();
        int len = json.length();
        int pos = 1; // skip [

        while (pos < len) {
            pos = skipWs(json, pos);
            if (pos >= len) break;
            if (json.charAt(pos) == ']') break;
            if (json.charAt(pos) == ',') { pos++; continue; }

            if (json.charAt(pos) == '{') {
                int objEnd = findMatchingBrace(json, pos);
                if (objEnd < 0) break;
                String objStr = json.substring(pos, objEnd + 1);
                Map<String, String> item = parseSimpleObject(objStr);
                if (!item.isEmpty()) result.add(item);
                pos = objEnd + 1;
            } else {
                pos++;
            }
        }

        return result;
    }

    /**
     * Parse a JSON object with only string values: {"name":"x","type":"int","register":"r31"}
     */
    private Map<String, String> parseSimpleObject(String json) {
        Map<String, String> item = new HashMap<>();
        int len = json.length();
        int pos = 1; // skip {

        while (pos < len) {
            pos = skipWs(json, pos);
            if (pos >= len) break;
            if (json.charAt(pos) == '}') break;
            if (json.charAt(pos) == ',') { pos++; continue; }

            // Read key
            if (json.charAt(pos) != '"') { pos++; continue; }
            int[] keyResult = readQuotedString(json, pos);
            if (keyResult == null) break;
            String key = json.substring(pos + 1, keyResult[0]);
            pos = keyResult[1];

            // Skip to colon
            pos = skipTo(json, pos, ':');
            if (pos >= len) break;
            pos++;
            pos = skipWs(json, pos);
            if (pos >= len) break;

            // Read value
            if (json.charAt(pos) == '"') {
                int[] valResult = readQuotedString(json, pos);
                if (valResult == null) break;
                String value = json.substring(pos + 1, valResult[0]);
                item.put(key, value);
                pos = valResult[1];
            } else {
                // Non-string value
                int valEnd = pos;
                while (valEnd < len && json.charAt(valEnd) != ',' && json.charAt(valEnd) != '}') {
                    valEnd++;
                }
                item.put(key, json.substring(pos, valEnd).trim());
                pos = valEnd;
            }
        }

        return item;
    }

    // ================================================================
    // Low-level JSON scanning helpers
    // ================================================================

    /** Read a quoted string starting at pos (which must be "). Returns [endQuoteIdx, nextPos]. */
    private int[] readQuotedString(String json, int pos) {
        if (pos >= json.length() || json.charAt(pos) != '"') return null;
        int i = pos + 1;
        int len = json.length();
        while (i < len) {
            char c = json.charAt(i);
            if (c == '\\') { i += 2; continue; }
            if (c == '"') return new int[] { i, i + 1 };
            i++;
        }
        return null;
    }

    /** Find matching } for { at pos, properly tracking nested braces and strings. */
    private int findMatchingBrace(String json, int pos) {
        if (pos >= json.length() || json.charAt(pos) != '{') return -1;
        int depth = 1;
        int i = pos + 1;
        int len = json.length();
        while (i < len && depth > 0) {
            char c = json.charAt(i);
            if (c == '"') {
                int[] sr = readQuotedString(json, i);
                if (sr == null) return -1;
                i = sr[1];
                continue;
            }
            if (c == '{') depth++;
            else if (c == '}') depth--;
            if (depth == 0) return i;
            i++;
        }
        return -1;
    }

    /** Find matching ] for [ at pos, properly tracking nested brackets and strings. */
    private int findMatchingBracket(String json, int pos) {
        if (pos >= json.length() || json.charAt(pos) != '[') return -1;
        int depth = 1;
        int i = pos + 1;
        int len = json.length();
        while (i < len && depth > 0) {
            char c = json.charAt(i);
            if (c == '"') {
                int[] sr = readQuotedString(json, i);
                if (sr == null) return -1;
                i = sr[1];
                continue;
            }
            if (c == '[') depth++;
            else if (c == ']') depth--;
            if (depth == 0) return i;
            i++;
        }
        return -1;
    }

    private int skipWs(String json, int pos) {
        while (pos < json.length() && Character.isWhitespace(json.charAt(pos))) pos++;
        return pos;
    }

    private int skipTo(String json, int pos, char target) {
        while (pos < json.length() && json.charAt(pos) != target) pos++;
        return pos;
    }
}
