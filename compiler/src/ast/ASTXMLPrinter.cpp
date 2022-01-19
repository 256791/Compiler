#include "AST.h"

void AST::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << ">\n";
    this->decls->printXML(indent);
    this->stmnts->printXML(indent);
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}

void Stmnt::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << "/>\n";
}

void CompoundStmnt::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << ">\n";
    for (Stmnt *stmnt : this->stmnts)
    {
        stmnt->printXML(indent + 1);
    }
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}

void Syscall::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << " type='" << ((this->type) == 'R' ? "Read" : "Write") << "'>\n";
    this->val->printXML(indent + 1);
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}


void VarDecl::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << " name='" << this->name << "'/>\n";
}

void ArrDecl::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << " name='" << this->name << "' size='";
    cout << this->size << "' indexed='[" << this->from << ":" << this->to << "]'/>\n";
}

void VarConst::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << " value='" << this->value << "'/>\n";
}

void VarRef::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << " name='" << this->name << "'/>\n";
}

void ArrRef::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << " name='" << this->name << "'>\n";
    this->at->printXML(indent + 1);
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}

void Expr::printXML(int indent)
{
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}

void Comp::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << " operator='" << this->op << "'>\n";
    this->a->printXML(indent + 1);
    this->b->printXML(indent + 1);
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}

void BinOpExpr::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << " operator='" << this->op << "'>\n";
    this->a->printXML(indent + 1);
    this->b->printXML(indent + 1);
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}

void IfStmnt::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << ">\n";
    this->cond->printXML(indent + 1);
    this->then->printXML(indent + 1);
    if(this->els != nullptr)
        this->els->printXML(indent + 1);
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}

void WhileStmnt::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << ">\n";
    this->cond->printXML(indent + 1);
    this->stmnts->printXML(indent + 1);
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}

void DoStmnt::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << ">\n";
    this->cond->printXML(indent + 1);
    this->stmnts->printXML(indent + 1);
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}

void ForStmnt::printXML(int indent)
{
    cout << string(indent, '\t') << "<" << this->NAME << ">\n";
    this->init->printXML(indent + 1);
    this->cond->printXML(indent + 1);
    this->after->printXML(indent + 1);
    this->stmnts->printXML(indent + 1);
    cout << string(indent, '\t') << "</" << this->NAME << ">\n";
}