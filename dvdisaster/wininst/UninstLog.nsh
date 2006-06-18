; Uninstall Log Macros & Functions

; WARNING!!! the file encoding is Win-1252
; Copyright (C) 2004-2006 THE dvdisaster'S COPYRIGHT HOLDER
; Copyright 2006 Lubos Stanek
; This file is distributed under the same license as the dvdisaster package.


; overwrite file macro
!macro FileOverwrite FilePath FileName
    FileWrite $UninstLog "F$OUTDIR\${FileName}$\r$\n"
    File "${FilePath}${FileName}"
!macroend
!define FileOverwrite "!insertmacro FileOverwrite"

; overwrite file macro
!macro FileOverwriteIncReboot FilePath FileName
    FileWrite $UninstLog "F$OUTDIR\${FileName}$\r$\n"
    File "${FilePath}${FileName}"
    ${If} ${Errors}
      Push $0
      GetTempFilename $0 $OUTDIR
      FileWrite $UninstLog "F$OUTDIR\$0$\r$\n"
      File /oname=$0 "${FilePath}${FileName}"
      Rename /REBOOTOK $0 ${FileName}
      ClearErrors
      Pop $0
    ${EndIf}
!macroend
!define FileOverwriteIncReboot "!insertmacro FileOverwriteIncReboot"

; overwrite file macro with error count
!macro FileOverwriteNoError FilePath FileName
    FileWrite $UninstLog "F$OUTDIR\${FileName}$\r$\n"
    File "${FilePath}${FileName}"
    ${If} ${Errors}
      IntOp $iSectionErrors $iSectionErrors + 1
      ClearErrors
    ${EndIf}
!macroend
!define FileOverwriteNoError "!insertmacro FileOverwriteNoError"

; output path macro
!macro SetOutPath Path
  ${Unless} ${FileExists} "$OUTDIR\${Path}"
    ${Unless} ${FileExists} "$OUTDIR\${Path}\*.*"
      FileWrite $UninstLog "D${PATH}$\r$\n"
    ${EndIf}
  ${EndIf}
  SetOutPath "${Path}"
!macroend
!define SetOutPath "!insertmacro SetOutPath"

; initlog macro
!macro InitLog LogFile Comment
  FileOpen $UninstLog "${LogFile}" w
  FileWrite $UninstLog "; Do not touch, this file was automatically generated!$\r$\n"
  FileWrite $UninstLog ";$\r$\n"
  FileWrite $UninstLog "; Uninstall log:$\r$\n"
  FileWrite $UninstLog "; ${Comment}$\r$\n"
  FileWrite $UninstLog ";$\r$\n"
  FileWrite $UninstLog "; Legend:$\r$\n"
  FileWrite $UninstLog "; Directory, File, Subkey, Value, Language$\r$\n"
  FileWrite $UninstLog "; Play it back in the reverse direction.$\r$\n"
  FileWrite $UninstLog ";$\r$\n"
!macroend
!define InitLog "!insertmacro InitLog"

; finishlog macro
!macro FinishLog Comment
  FileWrite $UninstLog "; ${Comment}$\r$\n"
  FileWrite $UninstLog "; Uninstall log end.$\r$\n"
  FileClose $UninstLog
!macroend
!define FinishLog "!insertmacro FinishLog"

; write file to uninstall log
Function FileToUninstallLog
  Exch $0
  FileWrite $UninstLog "F$OUTDIR\$0$\r$\n"
  Pop $0
FunctionEnd

; write file to uninstall log
Function FilePathToUninstallLog
  Exch $0
  FileWrite $UninstLog "F$0$\r$\n"
  Pop $0
FunctionEnd

; write path to uninstall log
Function PathToUninstallLog
  Exch $0
  FileWrite $UninstLog "D$0$\r$\n"
  Pop $0
FunctionEnd

; write registry subkey to uninstall log
Function RegKeyToUninstallLog
  Exch $0
  FileWrite $UninstLog "S$0$\r$\n"
  Pop $0
FunctionEnd

; write registry value to uninstall log
Function RegValueToUninstallLog
  Exch $0
  FileWrite $UninstLog "V$0$\r$\n"
  Pop $0
FunctionEnd

; write installer language to uninstall log
Function InstallerLanguageToUninstallLog
  Exch $0
  FileWrite $UninstLog "L$0$\r$\n"
  Pop $0
FunctionEnd
