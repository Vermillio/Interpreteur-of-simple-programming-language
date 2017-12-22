#include "task40\SimpleLanguageInterpretator.h"
#include <iostream>

using namespace std;

void _h()
{
	cout << "python <task40.exe> <code filename> <input filename> <output filename>  "

}

int main(int argc, char **argv)
{
	if (argc < 3 || argc > 3)
	{
		_h()
	}
	else
	{
		SimpleLanguageInterpretator compilator;
		compilator.runProgram(argv[1], argv[2], argv[3]);
	}
	return 0;
}