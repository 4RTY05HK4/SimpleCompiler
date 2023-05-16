#pragma warning(disable :4996)

#include "tokenizer.h"

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

	std::cout << "Class		Line	Pos.	Content" << std::endl;
	while (tokenizer.getToken() == 0 && tokenizer.tk_type != TK_EMPTY)
	{
		//if (tokenizer.tk_type != 7) std::cout << TKset[tokenizer.tk_type].ident << "        " << tokenizer.line << "        " << tokenizer.token_start_pos << "        " << tokenizer.token << std::endl;
		if (tokenizer.tk_type != 7) printf("%s		%d	%d	%s \n", TKset[tokenizer.tk_type].ident, tokenizer.line, tokenizer.token_start_pos, tokenizer.token);
		if (tokenizer.consumeToken() != 0)
		{
			std::cout << "Token Err" << std::endl;
			return -1;
		}
	}

	tokenizer.Done();

	getchar();
	return 0;
}