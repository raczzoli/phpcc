#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

enum ast_node_types {
	AST_NODE_PROGRAM_START=1,
	AST_NODE_VARIABLE,
	AST_NODE_LITERAL,
	AST_NODE_ASSIGNMENT,
	AST_NODE_OPERATION,
	AST_NODE_INVALID
};

struct ast_node_t {
	int type;

	union {
		struct {
			char *name;
		} variable;

		struct {
			char *value;
		} literal;

		struct {
			struct ast_node_t *variable;
			struct ast_node_t *expression;
		} assignment;

		struct {
			char *operator;
			struct ast_node_t *left;
			struct ast_node_t *right;
		} operation;
		
	} data;

	struct ast_node_t **children;
	int children_len;
};

char *parser_ast_type_str(int type);
struct ast_node_t *parser_parse_program(struct token_t *token);
void parser_print_ast(struct ast_node_t *head, int num_tabs);

#endif //PARSER_H