@echo off
setlocal

REM Attempt to find glslc from Vulkan SDK env
if defined VULKAN_SDK (
    set GLSLC="%VULKAN_SDK%\Bin\glslc.exe"
) else (
    where glslc >nul 2>nul
    if %errorlevel%==0 (
        for /f "delims=" %%i in ('where glslc') do set GLSLC="%%i"
    ) else (
        echo GLSLC not found. Please install the Vulkan SDK or add glslc to PATH.
        exit /b 1
    )
)

echo Using GLSLC: %GLSLC%

for %%F in (shaders\*.vert shaders\*.frag) do (
    %GLSLC% %%F -o %%F.spv
)

echo Done compiling shaders.
endlocal
