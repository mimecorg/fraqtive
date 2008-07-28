!define VERSION "0.4.4"

!define SRCDIR ".."
!define BUILDDIR "..\release"

!define QTDIR "D:\Qt4-msvc"

!define VCREDISTDIR "C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\BootStrapper\Packages\vcredist_x86\"

!include "MUI2.nsh"

Name "Fraqtive ${VERSION}"

OutFile "fraqtive-${VERSION}-win32.exe"

SetCompressor lzma

InstallDir $PROGRAMFILES\Fraqtive
InstallDirRegKey HKLM SOFTWARE\Fraqtive "InstallDir"

ShowInstDetails nevershow
ShowUninstDetails nevershow

!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install-blue.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall-blue.ico"

!define MUI_WELCOMEFINISHPAGE_BITMAP "images\wizard.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "images\wizard.bmp"

!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "images\header.bmp"
!define MUI_HEADERIMAGE_BITMAP_NOSTRETCH
!define MUI_HEADERIMAGE_RIGHT

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "${SRCDIR}\COPYING"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
  
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"

Section

    SetOutPath "$INSTDIR"

    File "${SRCDIR}\ChangeLog"
    File "${SRCDIR}\COPYING"
    File "${SRCDIR}\README"

    SetOutPath "$INSTDIR\bin"

    Delete "$INSTDIR\bin\*.*"

    File "${BUILDDIR}\fraqtive.exe"

    File "${QTDIR}\bin\QtCore4.dll"
    File "${QTDIR}\bin\QtGui4.dll"
    File "${QTDIR}\bin\QtOpenGL4.dll"

    SetOutPath "$INSTDIR\bin\imageformats"

    Delete "$INSTDIR\bin\imageformats\*.*"

    File "${QTDIR}\plugins\imageformats\qjpeg4.dll"
    File "${QTDIR}\plugins\imageformats\qtiff4.dll"

    SetOutPath "$TEMP"

    File "${VCREDISTDIR}\vcredist_x86.exe"
    ExecWait '"$TEMP\vcredist_x86.exe" /q:a /c:"msiexec /i vcredist.msi /qn"'
    Delete "$TEMP\vcredist_x86.exe"

    SetOutPath "$INSTDIR\bin"

    CreateShortCut "$SMPROGRAMS\Fraqtive.lnk" "$INSTDIR\bin\fraqtive.exe"
    CreateShortCut "$DESKTOP\Fraqtive.lnk" "$INSTDIR\bin\fraqtive.exe"

    WriteRegStr HKLM "SOFTWARE\Fraqtive" "InstallDir" "$INSTDIR"

    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Fraqtive" "DisplayName" "Fraqtive ${VERSION}"
    WriteRegStr HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Fraqtive" "UninstallString" '"$INSTDIR\uninstall.exe"'
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Fraqtive" "NoModify" 1
    WriteRegDWORD HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Fraqtive" "NoRepair" 1

    WriteUninstaller "uninstall.exe"

SectionEnd

Section "Uninstall"

    DeleteRegKey HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Fraqtive"
    DeleteRegKey HKLM "SOFTWARE\Fraqtive"

    Delete "$SMPROGRAMS\Fraqtive.lnk"
    Delete "$DESKTOP\Fraqtive.lnk"

    Delete "$INSTDIR\ChangeLog"
    Delete "$INSTDIR\COPYING"
    Delete "$INSTDIR\README"
    RMDir /r "$INSTDIR\bin"
    Delete "$INSTDIR\uninstall.exe"
    RMDir "$INSTDIR"

SectionEnd
