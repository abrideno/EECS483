/* File: dppmain.cc
 * ----------------
 * This file defines the main() routine for the preprocessor, 
 * the filtering tool which runs before the compiler.
 */
 
#include "scanner.h"
#include <string>
#include <iostream>

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
	int ch;
	bool quote = false;
	string s;
	while ((ch = cin.get()) != EOF)
	{
		if (ch == '"')
			quote = !quote;
		if (ch == '/')
		{
			if (!quote)
			{
				ch = cin.get();
				if (ch == '/')
				{
					getline(cin, s);
					putc('\n', stdout);
					continue;
				}
				if (ch == '*')
				{
					getline(cin, s, '*');
					ch = cin.get();
					if (ch == '/')
						continue;
					else
					{
						cerr << "Comment doesn't terminate" << endl;
						exit(1);
					}
				}
				else
					putc('/', stdout);
			}
		}
		putc(ch, stdout);
	}
	return 0;
}
