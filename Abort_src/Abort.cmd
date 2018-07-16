@echo off
tskill DShutdown
shutdown -a
erase /F /Q %TMP%\DSTime.DShutdown
erase /F /Q %TEMP%\DSTime.DShutdown