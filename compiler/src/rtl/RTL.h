#ifndef RTL_H
#define RTL_H
#include <vector>
#include <iostream>

#include "all/ALL.h"
#include "RegisterAllocator.h"

using namespace std;

class RTLObject;
class Reg;

class RTLNode
{
public:
    int line;

    virtual void assignReg(string name, Reg* reg);
    virtual vector<RTLObject* > getUseVector();
    virtual bool isRHS(string name);
    virtual vector<RTLNode* > expand(bool deep);
    virtual void resolveAddresses(vector<RTLObject *> &objects);
    virtual vector<Command> toAll() = 0;
    virtual void printNode() = 0;
};

class Flag : public RTLNode
{
private:
    static int ID;
public:
    static string getNewName();

    Flag();

    string name;
    vector<Command> toAll() override;
    void printNode() override;
};

class VReg;

class RTLObject : public RTLNode
{
private:
    static int ADR;

public:
    static int allocateVariable();
    static int allocateArray(int size);

    string name;
    int adr;

    virtual vector<RTLNode* > expandVariable(VReg* to);
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

    void assignReg(string name, Reg* reg) override;
    bool isRHS(string name) override;
    vector<RTLObject* > getUseVector() override;
    vector<RTLNode* > expandVariable(VReg* to) override;
    vector<Command> toAll() override;
    void printNode() override;
};

class Array : public RTLObject
{
public:
    Array(string name);

    int offset;
    int size;
    RTLObject *at;
    RTLObject *to;

    void assignReg(string name, Reg* reg) override;
    bool isRHS(string name) override;
    vector<RTLObject* > getUseVector() override;
    vector<RTLNode* > expandVariable(VReg* to) override;
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
    bool use_inc;
    RTLObject *use;
    RTLObject *to;

    void assignReg(string name, Reg* reg) override;
    vector<RTLObject* > getUseVector() override;
    vector<RTLNode* > expandVariable(VReg* to) override;
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

    void assignReg(string name, Reg* reg) override;
    bool isRHS(string name) override;
    vector<RTLObject* > getUseVector() override;
    vector<RTLNode* > expand(bool deep) override;
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

    void assignReg(string name, Reg* reg) override;
    bool isRHS(string name) override;
    vector<RTLObject* > getUseVector() override;
    vector<RTLNode* > expand(bool deep) override;
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

    void assignReg(string name, Reg* reg) override;
    bool isRHS(string name) override;
    vector<RTLObject* > getUseVector() override;
    vector<RTLNode* > expand(bool deep) override;
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
    Flag *destination;
    int offset;

    void assignReg(string name, Reg* reg) override;
    bool isRHS(string name) override;
    vector<RTLObject* > getUseVector() override;
    vector<RTLNode* > expand(bool deep) override;
    void resolveAddresses(vector<RTLObject *> &objects) override;
    vector<Command> toAll() override;
    void printNode() override;
};

class RTLProgram
{
public:
    vector<RTLObject *> objects;
    vector<RTLNode *> commands;

    RTLProgram(vector<RTLObject *> objects, vector<RTLNode *> commands);

    void printRTL();
    void resolveAddresses();
    void expandVariables(bool deep);
    void allocateRegisters();
    void allignRegisters();
    vector<Command> toAll();
};

#endif