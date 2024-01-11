#include "Editor.h"
#include <Windows.h>
#include <conio.h>

void gotoRowCol(int rpos, int cpos)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	int xpos = cpos, ypos = rpos;
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = xpos;
	scrn.Y = ypos;
	SetConsoleCursorPosition(hOuput, scrn);
}
Editor::Editor(File* f ,DOS* dos)
{
	text = &f->data;
	currentRow = 0, currentCol = 0;
	list<char> temp;
	text->push_back(temp);
	rit = text->begin();
	cit = (*rit).begin();
	open = f;
	dosshell = dos;
}

void Editor::print()
{
	for (auto r = text->begin(); r != text->end(); r++)
	{
		for (auto c = (*r).begin(); c != (*r).end(); c++)
		{
			cout << *c;
		}
		cout << endl;
	}
}
void Editor::clearline()
{
	gotoRowCol(currentRow, 0);
	cout << "                                                                                                                  ";
	gotoRowCol(currentRow, 0);
}


void Editor::printsingleline()
{
	clearline();
	for (auto col = (*rit).begin(); col != (*rit).end(); col++)
	{
		cout << *col;
	}
	gotoRowCol(currentRow, currentCol);
}

void Editor::deleteoption()
{
	auto temp = cit;
	if (currentCol == 0 && !((*rit).empty()))
	{
		temp++;
		(*rit).pop_front();
		printsingleline();

	}
	else if (currentCol < (*rit).size() && cit  != (*rit).end())
	{
		(*rit).erase(++cit);
		printsingleline();
	}
	cit = temp;
}

void Editor::updateUndo()
{
	static int limit = 5;
	if (undo.size() > limit)
	{
		undo.pop_front();
	}
	state s = savestate();
	undo.push_back(s);
}

Editor::state Editor::savestate()
{
	state* s = new state;
	s->r = 0;
	s->c = 0;
	s->text.push_back(list<char>());
	auto r_itr = s->text.begin();
	for (auto row = text->begin(); row != text->end(); row++)
	{
		if (row != text->begin())
		{
			s->text.push_back(list<char>());
			r_itr++;
		}
		for (auto col = (*row).begin(); col != (*row).end(); col++)
		{
			char ch = *col;
			(*r_itr).push_back(ch);
		}
	}
	s->c = currentCol;
	s->r = currentRow;
	return *s;
}

void Editor::enteroption()
{
	auto start = cit;
	auto end = cit;
	auto c = cit;
	auto prerow = rit;
	auto temprow = rit;
	if (currentCol != 0)
	{
		c++;
		start++;
	}
	rit++;
	text->insert(rit, list<char>());
	rit = ++temprow;
	for (;c != (*prerow).end(); c++)
	{
		(*rit).push_back(*c);
		end++;
	}
	if (currentCol == 0) {
		(*prerow).erase(start, end);
	}
	else {
		(*prerow).erase(start, ++end);
	}
	currentRow++;
	currentCol = 0;
	gotoRowCol(currentRow, currentCol);
	cit = (*rit).begin();
	system("cls");
	print();

}

void Editor::backspaceOption()
{
	if (currentCol == 1)
	{
		cit++;
		(*rit).pop_front();
		printsingleline();
		currentCol--;
	}
	else if (currentCol != 0)
	{
		auto temp = cit;
		temp--;
		(*rit).erase(cit);
		cit = temp;
		printsingleline();
		currentCol--;
	}
	else if (currentCol == 0 && currentRow != 0)
	{
		auto temp = rit;
		rit--;
		currentCol = (*rit).size();
		(*rit).splice((*rit).end(), (*temp));
		text->erase(temp);
		system("cls");
		print();
		currentRow--;
		cit = (*rit).begin();
		if (currentCol > 0)
		{
			advance(cit, currentCol - 1);
		}
		else
		{
			advance(cit, currentCol);
		}
	}
}

void Editor::insertthecharacter(char c)
{
	if (currentCol == 0)
	{
		(*rit).push_front(c);
		cit = (*rit).begin();
		currentCol++;
		printsingleline();
	}
	else
	{
		auto temp = cit;
		cit++;
		(*rit).insert(cit, c);
		cit = ++temp;
		currentCol++;
		printsingleline();
	}
}

void Editor::undoEditor()
{
	if (!undo.empty())
	{
		redo.push(savestate());
		loadState(undo.back());
		undo.pop_back();
		system("cls");
		print();
	}
}

void Editor::redoEditor()
{
	if (!redo.empty())
	{
		undo.push_back(savestate());
		loadState(redo.top());
		redo.pop();
		system("cls");
		print();
	}

}

void Editor::loadState(state s)
{
	*text = s.text;
	rit = text->begin();
	currentCol = s.c;
	currentRow = s.r;
	for (int idx = 0; idx < currentRow; idx++)
	{
		rit++;
	}
	cit = (*rit).begin();
	for (int idx = 0; idx < currentCol - 1; idx++)
	{
		cit++;
	}
}


void Editor::Edit()
{
	gotoRowCol(currentRow, currentCol);
	int d;
	char ch;
	system("Color F0");
	system("cls");
	print();

	while (true)
	{
		gotoRowCol(currentRow, currentCol);
		if (_kbhit)
		{
			d = _getch();
			if (d == 224)
			{
				d = _getch();
				if (d == 72)  //Up arrow key
				{
					if (currentRow > 0)
					{
						currentRow--;
						rit--;
						cit = (*rit).begin();
						if ((*rit).size() <= currentCol)
						{
							currentCol = (*rit).size();
						}
						if (currentCol > 0)
						{
							advance(cit, currentCol - 1);
						}
						else
						{
							advance(cit, currentCol);
						}
					}

				}

				else if (d == 80)  //down arrow
				{
					if (currentRow < text->size() - 1)
					{
						currentRow++;
						rit++;
						cit = (*rit).begin();
						if ((*rit).size() <= currentCol)
						{
							currentCol = (*rit).size();
						}
						if (currentCol > 0)
						{
							advance(cit, currentCol - 1);
						}
						else
						{
							advance(cit, currentCol);
						}

					}
				}

				else if (d == 75)  //left arrow
				{
					if (currentCol > 0)
					{
						currentCol--;
						if (currentCol != 0)
						{
							cit--;
						}
					}
					else if (currentCol == 0 && currentRow > 0)
					{
						currentRow--;
						rit--;
						cit = (*rit).begin();
						currentCol = (*rit).size();
						if (currentCol > 0)
						{
							advance(cit, currentCol - 1);
						}
						else
						{
							advance(cit, currentCol);
						}
					}
				}

				else if (d == 77)  //Right Arrow
				{
					if (currentCol < (*rit).size())
					{
						if (currentCol != 0)
						{
							cit++;
						}
						currentCol++;

					}
					else if (text->size() - 1 > currentRow)
					{
						currentRow++;
						rit++;
						cit = (*rit).begin();
						currentCol = 0;
					}
				}
				else if (d == 83) //delete
				{
					updateUndo();
					deleteoption();
				}

			}
			else if (d == 13)   //Enter Option
			{
				updateUndo();
				enteroption();

			}
			else if (d == 8)    //Backspace option
			{
				updateUndo();
				backspaceOption();
            }
			else if (d == 26)    //ctrl+y
			{
				undoEditor();
            }
			else if (d == 25)   //ctrl+y
			{
				redoEditor();
			}
			else if (d == 27)   //escape 
			{
				cout << endl;
				break;
            }
			else if (d == 9) //tab
			{
				dosshell->openThefileEditor(true, open);
				return;
            }
			else if (d == 124) //pipe
			{
				dosshell->openThefileEditor(false, open);
				return;
			}

			else
			{
				insertthecharacter(char(d));
				updateUndo();
			}

		}

	}
	system("cls");
	system("Color 0F");
}


