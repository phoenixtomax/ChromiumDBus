#ifndef DBUS_EXPORTED_OBJECT_H_
#define DBUS_EXPORTED_OBJECT_H_

#include "Bus.h"

#include "Message.h"

#include <dbus/dbus.h>
#include <functional>
#include <memory>
#include <map>
#include <string>

namespace DBus {

class ExportedObject {
public:
  //typedef std::function<void(MethodCall* method_call, ResponseSender sender)> MethodCallCallback;
  typedef std::function<void(MethodCall* method_call)> MethodCallCallback;

  ExportedObject(Bus* bus, const std::string& object_path);
  ~ExportedObject();

  bool Register();

  bool ExportMethodAndBlock(const std::string& interface_name,
                            const std::string& method_name,
                            MethodCallCallback method_call_callback);

private:
  typedef std::map<std::string, MethodCallCallback> MethodTable;

  static DBusHandlerResult HandleMessageThunk(DBusConnection* connection,
                                             DBusMessage* raw_message,
                                             void* user_data);
  static void OnUnregisteredThunk(DBusConnection* connection,
                                  void* user_data);
  DBusHandlerResult HandleMessage(DBusConnection* connection,
                                  DBusMessage* raw_message);
  void OnUnregistered(DBusConnection* connection);

  std::shared_ptr<Bus> bus_;
  std::string object_path_;
  bool object_is_registered_;

  MethodTable method_table_;
};

} // namespace DBus

#endif // DBUS_EXPORTED_OBJECT_H_