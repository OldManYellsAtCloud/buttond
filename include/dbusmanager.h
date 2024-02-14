#ifndef DBUSMANAGER_H
#define DBUSMANAGER_H

#include <sdbus-c++/sdbus-c++.h>

#define DBUS_SERVICE_NAME   "sgy.pine.buttond"
#define DBUS_OBJECT_PATH    "/sgy/pine/buttond"
#define DBUS_INTERFACE_NAME "sgy.pine.buttond"

class DbusManager
{
private:
    std::unique_ptr<sdbus::IObject> dbusObject;
    std::unique_ptr<sdbus::IConnection> dbusConnection;
public:
    DbusManager();
    ~DbusManager();
    void sendSignal(bool state);

};

#endif // DBUSMANAGER_H
