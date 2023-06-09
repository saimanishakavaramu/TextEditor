#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <tuple>

#include "ECTextDocument.h"

void ECInsertText :: Execute()
{
    doc.InsertCharAt(ch);
}

void ECInsertText :: UnExecute()
{
    doc.RemoveCharAt();
}


void ECBackSpace :: Execute()
{
    int row = doc.getY();
    int pos = doc.getX();

    if (row >= 1 && pos == 0)
    {
            doc.MergeAt();
            merged = true;
    }
    else if(row >=0 && pos >= 1)
    {
        ch = doc.getlist()[row][pos-1];
        doc.RemoveCharAt();
    }
}
 
void ECBackSpace :: UnExecute()
{
    if (merged)
    {
        doc.EnterAt();
    }
    else
    {
        doc.InsertCharAt(ch);
    }
}


void ECEnter :: Execute()
{
    doc.EnterAt();
}


void ECEnter :: UnExecute()
{
    doc.MergeAt();
}


void ECCursorUp :: Execute()
{
    int row = doc.getY();
    int pos = doc.getX();

    int rowSize = doc.getlist()[row].size();

    if (row != 0){
        int rowAbove = doc.getlist()[row - 1].size();
        if (rowAbove < pos)
        {
            textview.SetCursorX(rowAbove);
        }
        else
        {
            textview.SetCursorX(pos);
        }
        textview.SetCursorY(row - 1);
    }
}

void ECCursorDown :: Execute()
{
    int row = doc.getY();
    int pos = doc.getX();

    int rowSize = doc.getlist()[row].size();

    if (row < doc.getlist().size() - 1)
    {
        int rowBelow = doc.getlist()[row+1].size();

        if(rowBelow < pos)
        {
            textview.SetCursorX(rowBelow);
        }
        else
        {
            textview.SetCursorX(pos);
        }

        textview.SetCursorY(row + 1);
    }
}

void ECCursorLeft :: Execute()
{
    int row = doc.getY();
    int pos = doc.getX();

    if (pos > 0)
    {
        textview.SetCursorX(pos - 1);
    }
}

void ECCursorRight :: Execute()
{
    int row = doc.getY();
    int pos = doc.getX();
    int limit = doc.getlist()[row].size();

    if (pos < limit)
    {
        textview.SetCursorX(pos + 1);
    }
}


void ECControl :: Update() 
{
        int lastKey = textview.GetPressedKey();

        if (doc.insertMode == true)
        {
            if (isprint(lastKey))
            {
                ECInsertText *pCmd = new ECInsertText(this->doc, lastKey);
                histCmds.ExecuteCmd(pCmd);
            }
            else if(lastKey == BACKSPACE)
            {
                ECBackSpace *pCmd = new ECBackSpace(this->doc);
                histCmds.ExecuteCmd(pCmd);
            }
            else if(lastKey == ENTER)
            {
                ECEnter *pCmd = new ECEnter(this->doc);
                histCmds.ExecuteCmd(pCmd);
            }
            else if(lastKey == ESC)
            {
                doc.commandMode = true;
                doc.insertMode = false;
            }
        }
        if(lastKey == ARROW_UP)
        {
            ECCursorUp *pCmd = new ECCursorUp(this->doc, textview);
            pCmd->Execute();
        }
        else if(lastKey == ARROW_DOWN)
        {
            ECCursorDown *pCmd = new ECCursorDown(this->doc, textview);
            pCmd->Execute();
        }
        else if(lastKey == ARROW_LEFT)
        {
            ECCursorLeft *pCmd = new ECCursorLeft(this->doc, textview);
            pCmd->Execute();
        }
        else if(lastKey == ARROW_RIGHT)
        {
            ECCursorRight *pCmd = new ECCursorRight(this->doc, textview);
            pCmd->Execute();
        }
        else if(doc.commandMode == true && lastKey == 'i')
        {
            doc.commandMode = false;
            doc.insertMode = true;
            histCmds.Clear();
        }   
        else if(lastKey == CTRL_Z)
        {
            histCmds.Undo();
            
        }
        else if(lastKey == CTRL_Y)
        {
            histCmds.Redo();
        }
    }


    void ECDocument :: updateView()
    {
        textview.InitRows();
        for (auto row : list)
        {
            textview.AddRow(row);
        }
        writelines();

        if (keywordfile == true)
        {
            highlight();
            for (auto x : keyword_locations)
            {
                textview.SetColor(std::get<0>(x),  std::get<1>(x), std::get<2>(x), TEXT_COLOR_BLUE);
            }
        }
    }

    void ECDocument :: readlines(std::istream& infile)
    {
        std::string line;
        while (std::getline(infile, line)) 
        {
        list.push_back(line);
        }
    }

    void ECDocument :: writelines()
    {
        std::ofstream outfile(fileWrite);
        if (outfile.is_open()) 
        {  
            for (auto line : list) { 
                outfile << line << "\n";
            } 
        }
        outfile.close();
    }

    void ECDocument :: getkeyWords(std::istream& infile)
    {
        std::string word;
        while (std::getline(infile, word)) 
        {
            keywords.push_back(word);
        }
    }

    void ECDocument :: highlight()
    {
        for (int i = 0; i < list.size(); ++i) {
            const std::string& sentence = list[i];
            int startIdx = 0;
            int endIdx = 0;

            for (int j = 0; j < keywords.size(); ++j) {
                const std::string& keyword = keywords[j];
                startIdx = sentence.find(keyword);
                
                while (startIdx != std::string::npos) {
                    endIdx = startIdx + keyword.length() - 1;
                    keyword_locations.push_back(std::make_tuple(i, startIdx, endIdx));
                    startIdx = sentence.find(keyword, endIdx + 1);
                }
            }
        }
    }
