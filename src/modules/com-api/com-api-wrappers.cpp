#include "com-api.hpp"

#ifdef COM_API_HPP

namespace modules::com_api::wr {

auto stringToLpFileName(port_name_t name) -> LPCSTR { 
  return name.c_str(); 
} 

DWORD dwDesireAccess(COM_DW accessRight) { return accessRight; }

DWORD dwShareAccess(COM_DW sharingAccess) { return sharingAccess; }

DWORD dwCreationDisposition(COM_DW creationDisposition) {
  return creationDisposition;
}

DWORD dwFlagsAndAttributes(COM_DW fileFlags) { return fileFlags; }
}  // namespace modules::com_api::wr

#endif  // COM_API_HPP