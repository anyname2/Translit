#include <Windows.h>
#include <vector>

#include <iostream>

void getLocale() {
	GUITHREADINFO Gti;
	::ZeroMemory(&Gti, sizeof(GUITHREADINFO));
	Gti.cbSize = sizeof(GUITHREADINFO);
	::GetGUIThreadInfo(0, &Gti);
	DWORD dwThread = ::GetWindowThreadProcessId(Gti.hwndActive, 0);
	int curLocale = (int)(::GetKeyboardLayout(dwThread));

	std::cout << "Locale: " << std::hex << curLocale << std::endl;

	return;
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0 || wParam != 256) return CallNextHookEx(NULL, nCode, wParam, lParam);
	
	getLocale();

	KBDLLHOOKSTRUCT curKey = *(KBDLLHOOKSTRUCT*)lParam;

	std::cout << "Pressed: " << (char)curKey.vkCode << " - " << curKey.scanCode << std::endl << std::endl;


	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {

	HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, NULL, NULL);

	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(keyboardHook);

	return 0;
}