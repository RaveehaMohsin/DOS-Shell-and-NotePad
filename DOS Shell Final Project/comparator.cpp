#include<iostream>
#include "Comparator.h"
#include "File.h"
using namespace std;

bool Comparator::operator()(File* a, File* b) 
{
    return (a->priority < b->priority);
}
