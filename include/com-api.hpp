#include <windows.h>
#include <string>
#include <stdexcept>
#include <tchar.h>
#include <vector>

#ifndef MODULES_COM_API_HPP
#define MODULES_COM_API_HPP
    #define COM_API_SYSTEM_PREFIX "\\\\.\\COM"

    enum IOCode {
        ERROR_INVALID_VALUE         = static_cast<intptr_t>(-1),  // INVALID_HANDLE_VALUE is -1
        ERROR_INVALID_PORT          = ERROR_FILE_NOT_FOUND,
        ERROR_PORT_ACCESS_DENIED    = ERROR_ACCESS_DENIED,
        ERROR_INVALID_PARAM         = ERROR_INVALID_PARAMETER,
        QUERY_SUCCESS               = 0
    };

    enum PortStatus {
        PORT_AVAILABLE,
        PORT_NOT_AVAILABLE 
    };
    
    // Generic access rights (wrapper)
    #define COM_API_GENERIC_RWRD    (GENERIC_READ | GENERIC_WRITE)
    #define COM_API_GENERIC_R       (GENERIC_READ)
    #define COM_API_GENERIC_W       (GENERIC_WRITE)

    // for port information
    enum QueryInfoType {
        SHORTLY = 0,
        FULLY = 1
    };

    // Forward declaration
    typedef std::string     port_name_t;  
    typedef DWORD           COM_DW;

    namespace modules::com_api {
        namespace wr { 
            LPCSTR stringToLpFileName(port_name_t); 

            DWORD dwDesireAccess(COM_DW); 
            DWORD dwShareAccess(COM_DW);
            DWORD dwCreationDisposition(COM_DW);
            DWORD dwFlagsAndAttributes(COM_DW);
        }

        struct Port {
            int         attr_system_id;
            int         attr_baud_rate;
            PortStatus  attr_is_available; 

            COM_DW      attr_desired_access;
            COM_DW      attr_sharing_access; 
            COM_DW      attr_flags_creation;
            COM_DW      attr_file_flags;

            HANDLE      hCom;

            port_name_t attr_friendly_name;
            port_name_t attr_system_name;
            //

            Port(int id) : 
                attr_system_id      (0), 
                attr_baud_rate      (0),
                attr_desired_access (0),
                attr_sharing_access (0),
                attr_flags_creation (0),
                attr_file_flags     (0),
                attr_is_available   (PortStatus::PORT_NOT_AVAILABLE), 
                attr_system_name    (""),
                attr_friendly_name  (""),
                // hCom - handle to the port
                hCom                (nullptr)
            {
                attr_system_id = id;
            }

            ~Port() {
                if (hCom != nullptr) {
                    CloseHandle(hCom);
                }
            }

            // Queries the hardware for port information and updates the port attributes.
            IOCode      QueryPort();
            // Open the port handle
            IOCode      Open();
            // Closes the port handle
            IOCode      Close(); 
            // Returns the port status
            PortStatus  CheckPort();
            // Returns the system ID of the port
            int GetSystemID(); 
        };

        // TODO написать wrapper для работы с COM-портом
        struct PortInfo { 
            Port port; 

            // Коды ошибок, возникающие при работе с COM-портом
            DWORD _shortly_errors;

            // Тип контроля четности (например, четный, нечетный или отсутствие контроля)
            BYTE _shortly_parity;

            // Количество стоп-битов, используемых для завершения передачи байта данных
            BYTE _shortly_stopBits;

            // Количество бит в одном байте данных
            BYTE _shortly_byteSize;

            // Скорость передачи данных в бодах (битах в секунду)
            DWORD _shortly_baudRate;

            // Количество байтов, ожидающих обработки во входной очереди COM-порта
            DWORD _fully_cbInQue;

            // Количество байтов, ожидающих отправки в выходной очереди COM-порта
            DWORD _fully_cbOutQue;

            // Максимальное время ожидания между двумя последовательными байтами данных при чтении
            DWORD _fully_readIntervalTimeout;

            // Множитель для общего таймаута при чтении данных
            DWORD _fully_readTotalTimeoutMultiplier;

            // Константа для общего таймаута при чтении данных
            DWORD _fully_readTotalTimeoutConstant;

            // Множитель для общего таймаута при записи данных
            DWORD _fully_writeTotalTimeoutMultiplier;

            // Константа для общего таймаута при записи данных
            DWORD _fully_writeTotalTimeoutConstant;

            PortInfo(const Port& p) : 
                port(p), 
                _shortly_errors(0), 
                _fully_cbInQue(0), 
                _fully_cbOutQue(0), 
                _shortly_baudRate(0),  
                _shortly_byteSize(0), 
                _shortly_parity(0), 
                _shortly_stopBits(0),
                _fully_readIntervalTimeout(0), 
                _fully_readTotalTimeoutMultiplier(0), 
                _fully_readTotalTimeoutConstant(0),
                _fully_writeTotalTimeoutMultiplier(0), 
                _fully_writeTotalTimeoutConstant(0) 
            {}

            ~PortInfo() {}
        };

        typedef std::vector<Port> PortCollection;

        // Scans and returns a list of available COM ports.
        PortCollection  ScanPorts();        

        Port            QueryPortById(const PortCollection&, int);
        PortInfo        QueryPortInfo(const Port&, QueryInfoType);
        
        // HANDLE openPort(const std::string& portName, DWORD baudRate);
        
        // void closePort(HANDLE hCom);

        // bool readFromPort(HANDLE hCom, char* buffer, DWORD bufferSize, DWORD& bytesRead);

        // bool writeToPort(HANDLE hCom, const char* buffer, DWORD bufferSize, DWORD& bytesWritten);
    }

#endif 