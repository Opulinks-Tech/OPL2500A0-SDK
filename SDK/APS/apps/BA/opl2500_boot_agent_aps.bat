@ECHO OFF

SET TARGET_PATH=%1
SET OUTPUT_NAME=%2
REM #1 [$L] = .\Output\Objects\
REM #2 [@L] = opl2500_boot_agent_aps
REM (After build) .\opl2500_boot_agent_aps.bat $L @L

REM #Remove file
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.s37 DEL %TARGET_PATH%%OUTPUT_NAME%.s37 /Q
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.bin DEL %TARGET_PATH%%OUTPUT_NAME%.bin /Q
REM #Remove folder
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.s37 RD %TARGET_PATH%%OUTPUT_NAME%.s37 /Q
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.bin RD %TARGET_PATH%%OUTPUT_NAME%.bin /Q

SET FROM_ELF="C:\Keil_v5\ARM\ARMCC\bin\fromelf.exe"

%FROM_ELF% %TARGET_PATH%%OUTPUT_NAME%.axf --m32combined --output %TARGET_PATH%%OUTPUT_NAME%.s37
%FROM_ELF% %TARGET_PATH%%OUTPUT_NAME%.axf --bin --output %TARGET_PATH%%OUTPUT_NAME%.bin

REM If multiple binary file output, force add extension name ".bin"
IF EXIST %TARGET_PATH%%OUTPUT_NAME%.bin\* REN %TARGET_PATH%%OUTPUT_NAME%.bin\* *.bin
