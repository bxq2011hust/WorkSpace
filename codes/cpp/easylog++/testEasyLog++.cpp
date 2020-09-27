/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: testEasyLog++.cpp
* Purpose:
* Creation Date: 2017-07-28
* Created By: bxq2011hust@qq.com
* g++ testEasyLog++.cpp easylogging++.cc -std=c++11 -l pthread
* https://github.com/muflihun/easyloggingpp.git
*********************************************************/

#include "easylogging++.h"
#include <thread>
#include <chrono>
INITIALIZE_EASYLOGGINGPP

using namespace std;

int main(int argc, char *argv[])
{
    int times = 10;
    thread t([&]() {
        for (int i = 0; i < times; ++i)
        {
            this_thread::sleep_for(std::chrono::seconds(2));
            LOG(INFO) << "My first info log using default logger";
        }
    });
    t.join();
    return 0;
}
