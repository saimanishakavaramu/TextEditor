#ifndef ECTextDocument_h
#define ECTextDocument_h

#include "ECTextViewImp.h"
#include "ECObserver.h"
#include "ECCommand.h"

#include <vector>
#include <string>
#include <algorithm>

class ECDocument;

class ECInsertText : public ECCommand
{
public:
    ECInsertText(ECDocument &docIn, char chIn) : doc(docIn), ch(chIn) {}
    ~ECInsertText() {}
    virtual void Execute();
    virtual void UnExecute();

private:
    ECDocument &doc;
    char ch;
};

class ECBackSpace: public ECCommand
{
public:
    ECBackSpace(ECDocument &docIn) : doc(docIn){}
    ~ECBackSpace() {}
    virtual void Execute();
    virtual void UnExecute();

private:
    ECDocument &doc;
    char ch;
    bool merged = false;
};

class ECEnter : public ECCommand
{
public: 
    ECEnter(ECDocument &docIn) : doc(docIn) {}
    ~ECEnter() {}
    virtual void Execute();
    virtual void UnExecute();

private:
    ECDocument &doc;
};


class ECCursorUp : public ECCommand
{
public: 
    ECCursorUp(ECDocument &docIn, ECTextViewImp &textviewIn) : doc(docIn), textview(textviewIn) {}
    virtual void Execute();
    virtual void UnExecute() {};

private:
    ECDocument &doc;
    ECTextViewImp &textview;
};


class ECCursorDown : public ECCommand
{
public: 
    ECCursorDown(ECDocument &docIn, ECTextViewImp &textviewIn) : doc(docIn), textview(textviewIn) {}
    virtual void Execute();
    virtual void UnExecute() {};

private:
    ECDocument &doc;
    ECTextViewImp &textview;
};

class ECCursorLeft : public ECCommand
{
public: 
    ECCursorLeft(ECDocument &docIn, ECTextViewImp &textviewIn) : doc(docIn), textview(textviewIn) {}
    virtual void Execute();
    virtual void UnExecute() {};

private:
    ECDocument &doc;
    ECTextViewImp &textview;
};

class ECCursorRight : public ECCommand
{
public: 
    ECCursorRight(ECDocument &docIn, ECTextViewImp &textviewIn) : doc(docIn), textview(textviewIn) {}
    virtual void Execute();
    virtual void UnExecute() {};

private:
    ECDocument &doc;
    ECTextViewImp &textview;
};


//***Controller***
class ECControl : public ECObserver
{
public:
    ECControl(ECDocument &docIn, ECTextViewImp &textviewIn) : doc(docIn), textview(textviewIn) {} 
    virtual ~ECControl(){}
    virtual void Update();

private:
    ECDocument &doc;
    ECTextViewImp &textview;
    ECCommandHistory histCmds;
};


//***Model***
class ECDocument
{
public:
    ECDocument(ECTextViewImp &textviewIn) : docCtrl(ECControl(*this, textviewIn)), textview(textviewIn) {}
    virtual ~ECDocument() {} 

    void InsertCharAt(char ch)
    {
        int row = textview.GetCursorY();
        int pos = textview.GetCursorX();


        if (list.size() != 0)
        {
            list[row].insert(list[row].begin() + pos, ch);
            textview.SetCursorX(pos + 1);
        }

        updateView();
    }

    void RemoveCharAt()
    {
        int row = textview.GetCursorY();
        int pos = textview.GetCursorX();

        list[row].erase(list[row].begin()+pos-1);
        textview.SetCursorX(pos - 1);

        updateView();
    }

    void EnterAt()
    {
        int row = textview.GetCursorY();
        int pos = textview.GetCursorX();
        

        std::string behindC = list[row].substr(0, pos);
        std::string afterC = list[row].substr(pos);
        list.erase(list.begin() + row);
        list.insert(list.begin() + row, behindC);
        list.insert(list.begin() + row + 1, afterC);

        textview.SetCursorX(0);
        textview.SetCursorY(row + 1);

        updateView();
    }

    void MergeAt()
    {
        int row = textview.GetCursorY();

        int sizeLastRow = list[row-1].size();
        list[row-1].append(list[row]);
        for (int i = row; i < list.size() - 1; i++)
        {
            list[i] = list[i+1];
        }
        list.resize(list.size() - 1);

        textview.SetCursorY(row - 1);
        textview.SetCursorX(sizeLastRow);
        
        updateView();
    }

    void InitRow()
    {
    if (list.size() == 0)
    {
        list.push_back("");
    }
    }

    void setfileWrite(std::string filename) {
        fileWrite = filename;
    }

    void readlines(std::istream& infile);
    void writelines();
    
    void getkeyWords(std::istream& infile);
    void highlight();

    void updateView();

    std::vector<std::string> getlist() { return list; }
    int getX() { return textview.GetCursorX(); }
    int getY() { return textview.GetCursorY(); }

    bool commandMode = true;
    bool insertMode = false;

    bool keywordfile = false;

private:                  
    ECControl docCtrl;
    ECTextViewImp &textview;
    std::string fileWrite;
    std::vector<std::string> list;

    std::vector<std::string> keywords;
    std::vector<std::tuple<int, int, int>> keyword_locations;

};

#endif 