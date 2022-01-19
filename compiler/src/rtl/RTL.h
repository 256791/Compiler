#ifndef RTL_H
#define RTL_H
#include <vector>
#include <iostream>

#include "all/ALL.h"

using namespace std;

class RTLObject;

class RTLNode
{
public:
    int line;
    virtual void resolveAddresses(vector<RTLObject *> &objects) = 0;
    virtual vector<Command> toAll() = 0;
    virtual void printNode() = 0;
};

class Flag : public RTLNode
{
public:
    void resolveAddresses(vector<RTLObject *> &objects) override;
    vector<Command> toAll() override;
    void printNode() override;
};

class RTLObject : public RTLNode
{
private:
    static int ADR;

public:
    static int allocateVariable();
    static int allocateArray(int size);
    static RTLObject *expand(RTLObject *obj, vector<RTLNode *> &expanded);

    string name;
    int adr;

    void resolveAddresses(vector<RTLObject *> &objects) override;
    virtual vector<Command> toAll() = 0;
    virtual void printNode() = 0;
};

class VReg : public RTLObject
{
private:
    static int ID;

public:
    static string getNewName();

    VReg(string name);

    vector<Command> toAll() override;
    void printNode() override;
};

class Reg : public RTLObject
{
private:
    static int ID;

public:
    static string getNewName();

    Reg(char name);

    vector<Command> toAll() override;
    void printNode() override;
};

class Variable : public RTLObject
{
public:
    Variable(string name);

    vector<Command> toAll() override;
    void printNode() override;
};

class Array : public RTLObject
{
public:
    Array(string name);

    int offset;
    RTLObject *at;
    RTLObject *to;

    void resolveAddresses(vector<RTLObject *> &objects) override;
    vector<Command> toAll() override;
    void printNode() override;
};

class Constans : public RTLObject
{
private:
    static int ID;

public:
    static string getNewName();

    Constans(string name);
    int value;
    RTLObject *to;

    vector<Command> toAll() override;
    void printNode() override;
};

class Operation : public RTLNode
{
public:
    Operation(RTLObject *dest, RTLObject *operand_a, char operation, RTLObject *operand_b);

    RTLObject *dest;
    RTLObject *operand_a;
    RTLObject *operand_b;
    char operation;

    void resolveAddresses(vector<RTLObject *> &objects) override;
    vector<Command> toAll() override;
    void printNode() override;
};

class Assignment : public RTLNode
{
public:
    Assignment(RTLObject *dest, RTLObject *source);

    RTLObject *dest;
    RTLObject *source;

    void resolveAddresses(vector<RTLObject *> &objects) override;
    vector<Command> toAll() override;
    void printNode() override;
};

class Call : public RTLNode
{
public:
    Call(string command, RTLObject *operand);

    string command;
    RTLObject *operand;

    void resolveAddresses(vector<RTLObject *> &objects) override;
    vector<Command> toAll() override;
    void printNode() override;
};

class MemoryOP : public RTLNode
{
public:
    MemoryOP(string command, RTLObject *adr, RTLObject *val);

    string command;
    RTLObject *adr;
    RTLObject *val;

    void resolveAddresses(vector<RTLObject *> &objects) override;
    vector<Command> toAll() override;
    void printNode() override;
};

class Jump : public RTLNode
{
public:
    Jump();
    Jump(RTLNode *destination);
    Jump(char type, RTLNode *destination, RTLObject *on);

    char type;
    RTLObject *on;
    RTLNode *destination;
    int offset;

    void resolveAddresses(vector<RTLObject *> &objects) override;
    vector<Command> toAll() override;
    void printNode() override;
};

class Program
{
public:
    vector<RTLObject *> objects;
    vector<RTLNode *> commands;

    Program(vector<RTLObject *> objects, vector<RTLNode *> commands);

    void printRTL();
    void resolveAddresses();
    void expandVariables();
    void allocateRegisters();
    void allignRegisters();
    vector<Command> toAll();
};

#endif