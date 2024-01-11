#pragma once
#include <iostream>
#include <vector>
#include "Folder.h"
#include "File.h"
#include <iomanip>
#include <string>
#include <algorithm>
#include<deque>
#include <queue>
#include "DOS.h"
#include "comparator.h"

using namespace std;

class DOS {
public:
    Folder* root;
    Folder* currentFolder;
    char prompt;
    deque<File*> qprint;
    priority_queue<File*, vector<File*>, Comparator> pqueue;

    DOS(string name);

    void displayStudentDetails();

    void viewcurrentDirectory();

    string extractCommand(string& input);

    std::string toLower(const std::string& input);

    void displaypath();

    static vector<string> stringToVector(string line, string deli);

    void makeDirectory(string input);

    void changeDirectory(string input);

    void currentoPrevious();

    void currenttoroot();

    void changeattribute(string folderpath, bool hidden);

    void showattribonly(string input);

    void printingCurrentfolderProperties(Folder* currentFolder);
    
    void creatingfile(string input);

    void deleteaFile(string input);

    void convertExtensions(string input);

    void deleteaFolder(string input);

    void copyFile(string input);

    void moveFile(string input);

    void changePrompt();

    void editFile(string input);

    void findfile(string input);

    void findstringinonefile(string input);

    void findstringinallfile(string input);

    void formatDrive();

    void renameFile( string input);

    void displayDriveTree(Folder* r, int depth);

    void insertinQueue(string input);

    void displayQueue();

    void insertinPriorityqueue(string input);

    void displayPqueue();

    void openThefileEditor(bool next, File* f);

    void help(string input);

    void saveTree();

    void loadTree();

    void saveTreeRecursion(Folder* r, ofstream& fout, int depth);

    void loadTreeLoop(Folder* r, ifstream& fin);

    void main();
};

