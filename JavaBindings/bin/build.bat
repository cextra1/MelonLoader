@echo off
REM yeah, i'm not figuring out powershell and everything i'd need to make this good
bash -c "dos2unix ./build.sh"
bash -c ./build.sh
pause