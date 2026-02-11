param(
    [Parameter(Mandatory=$true)][string]$GhidraHome,
    [Parameter(Mandatory=$true)][string]$InputDol,
    [string]$ProjectDir = "$PSScriptRoot\..\projects",
    [string]$ProjectName = "crashwoc_gc_full",
    [string]$OutputDir = "$PSScriptRoot\..\raw"
)

$ErrorActionPreference = "Stop"

$scriptPath = Join-Path $PSScriptRoot "ghidra_scripts"
$analyze = Join-Path $GhidraHome "support\analyzeHeadless.bat"
if (!(Test-Path $analyze)) { throw "analyzeHeadless.bat not found: $analyze" }
if (!(Test-Path $InputDol)) { throw "Input DOL not found: $InputDol" }

New-Item -ItemType Directory -Force -Path $ProjectDir | Out-Null
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

$env:GHIDRA_OUTPUT_DIR = (Resolve-Path $OutputDir).Path

& $analyze $ProjectDir $ProjectName `
    -import $InputDol `
    -overwrite `
    -processor "PowerPC:BE:32:Gekko_Broadway" `
    -cspec "default" `
    -scriptPath $scriptPath `
    -postScript "ApplyGCNames.java" `
    -postScript "ApplyFullDebugInfo.java" `
    -postScript "ExportAll.java"

if ($LASTEXITCODE -ne 0) {
    throw "Ghidra headless failed with exit code $LASTEXITCODE"
}

Write-Host "Done. Output in: $OutputDir"
