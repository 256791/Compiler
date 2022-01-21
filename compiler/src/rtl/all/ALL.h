#ifndef ALL_H
#define ALL_H
#include <string>
#include <fstream>

using namespace std;

class Command
{
public:
    Command(string n) : name(n), line(0) {};
    string name;
    int line;

    virtual void toASM(ostream &out)
    {
        out << name << endl;
    };
};

class RegCommand : public Command
{
public:
    RegCommand(string n, char r) : Command(n), reg(r){};
    char reg;
    void toASM(ostream &out) override
    {
        out << name << ' ' << reg << endl;
    };
};

class JumpCommand : public Command
{
public:
    JumpCommand(string n, string f) : Command(n), flagname(f), offset(0){};
    string flagname;
    int offset;
    void toASM(ostream &out) override
    {
        out << name << ' ' << offset << endl;
    };
};

class FlagCommand : public Command
{
public:
    FlagCommand(string f) : Command(f){};
    void toASM(ostream &out) override
    {
        out << "(FLAG: " << name << ")\n";
    }
};
#endif