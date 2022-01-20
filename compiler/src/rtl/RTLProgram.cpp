#include "RTL.h"

RTLProgram::RTLProgram(vector<RTLObject *> objects, vector<RTLNode *> commands)
{
    for (RTLObject *obj : objects)
    {
        if (Array *arr = dynamic_cast<Array *>(obj))
            arr->adr = RTLObject::allocateArray(arr->size);
        else if (Variable *var = dynamic_cast<Variable *>(obj))
            var->adr = RTLObject::allocateVariable();
    }
    this->objects = objects;
    this->commands = commands;
}

void RTLProgram::resolveAddresses()
{
    for (RTLNode *node : this->commands)
        node->resolveAddresses(this->objects);
}

void RTLProgram::expandVariables(bool deep)
{
    int i = 0;
    while (i < this->commands.size())
    {
        vector<RTLNode *> nodes = this->commands[i]->expand(deep);
        this->commands.erase(this->commands.begin() + i);
        this->commands.insert(this->commands.begin() + i, nodes.begin(), nodes.end());
        i += nodes.size();
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

void RTLProgram::allocateRegisters()
{
    RegisterAllocator ra;
    for (int i = 0; i < this->commands.size(); i++)
    {
        this->commands[i]->line = i;
        for (RTLObject *use : this->commands[i]->getUseVector())
        {
            ra.markUse(use->name, i);
            bool flag = true;
            for (RTLObject *obj : objects)
            {
                if (obj->name == use->name)
                {
                    break;
                    flag = false;
                }
            }
            if (flag && use->adr != -1)
                objects.push_back(use);
        }
    }

    for (int i = 0; i < this->commands.size(); i++)
    {
        ra.clearUse();
        int end = 0;

        for (int j = i; j < this->commands.size(); j++)
        {
            RTLNode *node = this->commands[j];

            for (auto use : this->commands[j]->getUseVector())
            {
                ra.markUse(use->name, j);
            }

            if (dynamic_cast<Jump *>(node) || dynamic_cast<Flag *>(node))
            {
                for (int k = j + 1; k < this->commands.size(); k++)
                    for (auto use : this->commands[k]->getUseVector())
                        ra.markUse(use->name, j * 2 + k);
                end = j;
                break;
            }
        }

        int offset = 0;

        for (int j = i; j <= end; j++)
        {
            vector<RTLNode *> expanded;
            this->commands[j]->printNode();
            for (RTLObject *use : this->commands[j]->getUseVector())
            {
                
                string name = ra.allocate(use->name);
                char reg = ra.get(use->name);
                RTLObject *tosave = nullptr;
                for (RTLObject *obj : objects)
                {
                    if (obj->name == name)
                    {
                        tosave = obj;
                        break;
                    }
                }
                
                if (tosave != nullptr)
                {
                    expanded.push_back(new Assignment(tosave, new Reg(reg)));
                }

                if(this->commands[j]->isRHS(use->name)){
                    expanded.push_back(new Assignment(new Reg(reg), use));
                }

                this->commands[j]->assignReg(use->name, new Reg(reg));
                
            }

            this->commands.insert(this->commands.begin() + j, expanded.begin(), expanded.end());
            j += expanded.size();
            end += expanded.size();
            ra.next();
        }
        i = end;
    }
}