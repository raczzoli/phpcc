#ifndef PARSER_H
#define PARSER_H

enum ast_node_types {
	AST_NODE_PROGRAM_START,
	AST_NODE_VARIABLE,
	AST_NODE_LITERAL,
	AST_NODE_ASSIGNMENT,
	AST_NODE_OPERATION
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
};

#endif //PARSER_H