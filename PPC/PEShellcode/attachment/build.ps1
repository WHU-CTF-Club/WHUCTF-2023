Remove-Item -Force task.dll

Write-Output "Notice that this challenge must to be resolved under x86 environment!"
Write-Output "Now compiling and linking..."

cl /EHsc /O2 /std:c++20 task.cpp main.cpp gamemd.cpp task_blowfish.cpp /link /DLL /OUT:"task.dll" > "cl.log"

if (Test-Path "task.dll" -PathType Leaf)
{
    Write-Output "Successfully build task.dll, please use run.bat!"
}
else
{
    Write-Output "Failed to build the dll, you may check cl.log for detail."
}

Write-Output "Now cleaning up..."
Remove-Item -Force *.lib
Remove-Item -Force *.obj
Remove-Item -Force *.exp 
Write-Output "Done!"
