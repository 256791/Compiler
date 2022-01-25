#ifndef AST_H
#define AST_H
#include <string>
#include <iostream>
#include <algorithm>

#include "../rtl/RTL.h"

using namespace std;

class Stmnt
{
public:
    static const string NAME;

    Stmnt(int l);
    int lineno;

    virtual void printXML(int indent) = 0;
    virtual vector<RTLNode *> toRTL() = 0;
};

class CompoundStmnt : public Stmnt
{
public:
    static const string NAME;

    CompoundStmnt(int l, Stmnt *stmnt);
    CompoundStmnt(int l, CompoundStmnt *stmnts, Stmnt *stmnt);
    vector<Stmnt *> stmnts;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class Expr : public Stmnt
{
public:
    static const string NAME;

    Expr(int l);

    void printXML(int indent) override;
    virtual vector<RTLNode *> toRTL() = 0;
};

class Comp : public Expr
{
public:
    static const string NAME;

    Comp(int l, string op, Stmnt *a, Stmnt *b);

    string op;
    Expr *a;
    Expr *b;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
    vector<RTLNode *> toRTL(RTLNode* then);
};

class BinOpExpr : public Expr
{
public:
    static const string NAME;

    BinOpExpr(int l, char op, Stmnt *then, Stmnt *els);

    char op;
    Expr *a;
    Expr *b;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class VarDecl : public Stmnt
{
public:
    static const string NAME;

    VarDecl(int l, string name);

    string name;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class ArrDecl : public Stmnt
{
public:
    static const string NAME;

    ArrDecl(int l, string name, int from, int to);

    string name;
    int from;
    int to;
    int size;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class Syscall : public Stmnt
{
public:
    static const string NAME;

    Syscall(int l, Stmnt *val, char type);

    char type;
    Expr *val;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class IfStmnt : public Stmnt
{
public:
    static const string NAME;

    IfStmnt(int l, Stmnt *cond, CompoundStmnt *then);
    IfStmnt(int l, Stmnt *cond, CompoundStmnt *then, CompoundStmnt *els);

    Comp *cond;
    CompoundStmnt *then;
    CompoundStmnt *els;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class WhileStmnt : public Stmnt
{
public:
    static const string NAME;

    WhileStmnt(int l, Stmnt *cond, CompoundStmnt *stmnts);

    Comp *cond;
    CompoundStmnt *stmnts;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class DoStmnt : public Stmnt
{
public:
    static const string NAME;

    DoStmnt(int l, Stmnt *cond, CompoundStmnt *stmnts);

    Comp *cond;
    CompoundStmnt *stmnts;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class ForStmnt : public Stmnt
{
public:
    static const string NAME;

    ForStmnt(int l, string iterator, Stmnt *from, Stmnt *to, char type, CompoundStmnt *stmnts);

    Comp *cond;
    Stmnt *init;
    Stmnt *after;
    CompoundStmnt *stmnts;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class VarRef : public Expr
{
public:
    static const string NAME;

    VarRef(int l, string name);

    string name;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class ArrRef : public Expr
{
public:
    static const string NAME;

    ArrRef(int l, string name, Stmnt *stmnt);

    string name;
    Expr *at;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class VarConst : public Expr
{
public:
    static const string NAME;

    VarConst(int l, int value);

    int value;

    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class AST
{
public:
    static const string NAME;

    AST(CompoundStmnt *stmnts);
    AST(CompoundStmnt *decls, CompoundStmnt *stmnts);

    CompoundStmnt *decls;
    CompoundStmnt *stmnts;

    void printXML(int indent);
    RTLProgram *toRTL();
};

#endif