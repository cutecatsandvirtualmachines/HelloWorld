// recon_tool.cpp
#include <windows.h>
#include <iphlpapi.h>
#include <iostream>
#include <string>
#include <vector>
#include <setupapi.h>
#include <devguid.h>
#include <tchar.h>
#include <intrin.h>

#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "setupapi.lib")

void PrintSystemInfo() {
    TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);
    GetComputerName(computerName, &size);
    std::wcout << L"[+] Computer Name: " << computerName << std::endl;

    TCHAR username[UNLEN + 1];
    DWORD username_len = UNLEN + 1;
    GetUserName(username, &username_len);
    std::wcout << L"[+] Username: " << username << std::endl;

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::cout << "[+] CPU Architecture: ";
    switch (sysInfo.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_AMD64: std::cout << "x64 (AMD or Intel)"; break;
        case PROCESSOR_ARCHITECTURE_INTEL: std::cout << "x86"; break;
        default: std::cout << "Unknown"; break;
    }
    std::cout << std::endl;

    std::cout << "[+] Number of processors: " << sysInfo.dwNumberOfProcessors << std::endl;
}

void ListDrives() {
    DWORD drives = GetLogicalDrives();
    std::cout << "[+] Logical Drives: ";
    for (char i = 0; i < 26; i++) {
        if ((drives >> i) & 1) {
            std::cout << char('A' + i) << ":\\ ";
        }
    }
    std::cout << std::endl;
}

void ListConnectedDevices() {
    std::cout << "[+] Connected Devices:" << std::endl;

    HDEVINFO devInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_PRESENT | DIGCF_ALLCLASSES);
    if (devInfo == INVALID_HANDLE_VALUE) return;

    SP_DEVINFO_DATA devData;
    devData.cbSize = sizeof(SP_DEVINFO_DATA);

    for (DWORD i = 0; SetupDiEnumDeviceInfo(devInfo, i, &devData); i++) {
        TCHAR buf[1024];
        if (SetupDiGetDeviceRegistryProperty(devInfo, &devData, SPDRP_DEVICEDESC,
                                              NULL, (PBYTE)buf, sizeof(buf), NULL)) {
            std::wcout << L"  - " << buf << std::endl;
        }
    }

    SetupDiDestroyDeviceInfoList(devInfo);
}

int main() {
    std::cout << "=== Windows Recon Tool ===" << std::endl;
    PrintSystemInfo();
    std::cout << std::endl;

    ListDrives();
    std::cout << std::endl;

    ListConnectedDevices();
    std::cout << std::endl;

    return 0;
}
