/* File: dppmain.cc
 * ----------------
 * This file defines the main() routine for the preprocessor, 
 * the filtering tool which runs before the compiler.
 */
 
#include "scanner.h"
#include "errors.h"
#include <string>
#include <iostream>
#include <unordered_map>
#include <sstream>

using namespace std;

/* Function: main()
 * ----------------
 * Entry point to the preprocessor.
 * As given below, this "filter" just copies stdin to stdout unchanged.
 * It will be your job to actually do some filtering, either by hand-coded
 * transformation of the input or by setting up a lex scanner in the dpp.l
 * file and changing the main below to invoke it via yylex. When finished,
 * the preprocessor should echo stdin to stdout making the transformations
 * to strip comments and handle preprocessor directives.
 */
 
int main(int argc, char *argv[])
{
	unordered_map<string, string> macros;
	unordered_map<string, bool> macroValid;
	istringstream iss;
	string s, NAME;
	int ch, lineNum = 1;
	bool quote = false;
	ReportError error;

	
	while ((ch = cin.get()) != EOF)
	{
		//toggle quote if inside a string literal
		if (ch == '"')
			quote = !quote;
	
		//check for comments
		if (ch == '/' && !quote)
		{
			ch = cin.get();
			//enter if single line comment "//"
			if (ch == '/')
			{
				getline(cin, s);
				cout << '\n';
				lineNum++;
				continue;
			}
			//enter if multi line comment "/*"
			if (ch == '*')
			{
				while (ch != '/')
				{
					if (!getline(cin, s, '*'))
					{
						error.UntermComment();
						exit(1);
					}
					
					//iterate through comment, add newlines
					for (auto it = s.begin(); it != s.end(); ++it)
					{
						if (*it == '\n')
						{
							cout << '\n';
							lineNum++;
						}
					}
					
					ch = cin.get();
				}
				continue;
			}
			//enter if not a comment
			else
				cout << '/';
		}
		
		//check for macros
		if (ch == '#' && !quote)
		{
			ch = cin.get();
			//check for define
			if (ch == 'd')
			{
				cin.unget();
				getline(cin, s);
				lineNum++;
				iss.clear();
				iss.str(s);
				getline(iss, s, ' ');
				//first word is define
				if (s == "define")
				{
					getline(iss, NAME, ' ');
					iss.unget();
					//must have space between NAME and replacement
					if (iss.get() != ' ')
					{
						error.InvalidDirective(lineNum - 1);
						cout << '\n';
						continue;
					}
					for (auto it = NAME.begin(); it!= NAME.end(); ++it)
					{
						//NAME can only be uppercase
						if (*it < 'A' || *it > 'Z')
						{
							error.InvalidDirective(lineNum - 1);
							continue;
						}
					}
					getline(iss, s);
					
					macros[NAME] = s;

					cout << '\n';
					continue;
				}
				else
				{
					getline(iss, s);
					cin.putback('\n');
					for (auto it = s.rbegin(); it != s.rend(); ++it)
						cin.putback(*it);
					cin.putback(' ');
					lineNum--;
					error.InvalidDirective(lineNum);
					continue;
				}
			}
			//check for macro in hashtable
			else if (isalpha(ch))
			{
				NAME = "";
				while (isalpha(ch))
				{
						NAME.push_back(ch);
						ch = cin.get();
				}
				cin.unget();				
				//if macro found, replace NAME
				if (macros.find(NAME) != macros.end())
				{
					cout << macros[NAME];
					continue;
				}
				else
				{
					error.InvalidDirective(lineNum);
					continue;
				}
			}
			//dispose of line, print error
			else
			{
				error.InvalidDirective(lineNum);
				ch = cin.get();
				while (isalpha(ch))
					ch = cin.get();
			}
		}
		if (ch == '\n')
		{
			lineNum++;
			quote = false;
		}
		cout << (char)ch;
	}
	return 0;
}









