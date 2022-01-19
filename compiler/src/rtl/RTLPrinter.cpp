#include "RTL.h"

void Variable::printNode()
{
    cout << "Variable name: '" << this->name << "' adr: " << this->adr << "'" << endl;
}

void Array::printNode()
{
    if (this->at != nullptr)
    {
        cout << this->to->name << " <-- " << this->name << "(" << this->at->name << ")" << endl;
    }
    else
    {
        cout << "Array name: '" << this->name << "' adr: '" << this->adr
             << "' offset: '" << this->offset << "'" << endl;
    }
}

void Constans::printNode()
{
    cout << this->to->name << " <-- " << this->value << endl;
}

void VReg::printNode()
{
    cout << "VReg name: '" << this->name << "' adr: " << this->adr << "'" << endl;
}

void Reg::printNode()
{
    cout << "Reg" << endl;
}

void Operation::printNode()
{
    cout << this->dest->name << " <-- " << this->operand_a->name << ' '
         << this->operation << ' ' << this->operand_b->name << endl;
}

void Call::printNode()
{
    cout << this->command << '(' << this->operand->name << ')' << endl;
}

void Jump::printNode()
{
    switch (this->type)
    {
    case 'F':
        cout << "JUMP 1" << endl;
        break;
    case 'Z':
        cout << "JZERO " << this->destination->line << " (" << this->on->name << ')' << endl;
        break;
    case 'P':
        cout << "JPOS " << this->destination->line << " (" << this->on->name << ')' << endl;
        break;
    case 'N':
        cout << "JNEG " << this->destination->line << " (" << this->on->name << ')' << endl;
        break;
    case 'J':
        cout << "JUMP " << this->destination->line << endl;
        break;
    }
}

void Assignment::printNode()
{
    cout << this->dest->name << " <-- " << this->source->name << endl;
}

void MemoryOP::printNode()
{
    if (this->command == "STORE")
    {
        cout << "MEM(" << this->adr->name << ")"
             << " <-- " << this->val->name << endl;
    }
    else
    {
        cout << this->val->name << " <-- "
             << "MEM("
             << this->adr->name << ")" << endl;
    }
}

void Flag::printNode(){
    cout << "FLAG " << this->line << endl;
}