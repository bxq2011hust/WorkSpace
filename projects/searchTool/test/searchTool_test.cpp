#include "searchTool.h"
#include <iostream>
#include <list>

using namespace std;

int main(int argc, char **argv)
{
    const int dim = 64;
    const int dbIndex=2;
    SearchTool search(dim, "redispassword");
    if (!search.initSearchTool(100, 20, 0.7,dbIndex,HASH_SCAN_METHOD_MIH))
    {
        cout << "searchTool init fail." << endl;
        return -1;
    }
    cout << "select db "<<dbIndex<<" :" << (search.selectDb(dbIndex) ? "true" : "false") << endl;
    cout << "searchTool init ok." << endl;
    PersonInfo person = {"000", "name", "man", "2017-05-14"};
    for (int i = 0; i < 5; ++i)
    {
        person.id += to_string(i);
        person.name += to_string(i);
        search.addPerson(person);
    }
    person.name = "updateName";
    if (search.updatePersonInfo(person))
        cout << "update " << person.id << "'name to updateName success." << endl;
    else
        cout << "update fail." << endl;
    cout << "------------------------------------" << endl;
    cout << "-------------All person-------------" << endl;
    list<PersonInfo> all = search.getAllPersonInfo();
    for (auto p : all)
    {
        cout << p.id << endl
             << p.name << endl
             << p.sex << endl
             << p.timestamp << endl;
        cout << "-------------" << endl;
    }

    cout << endl
         << "------------------------------------" << endl;
    cout << "----------Get person faces----------" << endl;
    cout << person.id << "'s faces" << endl;
    FaceImageInfo images;
    images.id = person.id;
    images.feature.resize(dim, 256.2f);
    images.path = "~/images/face";
    if (search.addFace(images))
        cout << "addFace ok.  " << endl;
    else
        cout << "addFace fail." << endl;
    vector<string> facePaths = search.getFaces(person.id);
    for (auto path : facePaths)
        cout << path << endl;

    cout << endl
         << "------------------------------------" << endl;
    cout << "------------Query person------------" << endl;
    cout << "Query using id     : " << person.id << endl;
    vector<string> query_person = search.queryPerson(person.id);
    for (auto info : query_person)
        cout << info << endl;
    cout << "query using feature: " << endl;
    string id, path;
    float score = 0.0f;
    if (search.queryPerson(images.feature, id, path, score))
    {
        cout << "result id   : " << id << endl;
        cout << "result path : " << path << endl;
        cout << "result score: " << score << endl;
    }
    else
    {
        cout << "query using feature: fail" << endl;
    }
    cout << "Query finished. Delete person :" << person.id << endl;
    if (search.deletePerson(person.id))
        cout << person.id << " is deleted." << endl;
    query_person = search.queryPerson(person.id);
    if (query_person.empty())
        cout << person.id << " dosen't exist." << endl;
    cout << "There are " << search.personCount() << " person in blacklist." << endl;
    return 0;
}