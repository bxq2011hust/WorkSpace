#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

void printStack()
{
    int a[100];
    a[0] = 0;
    cout << a << endl;
}

int main()
{
    for (int i = 0; i < 4; i++)
    {
        thread a(printStack);
        a.detach();
    }
    this_thread::sleep_for(chrono::seconds(1));
    cout << "done" << endl;
}