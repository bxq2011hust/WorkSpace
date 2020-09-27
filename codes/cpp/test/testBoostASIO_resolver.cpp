/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: testBoostASIO_resolver.cpp
* Purpose:
* Creation Date: 2019-07-17
* Created By: bxq2011hust@qq.com
* g++ testBoostASIO_resolver.cpp -lboost_system -lpthread
*********************************************************/

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/version.hpp>
#include <boost/system/error_code.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost::asio;

int main(int argc, char *argv[])
{
    string host(argv[1]);
    string port(argv[2]);

    try
    {
        io_service ios;
        ip::tcp::resolver rslv(ios);

        boost::system::error_code ec;
// boost 1.66 above
#if BOOST_VERSION < 106600
        ip::tcp::resolver::query qry(boost::asio::ip::tcp::v4(), host, boost::lexical_cast<string>(port));
#else
        ip::tcp::resolver::iterator iter = rslv.resolve(boost::asio::ip::tcp::v4(), host, port, ec);
#endif
        cout << "host_name: " << qry.host_name() << ", service_name: " << qry.service_name() << endl;
        ip::tcp::resolver::iterator iter = rslv.resolve(qry, ec);
        ip::tcp::resolver::iterator end;
        for (; !ec && iter != end; ++iter)
        {
            ip::tcp::endpoint endpoint = iter->endpoint();
            cout << "query success!" << endl;
            cout << "host_ip: " << endpoint.address() << ", port: " << endpoint.port() << endl;
        }
    }
    catch (exception &e)
    {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
