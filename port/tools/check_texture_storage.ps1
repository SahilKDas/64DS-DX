param([string] $BuildDirectory = 'build/storage092-proof')
$ErrorActionPreference = 'Stop'
$repo = (Resolve-Path (Join-Path $PSScriptRoot '../..')).Path
$work = [IO.Path]::GetFullPath((Join-Path $repo $BuildDirectory))
New-Item -ItemType Directory -Force -Path $work | Out-Null
$baseline = & git -C $repo show '66c188cfea5a2404634f8e4f584478294fd11a5f:port/ntr/gx.cpp'
if ($LASTEXITCODE) { throw 'Cannot read pristine v0.9.2 baseline' }
[IO.File]::WriteAllLines((Join-Path $work 'baseline_gx.cpp'), $baseline)
$root = $repo.Replace('\', '/')
$cmake = @'
cmake_minimum_required(VERSION 3.16)
project(texture_storage_proof CXX)
set(CMAKE_CXX_STANDARD 17)
foreach(mode baseline optimized)
  add_executable(${mode} "@ROOT@/port/tests/texture_registry_storage.cpp"
    "@ROOT@/port/ntr/texture.cpp" "@ROOT@/port/ntr/smooth.cpp"
    "@ROOT@/port/ntr/ppu.cpp" "@ROOT@/port/ntr/ppu_sub.cpp" "@ROOT@/port/ntr/ppu_audit.cpp")
  target_include_directories(${mode} PRIVATE "@ROOT@/port/ntr/include")
  target_compile_options(${mode} PRIVATE /Gy /Gw /wd4311 /wd4312)
  target_link_options(${mode} PRIVATE /OPT:REF /INCREMENTAL:NO)
endforeach()
target_compile_definitions(baseline PRIVATE STORAGE_BASELINE_GX="${CMAKE_CURRENT_SOURCE_DIR}/baseline_gx.cpp")
'@
[IO.File]::WriteAllText((Join-Path $work 'CMakeLists.txt'), $cmake.Replace('@ROOT@', $root))
& cmake -S $work -B "$work/bin" -G 'Visual Studio 17 2022' -A Win32
if ($LASTEXITCODE) { throw 'Proof configure failed' }
& cmake --build "$work/bin" --config Release
if ($LASTEXITCODE) { throw 'Proof build failed' }
$original = & "$work/bin/Release/baseline.exe"
if ($LASTEXITCODE) { throw 'Baseline probe failed' }
$optimized = & "$work/bin/Release/optimized.exe"
if ($LASTEXITCODE) { throw 'Optimized probe failed' }
$original
$optimized
for ($i = 0; $i -lt 3; $i++) {
  $a = [regex]::Match($original[$i], 'hash=([0-9a-f]+)').Groups[1].Value
  $b = [regex]::Match($optimized[$i], 'hash=([0-9a-f]+)').Groups[1].Value
  if (-not $a -or $a -ne $b) { throw "Filter $i differs from v0.9.2" }
}
