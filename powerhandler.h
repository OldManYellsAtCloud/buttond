#ifndef POWERHANDLER_H
#define POWERHANDLER_H
#include "buttonhandler.h"

#include <string>

enum class State {
    SCREEN_ON, SCREEN_OFF
};

class PowerHandler : public ButtonHandler
{
private:
    FILE* eventSource;

    FILE* touchScreen;
    FILE* backlight;
    FILE* cpuFrequency;

    void turnOn();
    void turnOff();

    State current_state {State::SCREEN_ON};

    const char* INHIBIT_TOUCHSCREEN_PATH {"/sys/devices/platform/soc/1c2ac00.i2c/i2c-0/0-005d/input/input3/inhibited"};
    const char* TURN_OFF_TOUCHSCREEN {"1"};
    const char* TURN_ON_TOUCHSCREEN {"0"};

    const char* BACKLIGHT_BL_PATH {"/sys/class/backlight/backlight/bl_power"};
    const char* SWITCH_OFF_BACKLIGHT {"4"};
    const char* SWITCH_ON_BACKLIGHT {"0"};

    const char* CPU_GOVERNOR_PATH {"/sys/devices/system/cpu/cpufreq/policy0/scaling_governor"};
    const char* CPU_FREQUENCY_PATH {"/sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed"};

    const char* USERSPACE_GOVERNOR {"userspace"};
    const char* LOWEST_CPU_FREQUENCY {"648000"};
    const char* HIGHEST_CPU_FREQUENCY {"1152000"};


public:
    PowerHandler(const std::string& filepath);
    ~PowerHandler(){}

    void run();
};

#endif // POWERHANDLER_H
