#include "RegisterAllocator.h"

int RegisterAllocator::inRegs(string name)
{
    for (int i = 0; i < 7; i++)
        if (name == this->registers[i])
            return i;
    return 8;
}

int RegisterAllocator::getFree()
{
    for (int i = 0; i < 7; i++)
    {
        if (registers[i] == "")
            return i;

        // TODO WILL WORK WHEN ALLOCATION IN BASIC BLOCKS OTHERWISE LOOPS WILL CAUSE DATA LOSS
        if (usage[registers[i]].maxUse < this->line)
        {
            return i;
        }
    }
    return -1;
}

void RegisterAllocator::markUse(string name, int line)
{
    this->usage[name].uselist.push_back(line);
    if (this->usage[name].maxUse < line)
    {
        this->usage[name].maxUse = line;
    }
}

void RegisterAllocator::clearUse()
{
    this->usage.clear();
    // for (auto use : this->usage)
    // {
    //     use.second.uselist.clear();
    // }
}

void RegisterAllocator::prepareUseLists()
{
    for (auto use : this->usage)
    {
        sort(use.second.uselist.begin(), use.second.uselist.end());
    }
}

void RegisterAllocator::next()
{
    this->line++;
}

string RegisterAllocator::allocate(string name)
{
    if (this->get(name) != 0)
    {
        return "";
    }

    int reg = this->getFree();
    if (reg != -1)
    {
        this->registers[reg] = name;
        return "";
    }

    int nextUse[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
    for (int i = 0; i < 7; i++)
    {
        for (int use : this->usage[registers[i]].uselist)
        {
            if (use > this->line)
            {
                nextUse[i] = use;
                break;
            }
        }
        if (nextUse[i] == -1)
        {
            nextUse[i] = INTMAX_MAX;
        }
    }

    int max = 0;
    for (int i = 1; i < 7; i++)
        if (nextUse[max] < nextUse[i])
            max = i;

    string dropped = registers[max];
    registers[max] = name;
    return dropped;
}

char RegisterAllocator::get(string name)
{
    int at = this->inRegs(name);
    return this->names[at];
}

void RegisterAllocator::clearContext()
{
    for (int i = 0; i < 8; i++)
        this->registers[i] = "";
}

string *RegisterAllocator::getContext()
{
    return this->registers;
}