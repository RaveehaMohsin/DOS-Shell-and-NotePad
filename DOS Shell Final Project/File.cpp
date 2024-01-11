#include "File.h"
#include "Folder.h"
#include "Properties.h"
#include <fstream>
#include "DOS.h"



File::File(string name, Folder* p, string ext , chrono::time_point<chrono::system_clock> modified) {
    this->Name = name;
    this->parentFolder = p;
    this->Extension = ext;
    this->modified = modified;
}

File::File(string name, Folder* _p) : parentFolder(_p), modified(chrono::system_clock::now()), ishidden(false), edit(nullptr)
{
    string n = "D:\\3rd Semester\\DSA\\DOS Shell Final Project\\";
    string path = n + name;
    ifstream fin(path);
    char c;
    data.push_back(list<char>());
    auto rit = data.begin();
    while (fin.good())
    {

        fin.get(c);
        if (fin.eof())
            break;
        if (c == '\n')
        {
            data.push_back(list<char>());
            rit++;
        }
        else
        {
            (*rit).push_back(c);
        }
    }
    //Getting Real Name
    auto temp = DOS::stringToVector(name, "_");
    auto t1 = DOS::stringToVector(temp[temp.size() - 1], ".");
    this->Name = t1[0];
    this->Extension = t1[1];
}

string File::filefullname() {
    return Name + "." + Extension;
}

properties File::returnProperties() {
    return properties(ishidden, chrono::system_clock::to_time_t(modified), false, 1, filefullname());
}

void File::changeAttribute(bool hide) {
    ishidden = hide;
}

vector<string> File::returnpath() {
    Folder* cd = parentFolder;
    vector<string> path;
    path.push_back(this->Name);
    while (cd != nullptr) {
        path.push_back(cd->foldername);
        cd = cd->parent;
    }
    for (int i = 0; i < path.size() / 2; i++) {
        swap(path[i], path[path.size() - 1 - i]);
    }
    path[path.size() - 1] += "." + Extension;
    return path;
}

void File::showtheAttribute() {
    if (ishidden) {
        cout << "H\t";
    }
    else {
        cout << " \t";
        vector<string> path = returnpath();
        for (int i = 0; i < path.size(); i++) {
            cout << path[i];

            if (i == 0) {
                cout << ":";
            }
            if (i != path.size() - 1) {
                cout << "\\";
            }
        }
    }
}

bool File::containstring(string input)
{
    int i = 0;
    for (auto it = data.begin(); it != data.end(); it++)
    {
        for (auto inner = it->begin(); inner != it->end(); inner++)
        {
            char ch = *inner;
            if (ch == input[i])
            {
                i++;
                if (i == input.size())
                {
                    return true;
                }
            }
            else
            {
                i = 0;
            }
        }
    }
    return false;
}

string File::returnSaveName()
{
    auto path = returnpath();

    string savename;

    for (int i = 0; i < path.size(); i++)
    {
        savename += path[i] + ((i != path.size() - 1) ? "_" : "");
    }
    return savename;
}
void File::saveSelf()
{
    string completePath = "D:\\3rd Semester\\DSA\\DOS Shell Final Project\\" + returnSaveName();
    ofstream fout(completePath);

    for (auto r = data.begin(); r != data.end(); r++)
    {
        for (auto c = (*r).begin(); c != (*r).end(); c++)
        {
            fout << *c;
        }
        fout << endl;
    }
}
