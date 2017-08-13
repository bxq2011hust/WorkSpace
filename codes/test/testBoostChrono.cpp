/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: testBoostChrono.cpp
* Purpose:
* Creation Date: 2017-07-26
* Created By: bxq2011hust@qq.com
*********************************************************/

#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

int main()
{
    steady_clock::time_point t1 = steady_clock::now();
    cout<<"printing out 1000 starts...\n"<<endl;
    for(int i=0;i<1000;++i) cout<<"*";
    cout<<endl;

    steady_clock::time_point t2 = steady_clock::now();
    milliseconds timeUsed = duration_cast<milliseconds>(t2-t1);
    cout<<"time used: "<<timeUsed.count()<<" microseconds"<<endl;
    
}
