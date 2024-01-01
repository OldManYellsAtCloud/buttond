#include "powerhandler.h"

#include <cstdint>
#include <string.h>
#include <errno.h>
#include <poll.h>
#include <unistd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <loglibrary.h>

void PowerHandler::turnOn()
{
    DEBUG("Turn on screen");

    fwrite(TURN_ON_TOUCHSCREEN, 1, sizeof(TURN_ON_TOUCHSCREEN) / sizeof(TURN_ON_TOUCHSCREEN[0]), touchScreen);
    fwrite(SWITCH_ON_BACKLIGHT, 1, sizeof(SWITCH_ON_BACKLIGHT) / sizeof(SWITCH_ON_BACKLIGHT[0]), backlight);
    fwrite(HIGHEST_CPU_FREQUENCY, 1, sizeof(HIGHEST_CPU_FREQUENCY) / sizeof(HIGHEST_CPU_FREQUENCY[0]), cpuFrequency);

    for (auto& cpuCore: cpuCoreList){
        fwrite(TURN_ON_CPU_CORE, 1, sizeof(TURN_ON_CPU_CORE) / sizeof(TURN_ON_CPU_CORE[0]), cpuCore);
        fflush(cpuCore);
    }

    fflush(backlight);
    fflush(touchScreen);
    fflush(cpuFrequency);
    current_state = State::SCREEN_ON;
}

void PowerHandler::turnOff()
{
    DEBUG("Turn off screen");
    fwrite(TURN_OFF_TOUCHSCREEN, 1, sizeof(TURN_OFF_TOUCHSCREEN) / sizeof(TURN_OFF_TOUCHSCREEN[0]), touchScreen);
    fwrite(SWITCH_OFF_BACKLIGHT, 1, sizeof(SWITCH_OFF_BACKLIGHT) / sizeof(SWITCH_OFF_BACKLIGHT[0]), backlight);
    fwrite(LOWEST_CPU_FREQUENCY, 1, sizeof(LOWEST_CPU_FREQUENCY) / sizeof(LOWEST_CPU_FREQUENCY[0]), cpuFrequency);
    for (auto& cpuCore: cpuCoreList){
        fwrite(TURN_OFF_CPU_CORE, 1, sizeof(TURN_OFF_CPU_CORE) / sizeof(TURN_OFF_CPU_CORE[0]), cpuCore);
        fflush(cpuCore);
    }

    fflush(touchScreen);
    fflush(backlight);
    fflush(cpuFrequency);

    current_state = State::SCREEN_OFF;
}

PowerHandler::PowerHandler(SettingsLib* settings)
{
    auto buttonPath = settings->getValue("events", "power");
    auto tsInhibitPath = settings->getValue("hw", "ts_inhibit");

    eventSource = fopen(buttonPath.c_str(), "r");
    if (!eventSource) {
        ERROR("Could not open {}: {}", buttonPath, strerror(errno));
        exit(1);
    }

    FILE* cpu_governor = fopen(CPU_GOVERNOR_PATH, "r+");
    if (!cpu_governor) {
        ERROR("Could not open {}: {}", CPU_GOVERNOR_PATH, strerror(errno));
    }

    fwrite(USERSPACE_GOVERNOR, 1, 10, cpu_governor);
    fclose(cpu_governor);

    touchScreen = fopen(tsInhibitPath.c_str(), "r+");
    backlight = fopen(BACKLIGHT_BL_PATH, "r+");
    cpuFrequency = fopen(CPU_FREQUENCY_PATH, "r+");

    for (uint8_t i {}; i < 3; ++i)
        cpuCoreList[i] = fopen(CPU_CORE_LIST[i], "r+");


}

void PowerHandler::run(std::stop_token stopToken)
{
    struct pollfd pfd[1];
    pfd[0].fd = fileno(eventSource);
    pfd[0].events = POLLIN;

    int poll_timeout_ms = 2000;
    nfds_t nfds = 1;

    struct input_event event;

    while (!stopToken.stop_requested()){
        poll(pfd, nfds, poll_timeout_ms);
        if (pfd[0].revents & POLLIN) {
            fread(&event, sizeof(input_event), 1, eventSource);

            DEBUG("Power event - code: {}, type: {}, value: {}", event.code, event.type, event.value);

            if (event.type == EV_KEY && event.code == KEY_POWER && event.value == 1) {
                switch (current_state){
                case State::SCREEN_ON:
                    turnOff();
                    break;
                default:
                    turnOn();
                }
            }
        }
    }
    fclose(touchScreen);
    fclose(backlight);
    fclose(cpuFrequency);

    for (auto cpuCore: cpuCoreList)
        fclose(cpuCore);
}

