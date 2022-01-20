#ifndef REGISTER_ALLOCATOR_H
#define REGISTER_ALLOCATOR_H
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct VRegUse
{
    vector<int> uselist;
    int maxUse;
};

class RegisterAllocator
{
private:
    int line=0;
    string registers[8] = {"", "", "", "", "", "", "", ""};
    const char names[9] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 0};
    map<string, VRegUse> usage;

    int inRegs(string name);
    int getFree();
public:

    void markUse(string name, int line);
    void clearUse();
    void prepareUseLists();
    void next();
    string allocate(string name);
    char get(string name);
};
#endif