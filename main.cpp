#include <cstdio>
#include <thread>

#include <linux/input.h>
#include "volumehandler.h"
#include "powerhandler.h"

#include <signal.h>
#include <vector>

using namespace std;

std::vector<ButtonHandler> buttonHandlers;

void prepareToStop(int dummy){
    for (auto t: buttonHandlers)
        t.stop();
}

int main()
{
    VolumeHandler vh {"/dev/input/event1"};
    PowerHandler ph {"/dev/input/event0"};

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


