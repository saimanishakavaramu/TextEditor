// Test code for editor
#include "ECTextViewImp.h"
#include "ECObserver.h"
#include "ECTextDocument.h"
#include "ECCommand.h"

#include <iostream>
#include <fstream>
#include <filesystem>

using namespace  std;

int myCounter = 0;

int main(int argc, char *argv[])
{
    
    ECTextViewImp wndTest;
    wndTest.AddStatusRow("Editor", "For demo only", true);

    ECDocument doc(wndTest);

    //Keyword Highlighting
    const string keywordFilename = "keywords.txt";
    for (const auto& entry : std::__fs::filesystem::directory_iterator(".")) {
        if (entry.is_regular_file() && entry.path().filename() == keywordFilename) {
            doc.keywordfile = true;
            ifstream keyword(keywordFilename);
            doc.getkeyWords(keyword);
        }
    }

    //File I/O 
    if(argc == 2)
    {
        string filename = argv[1];
        doc.setfileWrite(filename);

        ifstream inputfile(filename);
        if (inputfile.good())
        {
            doc.readlines(inputfile);
            inputfile.close();
            doc.updateView();
        }
        else
        {
            std::ofstream file {argv[1]}; 
            doc.InitRow();
        }
    }
    else 
    {
        cout << "Usage: ./myeditor filename" << endl;  
        return 1;
    }

    ECControl docCtrl(doc, wndTest);
    wndTest.Attach(&docCtrl);
    wndTest.Show();

    return 0;
}
