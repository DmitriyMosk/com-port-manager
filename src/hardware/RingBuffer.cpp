#include "hardware.hpp"

namespace HW {

bool RingBuffer::write(const uint8_t* src, size_t len) {
    if (count.load() + len > data.size()) return false;
    
    for (size_t i = 0; i < len; ++i) {
        data[write_pos] = src[i];
        write_pos = (write_pos + 1) % data.size();
        count++;
    }
    return true;
}

bool RingBuffer::read(uint8_t* dst, size_t len) {
    if (count.load() < len) return false;
    
    for (size_t i = 0; i < len; ++i) {
        dst[i] = data[read_pos];
        read_pos = (read_pos + 1) % data.size();
        count--;
    }
    return true;
}

auto WirePort(const modules::com_api::Port& port, int baudRate, int mode) -> Wire {
    Wire wire;
    wire.port = port;
    wire.baudRate = baudRate;
    wire.connectionType = static_cast<ConnectionType>(mode);

    switch(wire.device) {
        case MICROPHONE_DEVICE:
            _microphone_wire_support(port, baudRate, mode);
            break;
        case SPEAKER_DEVICE:
            _speaker_dr_wire_support(port, baudRate, mode);
            break;
        default:
            _generic_dr_wire_support(port, baudRate, mode);
            break;
    }

    return wire;
}

}