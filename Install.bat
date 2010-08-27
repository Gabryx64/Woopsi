@echo off

:options
echo Woopsi install script for Windows
echo.
echo This script will compile libwoopsi and then install libwoopsi and libfreetype to your devkitPro directory.
echo.
echo Enter location of devKitPro directory or leave blank for default (C:\devkitPro)
set /p location=:
if "%location%"=="" goto defaultlocation
goto install

:defaultlocation
set location=C:\devkitPro

:install
echo.
echo Installing to %location%.

:compilewoopsi
echo.
echo Compiling libwoopsi...

cd .\Woopsi\libwoopsi
make clean
make

echo.
echo libwoopsi compiled.

:installwoopsi
echo.
echo Installing libwoopsi...
echo.

echo Removing installation (if any)
rmdir %location%\libwoopsi /s /q

echo Creating install directory
mkdir %location%\libwoopsi

echo Installing icon
xcopy icon %location%\libwoopsi\icon\ /q

echo Installing includes
xcopy include %location%\libwoopsi\include\ /q

echo Installing fonts
xcopy include\fonts %location%\libwoopsi\include\fonts\ /q

echo Installing library 
xcopy lib %location%\libwoopsi\lib\ /q

cd ..

:installlibfreetype
echo.
echo Installing libfreetype...
echo.

echo Removing installation (if any)
rmdir %location%\libfreetype /s /q

echo Creating install directory
mkdir %location%\libfreetype

echo Installing library
xcopy .\libs\libfreetype %location%\libfreetype /e /q