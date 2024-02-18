#ifndef DBUSMANAGER_H
#define DBUSMANAGER_H

#include <sdbus-c++/sdbus-c++.h>

#define DBUS_SERVICE_NAME   "org.gspine.button"
#define DBUS_OBJECT_PATH    "/org/gspine/button"
#define DBUS_INTERFACE_NAME "org.gspine.button"

#define POWER_BUTTON_PRESS      "powerButtonPress"
#define POWER_BUTTON_RELEASE    "powerButtonRelease"
#define VOLUME_DOWN_PRESS       "volumeDownPress"
#define VOLUME_DOWN_RELEASE     "volumeDownRelease"
#define VOLUME_UP_PRESS         "volumeUpPress"
#define VOLUME_UP_RELEASE       "volumeUpRelease"

class DbusManager
{
private:
    std::unique_ptr<sdbus::IObject> dbusObject;
    std::unique_ptr<sdbus::IConnection> dbusConnection;
public:
    DbusManager();
    ~DbusManager();
    void sendSignal(std::string signalName);

};

#endif // DBUSMANAGER_H
