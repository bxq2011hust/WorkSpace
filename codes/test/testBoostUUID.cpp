/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: testBoostUUID.cpp
* Purpose:
* Creation Date: 2018-09-21
* Created By: bxq2011hust@qq.com
*********************************************************/

#include <iostream>
//#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include <boost/range/algorithm/remove_if.hpp>
#include <boost/algorithm/string/classification.hpp>
using namespace std;

int main()
{
	boost::uuids::random_generator uuidGenerator;
	string s = to_string(uuidGenerator());
	cout<<s<<endl;
	s.erase(boost::remove_if(s, boost::is_any_of("-")),s.end());
	cout<<s<<endl;
	return 0;   
}
