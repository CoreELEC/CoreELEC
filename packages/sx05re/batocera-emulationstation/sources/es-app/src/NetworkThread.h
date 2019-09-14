#pragma once

#include "Window.h"
#include <thread>

class NetworkThread 
{
public:
    NetworkThread(Window * window);
    virtual ~NetworkThread();

private:
    Window*			mWindow;
    bool			mRunning;
    bool			mFirstRun;
	std::thread*	mThread;

    void run();
};


