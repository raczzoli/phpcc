#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static struct ast_node_t *parse_next(struct token_t **token);

static struct ast_node_t *create_node(int type)
{
	struct ast_node_t *node = (struct ast_node_t *) malloc(sizeof(struct ast_node_t));

	if (!node) {
		printf("Error allocating memory for AST node (of type: %d)!\n", type);
		return NULL;
	}

	node->type = type;

	return node;
}

static struct ast_node_t *create_program_node()
{
	return create_node(AST_NODE_PROGRAM_START);
}

static struct ast_node_t *create_variable(struct token_t *token)
{
	struct ast_node_t *node = create_node(AST_NODE_VARIABLE);

	if (!node)
		return NULL;

	node->data.variable.name = strdup(token->value);

	return node;
}

static struct ast_node_t *create_literal(char *value)
{
	struct ast_node_t *node = create_node(AST_NODE_LITERAL);

	if (!node)
		return NULL;

	node->data.literal.value = strdup(value);

	return node;
}

static struct ast_node_t *create_assignment(struct ast_node_t *var_node, struct token_t **expr_token)
{
	struct ast_node_t *node = create_node(AST_NODE_ASSIGNMENT);

	if (!node)
		return NULL;

	printf("CREATE ASSIGNMENT: var: %s\n", var_node->data.variable.name);

	*expr_token = (*expr_token)->next;

	node->data.assignment.variable = var_node;
	node->data.assignment.expression = parse_next(expr_token);

	if (!node->data.assignment.expression) {
		printf("Invalid expression!\n");
		return NULL;
	}

	printf("expression = %s\n", node->data.assignment.expression->data.literal.value);

	return node;
}

static struct ast_node_t *parse_next(struct token_t **token)
{
	struct token_t *t = *token;
	struct ast_node_t *node = NULL;

	*token = t->next;

	switch(t->type) {
		case TOKEN_VARIABLE:
			node = create_variable(t);

			if (t->next && t->next->type == TOKEN_ASSIGN) {
				node = create_assignment(node, token);
			}
		break;

		case TOKEN_NUMBER:
		case TOKEN_STRING:
			node = create_literal(t->value);
		break;

		case TOKEN_SEMICOLON: // no break
		default:
			;
	}

	return node;
}

struct ast_node_t *parse_tokens(struct token_t *token)
{
	struct ast_node_t *node = NULL;
	struct ast_node_t *head = create_program_node();

	if (!head) 
		return NULL;

	while (token) {
		parse_next(&token);
	}

	return node;
}
