#include "redisManager.h"
#include <iostream>
// #include <utility>

using namespace std;

int main(int argc, char **argv)
{
    RedisManager r("127.0.0.1", 6379);
    string set_key("set-key");
    if (!r.initRedisConnect())
        return 0;
    cout << "select db   : " << r.selectDb(10) << endl;
    // srt ops
    cout << "set add test: " << r.set_adds(set_key.c_str(), "test test2") << endl;
    cout << "set has test: " << r.set_IsMember(set_key.c_str(), "test") << endl;
    cout << "set size    : " << r.set_size(set_key.c_str()) << endl;

    cout << "set del test: " << r.set_dels(set_key.c_str(), "test test2") << endl;
    cout << "set has test: " << r.set_IsMember(set_key.c_str(), "test") << endl;
    cout << "set size    : " << r.set_size(set_key.c_str()) << endl;

    vector<string> members = r.set_getAll(set_key.c_str());
    for (unsigned i = 0; i < members.size(); ++i)
        cout << "set members " << i << " : " << members[i] << endl;

    // hash ops
    cout << "-------------------------" << endl;
    string hash_key("hash_key");
    cout << "hash add test1       : " << r.hash_add(hash_key.c_str(), "test1", "test") << endl;
    cout << "hash add test1(exist): " << r.hash_add(hash_key.c_str(), "test1", "test") << endl;
    cout << "hash set test1       : " << r.hash_set(hash_key.c_str(), "test1", "tt") << endl;
    cout << "hash get test1       : " << r.hash_get(hash_key.c_str(), "test1") << endl;
    cout << "hash set test1,test2 : " << r.hash_sets(hash_key.c_str(), "test1 tt1 test2 tt2") << endl;
    cout << "-------------------------" << endl;

    string test = hash_key + " test1 test2";
    cout << test << endl;
    vector<string> hash_keys = r.hash_gets(hash_key.c_str(), "test1 test2 test3");
    for (unsigned i = 0; i < hash_keys.size(); ++i)
        cout << "hash_gets value  :" << i << " : " << hash_keys[i] << endl;
    cout << "-------------------------" << endl;

    cout << "hash exist test1     : " << r.hash_exist(hash_key.c_str(), "test1") << endl;
    cout << "hash exist test3     : " << r.hash_exist(hash_key.c_str(), "test3") << endl;
    cout << "hash dels test test1 : " << r.hash_dels(hash_key.c_str(), "test test1") << endl;
    cout << "hash get count       : " << r.hash_get(hash_key.c_str(), "count") << endl;
    cout << "hash count-3         : " << r.hash_increase(hash_key.c_str(), "count", -3) << endl;
    cout << "-------------------------" << endl;

    hash_keys = r.hash_getAllKyes(hash_key.c_str());
    for (unsigned i = 0; i < hash_keys.size(); ++i)
        cout << "all hash_keys " << i << " : " << hash_keys[i] << endl;

    vector<pair<string, string>> keys = r.hash_getAll(hash_key.c_str());
    for (unsigned i = 0; i < keys.size(); ++i)
        cout << "hash_keys " << i << " : " << keys[i].first << " = " << keys[i].second << endl;
    r.bgsave(); //backfround save data

    return 0;
}
