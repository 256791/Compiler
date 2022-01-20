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
    cout << this->to->name << " <-- Const(" << (use_inc?"NULL":this->use->name) << ") " << this->value << endl;
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

    cout << this->dest->name << " <-- " << this->operand_a->name;
    if (Array *arr = dynamic_cast<Array *>(this->operand_a))
        cout << '(' << arr->at->name << ')';
    cout << ' ' << this->operation << ' ' << this->operand_b->name;
    if (Array *arr = dynamic_cast<Array *>(this->operand_b))
        cout << '(' << arr->at->name << ')';
    cout << endl;
}

void Call::printNode()
{
    cout << this->command << '(' << this->operand->name << ')' << endl;
}

void Jump::printNode()
{
    switch (this->type)
    {
    case 'Z':
        cout << "JZERO " << this->destination->name << " (" << this->on->name << ')' << endl;
        break;
    case 'P':
        cout << "JPOS " << this->destination->name << " (" << this->on->name << ')' << endl;
        break;
    case 'N':
        cout << "JNEG " << this->destination->name << " (" << this->on->name << ')' << endl;
        break;
    case 'J':
        cout << "JUMP " << this->destination->name << endl;
        break;
    }
}

void Assignment::printNode()
{
    cout << this->dest->name;
    if (Array *arr = dynamic_cast<Array *>(this->dest))
        cout << '(' << arr->at->name << ')';
    cout << " <-- " << this->source->name;
    if (Array *arr = dynamic_cast<Array *>(this->source))
        cout << '(' << arr->at->name << ')';
    cout << endl;
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

void Flag::printNode()
{
    cout << "F_" << this->name << endl;
}

void RTLProgram::printRTL()
{
    cout << "RTL OBJECTS" << endl
         << endl;
    for (auto node : this->objects)
    {
        node->printNode();
    }
    cout << endl
         << endl
         << "RTL COMMANDS" << endl
         << endl;
    for (auto node : this->commands)
    {
        node->printNode();
    }
}
