CXX = g++
CXXFLAGS = -std=c++11

myeditor: ECTextViewImp.cpp ECTextDocument.cpp ECCommand.cpp ECEditorTest.cpp
	$(CXX) $(CXXFLAGS) ECTextViewImp.cpp ECTextDocument.cpp ECCommand.cpp ECEditorTest.cpp -o myeditor