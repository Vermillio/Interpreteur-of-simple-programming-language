#pragma once
#include <map>
#include <string>
#include <vector>

using namespace std;

class SimpleLanguageInterpretator
{
	bool calcRelat(int val1, int val2, string rel);
	int calcExpr(map<char, int> &variables, string expr, bool &correct);
	void readFile(char * filename, vector<string> *code, bool &success);
	void execute(vector<string> &code, char * FileIn, char * fileOut);
public:
	SimpleLanguageInterpretator();
	~SimpleLanguageInterpretator();
	void runProgram(char * codeFile, char * inputFile, char * outputfile);
};

