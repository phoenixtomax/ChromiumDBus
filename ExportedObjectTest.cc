#include "Bus.h"
#include "exported_object.h"
#include "unistd.h"

#include <iostream>

static void TestMethod(DBus::MethodCall* method_call) {
  std::cout << "This is TestMethod of DBus." << std::endl;
}

int main() {
  DBus::Bus::Options option;
  option.bus_type = DBus::Bus::SESSION;

  // Set up bus
  DBus::Bus bus(option);
  bus.Connect();

  bus.RequestOwnership("org.DLNAMediaPlayer.TestObject", DBus::Bus::ServiceOwnershipOptions::REQUIRE_PRIMARY);

  // Set up exported object
  DBus::ExportedObject exported_object(&bus, "/org/DLNAMediaPlayer/TestObject");
  exported_object.Register();

  DBus::ExportedObject::MethodCallCallback method_cb = &TestMethod;
  exported_object.ExportMethodAndBlock("org.DLNAMediaPlayer.TestObject", "TestMethod", method_cb);

  while(1) {
    dbus_connection_read_write_dispatch(bus.connection_, 1000);
  }
  return 0;
}