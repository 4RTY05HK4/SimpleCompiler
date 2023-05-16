#pragma warning(disable :4996)

#include "tokenizer.h"
#include "compiler.h"
#include <unordered_map>

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
	std::vector<TtokenSet> tokenSet;
	std::cout << "Class		Line	Pos.	Content" << std::endl;
	while (tokenizer.getToken() == 0 && tokenizer.tk_type != TK_EMPTY)
	{
		//if (tokenizer.tk_type != 7) std::cout << TKset[tokenizer.tk_type].ident << "        " << tokenizer.line << "        " << tokenizer.token_start_pos << "        " << tokenizer.token << std::endl;
		if (tokenizer.tk_type != 7)
		{
			printf("%s		%d	%d	%s \n", TKset[tokenizer.tk_type].ident, tokenizer.line, tokenizer.token_start_pos, tokenizer.token);
			tokenSet.push_back({tokenizer.token, tokenizer.tk_type, tokenizer.line, tokenizer.token_start_pos});
		}
		if (tokenizer.consumeToken() != 0)
		{
			std::cout << "Token Err" << std::endl;
			return -1;
		}
	}

	tokenizer.Done();

	for (auto a : tokenSet) {
		printf("%d	%s	%d	%d \n", a.tk_type, a.token.c_str(), a.line, a.token_start_pos);
	}

	std::unordered_map<std::string, double> RANKS = {
		{"x", 1}, {"y", 2.5}
	};

	std::cout << RANKS["y"] << std::endl;
	std::cout << RANKS["x"] << std::endl;

	getchar();
	return 0;
}