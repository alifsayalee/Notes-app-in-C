# Notes-app-in-C
This is my first project on GitHub. It was my end-semester FoCP project. I hope to make my projects even better from now onwards!

I've installed Microsoft Visual Studio Code on Windows Subsystem for Linux (WSL) with Ubuntu 20.04.6 LTS. This project requires a system command to be run thus it is hard to implement it on Windows. First, you must install WSL and set it up for VS code. And then after configuring WSL with VS code, you must install Nano or the editor will not work.

This project includes the four main functions a notes app should have:
• Create a note
• View a note
• Edit a note
• Update a note

I've used file handling to implement these functions. 
The Create function creates a new .txt file and puts in the typed information into that .txt file as a note. It further adds the name of the note to a NoteNames.txt file so that it is easily retrievable when viewing a note.
The View function retrieves the selected note from note names displayed on the screen. If a wrong name is entered, it displays an error statement.
The Edit function is the most interesting as terminal editing is not possible in Windows. Hence, I've used WSL for my project and opened the note in Nano, a Linux-based terminal editor.
The Delete function performs the delete action and removes both the note (.txt file) and its name from the NoteNames.txt file.
