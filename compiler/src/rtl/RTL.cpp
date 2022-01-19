#include "RTL.h"
#include "RegisterAllocator.h"

int RTLObject::ADR = 0;

int Constans::ID = 0;
int VReg::ID = 0;

int RTLObject::allocateVariable()
{
    return ADR++;
}
int RTLObject::allocateArray(int size)
{
    int a = ADR;
    ADR += size;
    return a;
}

string Constans::getNewName()
{
    return "CONST_" + to_string(ID++);
}

string VReg::getNewName()
{
    return "VREG_" + to_string(ID++);
}

vector<Command> Variable::toAll() {}

vector<Command> Constans::toAll() {}

vector<Command> VReg::toAll() {}

vector<Command> Reg::toAll() {}

vector<Command> Operation::toAll() {}

vector<Command> Call::toAll() {}

vector<Command> Jump::toAll() {}

vector<Command> Array::toAll() {}

vector<Command> Assignment::toAll() {}

vector<Command> MemoryOP::toAll() {}

vector<Command> Flag::toAll() {}

Array::Array(string name)
{
    this->name = name;
    this->adr = -1;
}

Variable::Variable(string name)
{
    this->name = name;
    this->adr = -1;
}

Constans::Constans(string name)
{
    this->name = name;
    this->adr = -1;
}

VReg::VReg(string name)
{
    this->name = name;
    this->adr = -1;
}

Reg::Reg(char name)
{
    this->name = string(1, name);
    this->adr = -1;
}

Operation::Operation(RTLObject *dest, RTLObject *operand_a, char operation, RTLObject *operand_b)
{
    this->dest = dest;
    this->operand_a = operand_a;
    this->operand_b = operand_b;
    this->operation = operation;
}

Assignment::Assignment(RTLObject *dest, RTLObject *source)
{
    this->dest = dest;
    this->source = source;
}

Jump::Jump()
{
    this->type = 'F';
    this->on = nullptr;
}

Jump::Jump(RTLNode *destination)
{
    this->type = 'J';
    this->destination = destination;
    this->on = nullptr;
}

Jump::Jump(char type, RTLNode *destination, RTLObject *on)
{
    this->type = type;
    this->on = on;
    this->destination = destination;
}

MemoryOP::MemoryOP(string command, RTLObject *adr, RTLObject *val)
{
    this->command = command;
    this->adr = adr;
    this->val = val;
}

Call::Call(string command, RTLObject *operand)
{
    this->command = command;
    this->operand = operand;
}

Program::Program(vector<RTLObject *> objects, vector<RTLNode *> commands)
{
    this->objects = objects;
    this->commands = commands;
}

void RTLObject::resolveAddresses(vector<RTLObject *> &objects)
{
    for (RTLObject *obj : objects)
    {
        if (this->name == obj->name)
        {
            this->adr = obj->adr;
        }
    }
}

void Array::resolveAddresses(vector<RTLObject *> &objects)
{
    for (RTLObject *obj : objects)
    {
        if (this->name == obj->name)
        {
            Array *arr = dynamic_cast<Array *>(obj);
            this->adr = arr->adr;
            this->offset = arr->offset;
        }
    }
    this->at->resolveAddresses(objects);
}

void Operation::resolveAddresses(vector<RTLObject *> &objects)
{
    this->dest->resolveAddresses(objects);
    this->operand_a->resolveAddresses(objects);
    this->operand_b->resolveAddresses(objects);
}

void Assignment::resolveAddresses(vector<RTLObject *> &objects)
{
    this->source->resolveAddresses(objects);
    this->dest->resolveAddresses(objects);
}

void Call::resolveAddresses(vector<RTLObject *> &objects)
{
    this->operand->resolveAddresses(objects);
}

void Jump::resolveAddresses(vector<RTLObject *> &objects)
{
    if (this->on != nullptr)
    {
        this->on->resolveAddresses(objects);
    }
}

void MemoryOP::resolveAddresses(vector<RTLObject *> &objects) {}

void Flag::resolveAddresses(vector<RTLObject *> &objects) {}

void Program::printRTL()
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

RTLObject *RTLObject::expand(RTLObject *obj, vector<RTLNode *> &expanded)
{
    if (Array *arr = dynamic_cast<Array *>(obj))
    {
        if (Constans *con = dynamic_cast<Constans *>(arr->at))
        {
            Variable *replace = new Variable(arr->name + "_" + to_string(con->value));
            replace->adr = arr->adr + (con->value - arr->offset);
            return replace;
        }
        else
        {
            VReg *vreg = new VReg(VReg::getNewName());
            arr->to = vreg;
            expanded.push_back(arr);
            return vreg;
        }
    }
    else if (Constans *con = dynamic_cast<Constans *>(obj))
    {
        VReg *vreg = new VReg(VReg::getNewName());
        con->to = vreg;
        vreg->adr;
        expanded.push_back(con);
        return vreg;
    }
    else
    {
        return obj;
    }
}

void Program::resolveAddresses()
{
    for (RTLNode *node : this->commands)
        node->resolveAddresses(this->objects);
}

void Program::expandVariables()
{
    for (int i = 0; i < this->commands.size(); i++)
    {
        vector<RTLNode *>::iterator node = this->commands.begin() + i;
        vector<RTLNode *> expanded;

        if (Operation *operation = dynamic_cast<Operation *>(*node))
        {
            operation->operand_a = RTLObject::expand(operation->operand_a, expanded);
            operation->operand_b = RTLObject::expand(operation->operand_b, expanded);
        }
        else if (Assignment *assign = dynamic_cast<Assignment *>(*node))
        {
            assign->source = RTLObject::expand(assign->source, expanded);
        }
        else if (Call *call = dynamic_cast<Call *>(*node))
        {
            if (call->command == "PUT")
            {
                call->operand = RTLObject::expand(call->operand, expanded);
            }
            else if (call->command == "GET")
            {
                call->operand = RTLObject::expand(call->operand, expanded);
                if (expanded.size() > 0)
                {
                    if (Array *dest = dynamic_cast<Array *>(expanded[0]))
                    {
                        expanded[0] = new Assignment(dest, call->operand);
                        node++;
                    }
                }
            }
        }

        i += expanded.size();
        this->commands.insert(node, expanded.begin(), expanded.end());
    }
}

void fill(vector<RTLObject *> &objects, RTLObject *obj)
{
    for (auto o : objects)
        if (o->name == obj->name)
            return;
    if (obj->adr != -1)
    {
        objects.push_back(obj);
    }
}

Reg *allocate(RegisterAllocator &ra, RTLObject *obj, vector<RTLNode *> &expanded, vector<RTLObject *> objects, bool load = true)
{
    string tosave = ra.allocate(obj->name);
    char n = ra.get(obj->name);
    if (tosave != "")
    {
        RTLObject *dropped = nullptr;
        for (RTLObject *node : objects)
        {
            if (node->name == tosave)
            {
                dropped = node;
                break;
            }
        }

        if (dropped != nullptr)
        {
            if (Array *arr = dynamic_cast<Array *>(dropped))
            {
                Constans *adr = new Constans(Constans::getNewName());
                adr->to = new Reg('h');
                expanded.push_back(adr);

                // todo
                expanded.push_back(new MemoryOP("STORE", adr->to, new Reg(n)));
            }
            else // if(dynamic_cast<Variable *>(dropped))
            {
                Constans *adr = new Constans(Constans::getNewName());
                adr->to = new Reg('h');
                adr->value = dropped->adr;
                expanded.push_back(adr);
                expanded.push_back(new MemoryOP("STORE", adr->to, new Reg(n)));
            }
        }
    }
    if (load)
    {
        if (Array *arr = dynamic_cast<Array *>(obj))
        {
            Constans *adr = new Constans(Constans::getNewName());
            adr->to = new Reg(n);
            adr->value = arr->offset;
        }
        else if (dynamic_cast<Variable *>(obj))
        {
            Constans *adr = new Constans(Constans::getNewName());
            adr->to = new Reg(n);
            adr->value = obj->adr;
            expanded.push_back(adr);
            expanded.push_back(new MemoryOP("LOAD", adr->to, new Reg(n)));
        }
    }
    if (n == 0)
    {
        cerr << "ERROR\n";
    }
    return new Reg(n);
}

void markUse(RegisterAllocator &ra, vector<RTLNode *>::iterator node, vector<RTLObject *> &objects, int line)
{
    if (Operation *operation = dynamic_cast<Operation *>(*node))
    {
        ra.markUse(operation->operand_a->name, line);
        fill(objects, operation->operand_a);
        ra.markUse(operation->operand_b->name, line);
        fill(objects, operation->operand_b);
        ra.markUse(operation->dest->name, line);
        fill(objects, operation->dest);
    }
    else if (Assignment *assign = dynamic_cast<Assignment *>(*node))
    {
        ra.markUse(assign->source->name, line);
        fill(objects, assign->source);
        ra.markUse(assign->dest->name, line);
        fill(objects, assign->dest);
    }
    else if (Call *call = dynamic_cast<Call *>(*node))
    {
        ra.markUse(call->operand->name, line);
        fill(objects, call->operand);
    }
    else if (Array *arr = dynamic_cast<Array *>(*node))
    {
        ra.markUse(arr->at->name, line);
        fill(objects, arr->at);
        ra.markUse(arr->to->name, line);
        fill(objects, arr->to);
    }
    else if (Constans *con = dynamic_cast<Constans *>(*node))
    {
        ra.markUse(con->to->name, line);
        fill(objects, con->to);
    }
}

void Program::allocateRegisters()
{
    RegisterAllocator ra;

    for (int i = 0; i < this->commands.size(); i++)
    {
        vector<RTLNode *>::iterator node = this->commands.begin() + i;
        (*node)->line = i;
        markUse(ra, node, this->objects, i);
    }
    for (int i = 0; i < this->commands.size(); i++)
    {
        vector<RTLNode *>::iterator node = this->commands.begin() + i;
        if (Jump *jmp = dynamic_cast<Jump *>(*node))
        {
            if (jmp->destination->line < i)
            {
                for (int j = jmp->destination->line; j <= i; j++)
                {
                    node = this->commands.begin() + j;
                    markUse(ra, node, this->objects, i + j);
                    ;
                }
            }
        }
    }

    ra.prepareUseLists();

    for (int i = 0; i < this->commands.size(); i++)
    {
        vector<RTLNode *>::iterator node = this->commands.begin() + i;
        vector<RTLNode *> expanded;
        string tosave;

        if (Operation *operation = dynamic_cast<Operation *>(*node))
        {
            operation->dest = allocate(ra, operation->dest, expanded, this->objects, false);
            operation->operand_a = allocate(ra, operation->operand_a, expanded, this->objects);
            operation->operand_b = allocate(ra, operation->operand_b, expanded, this->objects);
        }
        else if (Assignment *assign = dynamic_cast<Assignment *>(*node))
        {
            assign->source = allocate(ra, assign->source, expanded, this->objects);
            assign->dest = allocate(ra, assign->dest, expanded, this->objects, false);
        }
        else if (Call *call = dynamic_cast<Call *>(*node))
        {
            call->operand = allocate(ra, call->operand, expanded, this->objects, call->command == "PUT");
        }
        else if (Array *arr = dynamic_cast<Array *>(*node))
        {
            arr->at = allocate(ra, arr->at, expanded, this->objects);
            arr->to = allocate(ra, arr->to, expanded, this->objects, false);
        }
        else if (Constans *con = dynamic_cast<Constans *>(*node))
        {
            con->to = allocate(ra, con->to, expanded, this->objects, false);
        }
        else if (Jump *jmp = dynamic_cast<Jump *>(*node))
        {
            if (jmp->on != nullptr)
            {
                jmp->on = allocate(ra, jmp->on, expanded, this->objects);
            }
        }

        i += expanded.size();
        this->commands.insert(node, expanded.begin(), expanded.end());
        ra.next();
    }
}