#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h> 
#include <vector>
#include <string>
//some global vars

DWORD pID = NULL;
HANDLE processHandle = NULL;
uintptr_t TargetAddress = NULL;
const int val = 1;

//strigns
std::string two = "2";
std::string u = "u";
std::string S = "S";
std::string dot = ".";
std::string t = "t";
std::string k = "k";
std::string o = "o";
std::string d = "d";
std::string r = "r";
std::string e = "e";
std::string l = "l";
std::string n = "n";
std::string i = "i";
std::string dash = "-";
std::string C = "C";
std::string c = "c";
std::string zero = "0";
std::string space = " ";
//gets the base addr of the module
uintptr_t dwGetModuleBaseAddress(const TCHAR* lpszModuleName, uintptr_t pID)
{
    uintptr_t dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnapshot, &ModuleEntry32))
    {
        do
        {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0)
            {
                dwModuleBaseAddress = (uintptr_t)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32));

    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

//my crappy function that reads a multilevel pointer
uintptr_t iniPRT(const char moduleName[], uintptr_t offsetGameToBaseAddress, std::vector<uintptr_t> Offsets)
{
    uintptr_t gameBaseAddress = dwGetModuleBaseAddress(_T(moduleName), pID);
    uintptr_t baseAddress = NULL;
    ReadProcessMemory(processHandle, (LPVOID)(gameBaseAddress + offsetGameToBaseAddress), &baseAddress, sizeof(baseAddress), NULL);
    uintptr_t Address = baseAddress;
    for (int i = 0; i < Offsets.size() - 1; i++)
    {
        ReadProcessMemory(processHandle, (LPVOID)(Address + Offsets.at(i)), &Address, sizeof(Address), NULL);
    }
    Address += Offsets.at(Offsets.size() - 1);
    return Address;
}

int main()
{
    std::string windowName = C + o + u + n + t + e + r + dash + S + t + r + i + k + e + space + two;
    HWND hGameWindow = FindWindow(NULL, windowName.c_str());
    std::string moduleName = c + l + i + e + n + t + dot + d + l + l;
    GetWindowThreadProcessId(hGameWindow, &pID);
    processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);

    WriteProcessMemory(processHandle, (LPVOID)(iniPRT(moduleName.c_str(), 0x01D10008, { 0x48 })), &val, sizeof(int), 0); // pointer to r_aoproxy_show 
    CloseHandle(processHandle);
    return 0;
}