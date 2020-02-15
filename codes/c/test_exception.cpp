#include <iostream>
#include <exception>
#include "c_exception.h"

using namespace std;

void setStore()
{
    cout << "setStore" << endl;
    throw runtime_error("ttttt");
}

void setStorage()
{
    cout << "setStorage" << endl;
    int i = 0;
LOOP:
    do
    {
        setStore();
        goto LOOP;
    } while (i < 5);
}

void execute()
{
    cout << "execute" << endl;
    fn_table *p = test2(setStorage);
    evmc_context context;
    context.fn_table = p;
    context.fn_table->throwException();

    delete p;
}

int main()
{
    typedef void (*func)();
    func c = execute;
    cout << "call ..." << endl;
    try
    {
        c();
    }
    catch (const std::exception &e)
    {
        cout << "catched:" << e.what() << endl;
    }
    cout << "done" << endl;
}