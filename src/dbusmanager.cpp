#include "dbusmanager.h"

void DbusManager::sendSignal(bool state)
{
    auto signal = dbusObject->createSignal(DBUS_INTERFACE_NAME, "lockStateChanged");
    signal << state;
    dbusObject->emitSignal(signal);
}

DbusManager::DbusManager()
{
    dbusConnection = sdbus::createSystemBusConnection(DBUS_SERVICE_NAME);
    dbusObject = sdbus::createObject(*dbusConnection, DBUS_OBJECT_PATH);
    dbusObject->registerSignal(DBUS_INTERFACE_NAME, "lockStateChanged", "b");
    dbusObject->finishRegistration();

    dbusConnection->enterEventLoopAsync();
}

DbusManager::~DbusManager()
{
    dbusObject->getConnection().leaveEventLoop();
}
