#include "com-api.hpp"

#include <sstream>
#include <iostream>

namespace modules::com_api {
    IOCode Port::QueryPort() {
        std::stringstream portName("");

        portName << COM_API_SYSTEM_PREFIX << this->attr_system_id;

        this->attr_system_name = portName.str();

        HANDLE hCom = CreateFileA(
            wr::stringToLpFileName(this->attr_system_name), 
            wr::dwDesireAccess(this->attr_desired_access),
            wr::dwShareAccess(this->attr_sharing_access), NULL,
            wr::dwCreationDisposition(this->attr_flags_creation),
            wr::dwFlagsAndAttributes(this->attr_file_flags), NULL
        );

        if (hCom == INVALID_HANDLE_VALUE) {
            DWORD error_code = GetLastError();

            switch (error_code) {
                case ERROR_FILE_NOT_FOUND:
                    return IOCode::ERROR_INVALID_PORT;
                    break;
                case ERROR_ACCESS_DENIED:
                    return IOCode::ERROR_PORT_ACCESS_DENIED;
                    break;
                case ERROR_INVALID_PARAMETER:
                    return IOCode::ERROR_INVALID_PARAM;
                    break;
                default:
                    return IOCode::ERROR_INVALID_VALUE;
                    break;
            }
        } else {
            this->hCom = hCom;
            this->attr_is_available = PortStatus::PORT_AVAILABLE;

            return IOCode::QUERY_SUCCESS;
        }
    }

    IOCode Port::Close() {
        if (this->hCom != nullptr) {
            return CloseHandle(this->hCom) ? IOCode::QUERY_SUCCESS : IOCode::ERROR_INVALID_VALUE;
        } else {
            return IOCode::ERROR_INVALID_VALUE;
        }
    }

    int Port::GetSystemID() {
        return attr_system_id;
    }

    PortStatus Port::CheckPort() {
        if (this->attr_is_available != PortStatus::PORT_AVAILABLE) {
            return PortStatus::PORT_NOT_AVAILABLE;
        }

        return PortStatus::PORT_AVAILABLE;
    }

    PortCollection ScanPorts() {
        PortCollection ports; 
        
        for (int i = 1; i < 256; i++) { 
            Port port(i);

            port.attr_desired_access    = COM_API_GENERIC_RWRD;
            port.attr_sharing_access    = FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE;
            port.attr_flags_creation    = OPEN_EXISTING;
            port.attr_file_flags        = FILE_ATTRIBUTE_NORMAL;

            IOCode status = port.QueryPort();

            if (status == IOCode::QUERY_SUCCESS) {
                char pathBuff[5000]; 

                QueryDosDeviceA(&(port.attr_system_name)[4], pathBuff, sizeof(pathBuff));
                port.attr_friendly_name = std::string(pathBuff);

                ports.push_back(port);
        
                if (port.Close() != IOCode::QUERY_SUCCESS) {
                    DWORD error = GetLastError();
                    std::cerr << "Error closing handle: " << error << std::endl;
                }
            }
        }
       
        return ports;
    }

    Port QueryPortById(const PortCollection& ports, int sysId) {
        for (const auto& port : ports) {
            if (port.attr_system_id == sysId) {
                return port;
            }
        }
        return Port(-1); // Return invalid port with negative system ID
    }

    PortInfo QueryPortInfo(const Port& port, QueryInfoType infoType) {
        Port tmpPort(port.attr_system_id); 
        PortInfo portInfo(port);

        // Открытие порта для получения информации
        IOCode status = tmpPort.QueryPort(); 

        if (status != IOCode::QUERY_SUCCESS) {
            DWORD error_code = GetLastError();
            std::cerr << "Ошибка при открытии порта: " << error_code << std::endl;
            return portInfo;
        }

        COMSTAT comStat;
        DWORD errors;
        if (ClearCommError(tmpPort.hCom, &errors, &comStat)) {
            portInfo.errors = errors;
            portInfo.cbInQue = comStat.cbInQue;
            portInfo.cbOutQue = comStat.cbOutQue;
        }

        DCB dcbSerialParams = { 0 };
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        if (GetCommState(tmpPort.hCom, &dcbSerialParams)) {
            portInfo.baudRate = dcbSerialParams.BaudRate;
            portInfo.byteSize = dcbSerialParams.ByteSize;
            portInfo.parity = dcbSerialParams.Parity;
            portInfo.stopBits = dcbSerialParams.StopBits;
        }

        // Получение информации о таймаутах порта
        COMMTIMEOUTS timeouts;
        if (GetCommTimeouts(tmpPort.hCom, &timeouts)) {
            portInfo.readIntervalTimeout = timeouts.ReadIntervalTimeout;
            portInfo.readTotalTimeoutMultiplier = timeouts.ReadTotalTimeoutMultiplier;
            portInfo.readTotalTimeoutConstant = timeouts.ReadTotalTimeoutConstant;
            portInfo.writeTotalTimeoutMultiplier = timeouts.WriteTotalTimeoutMultiplier;
            portInfo.writeTotalTimeoutConstant = timeouts.WriteTotalTimeoutConstant;
        }

        tmpPort.Close();

        return portInfo;
    }
}