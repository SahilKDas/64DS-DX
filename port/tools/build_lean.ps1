[CmdletBinding()]
param(
    [ValidatePattern('^[D-Zd-z]$')][string] $Drive = 'Z',
    [ValidateRange(1, 32)][int] $Jobs = 2
)
$ErrorActionPreference = 'Stop'
$taskRoot = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$taskDrive = $Drive.ToUpperInvariant() + ':'
$taskMapped = $false
$taskPushed = $false
try {
    if (Test-Path ($taskDrive + '\')) {
        throw "$taskDrive is occupied; choose a free letter with -Drive"
    }
    & subst $taskDrive $taskRoot
    if ($LASTEXITCODE) { throw 'Could not create the temporary short build path' }
    $taskMapped = $true
    $taskVswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
    $taskVs = & $taskVswhere -latest -version '[17.0,18.0)' -products '*' `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if ($LASTEXITCODE -or -not $taskVs) { throw 'Visual Studio C++ tools were not found' }
    $taskCmakeRoot = Join-Path $taskVs 'Common7/IDE/CommonExtensions/Microsoft/CMake'
    $taskCmake = Join-Path $taskCmakeRoot 'CMake/bin/cmake.exe'
    $taskVcvars = Join-Path $taskVs 'VC/Auxiliary/Build/vcvars32.bat'
    if (-not (Test-Path $taskCmake)) { throw 'Install the Visual Studio CMake tools component' }

    # A cache created through a different drive spelling cannot be reused.
    # --fresh resets CMake's generated metadata, not source files or ROM inputs.
    $taskFresh = ''
    $taskCache = Join-Path $taskRoot 'build/lean-release/CMakeCache.txt'
    if (Test-Path $taskCache) {
        $taskHome = Get-Content $taskCache | Where-Object { $_ -like 'CMAKE_HOME_DIRECTORY:INTERNAL=*' }
        $taskExpected = 'CMAKE_HOME_DIRECTORY:INTERNAL=' + $taskDrive + '/port'
        if ($taskHome -ne $taskExpected) { $taskFresh = '--fresh' }
        $taskCompiler = Get-Content $taskCache | Where-Object { $_ -like 'CMAKE_C_COMPILER:FILEPATH=*' }
        $taskCompilerPrefix = 'CMAKE_C_COMPILER:FILEPATH=' + $taskVs.Replace('\', '/') + '/VC/'
        if ($taskCompiler -notlike ($taskCompilerPrefix + '*')) { $taskFresh = '--fresh' }
    }
    Push-Location ($taskDrive + '/port')
    $taskPushed = $true
    $taskCommand = 'set "PATH=' + $taskCmakeRoot + '\Ninja;%PATH%" && call "' +
        $taskVcvars + '" >nul && "' + $taskCmake + '" --preset lean-release ' +
        $taskFresh + ' && "' + $taskCmake + '" --build --preset lean-release --parallel ' + $Jobs
    & cmd /d /c $taskCommand
    if ($LASTEXITCODE) { throw "Lean build failed: $LASTEXITCODE" }
    & python (Join-Path $taskRoot 'port/tools/storage_budget.py') $taskRoot --kind developer
    if ($LASTEXITCODE) { throw 'Developer checkout exceeds the storage budget or cannot be measured' }
} finally {
    if ($taskPushed) { Pop-Location }
    if ($taskMapped) {
        & subst $taskDrive /D
        if ($LASTEXITCODE) { Write-Warning "Could not remove temporary mapping $taskDrive" }
    }
}
