CC = g++
CXX = g++
CFLAGS = -g -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include/ -std=c++11
CXXFLAGS = -g -I/usr/include/dbus-1.0 -I/usr/lib/x86_64-linux-gnu/dbus-1.0/include/ --std=c++11
LDFLAGS = -g `pkg-config --cflags --libs dbus-1`
TARGET1 = ExportedObjectTest
TARGET2 = ObjectProxyTest
SRC = Bus.cc Message.cc exported_object.cc util.cc scoped_dbus_error.cc ObjectProxy.cc
OBJ = $(patsubst %.cc,%.o,$(SRC))

ALL:$(TARGET1) $(TARGET2)

$(TARGET1):$(OBJ)
	$(CC) -o ExportedObjectTest $(OBJ) ExportedObjectTest.cc $(CFLAGS) $(LDFLAGS)

$(TARGET2):$(OBJ)
	$(CC) -o ObjectProxyTest $(OBJ) ObjectProxyTest.cc $(CFLAGS) $(LDFLAGS)

#$(OBJ):$(SRC)
#	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	rm -rfv *.o ExportedObjectTest
