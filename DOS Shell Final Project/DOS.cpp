#include "DOS.h"
#include "Folder.h"
#include "File.h"
#include "Properties.h"
#include <Windows.h>
#include"comparator.h" 
#include "Editor.h"
#include <fstream>




void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


DOS::DOS(string name) 
{
    currentFolder = root = new Folder(name);
    prompt = '\\';
}

void DOS::displayStudentDetails()
{
    gotoxy(35, 0);
    cout << setfill('*') << setw(40) << '*' << endl;
    gotoxy(35, 1);
    cout << setfill(' ') << left << setw(20) << "Name:" << "Raveeha Mohsin" << endl;
    gotoxy(35, 2);
    cout << left << setw(20) << "Roll Number:" << "2022-CS-149" << endl;
    gotoxy(35, 3);
    cout << left << setw(20) << "Course:" << "Data Structures & Algorithms" << endl;
    gotoxy(35, 4);
    cout << setfill('*') << setw(40) << '*' << endl;
}

void DOS::viewcurrentDirectory() {
    vector<string> currentpathofdirectory = currentFolder->returnpath();
    for (int i = 0; i < currentpathofdirectory.size(); i++) {
        cout << currentpathofdirectory[i];

        if (i == 0) {
            cout << ":";
        }
        if (i != currentpathofdirectory.size() - 1) {
            cout << prompt;
        }
    }
}

string DOS::extractCommand(string& input) {
    string res = input.substr(0, input.find(' '));
    if (input.find(' ') != string::npos) {
        input = input.substr(input.find(' ') + 1, input.size());
    }
    else {
        input.clear();
    }
    return res;
}

std::string DOS::toLower(const std::string& input) {
    std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return result;
}

void DOS::displaypath() {
    viewcurrentDirectory();
    cout << ">";
}

vector<string> DOS::stringToVector(string line, string deli) {
    vector<string> mult;
    if (line == "")
        return mult;
    string r;
    while (line.find(deli) != string::npos) {
        r = line.substr(0, line.find(deli));
        line = line.substr(line.find(deli) + deli.length(), line.size());
        mult.push_back(r);
    }
    mult.push_back(line);
    return mult;
}

void DOS::makeDirectory(string input) {
    vector<string> path = stringToVector(input, "\\");
    try {
        if (path[path.size() - 1] == "") {
            throw " Directory with an empty name can't be created";
        }
        else if (path.size() == 1) {
            Folder* temp = currentFolder->findfolder(path[path.size() - 1]);
            if (temp == nullptr) {
                temp = currentFolder;
                temp->makeDirectory(path[path.size() - 1]);
            }
            else {
                throw "Directory with the given name already exists";
            }
        }
        else if (path.size() > 1) {
            Folder* temp = currentFolder;
            for (int idx = 0; idx < path.size() - 1; idx++) {
                temp = temp->findfolder(path[idx]);
                if (idx == 0 && root->foldername == path[idx]) {
                    temp = root;
                }
                else if (temp == nullptr) {
                    throw "One or More Directories Does not exist in the Path!";
                }
            }
            if (temp->findfolder(path[path.size() - 1]) == nullptr) {
                temp->makeDirectory(path[path.size() - 1]);
            }
            else {
                throw "Directory with the given name already exists in the Destination";
            }
        }
        cout << "Directory Made Successfully!!";
    }
    catch (const char* exception) {
        cout << "Failed to Make Directory" << endl << exception << endl;
    }
}

void DOS::changeDirectory(string input) {
    vector<string> path = stringToVector(input, "\\");
    try {
        if (path.size() == 1) {
            Folder* temp = currentFolder->findfolder(path[0]);
            if (temp != nullptr) {
                currentFolder = temp;
            }
            else {
                throw "Directory Does not Exist!";
            }
        }
        else if (path.size() > 1) {
            Folder* temp = currentFolder;
            for (int idx = 0; idx < path.size(); idx++) {
                temp = temp->findfolder(path[idx]);
                if (idx == 0 && root->foldername == path[idx]) {
                    temp = root;
                }
                else if (temp == nullptr) {
                    throw "One or More Directories Do not exist in the Path!";
                }
            }
            currentFolder = temp;
        }
        else {
            cout << "Current Directory is: ";
            viewcurrentDirectory();
        }
    }
    catch (const char* exception) {
        cout << "Failed to Change Directory" << endl << exception << endl;
    }
}

void DOS::currentoPrevious() {
    if (currentFolder->parent) {
        currentFolder = currentFolder->parent;
    }
}

void DOS::currenttoroot() {
    currentFolder = root;
}

void DOS::changeattribute(string folderpath, bool hidden) {
    vector<string> path = stringToVector(folderpath, "\\");
    try {
        if (path[path.size() - 1] == "") {
            throw "Wrong or incomplete input";
        }
        if (path.size() == 1) {
            Folder* temp = currentFolder->findfolder(path[path.size() - 1]);
            if (temp == nullptr) {
                temp = currentFolder;
                vector<string> file = stringToVector(path[path.size() - 1], ".");
                if (file.size() == 2) {
                    File* f = temp->findfile(file[0], file[1]);

                    if (f == nullptr) {
                        throw "There exists no file with this name";
                    }
                    else {
                        f->changeAttribute(hidden);
                    }
                }
                else {
                    throw "There exists no file or folder with this name";
                }
            }
            else {
                temp->changeAttrib(hidden);
            }
        }
        if (path.size() > 1) {
            Folder* temp = currentFolder;
            for (int i = 0; i < path.size() - 1; i++) {
                temp = temp->findfolder(path[i]);
                if (i == 0 && root->foldername == path[i]) {
                    temp = root;
                }
                else if (temp == nullptr) {
                    throw "Invalid Path for File or Directory";
                }
            }
            Folder* prev = temp;
            temp = temp->findfolder(path[path.size() - 1]);
            if (temp == nullptr) {
                temp = prev;
                vector<string> file = stringToVector(path[path.size() - 1], ".");
                if (file.size() == 2) {
                    File* f = temp->findfile(file[0], file[1]);

                    if (f == nullptr) {
                        throw "There exists no file with this name";
                    }
                    else {
                        f->changeAttribute(hidden);
                    }
                }
                else {
                    throw "There exists no file or folder with this name";
                }
            }
            else {
                temp->changeAttrib(hidden);
            }
        }
    }
    catch (const char* exception) {
        cout << "Failed to Change Attribute" << endl << exception << endl;
    }
}

void DOS::showattribonly(string input) {
    vector<string> path = stringToVector(input, "\\");
    try {
        if (path[path.size() - 1] == "") {
            throw "Wrong or incomplete input";
        }
        if (path.size() == 1) {
            Folder* temp = currentFolder->findfolder(path[path.size() - 1]);
            if (temp == nullptr) {
                temp = currentFolder;
                vector<string> file = stringToVector(path[path.size() - 1], ".");
                if (file.size() == 2) {
                    File* f = temp->findfile(file[0], file[1]);

                    if (f == nullptr) {
                        throw "There exists no file with this name";
                    }
                    else {
                        f->showtheAttribute();
                    }
                }
                else
                {
                    throw "There exists no file or folder with this name";
                }
            }
            else {
                temp->showtheAttribute();
            }
        }
        if (path.size() > 1) {
            Folder* temp = currentFolder;
            for (int i = 0; i < path.size() - 1; i++) 
            {
                temp = temp->findfolder(path[i]);
                if (i == 0 && root->foldername == path[i]) 
                {
                    temp = root;
                }
                else if (temp == nullptr) 
                {
                    throw "Invalid Path for File or Directory";
                }
            }
            Folder* prev = temp;
            temp = temp->findfolder(path[path.size() - 1]);
            if (temp == nullptr) {
                temp = prev;
                vector<string> file = stringToVector(path[path.size() - 1], ".");
                if (file.size() == 2) {
                    File* f = temp->findfile(file[0], file[1]);

                    if (f == nullptr) {
                        throw "There exists no file with this name";
                    }
                    else {
                        f->showtheAttribute();
                    }
                }
                else {
                    throw "There exists no file or folder with this name";
                }
            }
            else {
                temp->showtheAttribute();
            }
        }
    }
    catch (const char* exception) {
        cout << "Failed to Change Attribute" << endl << exception << endl;
    }
}

void DOS :: printingCurrentfolderProperties(Folder* currFolder)
{
    int totaldirectories = 0;
    int totolfiles = 0;
    vector<string>path = currFolder->returnpath();
    cout << "Directory of ";
    for (int idx = 0; idx < path.size(); idx++)
    {
        cout << path[idx];
        if (idx == 0 && path[idx] == root->foldername)
        {
            cout <<":";
        }
        if (idx != path.size() - 1)
        {
            cout << prompt;
        }
    }
    cout << endl << endl;
    vector <properties> prop = currFolder->returningfolderfileproperties();
    for (int i = 0; i < prop.size(); i++)
    {
        if (!prop[i].ishidden)
        {
            std::tm localTime;
            localtime_s(&localTime, &prop[i].createTime);

            cout << std::put_time(&localTime, "%a %b %d, %Y %T") << " ";
            if (prop[i].isDir)
            {
                cout << "<DIR>";
                totaldirectories++;
            }
            else
            {
                cout << std::setw(15) << prop[i].size << " bytes";
                totolfiles++;
            }
            cout << "  " << prop[i].name << endl;
        }
    }
    cout << endl << endl;
    cout << totolfiles << "     Files" << endl;
    cout << totaldirectories << "     Directories" << endl;

}

void DOS::creatingfile(string input)
{
    vector <string>path = stringToVector(input, "\\");
    try 
    {
        if (path[path.size() - 1] == "")
        {
            throw "Wrong or incomplete input";
        }
        else if (path.size() == 1)
        {
            vector<string>filepath = stringToVector(input , ".");
            if (filepath.size() == 2 && (filepath[1] == "txt" || filepath[1] == "text"))
            {
                File* f = currentFolder->findfile(filepath[0], filepath[1]);
                if (f == nullptr)
                {
                    currentFolder->createFile(filepath[0], filepath[1]);
                }
                else
                {
                    throw "Already a file exists with the same name";
                }
            }
            else 
            {
                throw "Incorrect format of making a file";
            }
        }
        else if (path.size() > 1)
        {
            Folder* temp = currentFolder;
            for (int idx = 0; idx < path.size() - 1; idx++)
            {
                temp = temp->findfolder(path[idx]);
                if (idx == 0 && root->foldername == path[idx])
                {
                    temp = root;
                }
                else if (temp == nullptr)
                {
                    throw "There exists no file or folder with this name";
                }
            }
            vector<string> filepathh = stringToVector(path[path.size() - 1], ".");
            if (filepathh.size() == 2 && (filepathh[1] == "text" || filepathh[1] == "txt")) 
            {
                File* temp1 = temp->findfile(filepathh[0], filepathh[1]);
                if (temp1 == nullptr)
                {
                    temp->createFile(filepathh[0], filepathh[1]);
                }
                else
                {
                    throw "Already a file exists";
                }

                
            }
            else
            {
                throw "Incorrect Format of making a File";
            }
            
        }
    }
    catch (const char* exception) {
        cout << "Failed to Make File" << endl << exception << endl;
    }
}

void DOS:: deleteaFile(string input)
{
    vector <string>path = stringToVector(input, "\\");
    try
    {
        if (path[path.size() - 1] == "")
        {
            throw "Wrong or incomplete input";
        }
        else if (path.size() == 1)
        {
            vector<string>filepath = stringToVector(input, ".");
            if (filepath.size() == 2 && (filepath[1] == "txt" || filepath[1] == "text"))
            {
                File* f = currentFolder->findfile(filepath[0], filepath[1]);
                if (f == nullptr)
                {
                    throw "No such file exists in this directory";
                }
                else
                {
                    currentFolder->deleteafile(filepath[0],filepath[1]);
                }
            }
            else
            {
                throw "Incorrect format of file";
            }
        }
        else if (path.size() > 1)
        {
            Folder* temp = currentFolder;
            for (int idx = 0; idx < path.size() - 1; idx++)
            {
                temp = temp->findfolder(path[idx]);
                if (idx == 0 && root->foldername == path[idx])
                {
                    temp = root;
                }
                else if (temp == nullptr)
                {
                    throw "There exists no file or folder with this name";
                }
            }
            vector<string> filepathh = stringToVector(path[path.size() - 1], ".");
            if (filepathh.size() == 2 && (filepathh[1] == "text" || filepathh[1] == "txt"))
            {
                File* temp1 = temp->findfile(filepathh[0], filepathh[1]);
                if (temp1 == nullptr)
                {
                    throw "No such file exists";
                }
                else
                {
                    temp->deleteafile(filepathh[0], filepathh[1]);
                }


            }
            else
            {
                throw "Incorrect Format of making a File";
            }

        }
    }
    catch (const char* exception) {
        cout << "Failed to Make File" << endl << exception << endl;
    }
}

void DOS::convertExtensions(string input)
{
    vector <string>path = stringToVector(input, "\\");
    try
    {
        if (path[path.size() - 1] == "")
        {
            throw "Wrong or incomplete input";
        }
        else if (path.size() == 1)
        {
            vector<string>filepath = stringToVector(input, ".");
            if (filepath.size() == 2)
            {
                if (filepath[0] != "txt" && filepath[0] != "text")
                {
                    throw "First Extension is Invalid";
                }
                if (filepath[1] != "txt" && filepath[1] != "text")
                {
                    throw "Second Extension is Invalid";
                }
                currentFolder->convert(filepath[0], filepath[1]);
            }
            else
            {
                throw "Incorrect format";
            }
        }
        else if (path.size() > 1)
        {
            Folder* temp = currentFolder;
            for (int idx = 0; idx < path.size() - 1; idx++)
            {
                temp = temp->findfolder(path[idx]);
                if (idx == 0 && root->foldername == path[idx])
                {
                    temp = root;
                }
                else if (temp == nullptr)
                {
                    throw "There exists no file or folder with this name";
                }
            }
            vector<string> filepathh = stringToVector(path[path.size() - 1], ".");
            if (filepathh.size() == 2)
            {
                if (filepathh[0] != "txt" && filepathh[0] != "text")
                {
                    throw "First Extension is Invalid";
                }
                if (filepathh[1] != "txt" && filepathh[1] != "text")
                {
                    throw "Second Extension is Invalid";
                }
                temp->convert(filepathh[0], filepathh[1]);
            }
            else
            {
                throw "Incorrect Format";
            }

        }
    }
    catch (const char* exception) {
        cout << "Failed for Conversion" << endl << exception << endl;
    }
}



void DOS::deleteaFolder(string input) 
{
    vector<string> path = stringToVector(input, "\\");
    try {
        if (path[path.size() - 1] == "") {
            throw " Directory with an empty name can't be deleted";
        }
        else if (path.size() == 1) {
            Folder* temp = currentFolder->findfolder(path[path.size() - 1]);
            if (temp == nullptr) 
            {
                throw "No such Directory is Found";
            }
            else {
                currentFolder->deleteafolder(path[0]);
            }
        }
        else if (path.size() > 1) {
            Folder* temp = currentFolder;
            for (int idx = 0; idx < path.size() - 1; idx++) {
                temp = temp->findfolder(path[idx]);
                if (idx == 0 && root->foldername == path[idx]) {
                    temp = root;
                }
                else if (temp == nullptr) {
                    throw "One or More Directories Does not exist in the Path!";
                }
            }
            if (temp->findfolder(path[path.size() - 1]) == nullptr) 
            {
                throw "Directory couldn't be found";
            }
            else {
                temp->deleteafolder(path[path.size() - 1]);
            }
        }
        cout << "Directory Deleted Successfully!!";
    }
    catch (const char* exception) {
        cout << "Failed to delete Directory" << endl << exception << endl;
    }
}

void DOS::copyFile(string input)
{
    File* copiedfile;
    vector <string> fullpath = stringToVector(input, " ");
    try {
        if (fullpath.size() == 2)
        {
            vector<string> firstpath = stringToVector(fullpath[0], "\\");
            if (firstpath[firstpath.size() - 1] == "")
            {
                throw "Invalid Syntax";
            }
            if (firstpath.size() == 1)
            {
                vector<string>firstfilename = stringToVector(firstpath[0], ".");
                if (firstfilename.size() == 2 && (firstfilename[1] == "txt" || firstfilename[1] == "text"))
                {
                    File* f = currentFolder->findfile(firstfilename[0], firstfilename[1]);
                    if (f == nullptr)
                    {
                        throw "Already a file exists with the same name";
                    }
                    else
                    {
                        copiedfile = f;
                      
                    }
                }
                else
                {
                    throw "Incorrect format";
                }

            }
            if (firstpath.size() > 1)
            {
                Folder* temp = currentFolder;
                for (int idx = 0; idx < firstpath.size() - 1; idx++)
                {
                    temp = temp->findfolder(firstpath[idx]);
                    if (idx == 0 && root->foldername == firstpath[idx])
                    {
                        temp = root;
                    }
                    else if (temp == nullptr)
                    {
                        throw "There exists no file or folder with this name";
                    }
                }
                vector<string> filepathh = stringToVector(firstpath[firstpath.size() - 1], ".");
                if (filepathh.size() == 2 && (filepathh[1] == "text" || filepathh[1] == "txt"))
                {
                    File* temp1 = temp->findfile(filepathh[0], filepathh[1]);
                    if (temp1 == nullptr)
                    {
                        throw "No such file exists";
                    }
                    else
                    {
                        copiedfile = temp1;
                    }
                }
                else
                {
                    throw "Incorrect Format";
                }
            }

            vector<string> secondpath = stringToVector(fullpath[1], "\\");
            if (secondpath[secondpath.size() - 1] == "")
            {
                throw "Invalid Syntax";
            }
            if (secondpath.size() == 1)
            {
                if (secondpath[0] == root->foldername)
                {
                    root->filenames.push_back(copiedfile);
                }
                else
                {
                    Folder* temp = currentFolder->findfolder(secondpath[0]);
                    if (temp != nullptr)
                    {
                        temp->filenames.push_back(copiedfile);
                    }
                    else
                    {
                        throw "Folder not found ";
                    }
                }

            }
            if (secondpath.size() > 1)
            {
                Folder* temp = currentFolder;
                for (int idx = 0; idx < secondpath.size(); idx++)
                {
                    temp = temp->findfolder(secondpath[idx]);
                    if (idx == 0 && root->foldername == secondpath[idx])
                    {
                        temp = root;
                    }
                    else if (temp == nullptr)
                    {
                        throw "There exists no file or folder with this name";
                    }
                }
                temp->filenames.push_back(copiedfile);
            }
        }
        else 
        {
            throw "Invalid Format";
        }
    }
    catch (const char* exception) {
        cout << "Failed to copy file" << endl << exception << endl;
    }
}

void DOS::moveFile(string input)
{
    File* movedFile;
    vector <string> fullpath = stringToVector(input, " ");
    try {
        if (fullpath.size() == 2)
        {
            vector<string> firstpath = stringToVector(fullpath[0], "\\");
            if (firstpath[firstpath.size() - 1] == "")
            {
                throw "Invalid Syntax";
            }
            if (firstpath.size() == 1)
            {
                vector<string>firstfilename = stringToVector(firstpath[0], ".");
                if (firstfilename.size() == 2 && (firstfilename[1] == "txt" || firstfilename[1] == "text"))
                {
                    File* f = currentFolder->findfile(firstfilename[0], firstfilename[1]);
                    if (f == nullptr)
                    {
                        throw "Already a file exists with the same name";
                    }
                    else
                    {
                        movedFile= f;
                        currentFolder->filenames.remove(f);

                    }
                }
                else
                {
                    throw "Incorrect format";
                }

            }
            if (firstpath.size() > 1)
            {
                Folder* temp = currentFolder;
                for (int idx = 0; idx < firstpath.size() - 1; idx++)
                {
                    temp = temp->findfolder(firstpath[idx]);
                    if (idx == 0 && root->foldername == firstpath[idx])
                    {
                        temp = root;
                    }
                    else if (temp == nullptr)
                    {
                        throw "There exists no file or folder with this name";
                    }
                }
                vector<string> filepathh = stringToVector(firstpath[firstpath.size() - 1], ".");
                if (filepathh.size() == 2 && (filepathh[1] == "text" || filepathh[1] == "txt"))
                {
                    File* temp1 = temp->findfile(filepathh[0], filepathh[1]);
                    if (temp1 == nullptr)
                    {
                        throw "No such file exists";
                    }
                    else
                    {
                        movedFile = temp1;
                        temp->filenames.remove(temp1);
                    }
                }
                else
                {
                    throw "Incorrect Format";
                }
            }

            vector<string> secondpath = stringToVector(fullpath[1], "\\");
            if (secondpath[secondpath.size() - 1] == "")
            {
                throw "Invalid Syntax";
            }
            if (secondpath.size() == 1)
            {
                if (secondpath[0] == root->foldername)
                {
                    root->filenames.push_back(movedFile);
                }
                else
                {
                    Folder* temp = currentFolder->findfolder(secondpath[0]);
                    if (temp != nullptr)
                    {
                        temp->filenames.push_back(movedFile);
                    }
                    else
                    {
                        throw "Folder not found ";
                    }
                }

            }
            if (secondpath.size() > 1)
            {
                Folder* temp = currentFolder;
                for (int idx = 0; idx < secondpath.size(); idx++)
                {
                    temp = temp->findfolder(secondpath[idx]);
                    if (idx == 0 && root->foldername == secondpath[idx])
                    {
                        temp = root;
                    }
                    else if (temp == nullptr)
                    {
                        throw "There exists no file or folder with this name";
                    }
                }
                temp->filenames.push_back(movedFile);
            }
        }
        else
        {
            throw "Invalid Format";
        }
    }
    catch (const char* exception) {
        cout << "Failed to copy file" << endl << exception << endl;
    }
}

void DOS::changePrompt()
{
    if (prompt == '\\')
    {
        prompt = '$';
    }
    else
    {
        prompt = '\\';
    }

}

void DOS::findfile(string input)
{
    try 
    {
        vector<string>filename = stringToVector(input, ".");
        if (filename.size() != 2)
        {
            throw "Incorrect Format";
        }
        else if (filename[1] != "text" && filename[1] != "txt")
        {
            throw "Invalid Name of File";
        }
        vector <string>paths;
        root->findallfiles(input, paths);
        if (paths.size() == 0)
        {
            throw "File does not exists in the Virtual Directory.";
        }
        cout << "File(s) Found:" << endl;
        for (int idx = 0; idx < paths.size(); idx++)
        {
            cout << paths[idx] << endl;
        }
        cout << endl << "Total Files found in Drive: " << paths.size() << endl;

    }
    catch (const char* exception) {
        cout << "Couldn't find file: " << endl << exception << endl;
    }

}

void DOS::findstringinallfile(string input)
{
    vector<string>path;
    root->findallfileswithstring(input, path);
    if (path.size() == 0)
    {
        cout << "String Not Found in any file" << endl;
        return;
    }
    cout << "String Found in Following Files: " << endl << endl;
    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i] << endl;
    }
    cout << endl;
    cout << "String Found in File(s): " << path.size() << endl;
}

void DOS::findstringinonefile(string input)
{
    string filenaam = extractCommand(input);
    vector<string>filepath = stringToVector(filenaam, "\\");
    try
    {
        if (filepath[filepath.size() - 1] == "")
        {
            throw "Invalid File.";
        }
        else if (filepath.size() == 1)
        {
            vector <string>filename = stringToVector(filepath[filepath.size() - 1], ".");
            if (filename.size() == 2 && (filename[1] == "text" || filename[1] == "txt"))
            {
                File* f = currentFolder->findfile(filename[0], filename[1]);
                if (f == nullptr)
                {
                    throw "File doesnt exist.";
                }
                else
                {
                    if (f->containstring(input))
                    {
                        cout << "String is Found in the File" << endl;
                    }
                    else
                    {
                        cout << "No such string is Found" << endl;
                    }
                }
            }
        }
        else if (filepath.size() > 1)
        {
            Folder* temp = currentFolder;
            for (int idx = 0; idx < filepath.size() - 1; idx++)
            {
                temp = temp->findfolder(filepath[idx]);
                if (idx == 0 && root->foldername == filepath[idx])
                {
                    temp = root;
                }
                else if (temp == nullptr)
                {
                    throw "Invalid.Folder doesnt exists.";
                }
            }
            vector <string>filename = stringToVector(filepath[filepath.size() - 1], ".");
            if (filename.size() == 2 && (filename[1] == "text" || filename[1] == "txt"))
            {
                File* temp1 = temp->findfile(filename[0], filename[1]);
                if (temp1 == nullptr)
                {
                    throw "No such file exists";
                }
                else
                {
                    if (temp1->containstring(input))
                    {
                        cout << "String is Found in the File" << endl;
                    }
                    else
                    {
                        cout << "No such string is found in the file" << endl;
                    }           
                }
            }
            else
            {
                throw "Incorrect Format of  File";
            }
        }
    }
    catch (const char* exception)
    {
        cout << "Couldn't find file: " << endl << exception << endl;
    }
}

void DOS::formatDrive()
{
    string name = root->foldername;
    delete root;
    root = currentFolder = new Folder(name);
}

void DOS::renameFile(string input) 
{
    File* f;
    vector<string> names = stringToVector(input, " ");
    try 
    {

        vector<string>firstname = stringToVector(names[0], ".");
        if (firstname.size() == 2 && (firstname[1] == "txt" || firstname[1] == "text"))
        {
            f = currentFolder->findfile(firstname[0], firstname[1]);
            if (f == nullptr)
            {
                throw "File couldnt be found.";
            }
        }
        else
        {
            throw "Invalid File format.";
        }
        vector<string>secondname = stringToVector(names[1], ".");
        if (secondname.size() == 2 && (secondname[1] == "txt" || secondname[1] == "text"))
        {
            f->Name = secondname[0];
            f->Extension = secondname[1];
        }       
        else
        {
            throw "Invalid File format.";
        }
    }
    catch (const char* exception)
    {
        cout << "Couldn't rename file: " << endl << exception << endl;
    }
}

void DOS::displayDriveTree(Folder* r, int depth)
{
    if (!r)
        return;

    for (auto childDirIt = r->subfolders.begin(); childDirIt != r->subfolders.end(); ++childDirIt)
    {
        Folder* temp = *childDirIt;
        cout << "|";
        for (int i = 0; i < depth; i++)
        {
            cout << "\t";
        }
        cout << "|--";
        cout << temp->foldername << endl;

        displayDriveTree(temp, depth + 1);
    }

    for (auto fileIt = r->filenames.begin(); fileIt != r->filenames.end(); ++fileIt)
    {
        File* temp = *fileIt;
        cout << "|";
        for (int i = 0; i < depth; i++)
        {
            cout << "\t";
        }
        cout << "|--";
        cout << temp->Name << "." << temp->Extension << endl;
    }
}

void DOS::insertinQueue(string input)
{
    vector<string>path = stringToVector(input, " ");
    vector<string>filepath = stringToVector(input, "\\");
    try
    {
        if (path.size() > 1)
        {
            throw "Invalid format.";
        }
        if (filepath[filepath.size() - 1] == "")
        {
            throw "Invalid File.";
        }
        else if (filepath.size() == 1)
        {
            vector <string>filename = stringToVector(filepath[filepath.size() - 1], ".");
            if (filename.size() == 2 && (filename[1] == "text" || filename[1] == "txt"))
            {
                File* f = currentFolder->findfile(filename[0], filename[1]);
                if (f == nullptr)
                {
                    throw "File doesnt exist.";
                }
                else
                {
                    qprint.push_back(f);
                    f->time = 50;
                    cout << "File Added to the Print Queue" << endl;
                }
            }
        }
        else if (filepath.size() > 1)
        {
            Folder* temp = currentFolder;
            for (int idx = 0; idx < filepath.size() - 1; idx++)
            {
                temp = temp->findfolder(filepath[idx]);
                if (idx == 0 && root->foldername == filepath[idx])
                {
                    temp = root;
                }
                else if (temp == nullptr)
                {
                    throw "Invalid.Folder doesnt exists.";
                }
            }
            vector <string>filename = stringToVector(filepath[filepath.size() - 1], ".");
            if (filename.size() == 2 && (filename[1] == "text" || filename[1] == "txt"))
            {
                File* temp1 = temp->findfile(filename[0], filename[1]);
                if (temp1 == nullptr)
                {
                    throw "No such file exists";
                }
                else
                {
                    qprint.push_back(temp1);
                    temp1->time = 50;
                    cout << "File Added to the Print Queue" << endl;
                }
            }
            else
            {
                throw "Incorrect Format of  File";
            }
        }
    }
    catch (const char* exception)
    {
        cout << "Couldn't insert in queue: " << endl << exception << endl;
    }
 
}

void DOS:: displayQueue()
{
    static int time = 5;
    cout << endl << setw(40) << left << "Filename" << setw(0) << left << "Time" << endl;
    if (qprint.empty())
        return;
    auto print_file = qprint.front();
    print_file->time -= time;
    if (print_file->time <= 0)
    {
        print_file->time = 0;
        qprint.pop_front();
    }
    string t1;
    for (File* temp : qprint)
    {
        t1 = temp->Name + "." + temp->Extension;
        cout << setw(40) << left << t1 << setw(0) << left << temp->time << " seconds" << endl;
    }
    cout << endl;
}

void DOS::insertinPriorityqueue(string input)
{
    vector<string>fullpath = stringToVector(input, " ");
    try {
        if (fullpath.size() == 2)
        {
            vector<string>filename = stringToVector(fullpath[0], ".");
            if (filename.size() == 2)
            {
               File* f= currentFolder->findfile(filename[0], filename[1]);
               if (f == nullptr)
               {
                   throw "File couldnt be found";
               }
               else
               {
                   if (fullpath[1] == "H" || fullpath[1] == "h")
                   {
                       f->priority = 1;
                   }
                   else if (fullpath[1] == "L" || fullpath[1] == "l")
                   {
                       f->priority = 0;
                   }
                   pqueue.push(f);
                   f->time = 50;
                   cout << "File Added to the Priority Print Queue" << endl;
               }
            }
        }
        else
        {
           throw "Invalid Format.Give the file name and Priority!";
        }
    }
    catch (const char* exception)
    {
        cout << "Couldn't insert in queue: " << endl << exception << endl;
    }

}

void DOS::displayPqueue()
{
    static int time = 5;
    cout << endl << setw(40) << left << "Filename" << setw(0) << left << "Time" << endl;
    if (pqueue.empty())
        return;
    else
    {
        auto f = pqueue.top();
        f->time = f->time - time;
        if (f->time <= 0)
        {
            f->time = 0;
            pqueue.pop();
        }
        auto temp = pqueue;
        string t1;
        while (temp.size() != 0)
        {
            t1 = temp.top()->Name + "." + temp.top()->Extension;
            cout << setw(50) << left << t1 << setw(0) << left << temp.top()->time << " seconds" << endl;
            temp.pop();
        }
        cout << endl;

    }
}

void DOS::editFile(string input)
{
    vector<string>filename1 = stringToVector(input, ".");
    File* f;
    try 
    {
        if (filename1.size() != 2)
        {
            throw "Invalid Format";
        }
        else if (filename1[1] != "text" && filename1[1] != "txt")
        {
            throw "Invalid Format";
        }
        else if (filename1.size() == 2)
        {
           f =  currentFolder->findfile(filename1[0], filename1[1]);
           if (f == nullptr)
           {
               throw "The File does not exists in the present directory";
           }
           else
           {
               if (f->edit == nullptr)
               {
                   f->edit = new Editor(f, this);
               }
               f->edit->Edit();
           }

        }
    }
    catch (const char* exception)
    {
        cout << "Couldn't open file for editting: " << endl << exception << endl;
    }

}
void DOS::openThefileEditor(bool next , File* f)
{
    bool isfound = false;
    if (next)
    {
        for (auto it = currentFolder->filenames.begin(); it != currentFolder->filenames.end();)
        {
            if (isfound)
            {
            A:
                if (!(*it)->edit)
                    (*it)->edit = new Editor((*it), this);
                (*it)->edit->Edit();
                return;
            }
            if ((*it) == f)
            {
                isfound = true;
            }
            it++;
            if (it == currentFolder->filenames.end())
            {
                it = currentFolder->filenames.begin();
                goto A;
            }
        }
    }
    else
    {
        auto it = currentFolder->filenames.rbegin();
        for (;it != currentFolder->filenames.rend();)
        {
            if (isfound)
            {
            B:
                if (!(*it)->edit)
                    (*it)->edit = new Editor((*it), this);
                (*it)->edit->Edit();
                return;
            }
            if ((*it) == f)
            {
                isfound = true;
            }
            it++;
            if (it == currentFolder->filenames.rend())
            {
                it = currentFolder->filenames.rbegin();
                goto B;
            }
        }
    }
}


void DOS::saveTreeRecursion(Folder* r, ofstream& fout, int depth)
{
    if (!r)
        return;
    for (Folder* temp : r->subfolders)
    {
        for (int i = 0; i < depth; i++)
        {
            fout << "-" << " ";
        }
        fout << "<DIR> " << temp->foldername << endl;
        saveTreeRecursion(temp, fout, depth + 1);
    }
    for (File* temp : r->filenames)
    {
        for (int i = 0; i < depth; i++)
        {
            fout << "-" << " ";
        }
        fout << "<File> " << temp->returnSaveName() << endl;
        temp->saveSelf();
    }
}

void DOS::saveTree()
{

    string name = "tree.txt";
    string path = "D:\\3rd Semester\\DSA\\DOS Shell Final Project\\" + name;
    ofstream fout(path);
    saveTreeRecursion(root, fout, 0);
    cout << "Tree Saved Successfully" << endl;

}

void DOS::loadTreeLoop(Folder* r, ifstream& fin)
{
    int depth = 0;
    int prev_depth = 0;
    string c;
    string name;
    bool check = false;
    while (fin.good())
    {
        fin >> c;

        if (fin.eof())
            break;
        while (c == "-")
        {
            fin >> c;
            depth++;
            check = true;
        }
        if (check)
        {
            if (depth > prev_depth)
                r = r->findfolder(name);
            else if (depth < prev_depth)
                r = r->parent;
        }
        else
        {
            while (r != root)
            {

                r = r->parent;
            }
        }
        if (c == "<DIR>")
        {
            fin >> name;
            r->makeDirectory(name);
            check = false;
        }
        else if (c == "<File>")
        {
            fin >> name;

            r->loadFile(name);
            check = false;
        }
        prev_depth = depth;
        depth = 0;
    }
}
void DOS::loadTree()
{

    string name = "tree.txt";
    string path = "D:\\3rd Semester\\DSA\\DOS Shell Final Project\\" + name;
    ifstream fin(path);
    formatDrive();
    loadTreeLoop(root, fin);
    currentFolder = root;

}

void DOS::help(string command)
{
    string input = toLower(command);
    if (input == "cd")
    {
        cout << "CD - Changes Directory" << endl;
        cout << "Syntax: CD [Path from current directory or Path from root]" << endl;
        cout << "Alternative Syntax: CD\t\tShows Current Directory" << endl;
    }
    else if (input == "cd.")
    {
        cout << "CD. - Shows Current Directory" << endl;
        cout << "Syntax: CD." << endl;
    }
    else if (input == "cd..")
    {
        cout << "CD.. - Changes Directory to Parent" << endl;
        cout << "Syntax: CD.." << endl;
    }
    else if (input == "cd\\")
    {
        cout << "CD\\ - Changes Directory to the Virtual Drive" << endl;
        cout << "Syntax: CD\\" << endl;
    }
    else if (input == "mkdir")
    {
        cout << "MKDIR - Makes a Directory" << endl;
        cout << "Syntax: MKDIR [Path from current directory or Path from root]\\Dir_Name " << endl;
    }
    else if (input == "attrib")
    {
        cout << "ATTRIB - Shows or Changes Attributes of a File or Directory" << endl;
        cout << "Syntax: ATTRIB [+/-:H] [Path from current directory or Path from root]\\Name " << endl;
    }
    else if (input == "dir")
    {
        cout << "DIR - Shows List in detail of sub-directories and files" << endl;
        cout << "Syntax: DIR [Path from current directory or Path from root]" << endl;
    }
    else if (input == "create")
    {
        cout << "CREATE - Creates a file in the present directory" << endl;
        cout << "Syntax: CREATE [filename].[extension]" << endl;
        cout << "Only \'.txt\' and \'.text\' extensions are allowed" << endl;
    }
    else if (input == "pwd")
    {
        cout << "PWD - Shows Present Working Directory" << endl;
        cout << "Syntax: PWD" << endl;
    }
    else if (input == "del")
    {
        cout << "DEL - Deletes a file from current directory" << endl;
        cout << "Syntax: DEL [filename].[extension]" << endl;
    }
    else if (input == "ver")
    {
        cout << "VER - Shows version of the program" << endl;
        cout << "Syntax: VER" << endl;
    }
    else if (input == "convert")
    {
        cout << "CONVERT - Converts extensions of all files in present directory" << endl;
        cout << "Syntax: CONVERT [extension1].[extension2]" << endl;
    }
    else if (input == "help")
    {
        cout << "HELP - Shows syntax of a command and its use" << endl;
        cout << "Syntax: HELP [Command_Name]" << endl;
    }
    else if (input == "prompt")
    {
        cout << "PROMPT - Changes Prompt" << endl;
        cout << "Syntax: PROMPT" << endl;
    }
    else if (input == "cls")
    {
        cout << "CLS - Clears Screen" << endl;
        cout << "Syntax: CLS" << endl;
    }
    else if (input == "exit")
    {
        cout << "EXIT - Ends the program" << endl;
        cout << "Syntax: EXIT" << endl;
    }
    else if (input == "copy")
    {
        cout << "COPY - Copies a file and pastes it in the provided destination" << endl;
        cout << "Syntax: COPY [filename].[extension] [Path from current directory or Path from root]" << endl;
    }
    else if (input == "edit")
    {
        cout << "EDIT - Opens the File in Editor" << endl;
        cout << "Syntax: EDIT [filename].[extension]" << endl;
    }
    else if (input == "find")
    {
        cout << "FIND - Finds the File in in the whole Drive" << endl;
        cout << "Syntax: FIND [filename].[extension]" << endl;
    }
    else if (input == "queue")
    {
        cout << "QUEUE - Displays print queue" << endl;
        cout << "Syntax: QUEUE" << endl;
    }
    else if (input == "pqueue")
    {
        cout << "PQUEUE - Displays priority print queue" << endl;
        cout << "Syntax: PQUEUE" << endl;
    }
    else if (input == "print")
    {
        cout << "PRINT - Adds file to the print queue" << endl;
        cout << "Syntax: PRINT [filename].[extension]" << endl;
    }
    else if (input == "pprint")
    {
        cout << "PPRINT - Adds file to the priority print queue" << endl;
        cout << "Syntax: PPRINT  [filename].[extension] [H/L]" << endl;
    }
    else if (input == "findf")
    {
        cout << "FINDF - Finds string in the file" << endl;
        cout << "Syntax: FINDF [filename].[extension] [string]" << endl;
    }
    else if (input == "findstr")
    {
        cout << "FINDSTR - Finds string in the all the files in Drive" << endl;
        cout << "Syntax: FINDSTR [string]" << endl;
    }
    else if (input == "rename")
    {
        cout << "RENAME - Renames the file in present in current directory" << endl;
        cout << "Syntax: RENAME [filename].[extension] [newfilename]" << endl;
    }
    else if (input == "rmdir")
    {
        cout << "RMDIR - Removes directory from a path or current directory" << endl;
        cout << "Syntax: RMDIR [Path from current directory or Path from root]\\Dir_Name" << endl;
    }
    else if (input == "format")
    {
        cout << "FORMAT - Formats the Drive" << endl;
        cout << "Syntax: FORMAT" << endl;
    }
    else if (input == "tree")
    {
        cout << "TREE - Show the tree of the whole Drive" << endl;
        cout << "Syntax: TREE" << endl;
    }
    else if (input == "save")
    {
        cout << "SAVE - Saves the Whole Drive on the Disk" << endl;
        cout << "Syntax: SAVE" << endl;
    }
    else if (input == "load")
    {
        cout << "LOAD - Loads the Drive from the Disk" << endl;
        cout << "Syntax: LOAD" << endl;
    }
    else
    {
        cout << "Command is not recognized" << endl;
    }

}
void DOS::main() {
    system("cls");
    displayStudentDetails();
    string input;
    string command;
    system("Color 0F");
    while (true) 
    {
        displaypath();
        getline(cin, input);
        command = extractCommand(input);
        command = toLower(command);

        if (command == "cd") {
            changeDirectory(input);
            cout << endl;
        }
        else if (command == "cd.") {
            cout << "Current Working Directory is: ";
            viewcurrentDirectory();
            cout << endl;
        }
        else if (command == "cd..") {
            currentoPrevious();
            cout << endl;
        }
        else if (command == "cd\\") {
            currenttoroot();
            cout << endl;
        }
        else if (command == "mkdir" || command == "md") {
            makeDirectory(input);
            cout << endl;
        }
        else if (command == "attrib") {
            vector<string> attribute = stringToVector(input, " ");
            if (attribute.size() == 2 && (attribute[0] == "+h" || attribute[0] == "-h")) {
                if (toLower(attribute[0]) == "+h") {
                    changeattribute(attribute[1], true);
                }
                else if (toLower(attribute[0]) == "-h") {
                    changeattribute(attribute[1], false);
                }
            }
            else if (attribute.size() == 1) {
                showattribonly(attribute[0]);
            }
            else {
                cout << "Invalid Input!!";
            }
            cout << endl;
        }
        else if (command == "dir")
        {
            vector <string>path = stringToVector(input, "\\");
            if (input == "")
            {
                printingCurrentfolderProperties(currentFolder);
                cout << endl;
            }
            else if (path.size() > 0)
            {
                Folder* temp = currentFolder;
                try
                {
                    for (int idx = 0; idx < path.size(); idx++)
                    {
                        temp = temp->findfolder(path[idx]);
                        if (idx == 0 && path[idx] == root->foldername)
                        {
                            temp = root;
                        }
                        if (temp == nullptr)
                        {
                            throw "No such file or folder in the path";

                        }
                    }
                    printingCurrentfolderProperties(temp);
                }
                catch (const char* exception) {
                    cout << "Failed to show directory properties!" << endl << exception << endl;
                }
                

            }
        }
        else if (command == "create")
        {
            creatingfile(input);
            cout << endl;
        }
        else if (command == "pwd")
        {
            cout << "Present Working Directory : ";
            viewcurrentDirectory();
            cout << endl;
        }
        else if (command == "ver")
        {
            cout << "Microsoft Windows [Version 10.0.19045.3803]";
            cout << endl;
        }
        else if (command == "del")
        {
            deleteaFile(input);
            cout << endl;
        }
        else if (command == "convert")
        {
            convertExtensions(input);
            cout << endl;
        }
        else if (command == "rmdir")
        {
            deleteaFolder(input);
            cout << endl;
        }
        else if (command == "copy")
        {
            copyFile(input);
            cout << endl;
        }

        else if (command == "move")
        {
            moveFile(input);
            cout << endl;
        }
        else if (command == "prompt")
        {
            changePrompt();
            cout << endl;
        }
        else if (command == "edit")
        {
            editFile(input);
            cout << endl;
        }
        else if (command == "find")
        {
            findfile(input);
            cout << endl;
        } 
        else if(command == "findf")
        {
              findstringinonefile(input);
              cout << endl;
        }
        else if (command == "findstr")
        {
            findstringinallfile(input);
            cout << endl;
        }
        else if (command == "format")
        {
            formatDrive();
            cout << endl;
        }      
        else if (command == "cls")
        {
            system("cls");
            displayStudentDetails();
        }
        else if (command == "rename")
        {
            renameFile(input);
            cout << endl;
        }
        else if (command == "tree")
        {
            cout << root->foldername;
            displayDriveTree(root, 0);
            cout << endl;
        }   
        else if (command == "print")
        {
            insertinQueue(input);
            cout << endl;
        }
        else if (command == "queue")
        {
            displayQueue();
            cout << endl;
        }
        else if (command == "pprint")
        {
            insertinPriorityqueue(input);
            cout << endl;
        }
        else if (command == "pqueue")
        {
            displayPqueue();
            cout << endl;
        }
        else if (command == "help")
        {
            help(input);
            cout << endl;
        }
        else if (command == "save")
        {
            saveTree();
            cout << endl;
        }
        else if (command == "load")
        {
            loadTree();
            cout << endl;
        }       
        else if (command == "exit") 
        {
            break;
        }
    }
}

