#pragma once
#include <iostream>
#include "File.h"
#include "DOS.h"
#include <string>
#include <stack>

using namespace std;

class Editor {
public:
	class state 
	{
	public:
		list<list<char>> text;
		int r, c;
	};
	list<list<char>>* text;
	list<list<char>>::iterator rit;
	list<char>::iterator cit;
	int currentRow = 0;
	int currentCol = 0;
	deque<state> undo;
	stack<state> redo;
	File* open;
	DOS* dosshell;

	Editor(File* f ,DOS* D);

	void Edit();

	void print();

	void printsingleline();

	void clearline();

	void deleteoption();

	void updateUndo();

	state savestate();

	void enteroption();

	void backspaceOption();

	void insertthecharacter(char c);

	void undoEditor();

	void redoEditor();

	void loadState(state s);

};