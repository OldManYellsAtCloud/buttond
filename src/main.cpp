#include <thread>
#include <vector>

#include <signal.h>
#include <linux/input.h>
#include <poll.h>
#include <settingslib.h>
#include <loglibrary.h>

#include "dbusmanager.h"


#define CONF_PATH  "/etc"

std::vector<std::jthread> threads;

void prepareToStop(int dummy){
    LOG("Stop requested, preparing to go down...");

    for (auto& t: threads)
        t.request_stop();
}

void powerButtonLoop(std::stop_token st, SettingsLib* settings, DbusManager* dbusManager){
    auto filepath = settings->getValue("events", "power");
    FILE* eventSource = fopen(filepath.c_str(), "r");
    if (!eventSource) {
        ERROR("Could not open {}: {}", filepath, strerror(errno));
        exit(1);
    }

    struct pollfd pfd[1];
    pfd[0].fd = fileno(eventSource);
    pfd[0].events = POLLIN;

    int poll_timeout_ms = 2000;
    nfds_t nfds = 1;

    struct input_event event;

    while (!st.stop_requested()){
        poll(pfd, nfds, poll_timeout_ms);
        if (pfd[0].revents & POLLIN) {
            fread(&event, sizeof(input_event), 1, eventSource);

            DEBUG("Power event - code: {}, type: {}, value: {}", event.code, event.type, event.value);

            if (event.type == EV_KEY && event.code == KEY_POWER) {
                if (event.value == 1){
                    dbusManager->sendSignal(POWER_BUTTON_PRESS);
                } else {
                    dbusManager->sendSignal(POWER_BUTTON_RELEASE);
                }
            }
        }
    }
}

void volumeButtonLoop(std::stop_token st, SettingsLib* settings, DbusManager* dbusManager){
    auto filepath = settings->getValue("events", "volume");
    FILE* eventSource = fopen(filepath.c_str(), "r");
    if (!eventSource) {
        ERROR("Could not open {}: {}", filepath, strerror(errno));
        exit(1);
    }

    struct pollfd pfd[1];
    pfd[0].fd = fileno(eventSource);
    pfd[0].events = POLLIN;

    int poll_timeout_ms = 2000;
    nfds_t nfds = 1;

    struct input_event event;

    while (!st.stop_requested()){
        poll(pfd, nfds, poll_timeout_ms);
        if (pfd[0].revents & POLLIN) {
            fread(&event, sizeof(input_event), 1, eventSource);

            DEBUG("Volume {} {}. Value: {}, time: {}.{}", (event.code == KEY_VOLUMEDOWN ? "down " : "up "),
                  (event.type == EV_KEY ? "pressed." : "released."),
                  event.value, event.time.tv_sec, event.time.tv_usec);

            if (event.type == EV_KEY && event.value == 1) {
                if (event.code == KEY_VOLUMEDOWN) {
                    dbusManager->sendSignal(VOLUME_DOWN_PRESS);
                } else {
                    dbusManager->sendSignal(VOLUME_UP_PRESS);
                }
            } else if (event.type == EV_KEY && event.value == 0) {
                if (event.code == KEY_VOLUMEDOWN) {
                    dbusManager->sendSignal(VOLUME_DOWN_RELEASE);
                } else {
                    dbusManager->sendSignal(VOLUME_UP_RELEASE);
                }
            }
        }
    }
    fclose(eventSource);
}


int main()
{
    DbusManager dbusManager{};

    SettingsLib settings{CONF_PATH};

    threads.push_back(std::jthread(volumeButtonLoop, &settings, &dbusManager));
    threads.push_back(std::jthread(powerButtonLoop, &settings, &dbusManager));

    signal(SIGINT, prepareToStop);
    signal(SIGTERM, prepareToStop);

    for(auto& t: threads)
        t.join();

    LOG("Done, byebye");
    return 0;
}


