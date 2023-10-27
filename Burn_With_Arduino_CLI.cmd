@echo off

for /f "tokens=*" %%a in (
'%USERPROFILE%\Downloads\serial-locate.exe --vid=2341 --pid=0043 --usb=true'
) do (
    set output=%%a
)

if not exist %USERPROFILE%\Downloads\arduino-cli_0.34.2_Windows_64bit (
cd %USERPROFILE%\Downloads && 	^
%SystemRoot%\System32\curl.exe "https://github.com/arduino/arduino-cli/releases/download/0.34.2/arduino-cli_0.34.2_Windows_64bit.zip" -L -O &&^
C:\PROGRA~1\7-Zip\7z.exe x arduino-cli_0.34.2_Windows_64bit.zip -O"%USERPROFILE%\Downloads\arduino-cli_0.34.2_Windows_64bit"
) else (
	echo arduino-cli found
)

set PATH=%USERPROFILE%\Downloads\arduino-cli_0.34.2_Windows_64bit;

arduino-cli core download arduino:avr
arduino-cli.exe core install arduino:avr
arduino-cli upload -p %output% --fqbn  arduino:avr:uno --input-file=EIE3105_ATmega328P_Application.hex
pause
