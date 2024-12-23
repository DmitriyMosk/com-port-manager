#include "com-api.hpp"

#ifdef MODULES_COM_API_HPP

namespace modules::com_api::wr {
LPCSTR stringToLpFileName(port_name_t name) { return name.c_str(); }

DWORD dwDesireAccess(COM_DW accessRight) { return accessRight; }

DWORD dwShareAccess(COM_DW sharingAccess) { return sharingAccess; }

DWORD dwCreationDisposition(COM_DW creationDisposition) {
  return creationDisposition;
}

DWORD dwFlagsAndAttributes(COM_DW fileFlags) { return fileFlags; }
}  // namespace modules::com_api::wr

#endif  // MODULES_COM_API_HPP