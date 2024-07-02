#include "dbusmanager.h"

void DbusManager::sendSignal(std::string signalName)
{
    sdbus::InterfaceName interfaceName{DBUS_INTERFACE_NAME};
    sdbus::SignalName _signalName{signalName};
    auto signal = dbusObject->createSignal(interfaceName, _signalName);
    dbusObject->emitSignal(signal);
}

DbusManager::DbusManager()
{
    sdbus::ServiceName serviceName{DBUS_SERVICE_NAME};
    dbusConnection = sdbus::createBusConnection(serviceName);

    sdbus::ObjectPath objectPath{DBUS_OBJECT_PATH};
    dbusObject = sdbus::createObject(*dbusConnection, std::move(objectPath));

    sdbus::InterfaceName interfaceName{DBUS_INTERFACE_NAME};

    dbusObject->addVTable(sdbus::SignalVTableItem{sdbus::MethodName{POWER_BUTTON_PRESS}, {}, {}}).forInterface(interfaceName);
    dbusObject->addVTable(sdbus::SignalVTableItem{sdbus::MethodName{POWER_BUTTON_RELEASE}, {}, {}}).forInterface(interfaceName);
    dbusObject->addVTable(sdbus::SignalVTableItem{sdbus::MethodName{VOLUME_DOWN_PRESS}, {}, {}}).forInterface(interfaceName);
    dbusObject->addVTable(sdbus::SignalVTableItem{sdbus::MethodName{VOLUME_DOWN_RELEASE}, {}, {}}).forInterface(interfaceName);
    dbusObject->addVTable(sdbus::SignalVTableItem{sdbus::MethodName{VOLUME_UP_PRESS}, {}, {}}).forInterface(interfaceName);
    dbusObject->addVTable(sdbus::SignalVTableItem{sdbus::MethodName{VOLUME_UP_RELEASE}, {}, {}}).forInterface(interfaceName);

    dbusConnection->enterEventLoopAsync();
}

DbusManager::~DbusManager()
{
    dbusObject->getConnection().leaveEventLoop();
}
