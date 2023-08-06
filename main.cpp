#include <iostream>
#include "event.h"
#include "circular.h"

using namespace std;

int main()
{
    CircularBuffer<int, 100> buf;
    buf << 2 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9;
    buf.write(10);
    //buf.clear();
    int a;
    buf >> a >> a >> a >> a;
    buf << 11 << 12 << 13;
    cout << buf;
    while(buf.isNotEmpty()){
        int v = 99;
        buf >> v;
        cout << v << " ";
    }
    cout << buf;

    EventRecord<> ev(vfoEvents::KEYPAD, 3u);
    EventLoop<> app;
    app.send(std::move(ev));
    return 0;
}
