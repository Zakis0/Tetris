#include "error.h"

void error(string str) {
    cerr << str << endl;
    exit(-1);
}