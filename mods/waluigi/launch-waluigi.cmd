@echo off
setlocal
set "SM64DS_CHARACTER=4"
set "SM64DS_CHARACTER_STATE=%~dp0selected-character.txt"
set "SM64DS_CHARACTER_PACK_DIR=%~dp0characters"
set "SM64DS_CHARACTER_PACK_PREVIEW=1"
set "SM64DS_WALUIGI_TEXTURE_DIR=%~dp0waluigi-assets"
start "Tango 0.3.7 - Waluigi" "%~dp0walk_window-waluigi.exe"
