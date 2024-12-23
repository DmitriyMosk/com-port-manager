#include "com-api.hpp"

#define HW_ASYNC_MODE 0x00000001
#define HW_SYNC_MODE  0x00000000

#ifndef HW_HPP
#define HW_HPP
namespace HW {
  
constexpr size_t BUFFER_SIZE = 1024;

enum SupportedDevice {
  GENERIC_DEVICE,     // use for text. datatravaling
  MICROPHONE_DEVICE,  // use for receive audio data
  SPEAKER_DEVICE,     // use for transmit audio data
};

enum ConnectionType {
  SYNCHRONOUS = HW_SYNC_MODE,
  ASYNCHRONOUS = HW_ASYNC_MODE
};

struct Wire {
  SupportedDevice device;
  ConnectionType connectionType;
  com_api::Port port;
  int baudRate;
};

auto WirePort(const modules::com_api::Port&, int, int) -> Wire;

void _microphone_wire_support(const modules::com_api::Port&, int, int);
void _generic_dr_wire_support(const modules::com_api::Port&, int, int);
void _speaker_dr_wire_support(const modules::com_api::Port&, int, int);
};  // namespace HW
#endif  // HW_HPP