#include "ObjectProxy.h"

#include <iostream>

namespace DBus {

ObjectProxy::ObjectProxy(Bus* bus,
                         const std::string& service_name,
                         const std::string& object_path)
    : bus_(bus),
      service_name_(service_name),
      object_path_(object_path) {

}

void ObjectProxy::CallMethod(MethodCall* method_call) {
  if (!bus_->Connect() ||
      !method_call->SetDestination(service_name_) ||
      !method_call->SetPath(object_path_)) {
    return;
  }

  DBusMessage* request_message = method_call->raw_message();
  bus_->Send(request_message, NULL);
}

}