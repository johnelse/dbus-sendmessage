#include <stdio.h>
#include <stdlib.h>

#include <dbus/dbus.h>

int main(int argc, char** argv)
{
    DBusError err;
    DBusConnection* conn;
    int ret;
    dbus_error_init(&err);

    conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
    if(dbus_error_is_set(&err))
    {
        fprintf(stderr, "Connection Error (%s)\n", err.message);
        dbus_error_free(&err);
    }
    if (NULL == conn)
    {
        exit(1);
    }

    DBusMessage* msg;
    DBusMessageIter args;
    DBusPendingCall* pending;

    msg = dbus_message_new_method_call("org.freedesktop.UDisks2",
            "/org/freedesktop/UDisks2/block_devices/sda",
            "org.freedesktop.DBus.Properties",
            "Get");
    if (NULL == msg)
    {
        fprintf(stderr, "Message Null\n");
        exit(1);
    }

    const char* param1 = "org.freedesktop.UDisks2.Block";
    const char* param2 = "Drive";

    // append arguments.
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &param1))
    {
        fprintf(stderr, "Out of memory!\n");
        exit(1);
    }
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &param2))
    {
        fprintf(stderr, "Out of memory!\n");
        exit(1);
    }

    // send message and get a handle for a reply
    if (!dbus_connection_send_with_reply(conn, msg, &pending, -1))
    {
        fprintf(stderr, "Out of memory!\n");
        exit(1);
    }
    if (NULL == pending)
    {
        fprintf(stderr, "Pending call null\n");
        exit(1);
    }

    dbus_connection_flush(conn);

    dbus_message_unref(msg);

    // get the reply
    char* stat;

    // block until we receive a reply
    dbus_pending_call_block(pending);

    // get the reply message
    msg = dbus_pending_call_steal_reply(pending);
    if (NULL == msg)
    {
        fprintf(stderr, "Reply Null\n");
        exit(1);
    }
    // free the pending message handle
    dbus_pending_call_unref(pending);

    // read the parameters
    if (!dbus_message_iter_init(msg, &args))
    fprintf(stderr, "Message has no arguments!\n");
    else if (DBUS_TYPE_VARIANT != dbus_message_iter_get_arg_type(&args))
    fprintf(stderr, "Argument is not a variant as a string!\n");
    else
    dbus_message_iter_get_basic(&args, &stat);

    printf("Got Reply: %s\n", stat);

    // free reply and close connection
    dbus_message_unref(msg);
}
