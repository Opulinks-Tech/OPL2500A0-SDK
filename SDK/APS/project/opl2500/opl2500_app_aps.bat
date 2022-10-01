@ECHO OFF
SET RUN_TYPE=%1
SET TARGET_PATH=%2
SET OUTPUT_NAME=%3
SET PROJ_ROOT_PATH=%4
REM #1 PRE/POST
REM #2 [$L] = ..\..\targets\opl2500\Output\Objects\ 
REM #3 [@L] = opl2500_rom_aps
REM #4 [trunk] = ..\..\..\

IF %RUN_TYPE% == POST ( GOTO POST )

REM *********************************************************************
REM pre-build

REM 1. Generate SVN header
SET CPU=APS
SET SVN_TOOL=%PROJ_ROOT_PATH%scripts\SubWCRev.exe
SET SVN_REV_FMT=%PROJ_ROOT_PATH%%CPU%\project\common\svn_rev_patch.txt 
SET SVN_REV_HDR=%PROJ_ROOT_PATH%%CPU%\project\common\svn_rev_patch.h
%SVN_TOOL%  %PROJ_ROOT_PATH%  %SVN_REV_FMT%  %SVN_REV_HDR%
GOTO DONE


:POST
REM #Remove file
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.s37 DEL %TARGET_PATH%%OUTPUT_NAME%.s37 /Q
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.bin DEL %TARGET_PATH%%OUTPUT_NAME%.bin /Q
REM #Remove folder
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.s37 RD %TARGET_PATH%%OUTPUT_NAME%.s37 /Q
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.bin RD %TARGET_PATH%%OUTPUT_NAME%.bin /Q

SET FROM_ELF=C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe
%FROM_ELF% %TARGET_PATH%%OUTPUT_NAME%.axf --m32combined --output %TARGET_PATH%%OUTPUT_NAME%.s37
%FROM_ELF% %TARGET_PATH%%OUTPUT_NAME%.axf --bin --output %TARGET_PATH%%OUTPUT_NAME%.bin
REM If multiple binary file output, force add extension name ".bin"
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.bin\* REN %TARGET_PATH%%OUTPUT_NAME%.bin\* *.bin



REM ====================================================================
REM Dump object sizes
SET SIZE_FILE=%LST_PATH%%OUTPUT_NAME%_size.txt
SET PATCH_LIB=%TARGET_PATH%opl2500_patch_aps.lib
SET AXF_FULL_PATH=%TARGET_PATH%%OUTPUT_NAME%.axf
SET ERRORLEVEL=0
IF EXIST %SIZE_FILE% ( DEL %SIZE_FILE% )
SET SDK_LIB=%TARGET_PATH%opl2500_sdk_aps.lib

ECHO. >> %SIZE_FILE%
ECHO %PATCH_LIB% >> %SIZE_FILE%
%FROM_ELF% --info=totals %PATCH_LIB% >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%
ECHO %SDK_LIB% >> %SIZE_FILE%
%FROM_ELF% --info=totals %SDK_LIB% >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%
ECHO %AXF_FULL_PATH% >> %SIZE_FILE%
%FROM_ELF% --info=totals %AXF_FULL_PATH% >> %SIZE_FILE%
ECHO. >> %SIZE_FILE%


:DONE
