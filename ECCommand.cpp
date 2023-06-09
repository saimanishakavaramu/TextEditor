#include "ECCommand.h"

// ******************************************************
// Implement command history
ECCommandHistory :: ECCommandHistory() : posCurrCmd(-1)
{
}

ECCommandHistory :: ~ECCommandHistory()
{
    for(unsigned int i=0; i<listCommands.size(); ++i)
    {
        delete listCommands[i];
    }
    listCommands.clear();
    posCurrCmd = -1;
}

bool ECCommandHistory :: Undo()
{
    for (int i = listCommands.size() - 1; i >=0; --i)
    {
        listCommands[i]->UnExecute();
    }
    return true;
}

bool ECCommandHistory :: Redo()
{
    for (int i = 0; i < listCommands.size(); ++i)
    {
        listCommands[i]->Execute();
    }
    return true;
}

void ECCommandHistory :: ExecuteCmd( ECCommand *pCmd )
{
    pCmd->Execute();
    listCommands.push_back(pCmd);
}

void ECCommandHistory :: Clear()
{
    for(int i=0; i<listCommands.size(); ++i)
    {
        delete listCommands[i];
    }
    listCommands.clear();
    posCurrCmd = -1;
}