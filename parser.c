#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"


static struct ast_node_t *parse_statement(struct token_t **token);

static void print_tabs(int num)
{
	for (int i=0;i<num;i++)
		printf("\t");
}

static struct ast_node_t *create_node(int type)
{
	struct ast_node_t *node = (struct ast_node_t *) malloc(sizeof(struct ast_node_t));

	if (!node) {
		printf("Error allocating memory for AST node (of type: %d)!\n", type);
		return NULL;
	}

	node->children_len = 0;
	node->type = type;

	return node;
}

static struct ast_node_t *create_variable(struct token_t *token)
{
	struct ast_node_t *node = create_node(AST_NODE_VARIABLE);

	if (!node)
		return NULL;

	node->data.variable.name = strdup(token->value);
	node->data.variable.name_len = strlen(node->data.variable.name);
	// TODO - needs to be determined (obviously)
	node->data.variable.type = INTEGER;

	return node;
}

static struct ast_node_t *create_literal(char *value, int type)
{
	struct ast_node_t *node = create_node(AST_NODE_LITERAL);

	if (!node)
		return NULL;

	node->data.literal.value = strdup(value);
	node->data.literal.value_len = strlen(node->data.literal.value);
	node->data.literal.type = type == TOKEN_NUMBER ? LIT_NUMBER : LIT_STRING;

	return node;
}

static struct ast_node_t *parse_expression(struct token_t **token)
{
	struct ast_node_t *node = NULL;
	struct token_t *t = *token;

	switch(t->type) {
		case TOKEN_VARIABLE:
			node = create_variable(t);
		break;
		case TOKEN_NUMBER:
		case TOKEN_STRING:
			node = create_literal(t->value, t->type);
		break;
		default:
			; // invalid expression or unhandled type for the moment
	}

	if (t->next && t->next->type == TOKEN_OPERATOR) {
		*token = t->next->next;

		struct ast_node_t *tmp = create_node(AST_NODE_OPERATION);
		
		tmp->data.operation.operator = t->next->value;
		tmp->data.operation.left = node;
		tmp->data.operation.right = parse_expression(token);

		node = tmp;
	}
	else 
		*token = (*token)->next;

	return node;
}

static struct ast_node_t *parse_assignment(struct ast_node_t *var_node, struct token_t **expr_token)
{
	struct ast_node_t *node = create_node(AST_NODE_ASSIGNMENT);

	if (!node)
		return NULL;

	*expr_token = (*expr_token)->next;

	node->data.assignment.variable = var_node;
	node->data.assignment.expression = parse_expression(expr_token);

	/* we will let the semantic analizor to handle the invalid expression
	if (!node->data.assignment.expression) {
		printf("Invalid expression!\n");
		return NULL;
	}
	*/

	return node;
}

static struct ast_node_t *parse_statement(struct token_t **token)
{
	struct token_t *t = *token;
	struct ast_node_t *node = NULL;

	*token = t->next;

	switch(t->type) {
		case TOKEN_VARIABLE:
			node = create_variable(t);

			if (t->next && t->next->type == TOKEN_ASSIGN) {
				node = parse_assignment(node, token);
			}
		break;

		case TOKEN_NUMBER:
		case TOKEN_STRING:
			node = create_literal(t->value, t->type);
		break;

		case TOKEN_SEMICOLON: // no break
		default:
			;
	}

	return node;
}

struct ast_node_t *parser_parse_program(struct token_t *token)
{
	struct ast_node_t *node = NULL;
	struct ast_node_t *head = create_node(AST_NODE_PROGRAM_START);

	if (!head) 
		return NULL;

	while (token) {
		node = parse_statement(&token);

		if (!node)
			continue;

		if (!head->children) 
			head->children = calloc(1, sizeof(struct ast_node_t *));
		else 
			head->children = realloc(head->children, (head->children_len+1) * sizeof(struct ast_node_t *));

		if (!head->children) {
			printf("Error allocating memory for program node children array!");
			return NULL;
		}

		head->children[head->children_len] = node;
		head->children_len++;
	}

	return head;
}

char *parser_ast_type_str(int type)
{
	switch(type) {
		case AST_NODE_PROGRAM_START : return "PROGRAM START";
		case AST_NODE_VARIABLE : return "VARIABLE";
		case AST_NODE_LITERAL : return "LITERAL";
		case AST_NODE_ASSIGNMENT : return "ASSIGNMENT";
		case AST_NODE_OPERATION : return "OPERATION";
	}

	return "";
}

void parser_print_ast(struct ast_node_t *head, int num_tabs)
{
	print_tabs(num_tabs);

	if (!head) {
		printf("|- INVALID");
		return;
	}

	char *type_str = parser_ast_type_str(head->type);

	printf("|- %s", type_str);

	switch(head->type) {
		case AST_NODE_ASSIGNMENT:
			printf("\n");
			struct ast_node_t *var = head->data.assignment.variable;
			parser_print_ast(var, num_tabs+1);
			parser_print_ast(head->data.assignment.expression, num_tabs+1);			
		break;
		case AST_NODE_OPERATION:
			printf("\n");
			print_tabs(num_tabs+1);
			printf("|- OPERATOR: %s\n", head->data.operation.operator);
			parser_print_ast(head->data.operation.left, num_tabs+1);
			parser_print_ast(head->data.operation.right, num_tabs+1);
		break;
		case AST_NODE_LITERAL:
			printf(": %s", head->data.literal.value);
		break;
		case AST_NODE_VARIABLE:
			printf(": %s", head->data.variable.name);
		break;
	}

	printf("\n");

	if (head->children_len > 0) {
		for (int i=0;i<head->children_len;i++) {
			parser_print_ast(head->children[i], num_tabs+1);
		}
	}
}