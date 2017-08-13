/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: shmFile-write.cpp
* Purpose:
* Creation Date: 2017-07-30
* Created By: bxq2011hust@qq.com
* g++ shmFile-write.cpp -std=c++11 -pthread -lrt -o write_shmFile
*********************************************************/

#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/string.hpp>
#include <boost/functional/hash.hpp> //boost::hash
#include <boost/unordered_map.hpp>   //boost::unordered_map
#include <functional>                //std::equal_to hash
#include <chrono>
#include <boost/lexical_cast.hpp>
// #include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>

struct interfaceInfo
{
    int count;
    int totalTime;
    int maxTime;
};

int main()
{
    using namespace boost::interprocess;
    using time_point = std::chrono::steady_clock::time_point;
    //Remove shared memory on construction and destruction
    struct shm_remove
    {
        shm_remove() { file_mapping::remove("MyMappedFile"); }
        ~shm_remove()
        { // file_mapping::remove("MyMappedFile");
        }
    } remover;

    //Note that unordered_map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
    //so the allocator must allocate that pair.
    typedef allocator<char, managed_mapped_file::segment_manager> CharAllocator;
    typedef basic_string<char, std::char_traits<char>, CharAllocator> shm_string;
    typedef shm_string KeyType;
    typedef interfaceInfo MappedType;
    typedef std::pair<const KeyType, MappedType> ValueType;

    //Create shared memory
    managed_mapped_file mfile(open_or_create, "MyMappedFile", 65536); //Mapped file size
                                                                      //Typedef the allocator
    typedef allocator<ValueType, managed_mapped_file::segment_manager> ShmemAllocator;
    //Create allocators
    CharAllocator charallocator(mfile.get_segment_manager());

    //Alias an unordered_map of ints that uses the previous STL-like allocator.
    typedef boost::unordered_map<KeyType, MappedType, boost::hash<KeyType>, std::equal_to<KeyType>, ShmemAllocator> SHMHashMap;

    //Construct a shared memory hash map.
    //Note that the first parameter is the initial bucket count and
    //after that, the hash function, the equality function and the allocator

    SHMHashMap *myhashmap = mfile.find_or_construct<SHMHashMap>("MyHashMap")       //object name
                            (100, boost::hash<KeyType>(), std::equal_to<KeyType>() //
                             , mfile.get_allocator<ValueType>()); //allocator instance
    time_point *myTime = mfile.find_or_construct<time_point>("MyTimePoint")();
    *myTime = std::chrono::system_clock::now();
    // KeyType myshmstring(mfile.get_segment_manager());
    KeyType myshmstring("test", charallocator);

    //Insert data in the hash map
    interfaceInfo info;

    for (int i = 0; i < 100; ++i)
    {
        std::string s = boost::lexical_cast<std::string>(i);
        myshmstring = s.c_str();
        info.count = i;
        info.totalTime = i + 1;
        info.maxTime = i + 2;
        myhashmap->insert(ValueType(myshmstring, info));
        std::cout << " bucket count: " << myhashmap->bucket_count()
                  << " | shm size: " << mfile.get_free_memory() << "/" << mfile.get_size() << " bytes | "
                  << " insert " << i << std::endl;
    }
    return 0;
}
