#ifndef DBUSMANAGER_H
#define DBUSMANAGER_H

#include <sdbus-c++/sdbus-c++.h>

#define DBUS_SERVICE_NAME   "sgy.pine.screenLock"
#define DBUS_OBJECT_PATH    "/sgy/pine/screenLock"
#define DBUS_INTERFACE_NAME "sgy.pine.screenLock"

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
