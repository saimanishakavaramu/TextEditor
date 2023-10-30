# TextEditor-like-Vim

About the Project:
This repository hosts a fundamental text editor I developed during my sophomore year for a C++ course. The editor facilitates functionalities such as reading and writing files, as well as basic text manipulations like adding, deleting, and editing text. Additionally, it gracefully wraps long paragraphs but currently does not support pagination.

Though effective in its core functions, the editor does have some known issues, particularly with erasing blank lines. The code is functional but can benefit from some clean-up and debugging.

I've not revisited this code post my class, but there are plans to refactor it for enhancements and bug fixes in the future.

Getting Started:

Change to the project's directory.
Use the make command in your terminal to compile the source files.
To initiate the editor, enter ./ECTextEditor filename.
Employ any existing .txt file from the directory.
Should the provided filename not be found, a new file bearing the mentioned name will be generated.
Key Functionalities:

Ctrl-q: Save and exit
Ctrl-s: Save changes
Arrow keys: Cursor movement
Enter/return: Initiate a new paragraph
Backspace: Erase content or merge with preceding line
Ctrl-z: Revert action
Ctrl-y: Reapply action
Alphabet keys: Input text
