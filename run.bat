call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

premake5 vs2017 || EXIT /B 1

IF NOT DEFINED config (
    msbuild || EXIT /B 1
) ELSE (
    msbuild /p:Configuration=%config% || EXIT /B 1
)

IF NOT DEFINED SKIP_RUN (
    .\.bin\main.exe || EXIT /B 1
)

