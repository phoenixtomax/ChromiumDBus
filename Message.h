#ifndef DBUS_MESSAGE_H_
#define DBUS_MESSAGE_H_

#include <dbus/dbus.h>

#include <stdint.h>
#include <string>
#include <memory>

namespace DBus {

class Message {
public:
  enum MessageType {
    MESSAGE_INVALID       = DBUS_MESSAGE_TYPE_INVALID,
    MESSAGE_METHOD_CALL   = DBUS_MESSAGE_TYPE_METHOD_CALL,
    MESSAGE_METHOD_RETURN = DBUS_MESSAGE_TYPE_METHOD_RETURN,
    MESSAGE_SIGNAL        = DBUS_MESSAGE_TYPE_SIGNAL,
    MESSAGE_ERROR         = DBUS_MESSAGE_TYPE_ERROR,
  };

  MessageType GetMessageType();

  std::string GetMessageTypeAsString();
  DBusMessage* raw_message() { return raw_message_; }

  bool SetDestination(const std::string& destination);
  bool SetPath(const std::string& path);
  bool SetInterface(const std::string& interface);
  bool SetMember(const std::string& member);
  bool SetErrorName(const std::string& error_name);
  bool SetSender(const std::string& sender);
  void SetSerial(uint32_t serial);
  void SetReplySerial(uint32_t reply_serial);

  std::string GetDestination();
  std::string GetPath();
  std::string GetInterface();
  std::string GetMember();
  std::string GetErrorName();
  std::string GetSender();
  std::string GetSignature();
  uint32_t GetSerial();
  uint32_t GetReplySerial();

protected:
  Message();
  virtual ~Message();

  void Init(DBusMessage* raw_message);

private:
  DBusMessage* raw_message_;
};

class MethodCall: public Message {
public:
  MethodCall(const std::string& interface,
             const std::string& method_name);

  static MethodCall* FromRawMessage(DBusMessage* raw_message);

private:
  MethodCall();

  //TODO: Is shared_ptr need copy constructor ?
  MethodCall(const MethodCall&);
  void operator=(const MethodCall&);
};

/*
class Response: public Message {
public:
  static std::shared_ptr<Response> FromRawMessage(DBusMessage* raw_message);
  static std::shared_ptr<Response> FromMethodCall(MethodCall* method_call);
  static std::shared_ptr<Response> CreateEmpty();

protected:
  Response();

  //TODO: Is shared_ptr need copy constructor ?
private:
  Response(const Response&);
  void operator=(const Response&);
};

class ErrorResponse: public Response {
public:
  static shared_ptr<ErrorResponse> FromRawMessage(DBusMessage* raw_message);
  static shared_ptr<ErrorResponse> FromMethodCall(MethodCall* method_call,
                                                  const std::string& error_name,
                                                  const std::string& error_message);
private:
  ErrorResponse();

  //TODO: Is shared_ptr need copy constructor ?
  ErrorResponse(const ErrorResponse&);
  void operator=(const ErrorResponse&);
};
*/

} // namespace DBus

#endif