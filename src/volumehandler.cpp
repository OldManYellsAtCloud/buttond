#include "volumehandler.h"
/*#include <iostream>*/
#include <errno.h>
#include <string.h>
#include <poll.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <unistd.h>
#include <loglibrary.h>

void VolumeHandler::volumeDown()
{
    auto new_pid = fork();

    if (new_pid < 0) {
        ERROR("Could not fork! Exiting");
        exit(1);
    }


    if (new_pid == 0){
        DEBUG("Volume down by 5%");
        if (execlp("pactl", "pactl", "set-sink-volume", "0", "-5%", (char *) NULL) == -1){
            ERROR("Could not lower the volume: {}", strerror(errno));
        }
    }
}

void VolumeHandler::volumeUp()
{
    auto new_pid = fork();

    if (new_pid < 0) {
        ERROR("Could not fork! Exiting.");
        exit(1);
    }

    if (new_pid == 0){
        DEBUG("Volume up to 5%");
        if (execlp("pactl", "pactl", "set-sink-volume", "0", "+5%", (char *) NULL) == -1){
            ERROR("Could not increase the volume: {}", strerror(errno));
        }
    }

}

VolumeHandler::VolumeHandler(SettingsLib* settings)
{
    auto filepath = settings->getValue("events", "volume");
    eventSource = fopen(filepath.c_str(), "r");
    if (!eventSource) {
        ERROR("Could not open {}: {}", filepath, strerror(errno));
        exit(1);
    }
}

void VolumeHandler::run(std::stop_token stopToken)
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

            DEBUG("Volume {} {}. Value: {}, time: {}.{}", (event.code == KEY_VOLUMEDOWN ? "down " : "up "),
                  (event.type == EV_KEY ? "pressed." : "released."),
                  event.value, event.time.tv_sec, event.time.tv_usec);

            if (event.type == EV_KEY && event.value == 1) {
                if (event.code == KEY_VOLUMEDOWN) {
                    volumeDown();
                } else {
                    volumeUp();
                }
            }
        }
    }
    fclose(eventSource);
}

