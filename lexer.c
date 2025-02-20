#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "lexer.h"


struct token_t *get_next_token(char **str) 
{
	char c;
	char *start;
	char *end;
	int len;
	struct token_t *token = (struct token_t *) malloc(sizeof(struct token_t));

	while (*(*str) == ' ' || *(*str) == '\n') {
		(*str)++;
	}

	c = *(*str);

	/** 
	** VARIABLES
	*/
	if (c == '$') {
		(*str)++;

		start = *str;

		while (isalnum(**str) || **str == '_' || **str == '$') {
			(*str)++;
		}

		len = *str - start;

		token->type = TOKEN_VARIABLE;
		token->value = strndup(start, len);
	}

	/** 
	** ASSIGN
	*/
	else if (c == '=') {
		token->type = TOKEN_ASSIGN;
		token->value = malloc(2);
		token->value[0] = '=';
		token->value[1] = '\0';

		(*str)++;	
	}

	/** 
	** NUMBERS
	*/
	else if ((c >= '0' && c <= '9') || c == '.') {
		start = *str;

		while ((**str >= '0' && **str <= '9') || **str == '.') {
			(*str)++;
		}

		len = *str - start;

		token->type = TOKEN_NUMBER;
		token->value = strndup(start, len);
	}

	/**
	** STRINGS
	*/
	else if (c == '"') {
		(*str)++;

		start = *str;
		end = strchr(*str, '"');

		if (end != NULL) {
			token->type = TOKEN_STRING;
			token->value = strndup(start, end-start);
		}
		else 
			token->type = TOKEN_INVALID;
	}
	/** 
	** OPERATORS
	*/
	else if (c == '+' || c == '-' || c == '*') {
		token->type = TOKEN_OPERATOR;
		token->value = malloc(2);
		token->value[0] = c;
		token->value[1] = '\0';

		(*str)++;
	}

	/** 
	** SEMICOLON
	*/
	else if (c == ';') {
		token->type = TOKEN_SEMICOLON;
		token->value = malloc(2);
		token->value[0] = ';';
		token->value[1] = '\0';


		(*str)++;
	}

	/**
	** SOMETHING ELSE (maybe invalid char)
	*/
	else {
		return NULL;
	}

	return token;
}

char *lexer_token_type_str(int type) 
{
	switch(type) {
		case TOKEN_VARIABLE : return "VARIABLE";
		case TOKEN_IDENTIFIER : return "IDENTIFIED";
		case TOKEN_ASSIGN : return "ASSIGN";
		case TOKEN_SEMICOLON : return "SEMICOLON";
		case TOKEN_KEYWORD : return "KEYWORD";

		// types
		case TOKEN_NUMBER : return "NUMBER";
		case TOKEN_STRING : return "STRING";

		// operators
		case TOKEN_OPERATOR : return "OPERATOR";
	}

	return "";
}

struct token_t *lexer_parse(char *str)
{
	struct token_t *head = NULL;
	struct token_t *current = NULL;
	struct token_t *token = NULL;

	while ((token = get_next_token(&str)) != NULL) {
		if (!current) {
			current = token;
			head = current;
		}
		else {
			current->next = token;
			current = token;
		} 
	}

	return head;
}