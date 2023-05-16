#pragma once

#include <iostream>
#include <cctype>
#include <string.h>

typedef enum ETokenType {
	TK_EMPTY = 0, //Terminates set of tokens
	TK_IDENT, //Identifier
	TK_KW, //Keyword
	TK_NUM, //Decimal number
	TK_XNUM, //Hexadecimal number
	TK_FNUM, //Floating point number
	TK_OP, //Operator
	TK_ERR //Token parsing error � unexpected input
} ETokenType;

typedef struct TKwSet {
	const char *ident;
	const int tk_id;
} TKwSet;

std::string opSet = "=<>+-*/&|()[]{},:;%#@ \n";

TKwSet TKset[8] = { "TK_EMPTY", 0,
"TK_IDNT", 1,
"TK_KW", 2,
"TK_NUM", 3,
"TK_XNUM", 4,
"TK_FNUM", 5,
"TK_OP", 6,
"TK_ERR", 7 };

TKwSet TKspecialSet[3] = { " ", 0,
"\n", 1,
"\377", 2, };

const std::size_t sizeOfKwSet = 3;
TKwSet KwSet[sizeOfKwSet] = { "if", 0, "for", 1, "while", 2 };

class TTokenizer {
private:
	FILE *fh; //filehandle to a tokenized file
	char buf[1024]; //buffer for holding a token
	std::string buf2;
public:
	const char *token; //points to a token
	ETokenType tk_type; //token type enum
	int tk_id; // token id for keywords and operators
	const char* fn; //Parsed file name
	TKwSet *kwSet; //keywords set (array)
	int token_pos; //token end position in a line of text
	int token_start_pos; //token start position in a line of text
	int token_len; // token length
	int line; //line number of input FILE
	int buf2Length;

	TTokenizer() : token(nullptr), tk_type(TK_EMPTY), tk_id(0), fn(nullptr), token_pos(0), token_start_pos(1), token_len(0), line(1), kwSet(KwSet), buf2Length(0) {}
	~TTokenizer() { if (fh) fclose(fh); }

	int init(const char *fn)
	{
		if ((fh = fopen(fn, "rt")) == NULL)
		{
			printf("File Err");
		}
		return 0;
	}

	int fileToBuffer()
	{
		if (!fh)
		{
			return -1;
		}
		int iter = 0;
		char c;
		while (!feof(fh))
		{
			c = fgetc(fh);
			buf2 += c;
			iter++;
		}
		buf2Length = iter;
		return 0;
	}

	int isANumber(std::string word)
	{
		int wlen = word.length();
		if (wlen > 1) wlen -= 1;
		for (int i = 0; i < wlen; i++)
		{
			if (isalpha(word[i])) return -1;
			if (!isdigit(word[i])) return -2;
		}
		return 0;
	}

	int isAKeyWord(std::string word)
	{
		std::string keyword;
		word = word.substr(0, word.length() - 1);
		for (int i = 0; i < sizeOfKwSet; i++)
		{
			keyword = kwSet[i].ident;
			if (word.compare(keyword) == 0) return 0;
		}
		return -1;
	}

	int isEmpty(std::string word)
	{
		std::string keyword;
		word = word.substr(0, word.length());
		for (int i = 0; i < 3; i++)
		{
			keyword = TKspecialSet[i].ident;
			if (word.compare(keyword) == 0)
			{
				if (word.find('\n') != std::string::npos)
				{
					line++;
					token_start_pos = 1;
					token_len = 0;
				}
				return 0;
			}
		}
		return -1;
	}

	int getToken()
	{
		std::string word;
		for (int i = 0; i < buf2Length; i++)
		{
			token_start_pos++;
			if (token_pos + i > buf2Length) return -1;
			word += buf2[token_pos + i];
			if (opSet.find(buf2[token_pos + i]) != std::string::npos)
			{
				if (word.find("_") != std::string::npos || isANumber(word) == -1 && word.find("0x") == std::string::npos && isAKeyWord(word) == -1)
				{
					strToChar(word.substr(0, i));
					token = &buf[token_pos];
					tk_type = TK_IDENT;
					token_pos = token_pos + i;
					token_start_pos = token_start_pos + token_len - word.substr(0, i).length() - 1;
					token_len = word.substr(0, i).length();
					return 0;
				}
				if (word.find("0x") != std::string::npos)
				{
					strToChar(word.substr(0, i));
					token = &buf[token_pos];
					tk_type = TK_XNUM;
					token_pos = token_pos + i;
					token_start_pos = token_start_pos + token_len - word.substr(0, i).length() - 1;
					token_len = word.substr(0, i).length();
					return 0;
				}
				if (word.find("e") != std::string::npos && word.find(".") != std::string::npos)
				{
					strToChar(word.substr(0, i));
					token = &buf[token_pos];
					tk_type = TK_FNUM;
					token_pos = token_pos + i;
					token_start_pos = token_start_pos + token_len - word.substr(0, i).length() - 1;
					token_len = word.substr(0, i).length();
					return 0;
				}
				if (isANumber(word) == 0)
				{
					strToChar(word.substr(0, i));
					token = &buf[token_pos];
					tk_type = TK_NUM;
					token_pos = token_pos + i;
					token_start_pos = token_start_pos + token_len - word.substr(0, i).length() - 1;
					token_len = word.substr(0, i).length();
					return 0;
				}
				if (isAKeyWord(word) == 0)
				{
					strToChar(word.substr(0, i));
					token = &buf[token_pos];
					tk_type = TK_KW;
					token_pos = token_pos + i;
					token_start_pos = token_start_pos + token_len - word.substr(0, i).length() - 1;
					token_len = word.substr(0, i).length();
					return 0;
				}
				if (isEmpty(word) == 0)
				{
					token = "";
					tk_type = TK_ERR;
					token_pos += 1;
					token_start_pos = token_start_pos + token_len;
					token_len = 0;
					return 0;
				}
				if (token == nullptr)
				{
					strToChar(word);
					token = &buf[token_pos];
					tk_type = TK_OP;
					token_pos = token_pos + 1;
					token_start_pos = token_start_pos + token_len - word.length();
					token_len = word.length();
					return 0;
				}
			}
		}
		return -1;
	}

	int consumeToken()
	{
		if (token == nullptr)
		{
			return -1;
		}
		token = nullptr;
		return 0;
	}

	int Done()
	{
		if (fh)
		{
			fclose(fh);
		}
		token = nullptr;
		tk_type = TK_EMPTY;
		return 0;
	}

	void strToChar(std::string word)
	{
		for (int j = 0; j < word.length(); j++)
		{
			buf[token_pos + j] = word[j];
		}
	}

};