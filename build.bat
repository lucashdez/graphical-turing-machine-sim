@echo off
set opts=-g
set code=%cd%
pushd build
REM CHANGE THIS 
clang %opts% -I"%code%\src\include" -I"%code%\src\impls" -I"C:\VulkanSDK\1.3.280.0\Include"  %code%\src\main.c  -luser32 -lgdi32 -l"C:\VulkanSDK\1.3.280.0\Lib\vulkan-1.lib" -o gtms.exe
popd
