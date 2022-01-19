#include <string>
using namespace std;

class Command
{
    virtual void toASM(FILE out);
};

class GetCom : Command
{
};
class PutCom : Command
{
};
class LoadCom : Command
{
};
class StoreCom : Command
{
};
class AddCom : Command
{
};
class SubCom : Command
{
};
class ShiftCom : Command
{
};
class SwapCom : Command
{
};
class ResetCom : Command
{
};
class IncCom : Command
{
};
class DecCom : Command
{
};
class JumpCom : Command
{
};
class JposCom : Command
{
};
class JzeroCom : Command
{
};
class JnegCom : Command
{
};
class HaltCom : Command
{
};