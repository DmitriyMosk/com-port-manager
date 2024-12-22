#include "com-api.hpp"

#define HW_ASYNC_MODE   0x00000001
#define HW_SYNC_MODE    0x00000000 

#ifndef HW_HPP
#define HW_HPP
    namespace HW {
        enum ConnectionType {
            SYNCHRONOUS     = HW_SYNC_MODE,
            ASYNCHRONOUS    = HW_ASYNC_MODE
        };

        void WirePort(const modules::com_api::Port &port, int baudRate, int mode); 

    }
#endif // HW_HPP