#include "Message.h"

#include <assert.h>

namespace DBus {

Message::Message()
    : raw_message_(NULL) {
}

Message::~Message() {
  if (raw_message_)
    dbus_message_unref(raw_message_);
}

void Message::Init(DBusMessage* raw_message) {
  assert(raw_message);
  raw_message_ = raw_message;
}

Message::MessageType Message::GetMessageType() {
  if(!raw_message_)
    return MESSAGE_INVALID;
  const int type = dbus_message_get_type(raw_message_);
  return static_cast<Message::MessageType>(type);
}

std::string Message::GetMessageTypeAsString() {
  switch (GetMessageType()) {
    case MESSAGE_INVALID:
      return "MESSAGE_INVALID";
    case MESSAGE_METHOD_CALL:
      return "MESSAGE_METHOD_CALL";
    case MESSAGE_METHOD_RETURN:
      return "MESSAGE_METHOD_RETURN";
    case MESSAGE_SIGNAL:
      return "MESSAGE_SIGNAL";
    case MESSAGE_ERROR:
      return "MESSAGE_ERROR";
  }
  return std::string();
}

bool Message::SetDestination(const std::string& destination) {
  return dbus_message_set_destination(raw_message_, destination.c_str());
}

bool Message::SetPath(const std::string& path) {
  return dbus_message_set_path(raw_message_, path.c_str());
}

bool Message::SetInterface(const std::string& interface) {
  return dbus_message_set_interface(raw_message_, interface.c_str());
}

bool Message::SetMember(const std::string& member) {
  return dbus_message_set_member(raw_message_, member.c_str());
}

void Message::SetSerial(uint32_t serial) {
  dbus_message_set_serial(raw_message_, serial);
}

void Message::SetReplySerial(uint32_t reply_serial) {
  dbus_message_set_reply_serial(raw_message_, reply_serial);
}

std::string Message::GetDestination() {
  const char* destination = dbus_message_get_destination(raw_message_);
  return destination ? destination : "";
}

std::string Message::GetPath() {
  const char* path = dbus_message_get_path(raw_message_);
  return path ? path : "";
}

std::string Message::GetInterface() {
  const char* interface = dbus_message_get_interface(raw_message_);
  return interface ? interface : "";
}

std::string Message::GetMember() {
  const char* member = dbus_message_get_member(raw_message_);
  return member ? member : "";
}

std::string Message::GetErrorName() {
  const char* error_name = dbus_message_get_error_name(raw_message_);
  return error_name ? error_name : "";
}

std::string Message::GetSender() {
  const char* sender = dbus_message_get_sender(raw_message_);
  return sender ? sender : "";
}

std::string Message::GetSignature() {
  const char* signature = dbus_message_get_signature(raw_message_);
  return signature ? signature : "";
}

uint32_t Message::GetSerial() {
  return dbus_message_get_serial(raw_message_);
}

uint32_t Message::GetReplySerial() {
  return dbus_message_get_reply_serial(raw_message_);
}

MethodCall::MethodCall(const std::string& interface_name,
                       const std::string& method_name)
      :Message() {
  Init(dbus_message_new(DBUS_MESSAGE_TYPE_METHOD_CALL));

  assert(SetInterface(interface_name));
  assert(SetMember(method_name));
}

MethodCall::MethodCall(): Message() {
}

MethodCall* MethodCall::FromRawMessage(DBusMessage* raw_message) {
  assert(DBUS_MESSAGE_TYPE_METHOD_CALL == dbus_message_get_type(raw_message));
  MethodCall* method_call = new MethodCall;
  method_call->Init(raw_message);

  return method_call;
}

} // namespace DBus