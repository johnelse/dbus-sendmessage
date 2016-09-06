sendmessage:
	g++ sendmessage.cpp -o $@ $(shell pkg-config --cflags --libs dbus-1)

clean:
	rm -f sendmessage
