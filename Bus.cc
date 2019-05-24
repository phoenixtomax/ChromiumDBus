#include "scoped_dbus_error.h"

#include "Bus.h"

#include <assert.h>
#include <iostream>
#include <memory>

namespace DBus {

Bus::Options::Options():bus_type(SESSION) {}
Bus::Options::~Options() {}

Bus::Bus(const Options& options): bus_type_(options.bus_type),
                                  connection_(NULL) {
}

bool Bus::Connect() {
  if (connection_)
    return true;

  ScopedDBusError error;
  if (bus_type_ == CUSTOM_ADDRESS) {
    //TODO:
  } else {
    const DBusBusType dbus_bus_type = static_cast<DBusBusType>(bus_type_);
    connection_ = dbus_bus_get(dbus_bus_type, error.get());
  }

  if (!connection_ && bus_type_ != CUSTOM_ADDRESS) {
    std::cout << "Failed to connect to the bus: "
              << (error.is_set() ? error.message() : "");
    return false;
  }

  return true;
}

/*
ObjectProxy* Bus::GetObjectProxy(const std::string& service_name,
                         const std::string& object_path) {
  if (object_proxy_.get())
    return object_proxy_.get();

  object_proxy_ = std::make_shared<ObjectProxy>(this, service_name, object_path);

  return object_proxy_.get();
}
*/

void Bus::Send(DBusMessage* request, uint32_t* serial) {
  const bool success = dbus_connection_send(connection_, request, serial);
  if (!success) {
    std::cout << "Unable to allocate memory." << std::endl;
  }
}

void Bus::RequestOwnership(const std::string& service_name,
                           ServiceOwnershipOptions options) {
  bool success = Connect();
  if (success) {
    if (owned_service_names_.find(service_name) != owned_service_names_.end()) {
      std::cout << "The service " << service_name <<" has been already requested." << std::endl;
      return;
    }

    ScopedDBusError error;
    const int result = dbus_bus_request_name(connection_,
                                             service_name.c_str(),
                                             options,
                                             error.get());
    if (result != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
      std::cout << "Failed to get the ownedship of " << service_name << ": "
                << (error.is_set() ? error.message() : "");
      return;
    }

    owned_service_names_.insert(service_name);
  }
}

bool Bus::TryRegisterObjectPath(const std::string& object_path,
                           const DBusObjectPathVTable* vtable,
                           void* user_data,
                           DBusError* error) {
  assert(connection_);
  assert(vtable);
  assert(user_data);
  assert(error);
  if (registered_object_paths_.find(object_path) != registered_object_paths_.end()) {
    std::cout << "Object path already registered: " << object_path;
    return false;
  }

  //dbus_bus_request_name(connection_, "org.chromium.TestObject", 0, error);
  const bool success = dbus_connection_try_register_object_path(
      connection_,
      object_path.c_str(),
      vtable,
      user_data,
      error);

  if (success)
    registered_object_paths_.insert(object_path);

  return success;
}

void Bus::UnregisterObjectPath(const std::string& object_path) {
  if (registered_object_paths_.find(object_path) == registered_object_paths_.end()) {
    std::cout << "Requested to unregister an unknown object path: " << object_path;
    return;
  }

  const bool success = dbus_connection_unregister_object_path(connection_, object_path.c_str());

  registered_object_paths_.erase(object_path);
}

} // namespace DBus
