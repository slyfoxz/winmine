#include<iostream>
#include<stdlib.h>
#include<Windows.h>
#include<TlHelp32.h>

using namespace std;

DWORD dwArrOffset = 0xAAA38;
DWORD rows, cols;
DWORD v5;
int **map1;

int main()
{
	HWND hwnd = FindWindowW(NULL, L"扫雷");
	if (hwnd == 0x00000000)
	{
		cout << "扫雷未运行\n";
		system("pause");
		return 0;
	}
	SetForegroundWindow(hwnd);
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	HANDLE handle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
	HANDLE hsnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	MODULEENTRY32 me;
	me.dwSize = sizeof(MODULEENTRY32);
	Module32First(hsnapshot, &me);
	HMODULE hModule = me.hModule;
	DWORD v1;
	ReadProcessMemory(handle, (LPCVOID)((DWORD64)hModule + dwArrOffset), &v1, 4, NULL);
	DWORD v2;
	ReadProcessMemory(handle, (LPCVOID)(v1 + 0x18), &v2, 4, NULL);
	ReadProcessMemory(handle, (LPCVOID)(v2 + 0x0c), &rows, 4, NULL);
	ReadProcessMemory(handle, (LPCVOID)(v2 + 0x10), &cols, 4, NULL);
	cout << "行:" << rows << " 列:" << cols << "\n";
	POINT point;
	point.x = 38;
	point.y = 38;
	ClientToScreen(hwnd, &point);
	SetCursorPos(point.x, point.y);
	Sleep(15);
	SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
	Sleep(15);
	SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
	Sleep(15);
	SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
	Sleep(15);
	SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
	cout << "等待2s生成雷区\n";
	Sleep(2000);
	DWORD bombaddr;
	BYTE block;
	ReadProcessMemory(handle, (LPCVOID)(v2 + 0x58), &v1, 4, NULL);
	ReadProcessMemory(handle, (LPCVOID)(v1 + 0x10), &bombaddr, 4, NULL);
	int sizet = rows * cols;
	map1 = new int*[rows];
	for (int i = 0; i < rows; i++)
		map1[i] = new int[cols];
	for (int i = 0; i < cols; i++)
	{
		ReadProcessMemory(handle, (LPCVOID)(bombaddr+i*8), &v1, 4, NULL);
		ReadProcessMemory(handle, (LPCVOID)(v1 + 0x10), &v5, 4, NULL);
		for (int i2 = 0; i2 < rows; i2++)
		{
			ReadProcessMemory(handle, (LPCVOID)v5, &block, 1, NULL);
			v5++;
			map1[i2][i]=block;
		}
	}
	cout << "雷区:\n";
	for (int i = 0; i < rows; i++)
	{
		for (int i2 = 0; i2 < cols; i2++)
		{
			cout << map1[i][i2] << " ";
		}
		cout << "\n";
	}
	for (int i = 0; i < rows; i++)
	{
		for (int i2 = 0; i2 < cols; i2++)
		{
			if (map1[i][i2] == 0)
			{
				SetCursorPos(point.x + i2 * 18, point.y + i * 18);
				Sleep(2);
				SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
				SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
				SendMessage(hwnd, WM_LBUTTONDOWN, 0, 0);
				SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
			}
		}
	}
	cout << "完成\n";
	for (int i = 0; i < rows; i++)
	{
		delete[] map1[i];
		map1[i] = NULL;
	}
	delete[] map1;
	map1 = NULL;
	CloseHandle(handle);
	CloseHandle(hsnapshot);
	system("pause");
	return 0;
}