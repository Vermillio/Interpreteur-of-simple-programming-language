#include "SimpleLanguageInterpretator.h"
#include <fstream>
#include <iostream>
using namespace std;


bool SimpleLanguageInterpretator::calcRelat(int val1, int val2, string rel)
{
	if (rel == "=") return val1 == val2;
	if (rel == "!=") return val1 != val2;
	if (rel == ">") return val1 > val2;
	if (rel == ">=") return val1 >= val2;
	if (rel == "<") return val1 < val2;
	if (rel == "<=") return val1 <= val2;
	return false;
}

int SimpleLanguageInterpretator::calcExpr(map<char, int>& variables, string expr, bool & correct)
{
	correct = true;
	vector<string> arr1;
	vector<string> arr2;
	string tmp = "";
	try
	{
		for (size_t i = 0; i < expr.size(); i++)
		{
			switch (expr[i])
			{
			case '*':
			{
				if (!tmp.empty()) { arr1.push_back(tmp); tmp.clear(); }
				if (arr2.back() == "+" || arr2.back() == "-") arr2.push_back("*");
				else
				{
					while (!arr2.empty())
					{
						arr1.push_back(arr2.back());
						arr2.pop_back();
					}
					arr2.push_back("*");
				}
			} break;
			case '/':
			{
				if (!tmp.empty()) { arr1.push_back(tmp); tmp.clear(); }
				if (arr2.back() == "+" || arr2.back() == "-") arr2.push_back("/");
				else
				{
					while (!arr2.empty())
					{
						arr1.push_back(arr2.back());
						arr2.pop_back();
					}
					arr2.push_back("/");
				}
			} break;
			case '+':
			{
				if (!tmp.empty()) { arr1.push_back(tmp); tmp.clear(); }
				while (!arr2.empty())
				{
					arr1.push_back(arr2.back());
					arr2.pop_back();
				}
				arr2.push_back("+");
			} break;
			case '-':
			{
				if (!tmp.empty()) { arr1.push_back(tmp); tmp.clear(); }
				if (arr1.empty() && arr2.empty()) arr1.push_back("0");
				else
					while (!arr2.empty())
					{
						arr1.push_back(arr2.back());
						arr2.pop_back();
					}
				arr2.push_back("-");

			} break;
			default:
			{
				if (expr[i] >= '0' && expr[i] <= '9')
				{
					tmp.push_back(expr[i]);
				}
				else if (expr[i] == 'a' && expr[i + 1] == 'b' && expr[i + 2] == 's')
				{
					size_t iter1 = i + 2, iter2 = i + 2;
					while (expr[iter1] != '(' && iter1 < expr.size()) iter1++;
					while (expr[iter2] != ')' && iter2 < expr.size()) iter2++;
					if (expr[iter1] != '(' || expr[iter2] != ')') throw 1;
					tmp.assign(expr.begin() + iter1 + 1, expr.begin() + iter2);
					bool res;
					int r = calcExpr(variables, tmp, res);
					if (!res) throw 1;
					tmp = "";
					i = iter2 + 1;
					arr1.push_back(to_string(abs(r)));
				}
				else if (expr[i] != ' ') arr1.push_back(to_string(variables[expr[i]]));
			}
			}
		}
	}
	catch (int)
	{
		correct = false;
		return 0;
	}
	if (tmp != "") arr1.push_back(tmp);
	tmp = "";
	while (!arr2.empty())
	{
		arr1.push_back(arr2.back());
		arr2.pop_back();
	}
	size_t ind = 2;
	while (ind < arr1.size())
	{
		if (arr1[ind] == "+")
		{
			arr1[ind - 2] = to_string(atoi(arr1[ind - 2].c_str()) + atoi(arr1[ind - 1].c_str()));
			arr1.erase(arr1.begin() + ind - 1);
			arr1.erase(arr1.begin() + ind - 1);
			ind--;
		}
		else if (arr1[ind] == "-")
		{
			arr1[ind - 2] = to_string(atoi(arr1[ind - 2].c_str()) - atoi(arr1[ind - 1].c_str()));
			arr1.erase(arr1.begin() + ind - 1);
			arr1.erase(arr1.begin() + ind - 1);
			ind--;
		}
		else if (arr1[ind] == "*")
		{
			arr1[ind - 2] = to_string(atoi(arr1[ind - 2].c_str()) * atoi(arr1[ind - 1].c_str()));
			arr1.erase(arr1.begin() + ind - 1);
			arr1.erase(arr1.begin() + ind - 1);
			ind--;
		}
		else if (arr1[ind] == "/")
		{
			arr1[ind - 2] = to_string(atoi(arr1[ind - 2].c_str()) / atoi(arr1[ind - 1].c_str()));
			arr1.erase(arr1.begin() + ind - 1);
			arr1.erase(arr1.begin() + ind - 1);
			ind--;
		}
		else ind++;
	}
	return atoi(arr1[0].c_str());
}

void SimpleLanguageInterpretator::readFile(char * filename, vector<string> *code, bool &success)
{
	code->clear();
	ifstream fin(filename);
	if (!fin.is_open())
	{
		success = false;
		return;
	}
	string tmp;
	while (getline(fin, tmp))
	{
		while (tmp != "" && tmp[0] == ' ')
			tmp.erase(0, 1);
		while (tmp != "" && tmp[tmp.size() - 1] == ' ')
			tmp.erase(tmp.size() - 1, 1);
		if (tmp == "" || (tmp[0] == '/' && tmp[1] == '/')) continue;
		code->push_back(tmp);
	}
	fin.close();
	success = true;
}

void SimpleLanguageInterpretator::execute(vector<string> &code, char * fileIn, char * fileOut)
{

	try
	{
		ofstream fout(fileOut, ios::out);
		ifstream fin(fileIn, ios::in);
		if (!fout.is_open() || !fin.is_open())
			throw "Could not open file.";

		string name; 
		name.assign(code[0].begin(), code[0].begin() + 5);
		if (name == "begin") 
			name.assign(code[0].begin() + 6, code[0].end());
		else throw "Invalid program name";
		fout << "Program <" << name << "> is running" << endl;

		map<int, int> labels;
		for (size_t cur = 1; cur < code.size(); cur++)
			if (code[cur][0] == '#')
			{
				string s; 
				s.assign(code[cur].begin() + 1, code[cur].end());
				int k = atoi(s.c_str());
				if (0 <= k && k <= 1000) labels[k] = cur;
				else throw "Invalid label";
			}

		map<char, int> variables;
		vector<pair<int, string>> buff;
		bool k = false;
		for (size_t cur = 1; cur < code.size(); cur++)
		{
			if (k)
				while (!buff.empty())
				{
					code[buff.back().first] = buff.back().second + code[buff.back().first];
					buff.pop_back();
					k = false;
				}
			else k = true;
			string s = code[cur];
			// Label
			if (s[0] == '#') 
				continue;
			// Operator go
			if (s[0] == 'g' && s[1] == 'o' && s[2] == ' ')
			{
				string t; 
				t.assign(s.begin() + 3, s.end());
				int k = atoi(t.c_str());
				if (labels.count(k)) 
					cur = labels[k];
				else throw "Label does not exist";
				continue;
			}
			// Operator :=
			if (s[1] == ':' && s[2] == '=')
			{
				string t; 
				t.assign(s.begin() + 3, s.end());
				bool b;
				int k = calcExpr(variables, t, b);
				if (!b) throw "Invalid expression";
				if (abs(k) <= 1000) 
					variables[s[0]] = k;
				else throw "Out of range of variable";
				continue;
			}
			// Operator input()
			if (s[0] == 'i' && s[1] == 'n' && s[2] == 'p' && s[3] == 'u' && s[4] == 't')
			{
				int k;
				fin >> k;
				variables[s[6]] = k;
				continue;
			}
			// Operator print()
			if (s[0] == 'p' && s[1] == 'r' && s[2] == 'i' && s[3] == 'n' && s[4] == 't')
			{
				string t;
				if (s[6] == '$')
				{
					t.assign(s.begin() + 7, s.end() - 1);
					fout << t << endl;
				}
				else
				{
					t.assign(s.begin() + 6, s.end() - 1);
					bool b; 
					int k;
					k = calcExpr(variables, t, b);
					if (!b) 
						throw "Invalid expression";
					fout << k << endl;
				}
				continue;
			}
			// Operator if
			if (s[0] == 'i' && s[1] == 'f')
			{
				int val1, val2; 
				string rel;
				size_t iter1 = 1, iter2 = 1, iter3, iter4;

				while (s[iter1] != '(' && iter1 < s.size()) 
					iter1++;

				if (s[iter1] != '(') 
					throw "Invalid relation";

				while (!(s[iter2] == '=' || s[iter2] == '<' || s[iter2] == '>') && iter2 < s.size()) 
					iter2++;

				if (!(s[iter2] == '=' || s[iter2] == '<' || s[iter2] == '>')) 
					throw "Invalid relation";

				if (s[iter2] == '<')
				{
					if (s[iter2 + 1] == '=')
					{
						rel = "<=";
						iter3 = iter2 + 1;
						iter2 -= 1;
					}
					else
					{
						rel = "<";
						iter3 = iter2 + 1;
					}
				}
				else if (s[iter2] == '>')
				{
					if (s[iter2 + 1] == '=')
					{
						rel = ">=";
						iter3 = iter2 + 1;
						iter2 -= 1;
					}
					else
					{
						rel = ">";
						iter3 = iter2 + 1;
					}
				}
				else if (s[iter2] == '=')
				{
					if (s[iter2 - 1] == '!')
					{
						rel = "!=";
						iter3 = iter2 + 1;
						iter2 -= 1;
					}
					else
					{
						rel = "=";
						iter3 = iter2 + 1;
					}
				}
				iter4 = iter3 - 1;
				int cnt = 0;
				while (cnt != -1 && cnt < (int)s.size())
				{
					iter4++;
					if (s[iter4] == '(') 
						cnt++;
					if (s[iter4] == ')') 
						cnt--;
				}
				if (s[iter4] != ')') 
					throw "Invalid relation";
				bool b; 
				string tmp;
				tmp.assign(s.begin() + iter1 + 1, s.begin() + iter2);
				val1 = calcExpr(variables, tmp, b); 
				if (!b) 
					throw "Invalid expression";
				tmp.assign(s.begin() + iter3, s.begin() + iter4);
				val2 = calcExpr(variables, tmp, b); 
				if (!b) 
					throw "Invalid expression";
				if (calcRelat(val1, val2, rel))
				{
					tmp.assign(s.begin(), s.begin() + iter4 + 2);
					pair<int, string> a(cur, tmp);
					buff.push_back(a);
					tmp.assign(s.begin() + iter4 + 2, s.end());
					code[cur] = tmp;
					cur--;
					k = false;
				}
				continue;
			}
			// Operator end
			if (s[0] == 'e' && s[1] == 'n' && s[2] == 'd') 
				break;
			throw "Unknown command";

		}
		fout << endl << endl << "Program terminated succesfully" << endl;
	}
	catch (const char* error)
	{
		cout << endl << endl << "Error occured: " << error << endl;
	}
}

SimpleLanguageInterpretator::SimpleLanguageInterpretator()
{
}

SimpleLanguageInterpretator::~SimpleLanguageInterpretator()
{
}

void SimpleLanguageInterpretator::runProgram(char * fileCode, char * fileIn, char * fileOut)
{
	bool success;
	vector<string> code;
	readFile(fileCode, &code, success);
	if (!success)
		return;
	execute(code, fileIn, fileOut);
}