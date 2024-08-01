@echo off
::clang main.c -o tetris.exe -luser32 -lgdi32 --target=x86_64-pc-windows-msvc -g -std=c99
clang main.c -o tetris.exe -luser32 -lgdi32 -O3 -std=c99
