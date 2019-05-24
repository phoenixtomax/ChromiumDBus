#include "util.h"

namespace DBus {

std::string GetAbsoluteMemberName(const std::string& interface_name,
                                  const std::string& member_name) {
  return interface_name + "." + member_name;
}

} // namespace DBus