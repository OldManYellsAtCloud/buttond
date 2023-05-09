#include "volumehandler.h"
/*#include <iostream>*/
#include <errno.h>
#include <string.h>
#include <poll.h>

#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <unistd.h>


void VolumeHandler::volumeDown()
{
    auto new_pid = fork();

    if (new_pid < 0) {
        /*std::cerr << "Coult not fork! Exiting" << std::endl;*/
        exit(1);
    }


    if (new_pid == 0){
        /*std::cout << "Volume dowxxxn by 5%" << std::endl;*/
        if (execlp("pactl", "pactl", "set-sink-volume", "0", "-5%", (char *) NULL) == -1){
            /*std::cerr << "Could not lower the volume: " << strerror(errno) << std::endl;*/
        }

        /*std::cout << "this should not be seen" << std::endl;*/
    }
}

void VolumeHandler::volumeUp()
{
    auto new_pid = fork();

    if (new_pid < 0) {
        /*std::cerr << "Coult not fork! Exiting" << std::endl;*/
        exit(1);
    }

    if (new_pid == 0){
        /*std::cout << "Volume up by 5%" << std::endl;*/
        if (execlp("pactl", "pactl", "set-sink-volume", "0", "+5%", (char *) NULL) == -1){
            /*std::cerr << "Could not increase the volume: " << strerror(errno) << std::endl;*/
        }

        /*std::cout << "this should not be seen" << std::endl;*/
    }

}

VolumeHandler::VolumeHandler(const std::string& filepath)
{
    eventSource = fopen(filepath.c_str(), "r");
    if (!eventSource) {
        /*std::cerr << "Could not open " << filepath << ": " << strerror(errno) << std::endl;*/
        exit(1);
    }
}

void VolumeHandler::run()
{
    struct pollfd pfd[1];
    pfd[0].fd = fileno(eventSource);
    pfd[0].events = POLLIN;

    int poll_timeout_ms = 2000;
    nfds_t nfds = 1;

    struct input_event event;

    while (!done){
        poll(pfd, nfds, poll_timeout_ms);
        if (pfd[0].revents & POLLIN) {
            fread(&event, sizeof(input_event), 1, eventSource);

            /*std::cout << "Volume " << (event.code == KEY_VOLUMEDOWN ? "down " : "up ")
                      << (event.type == EV_KEY ? "pressed." : "released.")
                      << " Value: " << event.value
                      << ", time: " << event.time.tv_sec << "." << event.time.tv_usec << std::endl;*/

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

