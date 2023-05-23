#pragma once

#include <vector>
#include <unordered_map>
#include <string>

typedef struct TtokenSet {
	std::string token;
	const int tk_type;
	const int line;
	const int token_start_pos;
} TtokenSet;

class CCompiler {
private:

public:
	std::vector<TtokenSet> tokenSet;
	std::unordered_map<std::string, double> vars;
	int lastLine;
	int aotil;
	int tokenIter;
	int currToken;
	
	void isAssign()
	{
		lastLine = tokenSet[tokenSet.size()-1].line;
		for (int i = 1; i <= lastLine; i++)
		{	
			for (int j = 0; j < tokenSet.size(); j++)
			{
				if (tokenSet[j].line == i) aotil++;
			}
			for (int j = 0; j < aotil; j++)
			{
				if (tokenSet[tokenIter + j].token == "=")
				{
					if (tokenSet[tokenIter + j - 1].tk_type == 1)
					{
						currToken = tokenIter + j + 1;
						vars.insert({ tokenSet[tokenIter + j - 1].token, isExprss() });
					}
				}
			}
			tokenIter += aotil;
			aotil = 0;
		}
	}

	double isExprss()
	{
		double pom = 0;
		if (tokenSet[currToken].token == "+")
		{
			currToken++;
			pom = isElem();

		}

		else if (tokenSet[currToken].token == "-")
		{
			currToken++;
			pom = -1 * isElem();
		}
		else 
		{ 
			//currToken++;
			pom = isElem(); 
			//currToken++;
		}
		while (currToken < tokenIter + aotil) 
		{
			if (tokenSet[currToken].token == "+")
			{
				currToken++;
				pom += isElem();
			}
			else if (tokenSet[currToken].token == "-")
			{
				currToken++;
				pom -= isElem();
			}
			else if (tokenSet[currToken].token == ")")
			{
				//currToken++;
				return pom;
			}
			else currToken++;
		}
		return pom;
	}

	double isElem()
	{
		double pom = isFact();
		currToken++;
		if (tokenSet[currToken].token == "*")
		{
			currToken++;
			pom = pom * isFact();
		}


		else if (tokenSet[currToken].token == "/")
		{
			currToken++;
			pom = pom / isFact();
		}
		return pom;
	}

	double isFact()
	{
		double pom;
		if (tokenSet[currToken].tk_type == 3)//|| tokenSet[currToken].tk_type == 4|| tokenSet[currToken].tk_type == 5)//num
		{
			pom = std::stod(tokenSet[currToken].token.c_str());

		}
		else if (tokenSet[currToken].tk_type == 1)
		{ 
			pom = vars[tokenSet[currToken].token];
			//currToken++;
		}
		else if (tokenSet[currToken].token == "(")
		{
			currToken++;
			pom = isExprss();
			//if (tokenSet[currToken].token == ")") currToken++;
			

		}
		return pom;
	}

};
