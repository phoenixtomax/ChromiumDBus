#include "Bus.h"
#include "ObjectProxy.h"
#include "unistd.h"

#include <iostream>

int main() {
  DBus::Bus::Options option;
  option.bus_type = DBus::Bus::SESSION;

  // Set up bus
  DBus::Bus bus(option);
  bus.Connect();

  bus.RequestOwnership("org.DLNAMediaPlayer.TestObject", DBus::Bus::ServiceOwnershipOptions::REQUIRE_PRIMARY);

  // Set up object proxy
  DBus::ObjectProxy object_proxy(&bus, "org.DLNAMediaPlayer.TestObject", "/org/DLNAMediaPlayer/TestObject");
  DBus::MethodCall method_call("org.DLNAMediaPlayer.TestObject", "TestMethod");
  object_proxy.CallMethod(&method_call);

  while(1) {
    dbus_connection_read_write_dispatch(bus.connection_, 1000);
  }
  return 0;
}