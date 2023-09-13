#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <locale>
#include <codecvt>
#include <string>
#include <chrono>
#include <thread>

std::string processName = "Lunar Client.exe";

int main() {
	while (true) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		std::wstring_convert<std::codecvt_utf8<WCHAR>>converter;

		HANDLE snapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

		PROCESSENTRY32W procEntry = { 0 };
		procEntry.dwSize = sizeof(PROCESSENTRY32);

		bool processFirst = Process32FirstW(snapshotHandle, &procEntry);
		if (processFirst == false) {
			std::cout << "Runtime error: 1";
			system("pause");
			return 1;
		}
		if (converter.to_bytes(procEntry.szExeFile) == processName) {
			HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, false, procEntry.th32ProcessID);
			bool processTermination = TerminateProcess(processHandle, 0);
			if (processTermination == 0) {
				std::cout << "Runtime error: 2";
				system("pause");
				return 2;
			}
		}

		while (Process32NextW(snapshotHandle, &procEntry) != false) {
			if (converter.to_bytes(procEntry.szExeFile) == processName) {
				HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, false, procEntry.th32ProcessID);
				bool processTermination = TerminateProcess(processHandle, 0);
			}
		}
	}
	return 0;
}