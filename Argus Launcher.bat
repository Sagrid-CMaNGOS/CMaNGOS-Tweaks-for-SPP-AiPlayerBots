@echo off
title Argus Launcher 1.0
color 0a
mode con: cols=60 lines=30

set "REALMD=C:\ARGUS\SERVER\realmd.exe"
set "MANGOSD=C:\ARGUS\SERVER\mangosd.exe"
set "CLIENT=C:\ARGUS\SERVER\Vanilla Client\Wow_tweaked.exe"
set "REALM_LOG=C:\ARGUS\SERVER\logs\realmd.log"
set "SERVER_LOG=C:\ARGUS\SERVER\logs\server.log"

cls
echo         _..._ ___
echo       .:::::::.  `"-._.-''.
echo  ,   /:::::::::\     ':    \                     _._
echo  \:-::::::::::::\     :.    ^|     /^|.-'         /:::\
echo   \::::::::\:::::^|    ':     ^|   ^|  /           ^|:::^|
echo    `:::::::^|:::::\     ':    ^|   `\ ^|    __     ^|\::/\
echo      `-:::-^|::::::^|    ':    ^|  .`\ .\_.'  `.__/      ^|
echo            ^|::::::\    ':.   ^|   \ ';:: /.-._   ,    /
echo            ^|:::::::^|    :.   /   ,`\;:: \'./0)  ^|_.-/
echo            ;:::::::^|    ':  ^|    \.`;::.   ``   ^|  ^|
echo             \::::::/    :'  /     _\::::'      /  /
echo              \::::^|   :'   /    ,=:;::/           ^|
echo               \:::^|   :'  ^|    (='` //        /   ^|
echo                \::\   `:  /     '--' ^|       /\   ^|
echo                 \:::.  `:_^|.-"""-.    \__.-'/::\  ^|
echo                  '::::.:::...:::. '.       /:::^|  ^|
echo                   '::/::::::::::::. '-.__.:::::^|  ^|
echo       ARGUS         ^|::::::::::::\::..../::::::^| /
echo       LAUNCHER      ^|:::::::::::::^|::::/::::::://
echo       1.0            \:::::::::::::'^|::/::::::::/
echo                     /\::::::::::::/  /:::::::/:^|
echo       SAGRID'S     ^|::';:::::::::/   ^|::::::/::;
echo       PERSONAL     ^|:::/`-:::::;;-._ ^|:::::/::/
echo       MADNESS      ^|:::^|  `-::::\   `^|::::/::/
echo                    ^|:::^|     \:::\   \:::/::/
echo                   /:::/       \:::\   \:/\:/
echo                  (_::/         \:::;__ \\_\\___
echo                  (_:/           \::):):)\:::):):)
echo                   `"             `""""`  `""""""`
if exist "%REALM_LOG%" echo. > "%REALM_LOG%"
if exist "%SERVER_LOG%" echo. > "%SERVER_LOG%"

start "" /min "%REALMD%"

:WAIT_REALM
timeout /t 2 >nul
netstat -an | find "LISTENING" | find ":3724" >nul 2>&1
if %errorlevel% neq 0 goto WAIT_REALM

start "" /min "%MANGOSD%"

:WAIT_HONOR
timeout /t 1 >nul
findstr /c:">> Loaded" "%SERVER_LOG%" | findstr /c:"honor standing" >nul 2>&1
if %errorlevel% neq 0 goto WAIT_HONOR

echo Honor loaded — PvP Ranks Updating...

"C:\Program Files\MariaDB 12.0\bin\mysql.exe" -u root -proot -D classiccharacters -e "CALL ApplyPvPRankLevelCap_Weekly();"

:WAIT_INIT
timeout /t 3 >nul
if not exist "%SERVER_LOG%" goto WAIT_INIT
findstr /c:"CMANGOS: World initialized" "%SERVER_LOG%" >nul 2>&1
if %errorlevel% neq 0 goto WAIT_INIT

start "" "%CLIENT%"

timeout /t 3 >nul
exit
