#include <thread>
#include <signal.h>
#include <vector>

#include <linux/input.h>
#include "volumehandler.h"
#include "powerhandler.h"
#include <settingslib.h>

#define CONF_PATH  "/etc"

std::vector<ButtonHandler> buttonHandlers;

void prepareToStop(int dummy){
    for (auto t: buttonHandlers)
        t.stop();
}

int main()
{
    SettingsLib settings{CONF_PATH};

    auto volumeEventPath = settings.getValue("events", "volume");
    auto powerEventPath = settings.getValue("events", "power");

    VolumeHandler vh {volumeEventPath};
    PowerHandler ph {powerEventPath};

    std::thread t1{&VolumeHandler::run, vh};
    std::thread t2{&PowerHandler::run, ph};

    buttonHandlers.push_back(vh);
    buttonHandlers.push_back(ph);

    signal(SIGINT, prepareToStop);
    signal(SIGTERM, prepareToStop);

    t1.join();
    t2.join();

    return 0;
}


