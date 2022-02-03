#include "RTL.h"
#include "ALLMDM.h"

Command *allignReg(char *regs, char reg)
{
    for (char i = 0; i < 8; i++)
    {
        if (reg == regs[i])
        {
            regs[i] = regs[0];
            regs[0] = reg;
            return new RegCommand("SWAP", 'a' + i);
        }
    }
    throw runtime_error("Trying to allign RTLObject whitch is not a register");
}

char findReg(char *regs, char reg)
{
    for (int i = 0; i < 8; i++)
        if (regs[i] == reg)
            return 'a' + i;
}

vector<Command *> Variable::toAll(char *regs)
{
    vector<Command *> commands;
    if (this->to->name[0] != regs[0])
        commands.push_back(allignReg(regs, this->to->name[0]));

    char reg = findReg(regs, this->at->name[0]);
    if (this->store)
        commands.push_back(new RegCommand("STORE", reg));
    else
        commands.push_back(new RegCommand("LOAD", reg));

    return commands;
}

vector<Command *> Constans::toAll(char *regs)
{
    long long zero = 0;
    vector<Command *> commands;
    if (this->use_inc)
    {
        int reg = findReg(regs, this->to->name[0]);
        commands.push_back(new RegCommand("RESET", reg));
        if (this->value > 0)
            for (long long i = 0; i < this->value; i++)
                commands.push_back(new RegCommand("INC", reg));
        else
            for (long long i = 0; i > this->value; i--)
                commands.push_back(new RegCommand("DEC", reg));
    }
    else
    {
        commands.push_back(allignReg(regs, this->to->name[0]));
        commands.push_back(new RegCommand("RESET", 'a'));
        commands.push_back(new RegCommand("RESET", 'h'));
        commands.push_back(new RegCommand("INC", 'h'));
        commands.push_back(new RegCommand("INC", 'h'));
        commands.push_back(new RegCommand("INC", 'h'));
        commands.push_back(new RegCommand("INC", 'h'));

        bool pos = (this->value > zero);
        long long val = this->value;
        if(val<zero)
            val = -val;
        long long s = 15;
        long long i = 0;
        while((s<<i) < val)
            i+=4;
        
        while( i > 0 )
        {
            for (long long j = (val>>i)&s; j > zero; j--)
                if(pos)
                    commands.push_back(new RegCommand("INC", 'a'));
                else
                    commands.push_back(new RegCommand("DEC", 'a'));
                    
            commands.push_back(new RegCommand("SHIFT", 'h'));
            i-=4;
        }
        for (long long j = val & s; j > zero; j--)
            if(pos)
                commands.push_back(new RegCommand("INC", 'a'));
            else
                commands.push_back(new RegCommand("DEC", 'a'));
    }
    return commands;
}

vector<Command *> VReg::toAll(char *regs) {}

vector<Command *> Reg::toAll(char *regs) {}

vector<Command *> Operation::toAll(char *regs)
{
    vector<Command *> commands;

    if (this->operation == '+')
    {
        if (this->dest->name[0] != regs[0])
            commands.push_back(allignReg(regs, this->dest->name[0]));
        if (this->dest->name == this->operand_a->name)
        {
            commands.push_back(new RegCommand("ADD", findReg(regs, this->operand_b->name[0])));
        }
        else if (this->dest->name == this->operand_b->name)
        {
            commands.push_back(new RegCommand("ADD", findReg(regs, this->operand_a->name[0])));
        }
        else
        {
            commands.push_back(new RegCommand("RESET", 'a'));
            commands.push_back(new RegCommand("ADD", findReg(regs, this->operand_a->name[0])));
            commands.push_back(new RegCommand("ADD", findReg(regs, this->operand_b->name[0])));
        }
    }
    else if (this->operation == '-')
    {
        if (this->dest->name[0] != regs[0])
            commands.push_back(allignReg(regs, this->dest->name[0]));
        if (this->dest->name == this->operand_a->name)
        {
            commands.push_back(new RegCommand("SUB", findReg(regs, this->operand_b->name[0])));
        }
        else
        {
            commands.push_back(new RegCommand("RESET", 'a'));
            commands.push_back(new RegCommand("ADD", findReg(regs, this->operand_a->name[0])));
            commands.push_back(new RegCommand("SUB", findReg(regs, this->operand_b->name[0])));
        }
    }
    else
    {
        char reg_a = findReg(regs, this->operand_a->name[0]);
        char reg_b = findReg(regs, this->operand_b->name[0]);
        char reg_d = findReg(regs, this->dest->name[0]);
        switch (this->operation)
        {
        case '/':
            commands = divide(reg_a, reg_b, reg_d, regs);
            break;
        case '*':
            commands = multiply(reg_a, reg_b, reg_d, regs);
            break;
        case '%':
            commands = modulo(reg_a, reg_b, reg_d, regs);
            break;
        }
    }

    return commands;
}

vector<Command *> Call::toAll(char *regs)
{
    vector<Command *> commands;
    if (this->operand->name[0] != regs[0])
        commands.push_back(allignReg(regs, this->operand->name[0]));
    commands.push_back(new Command(this->command));
    return commands;
}

vector<Command *> Jump::toAll(char *regs)
{
    vector<Command *> commands;
    if (this->on != nullptr && this->on->name[0] != regs[0])
        commands.push_back(allignReg(regs, this->on->name[0]));

    switch (this->type)
    {
    case 'Z':
        commands.push_back(new JumpCommand("JZERO", this->destination->name));
        break;
    case 'P':
        commands.push_back(new JumpCommand("JPOS", this->destination->name));
        break;
    case 'N':
        commands.push_back(new JumpCommand("JNEG", this->destination->name));
        break;
    case 'J':
        commands.push_back(new JumpCommand("JUMP", this->destination->name));
        break;
    }

    return commands;
}

vector<Command *> Array::toAll(char *regs)
{
    vector<Command *> commands;
    if (this->to->name[0] != regs[0])
        commands.push_back(allignReg(regs, this->to->name[0]));

    char reg = findReg(regs, this->at->name[0]);
    if (this->store)
        commands.push_back(new RegCommand("STORE", reg));
    else
        commands.push_back(new RegCommand("LOAD", reg));

    return commands;
}

vector<Command *> Assignment::toAll(char *regs)
{
    vector<Command *> commands;
    if (dynamic_cast<Reg *>(this->source))
    {
        if (dynamic_cast<Reg *>(this->dest))
        {
            if (this->dest->name[0] != regs[0])
            {
                commands.push_back(allignReg(regs, this->dest->name[0]));
            }
            commands.push_back(new RegCommand("ADD", findReg(regs, this->source->name[0])));
        }
        else
        {
            if (Array *arr = dynamic_cast<Array *>(this->dest))
            {
            }
            else if (Variable *arr = dynamic_cast<Variable *>(this->dest))
            {
            }
            commands.push_back(new RegCommand("STORE", this->source->name[0]));
        }
    }
    else
    {
        if (dynamic_cast<Reg *>(this->dest))
        {
        }
        else
        {
        }
    }
    return commands;
}

vector<Command *> Flag::toAll(char *regs)
{
    vector<Command *> commands;
    commands.push_back(new FlagCommand(this->name));
    return commands;
}