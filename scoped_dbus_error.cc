#include "scoped_dbus_error.h"

namespace DBus {

ScopedDBusError::ScopedDBusError() {
  dbus_error_init(&error_);
}

ScopedDBusError::~ScopedDBusError() {
  dbus_error_free(&error_);
}

bool ScopedDBusError::is_set() const {
  return dbus_error_is_set(&error_);
}

} // namespace DBus