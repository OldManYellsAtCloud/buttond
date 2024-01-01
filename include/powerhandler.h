#ifndef POWERHANDLER_H
#define POWERHANDLER_H
#include "buttonhandler.h"

#include <string>

#define TURN_OFF_TOUCHSCREEN "1"
#define TURN_ON_TOUCHSCREEN  "0"

#define SWITCH_OFF_BACKLIGHT "4"
#define SWITCH_ON_BACKLIGHT  "0"

#define TURN_OFF_CPU_CORE    "1"
#define TURN_ON_CPU_CORE     "0"

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

    FILE* cpuCoreList[3];

    void turnOn();
    void turnOff();

    State current_state {State::SCREEN_ON};

    const char* BACKLIGHT_BL_PATH {"/sys/class/backlight/backlight/bl_power"};

    const char* CPU_GOVERNOR_PATH {"/sys/devices/system/cpu/cpufreq/policy0/scaling_governor"};
    const char* CPU_FREQUENCY_PATH {"/sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed"};

    const char* USERSPACE_GOVERNOR {"userspace"};
    const char* LOWEST_CPU_FREQUENCY {"648000"};
    const char* HIGHEST_CPU_FREQUENCY {"1152000"};

    const char* CPU_CORE_LIST[3] {
        "/sys/devices/system/cpu/cpu1/online",
        "/sys/devices/system/cpu/cpu2/online",
        "/sys/devices/system/cpu/cpu3/online"
    };


public:
    PowerHandler(const std::string& buttonPath, const std::string& tsInhibitPath);
    ~PowerHandler(){}

    void run();
};

#endif // POWERHANDLER_H
