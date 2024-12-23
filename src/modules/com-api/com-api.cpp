#include "com-api.hpp"

#include <sstream>
#include <iostream>

namespace modules::com_api {
    IOCode Port::Open() {
        if (this->attr_is_available != PortStatus::PORT_AVAILABLE) {
            return IOCode::ERROR_INVALID_PORT;
        }

        return this->hCom != nullptr ? IOCode::QUERY_SUCCESS : IOCode::ERROR_INVALID_VALUE;
    }

    IOCode Port::QueryPort() {
        std::stringstream portName("");

        portName << COM_API_SYSTEM_PREFIX << this->attr_system_id;

        this->attr_system_name = portName.str();

        // tood change function 
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
            return IOCode::ERROR_INVALID_PORT;
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

                port.Close();
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
        PortInfo portInfo(tmpPort);

        // Открытие порта для получения информации
        IOCode status = tmpPort.QueryPort(); 

        if (status != IOCode::QUERY_SUCCESS) {
            tmpPort.Close();
            return portInfo;
        }

        COMSTAT comStat;
        DWORD errors;

        WINBOOL getCommError = ClearCommError(tmpPort.hCom, &errors, &comStat);

        DCB dcbSerialParams = { 0 };
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        
        WINBOOL getCommState = GetCommState(tmpPort.hCom, &dcbSerialParams);

        if (infoType == QueryInfoType::SHORTLY || infoType == QueryInfoType::FULLY) {   
            
            if (getCommError) {
                portInfo._shortly_errors_errors = errors;
            }

            if (getCommState) { 
                portInfo._shortly_state_baudRate  = dcbSerialParams.BaudRate;
                portInfo._shortly_state_byteSize  = dcbSerialParams.ByteSize;
                portInfo._shortly_state_stopBits  = dcbSerialParams.StopBits;
                portInfo._shortly_state_parity    = dcbSerialParams.Parity;
            }
        }

        if (infoType == QueryInfoType::FULLY) {
            if (getCommError) {
                portInfo._fully_errors_cbInQue     = comStat.cbInQue;
                portInfo._fully_errors_cbOutQue    = comStat.cbOutQue;
            }

            COMMTIMEOUTS timeouts;
            if (GetCommTimeouts(tmpPort.hCom, &timeouts)) {
                portInfo._fully_timeouts_readIntervalTimeout            = timeouts.ReadIntervalTimeout;
                portInfo._fully_timeouts_readTotalTimeoutMultiplier     = timeouts.ReadTotalTimeoutMultiplier;
                portInfo._fully_timeouts_readTotalTimeoutConstant       = timeouts.ReadTotalTimeoutConstant;
                portInfo._fully_timeouts_writeTotalTimeoutMultiplier    = timeouts.WriteTotalTimeoutMultiplier;
                portInfo._fully_timeouts_writeTotalTimeoutConstant      = timeouts.WriteTotalTimeoutConstant;
            }
        }

        tmpPort.Close();
        
        portInfo._qtype = infoType;
        return portInfo;
    }
}