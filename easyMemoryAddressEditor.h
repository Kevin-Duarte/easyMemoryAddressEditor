#pragma once
#undef UNICODE //Forces MSVS to use multi-byte character set
#undef _UNICODE // ^

#include <iostream>
#include <Windows.h>
#include <string>
#include <TlHelp32.h>
#include <tchar.h>
#include <vector>

class easyMemoryAddressEditor
{
private:
	HANDLE phandle;
	SIZE_T read_size;
	
public:
	DWORD processID = 0;

	easyMemoryAddressEditor(std::string windowName)
	{
		HWND hwnd = FindWindowA(NULL, windowName.c_str());
		if (!hwnd)
		{
			std::cerr << "Error: Could not find window" << std::endl;
			return;
		}
		
		
		if (!GetWindowThreadProcessId(hwnd, &processID))
		{
			std::cerr << "Error: Unable to get window thread process ID" << std::endl;
			return;
		}

		phandle = OpenProcess(PROCESS_ALL_ACCESS, 0, processID);
		if (!phandle)
		{
			std::cerr << "Error: Cannot open process for editing" << std::endl;
			return;
		}

	}


	/* 
		THIS SECTION IS 32 BIT
	*/

	void getValueMultiPointer(int &return_value, DWORD baseAddress, std::vector<DWORD> offsets)
	{
		for (int i = 0; i < offsets.size() - 1; i++) //All these addresses are pointers that are found with offsets
		{
			ReadProcessMemory(phandle, (void *)(baseAddress + offsets[i]), &baseAddress, sizeof(baseAddress), &read_size);
			if (read_size <= 0)
			{
				std::cerr << "Error: Invalid starting address or offsets" << std::endl;
				return;
			}
		}
		baseAddress = baseAddress + offsets[offsets.size() - 1];

		ReadProcessMemory(phandle, (void *)baseAddress, &return_value, sizeof(return_value), &read_size);
		if (read_size <= 0)
			std::cout << "Error: Could not read process memory" << std::endl;
	}

	void setValueMultiPointer(int new_value, DWORD baseAddress, std::vector<DWORD> offsets)
	{
		for (int i = 0; i < offsets.size() - 1; i++) //All these addresses are pointers that are found with offsets
		{
			ReadProcessMemory(phandle, (void *)(baseAddress + offsets[i]), &baseAddress, sizeof(baseAddress), &read_size);
			if (read_size <= 0)
			{
				std::cerr << "Error: Invalid starting address or offsets" << std::endl;
				return;
			}
		}
		baseAddress = baseAddress + offsets[offsets.size() - 1];

		WriteProcessMemory(phandle, (void *)baseAddress, &new_value, sizeof(new_value), &read_size);
		if (read_size <= 0)
			std::cout << "Error: Unable to write process memory" << std::endl;
	}

	void getModuleAddress(TCHAR *moduleName, DWORD &output_address)
	{
		output_address = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, processID);
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 moduleEntry32 = { 0 };
			moduleEntry32.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(hSnapshot, &moduleEntry32))
			{
				do
				{
					if (_tcscmp(moduleEntry32.szModule, moduleName) == 0)
					{
						output_address = (DWORD)moduleEntry32.modBaseAddr;
						break;
					}

				} while (Module32Next(hSnapshot, &moduleEntry32));

			}
			CloseHandle(hSnapshot);
		}
		else
			std::cerr << "Error: Invalid Handle Value. 32 bit program is trying to access 64 bit program. Compile for 64 bit." << std::endl;

		if (output_address == 0)
			std::cerr << "Error: Could not find a module named \"" << moduleName << "\"" << std::endl;
	}


	/*
		THIS SECTION IS 64 BIT
	*/

	void getValueMultiPointer(int &return_value, DWORD64 baseAddress, std::vector<DWORD> offsets)
	{
		for (int i = 0; i < offsets.size() - 1; i++) //All these addresses are pointers that are found with offsets
		{
			ReadProcessMemory(phandle, (void *)(baseAddress + offsets[i]), &baseAddress, sizeof(baseAddress), &read_size);
			if (read_size <= 0)
			{
				std::cerr << "Error: Invalid starting address or offsets" << std::endl;
				return;
			}
		}
		baseAddress = baseAddress + offsets[offsets.size() - 1];

		ReadProcessMemory(phandle, (void *)baseAddress, &return_value, sizeof(return_value), &read_size);
		if (read_size <= 0)
			std::cout << "Error: Could not read process memory" << std::endl;
	}


	
	void setValueMultiPointer(int new_value, DWORD64 baseAddress, std::vector<DWORD> offsets)
	{
		for (int i = 0; i < offsets.size() - 1; i++) //All these addresses are pointers that are found with offsets
		{
			ReadProcessMemory(phandle, (void *)(baseAddress + offsets[i]), &baseAddress, sizeof(baseAddress), &read_size);
			if (read_size <= 0)
			{
				std::cerr << "Error: Invalid starting address or offsets" << std::endl;
				return;
			}
		}
		baseAddress = baseAddress + offsets[offsets.size() - 1];

		WriteProcessMemory(phandle, (void *)baseAddress, &new_value, sizeof(new_value), &read_size);
		if (read_size <= 0)
			std::cout << "Error: Unable to write process memory" << std::endl;
	}
	
	void getModuleAddress(TCHAR *moduleName, DWORD64 &output_address)
	{
		output_address = 0;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE32 | TH32CS_SNAPMODULE, processID);
		if (hSnapshot != INVALID_HANDLE_VALUE)
		{
			MODULEENTRY32 moduleEntry32 = { 0 };
			moduleEntry32.dwSize = sizeof(MODULEENTRY32);
			if (Module32First(hSnapshot, &moduleEntry32))
			{
				do
				{
					if (_tcscmp(moduleEntry32.szModule, moduleName) == 0)
					{
						output_address = (DWORD64)moduleEntry32.modBaseAddr;
						break;
					}

				} while (Module32Next(hSnapshot, &moduleEntry32));

			}
			CloseHandle(hSnapshot);
		}
		else
			std::cerr << "Error: Invalid Handle Value. 64 bit program may be trying to access 32 bit program. Compile for 32 bit." << std::endl;

		if (output_address == 0)
			std::cerr << "Error: Could not find a module named \"" << moduleName << "\"" << std::endl;
	}


	~easyMemoryAddressEditor()
	{
		CloseHandle(phandle);
	}
};

