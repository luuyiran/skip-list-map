#include<stdio.h>
#include <time.h>
#include <map>
#include "skp_map.hpp"

static void test1() {
    skp_map<int, int> mmap;
    for (int i = 0; i < 20; i++)
        mmap.insert(i, i);

    mmap.print();
    for (int i = 0; i < 20; i++) {
        mmap.find(i);
        mmap.erase(i);
        printf("=============\n\n");
        mmap.print();
    }
}

int main() {

    test1();
    
    return 0;
}