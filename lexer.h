#ifndef LEXER_H
#define LEXER_H

enum token_type_t {
	TOKEN_VARIABLE = 1,
	TOKEN_IDENTIFIER,
	TOKEN_ASSIGN,
	TOKEN_SEMICOLON,
	TOKEN_KEYWORD,

	// types
	TOKEN_NUMBER,
	TOKEN_STRING,

	// operators
	TOKEN_OPERATOR,

	// invalid
	TOKEN_INVALID;
};


struct token_t {
	int type;
	char *value;
	struct token_t *before;
	struct token_t *next;
};


struct token_t *lexer_parse(char *str);
char *lexer_token_type_str(int type);

#endif // LEXER_H
