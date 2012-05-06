rem Command line build environments
call D:\Tornado2.2\host\x86-win32\bin\torVars.bat
make clean
make bootrom_uncmp.bin bootrom.bin
pause