#pragma once
#include <iostream>
#include <ctime>
#include <chrono>
#include <vector>
#include "Properties.h"
#include <list>

using namespace std;

class Folder;
class Editor;

class File {
public:
    string Name;
    string Extension;
    chrono::time_point<chrono::system_clock> modified;
    bool ishidden;
    Folder* parentFolder;
    int time = 0;
    int priority = 0;
    list<list<char>> data;
    Editor* edit;

    File(string name, Folder* p, string ext , chrono::time_point<chrono::system_clock> modified = chrono::system_clock::now());

    File(string name, Folder* f);

    string filefullname();

    properties returnProperties();

    void changeAttribute(bool hide);

    vector<string> returnpath();

    void showtheAttribute();

    bool containstring(string input);

    void saveSelf();

    string returnSaveName();
};
