#include <Windows.h>
#include <vector>

#define LOCALE_RUS 0x4190419
#define TRANSLIT_DEFAULT 1
#define TRANSLIT_SWITCH_KEY 69

struct Key {
	DWORD vkCode, scanCode;
	bool correctionRequired;
	Key(DWORD vkCode, DWORD scanCode, bool correctionRequired = 0) {
		this->vkCode = vkCode;
		this->scanCode = scanCode;
		this->correctionRequired = correctionRequired;
	}
};

struct Translation {
	bool correctionPossible;
	std::vector<Key> variants;
	Translation(bool correctionPossible, std::vector<Key> variants) {
		this->correctionPossible = correctionPossible;
		this->variants = variants;
	}
};

std::vector<Translation> translationMap = {
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(70, 33)))))), // A -> F
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(188, 51)))))), // B -> ,
	*(new Translation(1, *(new std::vector<Key>(27, *(new Key(0, 0, 0)))))), // C -> W, X(H)
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(76, 38)))))), // D -> L
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(84, 20)))))), // E -> T
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(65, 30)))))), // F -> A
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(85, 22)))))), // G -> U
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(219, 26)))))), // H -> [
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(66, 48)))))), // I -> B
	*(new Translation(1, *(new std::vector<Key>(27, *(new Key(0, 0, 0)))))), // J -> Q, Z(A), '(E), `(O), .(U)
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(82, 19)))))), // K -> R
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(75, 37)))))), // L -> K
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(86, 47)))))), // M -> V
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(89, 21)))))), // N -> Y
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(74, 36)))))), // O -> J
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(71, 34)))))), // P -> G
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(221, 27)))))), // Q -> ]
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(72, 35)))))), // R -> H
	*(new Translation(1, *(new std::vector<Key>(27, *(new Key(0, 0, 0)))))), // S -> C, I(H)
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(78, 49)))))), // T -> N
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(69, 18)))))), // U -> E
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(68, 47)))))), // V -> D
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(79, 24)))))), // W -> O
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(219, 26)))))), // X -> [
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(83, 31)))))), // Y -> S
	*(new Translation(1, *(new std::vector<Key>(27, *(new Key(0, 0, 0)))))), // Z -> P, ;(H)
	*(new Translation(0, *(new std::vector<Key>(1, *(new Key(77, 50))))))  // ' -> M
};

bool eatKey = 1,
	 ignore = 0,
	 correctionPossible = 0;
int lastLocale = -1,
	lastKeyIndex = 0;

void initializeTranslationMap() {
	translationMap[2].variants[2].vkCode = 87, translationMap[2].variants[2].scanCode = 17;
	translationMap[2].variants[7].correctionRequired = 1, translationMap[2].variants[7].vkCode = 88, translationMap[2].variants[7].scanCode = 45;
	
	translationMap[9].variants[9].vkCode = 81, translationMap[9].variants[9].scanCode = 16;
	translationMap[9].variants[0].correctionRequired = 1, translationMap[9].variants[0].vkCode = 90, translationMap[9].variants[0].scanCode = 44;
	translationMap[9].variants[4].correctionRequired = 1, translationMap[9].variants[4].vkCode = 222, translationMap[9].variants[4].scanCode = 40;
	translationMap[9].variants[14].correctionRequired = 1, translationMap[9].variants[14].vkCode = 192, translationMap[9].variants[14].scanCode = 41;
	translationMap[9].variants[20].correctionRequired = 1, translationMap[9].variants[20].vkCode = 190, translationMap[9].variants[20].scanCode = 52;

	translationMap[18].variants[18].vkCode = 67, translationMap[18].variants[18].scanCode = 46;
	translationMap[18].variants[7].correctionRequired = 1, translationMap[18].variants[7].vkCode = 73, translationMap[18].variants[7].scanCode = 23;

	translationMap[25].variants[25].vkCode = 80, translationMap[25].variants[25].scanCode = 25;
	translationMap[25].variants[7].correctionRequired = 1, translationMap[25].variants[7].vkCode = 186, translationMap[25].variants[7].scanCode = 39;
	
	return;
}

bool isRussianLocale() {
	GUITHREADINFO Gti;
	::ZeroMemory(&Gti, sizeof(GUITHREADINFO));
	Gti.cbSize = sizeof(GUITHREADINFO);
	::GetGUIThreadInfo(0, &Gti);
	DWORD dwThread = ::GetWindowThreadProcessId(Gti.hwndActive, 0);
	int curLocale = (int)(::GetKeyboardLayout(dwThread));
	if (lastLocale != LOCALE_RUS && curLocale == LOCALE_RUS) eatKey = TRANSLIT_DEFAULT;
	lastLocale = curLocale;
	return  curLocale == LOCALE_RUS;
}

KEYBDINPUT constructInput(KBDLLHOOKSTRUCT curKey) {
	KEYBDINPUT result;
	result.wVk = (WORD)curKey.vkCode;
	result.wScan = (WORD)curKey.scanCode;
	result.dwFlags = curKey.flags;
	result.time = curKey.time;
	result.dwExtraInfo = curKey.dwExtraInfo;
	return result;
}

INPUT constructMessage(KBDLLHOOKSTRUCT curKey) {
	INPUT result;
	result.type = INPUT_KEYBOARD;
	result.ki = constructInput(curKey);
	return result;
}

KBDLLHOOKSTRUCT translateKey(KBDLLHOOKSTRUCT curKey) {
	int translationIndex = curKey.vkCode == 222 ? 26 : curKey.vkCode - 65;

	if (correctionPossible && translationMap[lastKeyIndex].variants[translationIndex].correctionRequired) {
		KBDLLHOOKSTRUCT eraseKey = curKey;
		eraseKey.vkCode = 8, eraseKey.scanCode = 14;
		INPUT pInputs[1] = { constructMessage(eraseKey) };
		SendInput(1, pInputs, sizeof(INPUT));

		curKey.vkCode = translationMap[lastKeyIndex].variants[translationIndex].vkCode;
		curKey.scanCode = translationMap[lastKeyIndex].variants[translationIndex].scanCode;
		correctionPossible = 0;

		return curKey;
	}

	if (!translationMap[translationIndex].correctionPossible) {
		curKey.vkCode = translationMap[translationIndex].variants[0].vkCode;
		curKey.scanCode = translationMap[translationIndex].variants[0].scanCode;
		correctionPossible = 0;
	} else {
		curKey.vkCode = translationMap[translationIndex].variants[translationIndex].vkCode;
		curKey.scanCode = translationMap[translationIndex].variants[translationIndex].scanCode;
		correctionPossible = 1;
		lastKeyIndex = translationIndex;
	}
	return curKey;
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode < 0 || wParam != 256 || !isRussianLocale() || ignore) return CallNextHookEx(NULL, nCode, wParam, lParam);
	
	KBDLLHOOKSTRUCT curKey = *(KBDLLHOOKSTRUCT*)lParam;
	if (curKey.scanCode == TRANSLIT_SWITCH_KEY) eatKey = !eatKey;

	if (eatKey && (curKey.vkCode > 64 && curKey.vkCode < 91 || curKey.vkCode == 222)) {
		ignore = 1;

		curKey = translateKey(curKey);
		INPUT pInputs[1] = {constructMessage(curKey)};
		SendInput(1, pInputs, sizeof(INPUT));
		
		ignore = 0;
		return -1;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show) {

	initializeTranslationMap();

	HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, NULL, NULL);

	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(keyboardHook);

	return 0;
}