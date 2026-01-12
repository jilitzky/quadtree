$ErrorActionPreference = "Stop"

Push-Location "$PSScriptRoot/.."

if (Test-Path "build")
{
    Remove-Item -Path "build" -Recurse -Force
}

Pop-Location

& "$PSScriptRoot\generate_vs.ps1"
