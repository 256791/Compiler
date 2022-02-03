#include "ALLMDM.h"

vector<Command *> multiply(char a, char b, char result, char *regs)
{
    string f1 = Flag::getNewName();
    string f2 = Flag::getNewName();
    string f3 = Flag::getNewName();
    string f4 = Flag::getNewName();
    string f5 = Flag::getNewName();
    string f6 = Flag::getNewName();
    string f7 = Flag::getNewName();
    string f8 = Flag::getNewName();
    string f9 = Flag::getNewName();
    string f10 = Flag::getNewName();

    vector<Command *> n;

    n.push_back(new RegCommand("SWAP", b));
    n.push_back(new RegCommand("SWAP", 'c'));
    if(a == 'a' && b != 'c')
        n.push_back(new RegCommand("SWAP", b));
    else if(a != 'c')
        n.push_back(new RegCommand("SWAP", a));

    n.push_back(new RegCommand("RESET", 'd'));
    n.push_back(new RegCommand("RESET", 'h'));
    n.push_back(new JumpCommand("JNEG", f1));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new JumpCommand("JPOS", f2));
    n.push_back(new RegCommand("INC", 'h'));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'g'));
    n.push_back(new JumpCommand("JUMP", f2));
    n.push_back(new FlagCommand(f1));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new JumpCommand("JNEG", f3));
    n.push_back(new RegCommand("INC", 'h'));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'g'));
    n.push_back(new JumpCommand("JUMP", f2));
    n.push_back(new FlagCommand(f3));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'g'));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'g'));
    n.push_back(new FlagCommand(f2));

    n.push_back(new RegCommand("RESET", 'f'));
    n.push_back(new RegCommand("RESET", 'g'));
    n.push_back(new RegCommand("INC", 'f'));
    n.push_back(new RegCommand("DEC", 'g'));

    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("ADD", 'b'));
    n.push_back(new RegCommand("SUB", 'c'));
    n.push_back(new JumpCommand("JNEG", f4));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new JumpCommand("JUMP", f5));
    n.push_back(new FlagCommand(f4));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new FlagCommand(f5));

    n.push_back(new JumpCommand("JZERO", f6));
    n.push_back(new FlagCommand(f7));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("ADD", 'b'));
    n.push_back(new RegCommand("SHIFT", 'g'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("ADD", 'b'));
    n.push_back(new RegCommand("SUB", 'e'));
    n.push_back(new JumpCommand("JZERO", f8));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("ADD", 'c'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new FlagCommand(f8));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("SHIFT", 'g'));
    n.push_back(new JumpCommand("JPOS", f7));
    n.push_back(new FlagCommand(f6));

    n.push_back(new RegCommand("SWAP", 'h'));
    n.push_back(new JumpCommand("JZERO", f9));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'd'));
    n.push_back(new JumpCommand("JUMP", f10));
    n.push_back(new FlagCommand(f9));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new FlagCommand(f10));

    n.push_back(new RegCommand("SWAP", result));

    return n;
}

vector<Command *> divide(char a, char b, char result, char *regs)
{
    vector<Command *> n;
    string f1 = Flag::getNewName();
    string f2 = Flag::getNewName();
    string f3 = Flag::getNewName();
    string f4 = Flag::getNewName();
    string f5 = Flag::getNewName();
    string f6 = Flag::getNewName();
    string f7 = Flag::getNewName();
    string f8 = Flag::getNewName();
    string f9 = Flag::getNewName();
    string f10 = Flag::getNewName();
    string f11 = Flag::getNewName();
    string f12 = Flag::getNewName();


    n.push_back(new RegCommand("SWAP", a));
    n.push_back(new RegCommand("SWAP", 'b'));
    if (b == 'a' && a != 'b')
        n.push_back(new RegCommand("SWAP", a));
    else if (b != 'b')
        n.push_back(new RegCommand("SWAP", b));
    n.push_back(new RegCommand("SWAP", 'c'));

    n.push_back(new RegCommand("RESET", 'g'));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JZERO", f1));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new JumpCommand("JZERO", f1));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("RESET", 'h'));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new JumpCommand("JNEG", f2));
    n.push_back(new JumpCommand("JUMP", f3));
    n.push_back(new FlagCommand(f2));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'c'));
    n.push_back(new RegCommand("INC", 'h'));
    n.push_back(new FlagCommand(f3));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JNEG", f4));
    n.push_back(new JumpCommand("JUMP", f5));
    n.push_back(new FlagCommand(f4));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'b'));
    n.push_back(new RegCommand("DEC", 'h'));
    n.push_back(new FlagCommand(f5));
    n.push_back(new RegCommand("SWAP", 'b'));

    n.push_back(new RegCommand("RESET", 'd'));
    n.push_back(new RegCommand("RESET", 'e'));
    n.push_back(new RegCommand("RESET", 'g'));
    n.push_back(new RegCommand("RESET", 'f'));
    n.push_back(new RegCommand("INC", 'f'));
    n.push_back(new RegCommand("INC", 'e'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("ADD", 'c'));
    n.push_back(new RegCommand("SWAP", 'd'));

    n.push_back(new FlagCommand(f6));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("ADD", 'b'));
    n.push_back(new RegCommand("SUB", 'd'));
    n.push_back(new JumpCommand("JNEG", f7));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("INC", 'g'));
    n.push_back(new JumpCommand("JUMP", f6));
    n.push_back(new FlagCommand(f7));

    n.push_back(new RegCommand("DEC", 'f'));
    n.push_back(new RegCommand("DEC", 'f'));
    n.push_back(new RegCommand("DEC", 'g'));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("ADD", 'c'));
    n.push_back(new RegCommand("SHIFT", 'g'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("RESET", 'g'));

    n.push_back(new RegCommand("SWAP", 'e'));

    n.push_back(new FlagCommand(f8));
    n.push_back(new JumpCommand("JZERO", f9));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("ADD", 'b'));
    n.push_back(new RegCommand("SUB", 'd'));
    n.push_back(new JumpCommand("JNEG", f10));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new RegCommand("ADD", 'e'));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new FlagCommand(f10));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new JumpCommand("JUMP", f8));
    n.push_back(new FlagCommand(f9));

    n.push_back(new RegCommand("SWAP", 'h'));
    n.push_back(new JumpCommand("JZERO", f11));
    //
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JZERO", f12));
    n.push_back(new RegCommand("INC", 'g'));
    n.push_back(new FlagCommand(f12));
    n.push_back(new RegCommand("SWAP", 'b'));
    //
    // n.push_back(new RegCommand("INC", 'g'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'g'));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new FlagCommand(f11));

    n.push_back(new FlagCommand(f1));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new RegCommand("SWAP", result));
    return n;
}

vector<Command *> modulo(char a, char b, char result, char *regs)
{
    vector<Command *> n;
    string f1 = Flag::getNewName();
    string f2 = Flag::getNewName();
    string f3 = Flag::getNewName();
    string f4 = Flag::getNewName();
    string f5 = Flag::getNewName();
    string f6 = Flag::getNewName();
    string f7 = Flag::getNewName();
    string f8 = Flag::getNewName();
    string f9 = Flag::getNewName();
    string f10 = Flag::getNewName();
    string f11 = Flag::getNewName();
    string f12 = Flag::getNewName();
    string f13 = Flag::getNewName();
    string f14 = Flag::getNewName();
    string f15 = Flag::getNewName();
    string f16 = Flag::getNewName();
    string f17 = Flag::getNewName();
    string f18 = Flag::getNewName();
    string f19 = Flag::getNewName();

    n.push_back(new RegCommand("SWAP", a));
    n.push_back(new RegCommand("SWAP", 'b'));
    if(b == 'a' && a != 'b')
        n.push_back(new RegCommand("SWAP", a));
    else if (b != 'b')
        n.push_back(new RegCommand("SWAP", b));
    n.push_back(new RegCommand("SWAP", 'c'));

    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JZERO", f1));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new JumpCommand("JZERO", f12));
    n.push_back(new JumpCommand("JUMP", f13));
    n.push_back(new FlagCommand(f12));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JUMP", f1));
    n.push_back(new FlagCommand(f13));
    n.push_back(new RegCommand("SWAP", 'c'));

    n.push_back(new RegCommand("RESET", 'h'));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new JumpCommand("JNEG", f2));
    n.push_back(new JumpCommand("JUMP", f3));
    n.push_back(new FlagCommand(f2));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'c'));
    n.push_back(new RegCommand("INC", 'h'));
    n.push_back(new FlagCommand(f3));
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JNEG", f4));
    n.push_back(new JumpCommand("JUMP", f5));
    n.push_back(new FlagCommand(f4));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'b'));
    n.push_back(new RegCommand("INC", 'h'));
    n.push_back(new RegCommand("INC", 'h'));
    n.push_back(new FlagCommand(f5));
    n.push_back(new RegCommand("SWAP", 'b'));

    n.push_back(new RegCommand("RESET", 'd'));
    n.push_back(new RegCommand("RESET", 'e'));
    n.push_back(new RegCommand("RESET", 'g'));
    n.push_back(new RegCommand("RESET", 'f'));
    n.push_back(new RegCommand("INC", 'f'));
    n.push_back(new RegCommand("INC", 'e'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("ADD", 'c'));
    n.push_back(new RegCommand("SWAP", 'd'));

    n.push_back(new FlagCommand(f6));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("ADD", 'b'));
    n.push_back(new RegCommand("SUB", 'd'));
    n.push_back(new JumpCommand("JNEG", f7));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("INC", 'g'));
    n.push_back(new JumpCommand("JUMP", f6));
    n.push_back(new FlagCommand(f7));

    n.push_back(new RegCommand("DEC", 'f'));
    n.push_back(new RegCommand("DEC", 'f'));
    n.push_back(new RegCommand("DEC", 'g'));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("ADD", 'c'));
    n.push_back(new RegCommand("SHIFT", 'g'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("RESET", 'g'));

    n.push_back(new RegCommand("SWAP", 'e'));

    n.push_back(new FlagCommand(f8));
    n.push_back(new JumpCommand("JZERO", f9));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("ADD", 'b'));
    n.push_back(new RegCommand("SUB", 'd'));
    n.push_back(new JumpCommand("JNEG", f10));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new RegCommand("ADD", 'e'));
    n.push_back(new RegCommand("SWAP", 'g'));
    n.push_back(new FlagCommand(f10));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new RegCommand("SWAP", 'd'));
    n.push_back(new RegCommand("SWAP", 'e'));
    n.push_back(new RegCommand("SHIFT", 'f'));
    n.push_back(new JumpCommand("JUMP", f8));
    n.push_back(new FlagCommand(f9));

    n.push_back(new RegCommand("SWAP", 'h'));
    n.push_back(new JumpCommand("JZERO", f11));
    n.push_back(new RegCommand("DEC", 'a'));
    n.push_back(new JumpCommand("JZERO", f14));
    n.push_back(new RegCommand("DEC", 'a'));
    n.push_back(new JumpCommand("JZERO", f15));
    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'b'));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JUMP", f11));
    n.push_back(new FlagCommand(f15));
    // neg pos
    //b1
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JZERO", f18));
    n.push_back(new RegCommand("SWAP", 'b'));
    //e1
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("SUB", 'b'));
    //b1
    n.push_back(new FlagCommand(f18));
    //e1
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JUMP", f11));
    n.push_back(new FlagCommand(f14));
    //b2
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new JumpCommand("JZERO", f17));
    n.push_back(new RegCommand("SWAP", 'b'));
    //e2
    n.push_back(new RegCommand("SWAP", 'c'));
    n.push_back(new RegCommand("SUB", 'b'));
    //b2
    n.push_back(new FlagCommand(f17));
    //e2
    n.push_back(new RegCommand("SWAP", 'b'));

    n.push_back(new RegCommand("RESET", 'a'));
    n.push_back(new RegCommand("SUB", 'b'));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new FlagCommand(f11));

    n.push_back(new FlagCommand(f1));
    n.push_back(new RegCommand("SWAP", 'b'));
    n.push_back(new RegCommand("SWAP", result));

    return n;
}
