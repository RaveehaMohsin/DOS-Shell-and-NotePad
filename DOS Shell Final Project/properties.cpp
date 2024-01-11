#include "Properties.h"

properties::properties(bool hid, time_t time, bool isd, int s, string _name) {
    this->ishidden = hid;
    this->createTime = time;
    this->isDir = isd;
    this->size = s;
    this->name = _name;
}
