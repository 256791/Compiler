#ifndef AST_H
#define AST_H
#include <string>
#include <iostream>
#include <algorithm>
#include <tuple>

#include "../rtl/RTL.h"

using namespace std;

class Stmnt
{
public:
    static const string NAME;

    Stmnt(int l);
    int lineno;

    virtual bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) = 0;
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

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class Expr : public Stmnt
{
public:
    static const string NAME;

    Expr(int l);

    virtual bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) = 0;
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

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
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

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class VarDecl : public Stmnt
{
public:
    static const string NAME;

    VarDecl(int l, string name);

    string name;

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class ArrDecl : public Stmnt
{
public:
    static const string NAME;

    ArrDecl(int l, string name, long long from, long long to);

    string name;
    long long from;
    long long to;
    long long size;

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
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

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
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

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
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

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
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

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class VarRef;

class ForStmnt : public Stmnt
{
public:
    static const string NAME;

    ForStmnt(int l, string iterator, Stmnt *from, Stmnt *to, char type, CompoundStmnt *stmnts);

    VarRef* iter;
    Expr* from;
    Expr* to;
    
    Comp *cond;
    Stmnt *init;
    Stmnt *after;
    CompoundStmnt *stmnts;

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class VarRef : public Expr
{
public:
    static const string NAME;

    VarRef(int l, string name);

    string name;

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
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

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
    void printXML(int indent) override;
    vector<RTLNode *> toRTL() override;
};

class VarConst : public Expr
{
public:
    static const string NAME;

    VarConst(int l, long long value);

    long long value;

    bool checkVariables(vector<tuple<string,bool,bool> > &variables,vector<string> iterators) override;
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

    bool checkVariables();
    void printXML(int indent);
    RTLProgram *toRTL();
};

#endif