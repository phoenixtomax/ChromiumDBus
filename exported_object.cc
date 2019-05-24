#include "exported_object.h"

#include "assert.h"
#include "scoped_dbus_error.h"
#include "util.h"

#include <iostream>

namespace DBus {

ExportedObject::ExportedObject(Bus* bus, const std::string& object_path):
    bus_(bus),
    object_path_(object_path),
    object_is_registered_(false) {
}

ExportedObject::~ExportedObject() {

}

bool ExportedObject::Register() {
  assert(bus_);
  if (object_is_registered_)
    return true;

  ScopedDBusError error;

  DBusObjectPathVTable vtable = {};
  vtable.message_function = &ExportedObject::HandleMessageThunk;
  vtable.unregister_function = &ExportedObject::OnUnregisteredThunk;
  const bool success = bus_->TryRegisterObjectPath(object_path_,
                                                   &vtable,
                                                   this,
                                                   error.get());
  if (!success) {
    std::cout << "Failed to register the object: " << object_path_
              << ": " << (error.is_set() ? error.message() : "");
    return false;
  }

  object_is_registered_ = true;
  return true;
}

DBusHandlerResult ExportedObject::HandleMessageThunk(DBusConnection* connection,
                                                     DBusMessage* raw_message,
                                                     void* user_data) {
  ExportedObject* self = reinterpret_cast<ExportedObject*>(user_data);
  return self->HandleMessage(connection, raw_message);
}

void ExportedObject::OnUnregisteredThunk(DBusConnection* connection,
                                         void* user_data) {
  ExportedObject* self = reinterpret_cast<ExportedObject*>(user_data);
  return self->OnUnregistered(connection);
}

void ExportedObject::OnUnregistered(DBusConnection* connection) {

}

DBusHandlerResult ExportedObject::HandleMessage(DBusConnection* connection,
                                DBusMessage* raw_message) {
  dbus_message_ref(raw_message);
  std::shared_ptr<MethodCall> method_call(MethodCall::FromRawMessage(raw_message));
  const std::string interface = method_call->GetInterface();
  const std::string member = method_call->GetMember();

  if (interface.empty()) {
    std::cout << "Interface is missing: ";//<< method_call->ToString();
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  const std::string absolute_method_name = GetAbsoluteMemberName(interface, member);
  MethodTable::const_iterator iter = method_table_.find(absolute_method_name);
  if (iter == method_table_.end()) {
    std::cout << "Unknown method: ";//<< method_call->ToString();
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
  }

  // Synchronous and no response.
  MethodCall* method = method_call.get();
  iter->second(method);

  return DBUS_HANDLER_RESULT_HANDLED;
}

bool ExportedObject::ExportMethodAndBlock(const std::string& interface_name,
                                          const std::string& method_name,
                                          MethodCallCallback method_call_callback) {
  const std::string absolute_method_name = GetAbsoluteMemberName(interface_name, method_name);
  //MethodTable::const_iterator iter = ;

  if (method_table_.find(absolute_method_name) != method_table_.end()) {
    std::cout << absolute_method_name << " is already exported.";
    return false;
  }

  if (!bus_->Connect())
    return false;
  if (!Register())
    return false;

  method_table_[absolute_method_name] = method_call_callback;

  return true;
}

} // namespace DBus