#pragma once
#include <iostream>
#include <ctime>
using namespace std;

class properties {
public:
    bool ishidden;
    time_t createTime;
    bool isDir;
    int size;
    string name;

    properties(bool hid, time_t time, bool isd, int s, string _name);
};
