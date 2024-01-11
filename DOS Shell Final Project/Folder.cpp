#include "Folder.h"
#include "File.h"
#include "Properties.h"
#include "DOS.h"

Folder::Folder(string name, Folder* parent_, list<Folder*> sf, list<File*> files, chrono::time_point<chrono::system_clock> created_, bool _prop)
{
    this->foldername = name;
    this->subfolders = sf;
    this->filenames = files;
    this->parent = parent_;
    this->created = created_;
    this->ishidden = _prop;
}

vector<string> Folder::returnpath() {
    Folder* currentfolderparent = parent;
    vector<string> _path;
    _path.push_back(this->foldername);
    while (currentfolderparent) {
        _path.push_back(currentfolderparent->foldername);
        currentfolderparent = currentfolderparent->parent;
    }

    reverse(_path.begin(), _path.end());

    return _path;
}

void Folder::makeDirectory(string name) {
    Folder* newFolder = new Folder(name, this);
    subfolders.push_back(newFolder);
}

void Folder::createFile(string name, string ext) {
    File* newFile = new File(name, this, ext);
    filenames.push_back(newFile);
}

Folder* Folder::findfolder(string name) {
    for (auto it = subfolders.begin(); it != subfolders.end(); ++it) {
        if ((*it)->foldername == name) {
            return *it;
        }
    }
    return nullptr;
}

File* Folder::findfile(string name, string ext) {
    for (auto it = filenames.begin(); it != filenames.end(); ++it) {
        if ((*it)->Name == name && (*it)->Extension == ext) {
            return *it;
        }
    }
    return nullptr;
}

void Folder::showtheAttribute() {
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

void Folder::changeAttrib(bool hide) {
    ishidden = hide;
}

properties Folder::returnProperties() {
    return properties(ishidden, chrono::system_clock::to_time_t(created), true, 0, foldername);
}

vector<properties> Folder::returningfolderfileproperties() {
    vector<properties> prop;

    for (auto itd = subfolders.begin(); itd != subfolders.end(); ++itd) {
        prop.push_back((*itd)->returnProperties());
    }

    for (auto itf = filenames.begin(); itf != filenames.end(); ++itf) {
        prop.push_back((*itf)->returnProperties());
    }

    return prop;
}

void Folder::convert(string ext1, string ext2) 
{
    for (auto itf = filenames.begin(); itf != filenames.end(); ++itf) {
        if ((*itf)->Extension == ext1) {
            (*itf)->Extension = ext2;
        }
    }
}

void Folder::deleteafile(string name, string ext) {
    for (auto itf = filenames.begin(); itf != filenames.end(); ++itf) 
    {
        if ((*itf)->Name == name && (*itf)->Extension == ext) {
            filenames.erase(itf);
            return;
        }
    }
    throw "File does not exist in the Directory";
}


void Folder::deleteafolder(string name) {
    for (auto itf = subfolders.begin(); itf != subfolders.end(); ++itf)
    {
        if ((*itf)->foldername == name ) 
        {
            subfolders.erase(itf);
            return;
        }
    }
    throw "Folder does not exist in the Directory";
}

void Folder::findallfiles(string input, vector<string>& paths)
{
    vector<string>filename = DOS::stringToVector(input, ".");
    if (filename.size() == 2)
    {
        File* f = findfile(filename[0], filename[1]);
        if (f)
        {
           vector<string>pathoffile =  f->returnpath();
           string temp;
           for (int idx = 0; idx < pathoffile.size(); idx++)
           {
               temp = temp + pathoffile[idx];
               if (idx == 0)
               {
                    temp = temp + ":";
               }
               else if (idx < pathoffile.size() - 1)
               {
                   temp = temp + "\\";
               }
           }
           paths.push_back(temp);
        }
        for (auto itd = subfolders.begin(); itd != subfolders.end(); ++itd)
        {
            (*itd)->findallfiles(input, paths);
        }
    }
    else
    {
        cout << "Incorrect format of file";
    }
}

void Folder::findallfileswithstring(string input, vector <string>& paths)
{
    for (auto itf = filenames.begin(); itf != filenames.end(); itf++)
    {
        if ((*itf)->containstring(input))
        {
           vector<string>path = (*itf)->returnpath();
           string temp;
           for (int idx = 0; idx < path.size(); idx++)
           {
               temp = temp + path[idx];
               if (idx == 0)
               {
                   temp = temp + ":";
               }
               else if (idx < path.size() - 1)
               {
                   temp = temp + "\\";
               }
           }
           paths.push_back(temp);
        }
    }
    for (auto itd = subfolders.begin(); itd != subfolders.end(); itd++)
    {
        (*itd)->findallfileswithstring(input, paths);
    }
}

void Folder::loadFile(string str)
{
    filenames.push_back(new File(str, this));
}


