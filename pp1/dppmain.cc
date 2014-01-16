/* File: dppmain.cc
 * ----------------
 * This file defines the main() routine for the preprocessor, 
 * the filtering tool which runs before the compiler.
 */
 
#include "scanner.h"
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
	int ch;
	bool quote = false;

	
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
				putc('\n', stdout);
				continue;
			}
			//enter if multi line comment "/*"
			if (ch == '*')
			{
				while (ch != '/')
				{
					if (!getline(cin, s, '*'))
					{
						cerr << "Comment doesn't terminate" << endl;
						exit(1);
					}
					
					//iterate through comment, add newlines
					for (auto it = s.begin(); it != s.end(); ++it)
						if (*it == '\n')
							putc('\n', stdout);
					
					ch = cin.get();
				}
				continue;
			}
			//enter if not a comment
			else
				putc('/', stdout);
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
						cerr << "misuse of macro\n" << endl;
						continue;
					}
					for (auto it = NAME.begin(); it!= NAME.end(); ++it)
					{
						//NAME can only be uppercase
						if (*it < 'A' || *it > 'Z')
						{
							cerr << "NAME must be uppercase\n" << endl;
							continue;
						}
					}
					getline(iss, s);
					
					macros[NAME] = s;
					macroValid[NAME] = true;
						
					cout << '\n';
					continue;
				}
				else
				{
					cerr << "misuse of macro\n" << endl;
					continue;
				}
			}
			//check for macro in hashtable
			else if (ch >= 'A' && ch <= 'Z')
			{
				NAME = ch;
				while (!macroValid[NAME])
				{
					ch = cin.get();
					//ch must be uppercase
					if (ch < 'A' || ch > 'Z')
					{
						cerr << "\nmacro not found\n" << endl;
						break;
					}
					else
						NAME.push_back(ch);
				}
				//if macro found, replace NAME
				if (macroValid[NAME])
				{
					cout << macros[NAME];
					continue;
				}
			}
			//dispose of line, print error
			else
			{
				cerr << "misused macro\n" << endl;
				getline(cin, s);
				continue;
			}
		}
		
		cout << (char)ch;
	}
	return 0;
}









