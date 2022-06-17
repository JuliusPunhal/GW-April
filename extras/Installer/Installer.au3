#Region ;**** Directives created by AutoIt3Wrapper_GUI ****
#AutoIt3Wrapper_Icon=data\Installer.ico
#EndRegion ;**** Directives created by AutoIt3Wrapper_GUI ****

#include <FileConstants.au3>


FileInstall("GW-April.dll", "GW-April.dll", $FC_OVERWRITE)
FileInstall("Injector.exe", "Injector.exe")

EnsureDirExists(@AppDataDir & "\GW-April\Fonts")
FileInstall("..\..\data\Fonts\ABeeZee-Regular.ttf", @AppDataDir & "\GW-April\Fonts\ABeeZee-Regular.ttf")
FileInstall("..\..\data\Fonts\Karla-Regular.ttf", @AppDataDir & "\GW-April\Fonts\Karla-Regular.ttf")
FileInstall("..\..\data\Fonts\ProggyClean.ttf", @AppDataDir & "\GW-April\Fonts\ProggyClean.ttf")
FileInstall("..\..\data\Fonts\Spartan-Regular.ttf", @AppDataDir & "\GW-April\Fonts\Spartan-Regular.ttf")
FileInstall("..\..\data\Fonts\WorkSans-Regular.ttf", @AppDataDir & "\GW-April\Fonts\WorkSans-Regular.ttf")

; ---

Func EnsureDirExists($path)
	If not FileExists($path) Then
		DirCreate($path)
	EndIf
EndFunc
