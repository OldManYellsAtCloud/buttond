#ifndef VOLUMEHANDLER_H
#define VOLUMEHANDLER_H

#include "buttonhandler.h"
#include <string>

class VolumeHandler : public ButtonHandler
{
private:
    FILE* eventSource;

    void volumeDown();
    void volumeUp();

public:
    VolumeHandler(const std::string& filepath);
    ~VolumeHandler(){}

    void run();
};

#endif // VOLUMEHANDLER_H
