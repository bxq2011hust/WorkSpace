/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: shmFile-read.cpp
* Purpose:
* Creation Date: 2017-07-30
* Created By: bxq2011hust@qq.com
* g++ shmFile-read.cpp -std=c++11 -pthread -lrt -o read_shmFile
*********************************************************/

#include <boost/interprocess/managed_mapped_file.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/interprocess/containers/string.hpp>
#include <boost/functional/hash.hpp>   //boost::hash
#include <boost/unordered_map.hpp>     //boost::unordered_map 
#include <chrono>
#include <functional>                  //std::equal_to hash
#include <iostream>
#include <ctime>
// #include <string>

struct interfaceInfo
{
    int count;
    int totalTime;
    int maxTime;
};

int main ()
{
   using namespace boost::interprocess;
   using time_point = std::chrono::steady_clock::time_point;
   //Remove shared memory on construction and destruction
//   struct shm_remove
//   {
//      shm_remove() { file_mapping::remove("MyMappedFile"); }
//      ~shm_remove(){ // file_mapping::remove("MyMappedFile"); 
//         }
//   } remover;

   //Note that unordered_map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
   //so the allocator must allocate that pair.
   typedef allocator<char, managed_mapped_file::segment_manager>   char_allocator;
   typedef basic_string<char, std::char_traits<char>, char_allocator> shm_string;
   typedef shm_string KeyType;
   typedef interfaceInfo  MappedType;
   typedef std::pair<const KeyType, MappedType> ValueType;

  //Create shared memory
  managed_mapped_file mfile(open_or_create, "MyMappedFile", 65536);//Mapped file size
  //Typedef the allocator
   typedef allocator<ValueType, managed_mapped_file::segment_manager> ShmemAllocator;

   //Alias an unordered_map of ints that uses the previous STL-like allocator.
   typedef boost::unordered_map
      < KeyType , MappedType , boost::hash<KeyType>  
      ,std::equal_to<KeyType> , ShmemAllocator> SHMHashMap;

   //Construct a shared memory hash map.
   //Note that the first parameter is the initial bucket count and
   //after that, the hash function, the equality function and the allocator

   SHMHashMap *myhashmap = mfile.find_or_construct<SHMHashMap>("MyHashMap")  //object name
      ( 100, boost::hash<KeyType>(), std::equal_to<KeyType>()                  //
      , mfile.get_allocator<ValueType>());                         //allocator instance
time_point *myTime=mfile.find_or_construct<time_point>("MyTimePoint")();

std::time_t tt = std::chrono::steady_clock::to_time_t(*myTime);
std::cout << "time_point tp is: " << ctime(&tt)<<std::endl;

      if(myhashmap==nullptr) 
            std::cout<<"find_or_construct MyHashMap error"<<std::endl;
      else 
            std::cout<<"start reading "<<myhashmap->size()
                  <<" items. bucket count: "<<myhashmap->bucket_count()
                  <<" used/total: "<<mfile.get_free_memory()<<"/"
                  <<mfile.get_size()<<std::endl;
   //Insert data in the hash map
   int i=0;
   for(auto item = myhashmap->begin(); item!= myhashmap->end(); ++item)
   {
     std::cout<<"read "<<i<<" key: "<<item->first
            <<" value: "<<item->second.count<<" "
            <<item->second.totalTime<<" "
            <<item->second.maxTime<<std::endl;
      ++i;
   }
   file_mapping::remove("MyMappedFile");
   return 0;
}
