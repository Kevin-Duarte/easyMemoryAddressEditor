# easyMemoryAddressEditor
Simple .h file that edits memory address for 32-bit and 64-bit. Only works on Windows OS (Please contribute for Linux support)!

**Read this if you changed the character-set of Visual Studio or if you are getting errors:** Do not set Visual Studio to use **UNICODE** character-sets, instead set it to **Not Set** as the easyMemoryAddressEditor.h uses Multi-byte character sets. To change this, go to Project -> Properties (ALT + F7) -> Configuration properties -> General -> Character Set

Example: 
```
#include <iostream>
#include <string>
#include <vector>

#include "easyMemoryAddressEditor.h"

int main()
{
	//Grabbing onto target program using its name
	easyMemoryAddressEditor memoryEditor("Cities: Skylines");
	std::cout <<"Process ID: "<< memoryEditor.processID << std::endl;

	//Getting the DLL of the target program named "mono.dll"
	DWORD64 dllAddress;
	memoryEditor.getModuleAddress("mono.dll", dllAddress);
	std::cout << "dll Address: " << std::hex << dllAddress << std::endl;

	//Setting the offsets of the multipointer that points to the money
	std::vector<DWORD> moneyAddressOffsets;
	moneyAddressOffsets.push_back(0x0028EB40);
	moneyAddressOffsets.push_back(0x0);
	moneyAddressOffsets.push_back(0x20);
	moneyAddressOffsets.push_back(0x1B0);
	moneyAddressOffsets.push_back(0xA8);
	moneyAddressOffsets.push_back(0x1C8);

	//Setting the money using the dll address' memory with the offsets for money. 
	int new_money = 100;
	memoryEditor.getValueMultiPointer(new_money, dllAddress, moneyAddressOffsets);

	//Reading the money using the dll address' memory with the offsets for money
	int money = 0;
	memoryEditor.getValueMultiPointer(money, dllAddress, moneyAddressOffsets);
	std::cout <<std::dec<< "Money: " << money << std::endl;

	std::cin.get();
	return 0;

}
```
