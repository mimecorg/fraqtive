@echo off

set BUILDDIR="..\release"

set UPXDIR="C:\Program Files\UPX"

if exist bin del /q /s bin\*.*
if not exist bin mkdir bin

copy %BUILDDIR%\fraqtive.exe bin

%UPXDIR%\upx.exe --lzma bin\fraqtive.exe
