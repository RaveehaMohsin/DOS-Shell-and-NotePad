#pragma once
#include <iostream>
#include <list>
#include <ctime>
#include <chrono>
#include <vector>
#include "Properties.h"
#include "File.h"

using namespace std;

class File;

class Folder {
public:
    string foldername;
    list<Folder*> subfolders;
    list<File*> filenames;
    Folder* parent;
    string owner;
    chrono::time_point<chrono::system_clock> created;
    bool ishidden;

    Folder(string name = "null", Folder* parent_ = nullptr, list<Folder*> sf = list<Folder*>(), list<File*> files = list<File*>(), chrono::time_point<chrono::system_clock> created_ = chrono::system_clock::now(), bool _prop = false);

    vector<string> returnpath();

    void makeDirectory(string name);

    void createFile(string name, string ext);

    Folder* findfolder(string name);

    File* findfile(string name, string ext);

    void showtheAttribute();

    void changeAttrib(bool hide);

    properties returnProperties();

    vector<properties> returningfolderfileproperties();

    void convert(string ext1, string ext2);

    void deleteafile(string name, string ext);

    void deleteafolder(string name);

    void findallfiles(string input, vector <string>& paths);

    void findallfileswithstring(string input, vector <string>& paths);

    void loadFile(string str);
};
