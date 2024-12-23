#ifndef COM_API_HPP
#define COM_API_HPP

#include <windows.h>

#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

constexpr const char* COM_API_SYSTEM_PREFIX = "\\\\.\\COM";

enum IOCode {
  ERROR_INVALID_VALUE =
      static_cast<intptr_t>(-1),  // INVALID_HANDLE_VALUE is -1
  ERROR_INVALID_PORT = ERROR_FILE_NOT_FOUND,
  ERROR_PORT_ACCESS_DENIED = ERROR_ACCESS_DENIED,
  ERROR_INVALID_PARAM = ERROR_INVALID_PARAMETER,
  QUERY_SUCCESS = 0
};

enum PortStatus : std::uint8_t { PORT_AVAILABLE, PORT_NOT_AVAILABLE };

// Generic access rights (wrapper)
#define COM_API_GENERIC_RW  (GENERIC_READ | GENERIC_WRITE)
#define COM_API_GENERIC_R   (GENERIC_READ)
#define COM_API_GENERIC_W   (GENERIC_WRITE)

// for port information
enum QueryInfoType { SHORTLY = 0, FULLY = 1 }; 

// Forward declaration
typedef std::string port_name_t;
typedef DWORD COM_DW;

namespace modules::com_api {
namespace wr {
auto stringToLpFileName(port_name_t portName)   -> LPCSTR;
auto dwDesireAccess(COM_DW access)              -> DWORD;
auto dwShareAccess(COM_DW access)               -> DWORD;
auto dwCreationDisposition(COM_DW disposition)  -> DWORD;
auto dwFlagsAndAttributes(COM_DW attributes)    -> DWORD;
}  // namespace wr 

struct Port {
  int attr_system_id;
  int attr_baud_rate;
  PortStatus attr_is_available;

  COM_DW attr_desired_access;
  COM_DW attr_sharing_access;
  COM_DW attr_flags_creation;
  COM_DW attr_file_flags;

  HANDLE hCom;

  port_name_t attr_friendly_name;
  port_name_t attr_system_name;
  //

  Port(int id)
      : attr_system_id(id), 
        attr_baud_rate(0),
        attr_desired_access(0),
        attr_sharing_access(0),
        attr_flags_creation(0),
        attr_file_flags(0),
        attr_is_available(PortStatus::PORT_NOT_AVAILABLE),
        attr_system_name(""),
        attr_friendly_name(""),
        // hCom - handle to the port
        hCom(nullptr) {}

  Port(const Port &p)
      : attr_system_id(p.attr_system_id), 
        attr_baud_rate(p.attr_baud_rate),
        attr_desired_access(p.attr_desired_access),
        attr_sharing_access(p.attr_sharing_access),
        attr_flags_creation(p.attr_flags_creation),
        attr_file_flags(p.attr_file_flags),
        attr_is_available(p.attr_is_available),
        attr_system_name(p.attr_system_name),
        attr_friendly_name(p.attr_friendly_name),
        // hCom - handle to the port
        hCom(p.hCom) {}


  ~Port() {
    if (hCom != nullptr) {
      CloseHandle(hCom);
    }
  }

  // Queries the hardware for port information and updates the port attributes.
  auto QueryPort() -> IOCode;
  // Open the port handle
  auto Open() -> IOCode;
  // Closes the port handle
  auto Close() -> IOCode;
  // Returns the port status
  auto CheckPort() -> PortStatus;
  // Returns the system ID of the port
  auto GetSystemID() -> int;
};;

// TODO написать wrapper для работы с COM-портом
struct PortInfo {
  Port port;

  QueryInfoType _qtype;

  // Коды ошибок, возникающие при работе с COM-портом
  DWORD _shortly_errors_errors;

  // Тип контроля четности (например, четный, нечетный или отсутствие контроля)
  BYTE _shortly_state_parity;

  // Количество стоп-битов, используемых для завершения передачи байта данных
  BYTE _shortly_state_stopBits;

  // Количество бит в одном байте данных
  BYTE _shortly_state_byteSize;

  // Скорость передачи данных в бодах (битах в секунду)
  DWORD _shortly_state_baudRate;

  // Количество байтов, ожидающих обработки во входной очереди COM-порта
  DWORD _fully_errors_cbInQue;

  // Количество байтов, ожидающих отправки в выходной очереди COM-порта
  DWORD _fully_errors_cbOutQue;

  // Максимальное время ожидания между двумя последовательными байтами данных
  // при чтении
  DWORD _fully_timeouts_readIntervalTimeout;

  // Множитель timeouts_для общего таймаута при чтении данных
  DWORD _fully_timeouts_readTotalTimeoutMultiplier;

  // Константа timeouts_для общего таймаута при чтении данных
  DWORD _fully_timeouts_readTotalTimeoutConstant;

  // Множитель timeouts_для общего таймаута при записи данных
  DWORD _fully_timeouts_writeTotalTimeoutMultiplier;

  // Константа timeouts_для общего таймаута при записи данных
  DWORD _fully_timeouts_writeTotalTimeoutConstant;

  PortInfo(const Port& p)
      : port(p),
        _shortly_errors_errors(0),
        _fully_errors_cbInQue(0),
        _fully_errors_cbOutQue(0),
        _shortly_state_baudRate(0),
        _shortly_state_byteSize(0),
        _shortly_state_parity(0),
        _shortly_state_stopBits(0),
        _fully_timeouts_readIntervalTimeout(0),
        _fully_timeouts_readTotalTimeoutMultiplier(0),
        _fully_timeouts_readTotalTimeoutConstant(0),
        _fully_timeouts_writeTotalTimeoutMultiplier(0),
        _fully_timeouts_writeTotalTimeoutConstant(0) {}

  ~PortInfo() {}
};

typedef std::vector<Port> PortCollection;

// Scans and returns a list of available COM ports.
PortCollection ScanPorts();

Port QueryPortById(const PortCollection&, int);
PortInfo QueryPortInfo(const Port&, QueryInfoType);

// bool readFromPort(HANDLE hCom, char* buffer, DWORD bufferSize, DWORD&
// bytesRead);

// bool writeToPort(HANDLE hCom, const char* buffer, DWORD bufferSize, DWORD&
// bytesWritten);
}  // namespace modules::com_api

#endif