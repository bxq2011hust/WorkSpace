/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: testBoostASIO.cpp
* Purpose:
* Creation Date: 2017-07-26
* Created By: bxq2011hust@qq.com
* g++ testBoostASIO.cpp -l boost_system
*********************************************************/

#include <iostream>    
#include <boost/asio.hpp>  
#include <boost/thread.hpp>  
#include <boost/date_time/posix_time/posix_time.hpp>  
using namespace std;    
   

void Print(const boost::system::error_code &ec,  
        boost::asio::deadline_timer* pt,  
        int * pcount)  
{  
    if (*pcount < 3)  
    {  
        cout<<"count = "<<*pcount<<endl;  
        cout<<boost::this_thread::get_id()<<endl;  
        (*pcount) ++;  

        pt->expires_at(pt->expires_at() + boost::posix_time::seconds(5)) ;  

        pt->async_wait(boost::bind(Print, boost::asio::placeholders::error, pt, pcount));  

    }  
}  
int main()  
{    
    cout<<boost::this_thread::get_id()<<endl;  
    boost::asio::io_service io;  
    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));  
    int count = 0;  
    t.async_wait(boost::bind(Print, boost::asio::placeholders::error, &t, &count));  
    cout<<"to run"<<endl;  
    io.run();  
    cout << "Final count is " << count << "\n";  
    cout<<"exit"<<endl;  
    return 0;    
}   
