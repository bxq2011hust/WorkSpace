/*********************************************************
*   Copyright (C) 2017 All rights reserved.
*   
* File Name: shm-read.cpp
* Purpose:
* Creation Date: 2017-07-30
* Created By: bxq2011hust@qq.com
* g++ shm-read.cpp -std=c++11 -pthread -lrt -o read_shm
*********************************************************/

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>

#include <boost/unordered_map.hpp>     //boost::unordered_map 
#include <functional>                  //std::equal_to hash
#include <iostream>


int main ()
{
   using namespace boost::interprocess;
   //Remove shared memory on construction and destruction
//   struct shm_remove
//   {
//      shm_remove() { shared_memory_object::remove("MySharedMemory"); }
//      ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
//   } remover;

   //Note that unordered_map<Key, MappedType>'s value_type is std::pair<const Key, MappedType>,
   //so the allocator must allocate that pair.
   typedef int    KeyType;
   typedef int  MappedType;
   typedef std::pair<const int, MappedType> ValueType;

   //Create shared memory
   managed_shared_memory segment(open_or_create, "MySharedMemory", 65536);
  //Typedef the allocator
   typedef allocator<ValueType, managed_shared_memory::segment_manager> ShmemAllocator;

   //Alias an unordered_map of ints that uses the previous STL-like allocator.
   typedef boost::unordered_map
      < KeyType               , MappedType
      , std::hash<KeyType>  ,std::equal_to<KeyType>
      , ShmemAllocator>
   SHMHashMap;

   //Construct a shared memory hash map.
   //Note that the first parameter is the initial bucket count and
   //after that, the hash function, the equality function and the allocator

   SHMHashMap *myhashmap = segment.find_or_construct<SHMHashMap>("MyHashMap")  //object name
      ( 3, std::hash<int>(), std::equal_to<int>()                  //
      , segment.get_allocator<ValueType>());                         //allocator instance

  if(myhashmap==nullptr) 
    std::cout<<"find_or_construct MyHashMap error"<<std::endl;
  else 
    std::cout<<"start reading "<<myhashmap->size()<<" items"<<"bucket count: "<<myhashmap->bucket_count()<<std::endl;
   //Insert data in the hash map
   int i=0;
   for(auto item = myhashmap->begin(); item!= myhashmap->end(); ++item)
   {
      std::cout<<"read "<<i<<" key: "<<item->first<<" value: "<<item->second<<std::endl;
      ++i;
   }
   shared_memory_object::remove("MySharedMemory");

   return 0;
}
