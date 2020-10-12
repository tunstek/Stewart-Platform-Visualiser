#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>


template <class T>
class memMapping
{
public:
	HANDLE hMapFile;
	unsigned char* mapFileBuffer;
	T* memPtr;

	T* init(const char* memFilepath) {
		// convert memFilepath to LPCWSTR
		wchar_t* wString = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, memFilepath, -1, wString, 4096);

		hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(T), wString);
		if (GetLastError() == ERROR_ALREADY_EXISTS) {
			std::cout << "Opening existing mmf." << std::endl;
		}
		if (!hMapFile)
		{
			std::cerr << "CreateFileMapping failed" << std::endl;
		}
		mapFileBuffer = (unsigned char*)MapViewOfFile(hMapFile, FILE_MAP_WRITE, 0, 0, sizeof(T));
		if (!mapFileBuffer)
		{
			std::cerr << "MapViewOfFile readonly access failed." << std::endl;
		}
		return (T*)mapFileBuffer;
	}
};

