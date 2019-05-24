#ifndef DBUS_UTIL_H_
#define DBUS_UTIL_H_

#include <string>

namespace DBus {

std::string GetAbsoluteMemberName(const std::string& interface_name,
                                  const std::string& member_name);

} // namespace DBus

#endif