#include "RTL.h"

long long RTLObject::ADR = 0;

int Constans::ID = 0;
int VReg::ID = 0;
int Flag::ID = 0;

long long RTLObject::allocateVariable()
{
    return ADR++;
}
long long RTLObject::allocateArray(long long size)
{
    long long a = ADR;
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

string Flag::getNewName()
{
    return "FLAG_" + to_string(ID++);
}

Array::Array(string name)
{
    this->name = name;
    this->adr = -1;
    this->size = -1;
    this->at = nullptr;
    this->to = nullptr;
    this->store = false;
}

Variable::Variable(string name)
{
    this->name = name;
    this->adr = -1;
    this->to = nullptr;
    this->at = nullptr;
    this->store = false;
}

Constans::Constans(string name)
{
    this->name = name;
    this->adr = -1;
    this->use_inc = false;
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

Jump::Jump(RTLNode *destination)
{
    this->type = 'J';
    this->destination = dynamic_cast<Flag *>(destination);
    this->on = nullptr;
}

Jump::Jump(char type, RTLNode *destination, RTLObject *on)
{
    this->type = type;
    this->on = on;
    this->destination = dynamic_cast<Flag *>(destination);
}

Call::Call(string command, RTLObject *operand)
{
    this->command = command;
    this->operand = operand;
}

Flag::Flag()
{
    this->name = Flag::getNewName();
}

void RTLNode::resolveAddresses(vector<RTLObject *> &objects) {}

void RTLObject::resolveAddresses(vector<RTLObject *> &objects)
{
    for (RTLObject *obj : objects)
    {
        if (this->name == obj->name)
        {
            this->adr = obj->adr;
            return;
        }
    }
    if (dynamic_cast<Variable *>(this))
    {
        this->adr = RTLObject::allocateVariable();
        Variable* cp = new Variable(this->name);
        cp->adr = this->adr;
        objects.push_back(cp);
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
            this->size = arr->size;
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

vector<RTLObject *> RTLNode::getUseVector()
{
    return vector<RTLObject *>();
};

vector<RTLObject *> Variable::getUseVector()
{
    vector<RTLObject *> objects;
    objects.push_back(this->at);
    objects.push_back(this->to);
    return objects;
};

vector<RTLObject *> Array::getUseVector()
{
    vector<RTLObject *> objects;
    objects.push_back(this->at);
    objects.push_back(this->to);
    return objects;
};

vector<RTLObject *> Constans::getUseVector()
{
    vector<RTLObject *> objects;
    objects.push_back(this->to);
    return objects;
};

vector<RTLObject *> Operation::getUseVector()
{
    vector<RTLObject *> objects;
    if (dynamic_cast<VReg *>(this->dest))
        objects.push_back(this->dest);
    if (dynamic_cast<VReg *>(this->operand_a))
        objects.push_back(this->operand_a);
    if (dynamic_cast<VReg *>(this->operand_b))
        objects.push_back(this->operand_b);
    return objects;
};

vector<RTLObject *> Assignment::getUseVector()
{
    vector<RTLObject *> objects;
    if (dynamic_cast<VReg *>(this->dest))
        objects.push_back(this->dest);
    if (dynamic_cast<VReg *>(this->source))
        objects.push_back(this->source);
    return objects;
};

vector<RTLObject *> Call::getUseVector()
{
    vector<RTLObject *> objects;
    objects.push_back(this->operand);
    return objects;
};

vector<RTLObject *> Jump::getUseVector()
{
    vector<RTLObject *> objects;
    if (this->on != nullptr)
        objects.push_back(this->on);
    return objects;
};

bool RTLNode::isRHS(string name)
{
    return false;
}

bool Variable::isRHS(string name)
{
    return this->at->name == name;
}

bool Array::isRHS(string name)
{
    return this->at->name == name;
}

bool Operation::isRHS(string name)
{
    return this->operand_a->name == name || this->operand_b->name == name;
}

bool Assignment::isRHS(string name)
{
    return this->source->name == name;
}

bool Call::isRHS(string name)
{
    return this->command == "PUT";
}

bool Jump::isRHS(string name)
{
    return true;
}

void RTLNode::assignReg(string name, Reg *reg)
{
    cerr << "This RTLObject does not use registers!\n";
    throw this;
};

void Variable::assignReg(string name, Reg *reg)
{
    if (this->to->name == name)
        this->to = reg;
    else if (this->at->name == name)
        this->at = reg;
};

void Array::assignReg(string name, Reg *reg)
{
    if (this->to->name == name)
        this->to = reg;
    else if (this->at->name == name)
        this->at = reg;
};

void Constans::assignReg(string name, Reg *reg)
{
    if (this->to->name == name)
    {
        this->to = reg;
    }
    else if (!this->use_inc && this->use->name == name)
    {
        this->use = reg;
    }
};

void Operation::assignReg(string name, Reg *reg)
{
    if (this->dest->name == name)
    {
        this->dest = reg;
    }
    else if (this->operand_a->name == name)
    {
        this->operand_a = reg;
    }
    else if (this->operand_b->name == name)
    {
        this->operand_b = reg;
    }
};

void Assignment::assignReg(string name, Reg *reg)
{
    if (this->dest->name == name)
    {
        this->dest = reg;
    }
    else if (this->source->name == name)
    {
        this->source = reg;
    }
};

void Call::assignReg(string name, Reg *reg)
{
    this->operand = reg;
};

void Jump::assignReg(string name, Reg *reg)
{
    this->on = reg;
};

vector<RTLNode *> RTLNode::expand(bool deep)
{
    vector<RTLNode *> nodes;
    nodes.push_back(this);
    return nodes;
}

vector<RTLNode *> Jump::expand(bool deep)
{
    vector<RTLNode *> nodes;
    nodes.push_back(this);
    return nodes;
}

vector<RTLNode *> RTLObject::expandVariable(VReg *to)
{
    cerr << "This RTLObject cant be expanded!\n";
    throw;
}

vector<RTLNode *> Constans::expandVariable(VReg *to)
{
    vector<RTLNode *> nodes;
    this->to = to;
    // if (this->value > 65536 && to->adr == -1)
    //     this->to->adr = allocateVariable();

    // this->use = new VReg(VReg::getNewName());
    // this->use_inc = true;
    this->use_inc = false;
    this->use = new Reg('h');

    nodes.push_back(this);

    return nodes;
}

vector<RTLNode *> Variable::expandVariable(VReg *to)
{
    vector<RTLNode *> nodes;
    this->to = to;
    VReg *reg = new VReg(VReg::getNewName());
    this->at = reg;

    Constans *con = new Constans(Constans::getNewName());
    con->value = this->adr;
    nodes = con->expandVariable(reg);

    nodes.push_back(this);
    return nodes;
}

vector<RTLNode *> Array::expandVariable(VReg *to)
{
    vector<RTLNode *> nodes;

    if (Constans *con = dynamic_cast<Constans *>(this->at))
    {
        long long adr = this->adr - this->offset + con->value;
        Variable *var = new Variable(this->name + "_C");
        var->adr = adr;
        var->store = this->store;
        nodes = var->expandVariable(to);
    }

    else if (Variable *var = dynamic_cast<Variable *>(this->at))
    {
        this->to = to;
        long long adr = this->adr - this->offset;

        VReg *atvreg = new VReg(VReg::getNewName());
        nodes = var->expandVariable(atvreg);

        VReg *adrvreg = new VReg(VReg::getNewName());
        Constans *con = new Constans(Constans::getNewName());
        con->value = adr;
        vector<RTLNode *> to_append = con->expandVariable(adrvreg);
        nodes.insert(nodes.end(), to_append.begin(), to_append.end());

        Operation* op = new Operation(adrvreg, adrvreg, '+', atvreg);
        nodes.push_back(op);

        this->at = adrvreg;

        nodes.push_back(this);
    }

    else if (VReg *vreg = dynamic_cast<VReg *>(this->at))
    {
        this->to = to;
        long long adr = this->adr - this->offset;

        VReg *adrvreg = new VReg(VReg::getNewName());
        Constans *con = new Constans(Constans::getNewName());
        con->value = adr;
        nodes = con->expandVariable(adrvreg);

        Operation* op = new Operation(adrvreg, adrvreg, '+', vreg);
        nodes.push_back(op);

        this->at = adrvreg;

        nodes.push_back(this);
    }

    return nodes;
}

vector<RTLNode *> Assignment::expand(bool deep)
{
    vector<RTLNode *> nodes;
    if (!(dynamic_cast<VReg *>(this->source) || dynamic_cast<Reg *>(this->source)))
    {
        if (!(dynamic_cast<VReg *>(this->dest) || dynamic_cast<Reg *>(this->dest)))
        {
            VReg *vreg = new VReg(VReg::getNewName());

            Assignment *assignment = new Assignment(vreg, this->source);
            nodes = assignment->expand(deep);

            this->source = vreg;
            nodes.push_back(this);

            return nodes;
        }
    }
    if (deep)
    {
        if (!(dynamic_cast<VReg *>(this->source) || dynamic_cast<Reg *>(this->source)))
        {
            nodes = this->source->expandVariable(dynamic_cast<VReg *>(this->dest));
            return nodes;
        }
        if (dynamic_cast<Array *>(this->dest) || dynamic_cast<Variable *>(this->dest))
        {
            if(Array* arr = dynamic_cast<Array *>(this->dest))
                arr->store = true;
            if(Variable* var = dynamic_cast<Variable *>(this->dest))
                var->store = true;
            nodes = this->dest->expandVariable(dynamic_cast<VReg *>(this->source));
            return nodes;
        }
    }

    nodes.push_back(this);
    return nodes;
}

vector<RTLNode *> Operation::expand(bool deep)
{
    vector<RTLNode *> nodes;
    if (!(dynamic_cast<VReg *>(this->operand_a) || dynamic_cast<Reg *>(this->operand_a)))
    {
        VReg *vreg = new VReg(VReg::getNewName());

        Assignment *assignment = new Assignment(vreg, this->operand_a);
        nodes = assignment->expand(deep);

        this->operand_a = vreg;
    }

    if (!(dynamic_cast<VReg *>(this->operand_b) || dynamic_cast<Reg *>(this->operand_b)))
    {
        VReg *vreg = new VReg(VReg::getNewName());

        Assignment *assignment = new Assignment(vreg, this->operand_b);
        vector<RTLNode *> to_append = assignment->expand(deep);
        nodes.insert(nodes.end(), to_append.begin(), to_append.end());

        this->operand_b = vreg;
    }

    if (!(dynamic_cast<VReg *>(this->dest) || dynamic_cast<Reg *>(this->dest)))
    {
        VReg *vreg = new VReg(VReg::getNewName());
        vreg->adr = RTLObject::allocateVariable();

        Assignment *assignment = new Assignment(vreg, this->dest);
        vector<RTLNode *> to_append = assignment->expand(deep);

        this->dest = vreg;

        nodes.push_back(this);
        nodes.insert(nodes.end(), to_append.begin(), to_append.end());
    }
    else
    {
        nodes.push_back(this);
    }

    return nodes;
}

vector<RTLNode *> Call::expand(bool deep)
{
    vector<RTLNode *> nodes;
    if (this->command == "GET")
    {
        if (!(dynamic_cast<VReg *>(this->operand) || dynamic_cast<Reg *>(this->operand)))
        {
            VReg *vreg = new VReg(VReg::getNewName());
            vreg->adr = RTLObject::allocateVariable();

            Assignment *assignment = new Assignment(this->operand, vreg);
            vector<RTLNode *> to_append = assignment->expand(deep);

            this->operand = vreg;

            nodes.push_back(this);
            nodes.insert(nodes.end(), to_append.begin(), to_append.end());
        }
        else
        {
            nodes.push_back(this);
        }
    }
    else
    {
        if (!(dynamic_cast<VReg *>(this->operand) || dynamic_cast<Reg *>(this->operand)))
        {
            VReg *vreg = new VReg(VReg::getNewName());

            Assignment *assignment = new Assignment(vreg, this->operand);
            nodes = assignment->expand(deep);

            this->operand = vreg;

            nodes.push_back(this);
        }
        else
        {
            nodes.push_back(this);
        }
    }
    return nodes;
}
