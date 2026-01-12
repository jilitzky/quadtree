$ErrorActionPreference = "Stop"

Push-Location "$PSScriptRoot/.."

cmake -B "build" .

Pop-Location
