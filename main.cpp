#pragma warning(disable :4996)

#include "tokenizer.h"
#include "compiler.h"
#include <utility>

int main() {

	TTokenizer tokenizer;
	if (tokenizer.init("Tfile") != 0)
	{
		std::cout << "File Err" << std::endl;
		return -1;
	}
	if (tokenizer.fileToBuffer() != 0)
	{
		std::cout << "File Err" << std::endl;
		return -1;
	}
	std::cout << "File size: " << tokenizer.buf2Length << std::endl;
	CCompiler comp;
	//std::cout << "Class		Line	Pos.	Content" << std::endl;
	while (tokenizer.getToken() == 0 && tokenizer.tk_type != TK_EMPTY)
	{
		//if (tokenizer.tk_type != 7) std::cout << TKset[tokenizer.tk_type].ident << "        " << tokenizer.line << "        " << tokenizer.token_start_pos << "        " << tokenizer.token << std::endl;
		if (tokenizer.tk_type != 7)
		{
			//printf("%s		%d	%d	%s \n", TKset[tokenizer.tk_type].ident, tokenizer.line, tokenizer.token_start_pos, tokenizer.token);
			comp.tokenSet.push_back({ tokenizer.token, tokenizer.tk_type, tokenizer.line, tokenizer.token_start_pos });
		}
		if (tokenizer.consumeToken() != 0)
		{
			std::cout << "Token Err" << std::endl;
			return -1;
		}
	}

	tokenizer.Done();

	std::cout << "Class	Content	Line	Pos." << std::endl;
	for (auto a : comp.tokenSet) {
		printf("%d	%s	%d	%d \n", a.tk_type, a.token.c_str(), a.line, a.token_start_pos);
	}

	//std::string variable = "x1";
	//double value = 2.0;

	//comp.vars.insert({variable, value});
	//std::cout << comp.vars[variable] << std::endl;

	comp.isAssign();

	for (const auto& elem : comp.vars)
	{
		std::cout << elem.first << " : " << elem.second << "\n";
	}

	getchar();
	return 0;
}