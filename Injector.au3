
; Originally built by Der Moench


Global Const $hKernel32 = DllOpen("kernel32.dll")

Global $mCharAddr


Func OpenProcess($iPID)
	Local $aProcess = DllCall($hKernel32, _
			"int", "OpenProcess", _
			"int", 0x1F0FFF, _
			"int", 1, _
			"int", $iPID)

	Return $aProcess[0]
EndFunc   ;==>OpenProcess

Func ReadStructure($hProcess, $iAddress, ByRef $aStructure)
	Local $lRet = DllCall($hKernel32, _
			"int", "ReadProcessMemory", _
			"int", $hProcess, _
			"int", $iAddress, _
			"ptr", DllStructGetPtr($aStructure), _
			"int", DllStructGetSize($aStructure), _
			"int", "")

	Return $lRet[0]
EndFunc   ;==>ReadStructure

Func SwapEndianness($aHex)
	Return Hex(Binary(Dec($aHex)))
EndFunc   ;==>SwapEndianness

Func ScanForPtr($aProc, $aByteString, $aOffset = 0, $aStartAddr = 0x401000, $aEndAddr = 0x900000)
	Local $lSystemInfoBuffer = DllStructCreate('word;word;dword;ptr;ptr;dword;dword;dword;dword;word;word'), $lTmp
	DllCall($hKernel32, 'int', 'GetSystemInfo', 'ptr', DllStructGetPtr($lSystemInfoBuffer))
	Local $lBuffer = DllStructCreate('byte[' & DllStructGetData($lSystemInfoBuffer, 3) & ']')
	For $iAddr = $aStartAddr To $aEndAddr Step DllStructGetData($lSystemInfoBuffer, 3)
		ReadStructure($aProc, $iAddr, $lBuffer)
		StringRegExp(DllStructGetData($lBuffer, 1), $aByteString, 1, 2)
		$lTmp = @extended
		If @error = 0 Then Return '0x' & SwapEndianness(StringMid(DllStructGetData($lBuffer, 1),($lTmp - StringLen($aByteString)) + ($aOffset * 2),8))
	Next
	Return SetError(1, 0, "0x000000")
EndFunc   ;==>ScanForPtr

Func CloseHandle($iHandle)
	DllCall($hKernel32, _
			"int", "CloseHandle", _
			"int", $iHandle)
EndFunc   ;==>CloseHandle

Func ReadMem($hProcess, $iAddress, $aType = "dword")
	Local $dBuffer = DllStructCreate($aType)

	DllCall($hKernel32, _
			"int", "ReadProcessMemory", _
			"int", $hProcess, _
			"int", $iAddress, _
			"ptr", DllStructGetPtr($dBuffer), _
			"int", DllStructGetSize($dBuffer), _
			"int", "")

	Return DllStructGetData($dBuffer, 1)
EndFunc   ;==>ReadMem

Func GetInitProc($lCharname)
	Local $lHProc
	Local $lProc = WinList("[CLASS:ArenaNet_Dx_Window_Class; REGEXPTITLE:^\D+$]")
;~ 	Local $lWinList = WinList("Guild Wars - Branch: LIVE - BuildId: 36849")
	For $i = 1 To $lProc[0][0]
		$lHProc = OpenProcess(WinGetProcess($lProc[$i][1]))
		If ReadMem($lHProc, $mCharAddr, 'wchar[30]') == $lCharname Then
			CloseHandle($lHProc)
			Return WinGetProcess($lProc[$i][1])
		EndIf
		CloseHandle($lHProc)
	Next
	Return 0
EndFunc   ;==>GetInitProc

Func CharacterSelector()
	Local $lWinList = WinList("[CLASS:ArenaNet_Dx_Window_Class; REGEXPTITLE:^\D+$]")
;~ 	Local $lWinList = WinList("Guild Wars - Branch: LIVE - BuildId: 36849")
	Switch $lWinList[0][0]
		Case 0
			Exit MsgBox(0, "Error", "No Guild Wars Clients were found.")
		Case 1
			Return WinGetProcess($lWinList[1][1])
		Case Else
			Local $lCharStr = "", $lFirstChar, $lHProc
			For $winCount = 1 To $lWinList[0][0]
				$lHProc = OpenProcess(WinGetProcess($lWinList[$winCount][1]))
				If $winCount = 1 Then $mCharAddr = ScanForPtr($lHProc,'90909066C705', 6)
				$lCharStr &= ReadMem($lHProc, $mCharAddr, 'wchar[30]')
				If $winCount = 1 Then $lFirstChar = $lCharStr
				If $winCount <> $lWinList[0][0] Then $lCharStr &= "|"
				CloseHandle($lHProc)
			Next

			Local $GUICharSelector = GUICreate("Character Select", 171, 35)
			Local $ComboCharSelector = GUICtrlCreateCombo("", 8, 8, 153, 25)
			GUICtrlSetData($ComboCharSelector, "Select Character...|" & $lCharStr, "Select Character...")
			GUISetState(@SW_SHOW, $GUICharSelector)

			While 1
				Switch GUIGetMsg()
					Case $ComboCharSelector
						Local $tmp = GUICtrlRead($ComboCharSelector)
						GUIDelete($GUICharSelector)
						Return GetInitProc($tmp)
					Case -3
						Exit
				EndSwitch
				Sleep(25)
			WEnd
	EndSwitch
EndFunc   ;==>CharacterSelector

Func _InjectDll($ProcessHwnd, $DllPath)
	If $ProcessHwnd == 0 Then Return SetError(1, "", False)
	If Not (FileExists($DllPath)) Then Return SetError(2, "", False)
	If Not (StringRight($DllPath, 4) == ".dll") Then Return SetError(3, "", False)

	Local $DLL_Path = DllStructCreate("char[255]")
	DllCall($hKernel32, "DWORD", "GetFullPathNameA", "str", $DllPath, "DWORD", 255, "ptr", DllStructGetPtr($DLL_Path), "int", 0)
	If @error Then Return SetError(5, "", False)

	Local $hModule = DllCall($hKernel32, "DWORD", "GetModuleHandleA", "str", "kernel32.dll")
	If @error Then Return SetError(7, "", False)

	Local $lpStartAddress = DllCall($hKernel32, "DWORD", "GetProcAddress", "DWORD", $hModule[0], "str", "LoadLibraryA")
	If @error Then Return SetError(8, "", False)

	Local $lpParameter = DllCall($hKernel32, "DWORD", "VirtualAllocEx", "int", $ProcessHwnd, "int", 0, "ULONG_PTR", DllStructGetSize($DLL_Path), "DWORD", 0x3000, "int", 4)
	If @error Then Return SetError(9, "", False)

	DllCall($hKernel32, "BOOL", "WriteProcessMemory", "int", $ProcessHwnd, "DWORD", $lpParameter[0], "str", DllStructGetData($DLL_Path, 1), "ULONG_PTR", DllStructGetSize($DLL_Path), "int", 0)
	If @error Then Return SetError(10, "", False)

	Local $hThread = DllCall($hKernel32, "int", "CreateRemoteThread", "DWORD", $ProcessHwnd, "int", 0, "int", 0, "DWORD", $lpStartAddress[0], "DWORD", $lpParameter[0], "int", 0, "int", 0)
	If @error Then Return SetError(11, "", False)

	Return SetError(0, "", True)
EndFunc   ;==>_InjectDll


Global $iProc = CharacterSelector()
Global $hProc = OpenProcess($iProc)


Func Inject($path)
	If (_InjectDll($hProc, $path)) Then
		MsgBox(0, "Injector", "Injection successful!", 3)
	Else
		MsgBox(0, "Injector", "Injection failed!", 0)
	EndIf
EndFunc


If (FileExists(@ScriptDir & "\April.dll")) Then
	Inject(@ScriptDir & "\April.dll")

ElseIf (FileExists(@ScriptDir & "\April_d.dll")) Then
	Inject(@ScriptDir & "\April_d.dll")

Else
	MsgBox(0, "Injector", "April.dll not found in this directory!")
EndIf
