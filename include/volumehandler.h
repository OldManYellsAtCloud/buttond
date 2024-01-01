#ifndef VOLUMEHANDLER_H
#define VOLUMEHANDLER_H

#include "buttonhandler.h"
#include <settingslib.h>
#include <string>

class VolumeHandler : public ButtonHandler
{
private:
    FILE* eventSource;

    void volumeDown();
    void volumeUp();

public:
    VolumeHandler(SettingsLib* settings);
    ~VolumeHandler(){}

    void run(std::stop_token stopToken);
};

#endif // VOLUMEHANDLER_H
