#ifndef DBUS_BUS_H_
#define DBUS_BUS_H_

#include <stdint.h>
#include <string>
#include <set>
#include <dbus/dbus.h>

namespace DBus {

class Bus {
public:
  enum BusType {
    SESSION = DBUS_BUS_SESSION,
    SYSTEM  = DBUS_BUS_SYSTEM,
    CUSTOM_ADDRESS,
  };

  typedef struct Options {
    Options();
    ~Options();

    BusType bus_type;
    //ConnectionType connection_type;

    //std::string address;
  }Options;

  enum ServiceOwnershipOptions {
    REQUIRE_PRIMARY = (DBUS_NAME_FLAG_DO_NOT_QUEUE |
                       DBUS_NAME_FLAG_REPLACE_EXISTING),
    REQUIRE_PRIMARY_ALLOW_REPLACEMENT = (REQUIRE_PRIMARY |
                       DBUS_NAME_FLAG_ALLOW_REPLACEMENT),
  };

  enum {
    TIMEOUT_USE_DEFAULT = -1,
    TIMEOUT_INFINITE = 0x7fffffff,
  };

  explicit Bus(const Options& options);
  bool Connect();

  void Send(DBusMessage* request, uint32_t* serial);
/*
  ObjectProxy* GetObjectProxy(const std::string& service_name,
                              const std::string& object_path);
*/
  void RequestOwnership(const std::string& service_name,
                        ServiceOwnershipOptions options);
  bool TryRegisterObjectPath(const std::string& object_path,
                             const DBusObjectPathVTable* vtable,
                             void* user_data,
                             DBusError* error);

  void UnregisterObjectPath(const std::string& object_path);
  DBusConnection* connection_;
private:
  // TODO:Management of multiple ObjectProxies
  const BusType bus_type_;
  //shared_ptr<ObjectProxy> object_proxy_;

  std::set<std::string> registered_object_paths_;
  std::set<std::string> owned_service_names_;
};

} // namespace DBus

#endif // DBUS_BUS_H_