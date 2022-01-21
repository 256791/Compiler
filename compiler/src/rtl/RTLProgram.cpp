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

void RTLProgram::allocateRegisters()
{
    int g = 0;
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
        ra.clearContext();
        int end = 0;
        for (int j = i; j < this->commands.size(); j++)
        {
            RTLNode *node = this->commands[j];

            for (auto use : this->commands[j]->getUseVector())
            {
                ra.markUse(use->name, j);
            }

            if (dynamic_cast<Flag *>(node))
            {
                end = j;
                break;
            }
        }
        if (end == 0)
            end = this->commands.size() - 1;

        for (int j = i; j <= end; j++)
        {
            vector<RTLNode *> expanded;
            for (RTLObject *use : this->commands[j]->getUseVector())
            {
                string name = ra.allocate(use->name);
                char reg = ra.get(use->name);

                // if (name != "")
                // {
                //     RTLObject *tosave = nullptr;
                //     for (RTLObject *obj : objects)
                //     {
                //         if (obj->name == name)
                //         {
                //             tosave = obj;
                //             break;
                //         }
                //     }
                //     if (tosave != nullptr)
                //     {
                //         expanded.push_back(new Assignment(tosave, new Reg(reg)));
                //     }
                // }

                // if (this->commands[j]->isRHS(use->name))
                //     expanded.push_back(new Assignment(new Reg(reg), use));

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

void RTLProgram::allignRegisters()
{
}

vector<Command *> RTLProgram::toAll()
{
    vector<Command *> program;
    char regs[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

    for (RTLNode *node : this->commands)
    {
        vector<Command *> all = node->toAll(regs);
        program.insert(program.end(), all.begin(), all.end());
    }
    program.push_back(new Command("HALT"));

    vector<FlagCommand *> flags;
    for (int line = 0, i = 0; i < program.size(); i++)
    {
        if (FlagCommand *flag = dynamic_cast<FlagCommand *>(program[i]))
        {
            flag->line = line;
            flags.push_back(flag);
        }
        else
        {
            program[i]->line = line;
            line++;
        }
    }
    for (int line = 0, i = 0; i < program.size(); i++)
    {
        if (JumpCommand *jump = dynamic_cast<JumpCommand *>(program[i]))
        {
            for (FlagCommand *f : flags)
            {
                if (f->name == jump->flagname)
                {
                    jump->offset = f->line - jump->line;
                    break;
                }
            }
        }
    }

    return program;
}