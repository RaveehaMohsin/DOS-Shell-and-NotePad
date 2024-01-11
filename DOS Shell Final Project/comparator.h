#pragma once
#include <iostream>
#include "File.h"

using namespace std;

class File;

class Comparator 
{
public:
	bool operator()(File* a, File* b);
};
