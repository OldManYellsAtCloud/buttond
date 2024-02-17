#include "dbusmanager.h"

void DbusManager::sendSignal(std::string signalName)
{
    auto signal = dbusObject->createSignal(DBUS_INTERFACE_NAME, signalName);
    dbusObject->emitSignal(signal);
}

DbusManager::DbusManager()
{
    dbusConnection = sdbus::createSystemBusConnection(DBUS_SERVICE_NAME);
    dbusObject = sdbus::createObject(*dbusConnection, DBUS_OBJECT_PATH);

    dbusObject->registerSignal(DBUS_INTERFACE_NAME, POWER_BUTTON_PRESS, "");
    dbusObject->registerSignal(DBUS_INTERFACE_NAME, POWER_BUTTON_RELEASE, "");

    dbusObject->registerSignal(DBUS_INTERFACE_NAME, VOLUME_DOWN_PRESS, "");
    dbusObject->registerSignal(DBUS_INTERFACE_NAME, VOLUME_DOWN_RELEASE, "");

    dbusObject->registerSignal(DBUS_INTERFACE_NAME, VOLUME_UP_PRESS, "");
    dbusObject->registerSignal(DBUS_INTERFACE_NAME, VOLUME_UP_RELEASE, "");

    dbusObject->finishRegistration();
    dbusConnection->enterEventLoopAsync();
}

DbusManager::~DbusManager()
{
    dbusObject->getConnection().leaveEventLoop();
}
