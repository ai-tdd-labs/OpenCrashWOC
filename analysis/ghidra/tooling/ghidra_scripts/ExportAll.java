/* ###
 * Export comprehensive analysis: decompiled code, functions, strings
 * Adapted from crimson project for Crash Bandicoot WoC decomp
 * @category Export
 */

import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.data.DataType;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.*;

public class ExportAll extends GhidraScript {

    private String outputDir;
    private String programName;

    @Override
    public void run() throws Exception {
        outputDir = System.getenv("GHIDRA_OUTPUT_DIR");
        if (outputDir == null || outputDir.isEmpty()) {
            outputDir = ".";
        }

        programName = currentProgram.getName().replaceAll("[^a-zA-Z0-9._-]", "_");

        println("=== CrashWOC Export Starting ===");
        println("Output directory: " + outputDir);
        println("Program: " + currentProgram.getName());
        println("Architecture: " + currentProgram.getLanguage().getProcessor());
        println("");

        exportSummary();
        exportDecompiled();
        exportFunctions();
        exportStrings();

        println("");
        println("=== Export complete ===");
    }

    private void exportSummary() throws Exception {
        File outputFile = new File(outputDir, programName + "_summary.txt");
        println("Exporting summary to: " + outputFile.getName());

        try (PrintWriter writer = new PrintWriter(new FileWriter(outputFile))) {
            writer.println("Crash Bandicoot: Wrath of Cortex - Binary Analysis Summary");
            writer.println("==========================================================");
            writer.println("");
            writer.println("File: " + currentProgram.getName());
            writer.println("Architecture: " + currentProgram.getLanguage().getProcessor());
            writer.println("Address Size: " + currentProgram.getLanguage().getLanguageDescription().getSize() + " bit");
            writer.println("Endianness: " + (currentProgram.getLanguage().isBigEndian() ? "Big Endian" : "Little Endian"));
            writer.println("Compiler: " + currentProgram.getCompilerSpec().getCompilerSpecID());
            writer.println("");

            int totalFuncs = 0, externalFuncs = 0, thunkFuncs = 0, namedFuncs = 0;
            FunctionIterator funcs = currentProgram.getFunctionManager().getFunctions(true);
            while (funcs.hasNext()) {
                Function f = funcs.next();
                totalFuncs++;
                if (f.isExternal()) externalFuncs++;
                if (f.isThunk()) thunkFuncs++;
                if (!f.getName().startsWith("FUN_") && !f.getName().startsWith("thunk_")) namedFuncs++;
            }

            writer.println("Functions:");
            writer.println("  Total: " + totalFuncs);
            writer.println("  Named: " + namedFuncs);
            writer.println("  External: " + externalFuncs);
            writer.println("  Thunks: " + thunkFuncs);
            writer.println("  Auto-named (FUN_): " + (totalFuncs - externalFuncs - thunkFuncs - namedFuncs));
            writer.println("");

            writer.println("Memory Sections:");
            for (var block : currentProgram.getMemory().getBlocks()) {
                writer.println("  " + block.getName() + ": " + block.getStart() + " - " + block.getEnd() +
                    " (" + block.getSize() + " bytes)" +
                    (block.isExecute() ? " [X]" : "") +
                    (block.isWrite() ? " [W]" : "") +
                    (block.isRead() ? " [R]" : ""));
            }
        }
    }

    private void exportDecompiled() throws Exception {
        File outputFile = new File(outputDir, programName + "_decompiled.c");
        File tempFile = new File(outputDir, programName + "_decompiled.c.tmp");
        println("Exporting decompiled code to: " + outputFile.getName());

        DecompInterface decompiler = new DecompInterface();
        DecompileOptions options = new DecompileOptions();
        decompiler.setOptions(options);

        if (!decompiler.openProgram(currentProgram)) {
            printerr("Failed to initialize decompiler");
            return;
        }

        try (PrintWriter writer = new PrintWriter(new FileWriter(tempFile))) {
            writer.println("/* Decompiled from: " + currentProgram.getName() + " */");
            writer.println("/* Architecture: " + currentProgram.getLanguage().getProcessor() + " */");
            writer.println("");

            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            int count = 0;
            int failed = 0;

            while (functions.hasNext()) {
                Function func = functions.next();
                if (func.isExternal() || func.isThunk()) continue;

                monitor.setMessage("Decompiling: " + func.getName());

                // Use per-function timeout, but never stop the global export loop early.
                DecompileResults results = decompiler.decompileFunction(func, 60, monitor);
                if (results.decompileCompleted()) {
                    writer.println("/* " + func.getName() + " @ " + func.getEntryPoint() + " */");
                    writer.println(results.getDecompiledFunction().getC());
                    writer.println("");
                    count++;
                } else {
                    writer.println("/* FAILED: " + func.getName() + " @ " + func.getEntryPoint() + " */");
                    writer.println("/* Error: " + results.getErrorMessage() + " */");
                    writer.println("");
                    failed++;
                }
            }
            println("  Decompiled " + count + " functions (" + failed + " failed)");
        } finally {
            decompiler.dispose();
        }

        // Atomic-ish replace: keep old export if this run did not complete successfully.
        if (outputFile.exists() && !outputFile.delete()) {
            printerr("WARNING: failed to delete old decompiled export: " + outputFile.getAbsolutePath());
        }
        if (!tempFile.renameTo(outputFile)) {
            throw new RuntimeException("Failed to move temp decompiled export to final path: " + outputFile.getAbsolutePath());
        }
    }

    private void exportFunctions() throws Exception {
        File outputFile = new File(outputDir, programName + "_functions.json");
        println("Exporting functions to: " + outputFile.getName());

        try (PrintWriter writer = new PrintWriter(new FileWriter(outputFile))) {
            writer.println("[");

            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            boolean first = true;
            int count = 0;

            while (functions.hasNext()) {
                Function func = functions.next();
                if (!first) writer.println(",");
                first = false;

                writer.println("  {");
                writer.println("    \"name\": \"" + escapeJson(func.getName()) + "\",");
                writer.println("    \"address\": \"" + func.getEntryPoint() + "\",");
                writer.println("    \"size\": " + func.getBody().getNumAddresses() + ",");
                writer.println("    \"signature\": \"" + escapeJson(func.getPrototypeString(false, false)) + "\",");
                writer.println("    \"external\": " + func.isExternal() + ",");
                writer.println("    \"thunk\": " + func.isThunk() + ",");

                Set<Function> calls = func.getCalledFunctions(monitor);
                writer.print("    \"calls\": [");
                int callIdx = 0;
                for (Function calledFunc : calls) {
                    if (callIdx > 0) writer.print(", ");
                    writer.print("\"" + escapeJson(calledFunc.getName()) + "\"");
                    callIdx++;
                }
                writer.println("]");
                writer.print("  }");
                count++;
            }

            writer.println();
            writer.println("]");
            println("  Exported " + count + " functions");
        }
    }

    private void exportStrings() throws Exception {
        File outputFile = new File(outputDir, programName + "_strings.txt");
        println("Exporting strings to: " + outputFile.getName());

        try (PrintWriter writer = new PrintWriter(new FileWriter(outputFile))) {
            DataIterator dataIterator = currentProgram.getListing().getDefinedData(true);
            int count = 0;

            while (dataIterator.hasNext()) {
                Data data = dataIterator.next();
                DataType dt = data.getBaseDataType();
                String typeName = dt.getName().toLowerCase();

                if (typeName.contains("string") || typeName.contains("unicode")) {
                    Object value = data.getValue();
                    if (value instanceof String) {
                        String str = (String) value;
                        if (str.length() >= 3) {
                            writer.println(data.getAddress() + ": " + str);
                            count++;
                        }
                    }
                }
            }
            println("  Exported " + count + " strings");
        }
    }

    private String escapeJson(String s) {
        if (s == null) return "";
        return s.replace("\\", "\\\\")
                .replace("\"", "\\\"")
                .replace("\n", "\\n")
                .replace("\r", "\\r")
                .replace("\t", "\\t");
    }
}
