#include "com-api.hpp"

namespace modules::com_api {

    std::vector<ComPortInfo> scanPorts() {
        std::vector<ComPortInfo> comPorts;
        HDEVINFO deviceInfoSet = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_PRESENT);
        if (deviceInfoSet == INVALID_HANDLE_VALUE) {
            return comPorts;
        }

        SP_DEVINFO_DATA deviceInfoData;
        deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

        for (DWORD i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &deviceInfoData); ++i) {
            char portName[256];
            char friendlyName[256];

            if (SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_FRIENDLYNAME, nullptr, (PBYTE)friendlyName, sizeof(friendlyName), nullptr) &&
                SetupDiGetDeviceRegistryProperty(deviceInfoSet, &deviceInfoData, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, nullptr, (PBYTE)portName, sizeof(portName), nullptr)) {
                comPorts.push_back({portName, friendlyName});
            }
        }

        SetupDiDestroyDeviceInfoList(deviceInfoSet);
        return comPorts;
    }

    // HANDLE openPort(const std::string& portName, DWORD baudRate) {
    //     std::wstring widePortName(portName.begin(), portName.end());
    //     HANDLE hCom = CreateFile(widePortName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
    //     if (hCom == INVALID_HANDLE_VALUE) {
    //         return INVALID_HANDLE_VALUE;
    //     }

    //     DCB dcb;
    //     SecureZeroMemory(&dcb, sizeof(DCB));
    //     dcb.DCBlength = sizeof(DCB);

    //     if (!GetCommState(hCom, &dcb)) {
    //         CloseHandle(hCom);
    //         return INVALID_HANDLE_VALUE;
    //     }

    //     dcb.BaudRate = baudRate;
    //     dcb.ByteSize = 8;
    //     dcb.StopBits = ONESTOPBIT;
    //     dcb.Parity = NOPARITY;

    //     if (!SetCommState(hCom, &dcb)) {
    //         CloseHandle(hCom);
    //         return INVALID_HANDLE_VALUE;
    //     }

    //     return hCom;
    // }

    // void closePort(HANDLE hCom) {
    //     CloseHandle(hCom);
    // }

    // bool readFromPort(HANDLE hCom, char* buffer, DWORD bufferSize, DWORD& bytesRead) {
    //     return ReadFile(hCom, buffer, bufferSize, &bytesRead, nullptr);
    // }

    // bool writeToPort(HANDLE hCom, const char* buffer, DWORD bufferSize, DWORD& bytesWritten) {
    //     return WriteFile(hCom, buffer, bufferSize, &bytesWritten, nullptr);
    // }

}