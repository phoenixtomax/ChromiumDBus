#ifndef DBUS_SCOPED_DBUS_ERROR_H_
#define DBUS_SCOPED_DBUS_ERROR_H_

#include <dbus/dbus.h>

namespace DBus {

class ScopedDBusError {
public:
  ScopedDBusError();
  ~ScopedDBusError();

  DBusError* get() { return &error_; }
  bool is_set() const;
  const char* name() { return error_.name; }
  const char* message() { return error_.message; }

private:
  DBusError error_;
};

} // namespace DBus

#endif //DBUS_SCOPED_DBUS_ERROR_H_