#include "Bus.h"
#include "Message.h"

#include <dbus/dbus.h>
#include <memory>

namespace DBus {

class ObjectProxy {
public:
  ObjectProxy(Bus* bus, const std::string& service_name, const std::string& object_path);

  //typedef std::function<void(ObjectProxy&, Response*)> ResponseCallback;
  //typedef std::function<void(ObjectProxy&, ErrorResponse*)> ErrorCallback;

  void CallMethod(MethodCall* method_call);
private:

  std::shared_ptr<Bus> bus_;
  std::string service_name_;
  std::string object_path_;

  ObjectProxy(const ObjectProxy&);
  void operator=(const ObjectProxy&);
};

} // namespace DBus