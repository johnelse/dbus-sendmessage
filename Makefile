sendmessage:
	g++ sendmessage.c -o $@ $(shell pkg-config --cflags --libs dbus-1)

clean:
	rm -f sendmessage
