#include <thread>
#include <signal.h>
#include <vector>

#include <linux/input.h>
#include "volumehandler.h"
#include "powerhandler.h"
#include <settingslib.h>
#include <loglibrary.h>

#define CONF_PATH  "/etc"

std::vector<std::jthread> threads;

void prepareToStop(int dummy){
    LOG("Stop requested, preparing to go down...");

    for (auto& t: threads)
        t.request_stop();
}

int main()
{
    SettingsLib settings{CONF_PATH};

    VolumeHandler vh {&settings};
    PowerHandler ph {&settings};

    threads.push_back(std::jthread(&VolumeHandler::run, vh));
    threads.push_back(std::jthread(&PowerHandler::run, ph));

    signal(SIGINT, prepareToStop);
    signal(SIGTERM, prepareToStop);

    for(auto& t: threads)
        t.join();

    LOG("Done, byebye");
    return 0;
}


