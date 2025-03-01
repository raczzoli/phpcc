#include <stdlib.h>
#include <stdio.h>

#include "semantic.h"

static int analize_ast(struct ast_node_t *node, struct symbol **symbol_table_head, struct symbol **current_symbol);

int semantic_analize_ast(struct ast_node_t *head, struct symbol **symbol_table_head)
{
	struct ast_node_t *node = head;
	struct symbol *current_symbol = NULL;
	int ret = 0;

	if (node->type != AST_NODE_PROGRAM_START) {
		printf("Invalid AST structure (type of PROGRAM_START expected at the beginning of the tree)!\n");
		return -1;
	}

	if (node->children_len > 0) {
		for (int i=0;i<node->children_len;i++) {
			ret = analize_ast(node->children[i], symbol_table_head, &current_symbol);
			if (ret != 0) {
				break;
			}
		}	
	}

	symbol_print_table(*symbol_table_head);

	return ret;
}

static int analize_ast(struct ast_node_t *node, struct symbol **symbol_table_head, struct symbol **current_symbol)
{
	int ret = 0;
	struct symbol *symbol;

	switch(node->type)
	{
		case AST_NODE_VARIABLE:
			if (!symbol_lookup(*symbol_table_head, node)) {
				symbol = symbol_insert(*current_symbol, node);

				if (!symbol) 
					return -1;

				if (!*symbol_table_head)
					*symbol_table_head = symbol;

				*current_symbol = symbol;
			}
		break;
		case AST_NODE_ASSIGNMENT:
			ret = analize_ast(node->data.assignment.variable, symbol_table_head, current_symbol);

			if (ret != 0) 
				return ret;

			if (!node->data.assignment.expression) {
				// TODO: we should print out also where and why is invalid...
				printf("Invalid expression!\n");
				return -1;
			}

			ret = analize_ast(node->data.assignment.expression, symbol_table_head, current_symbol);

			if (ret != 0)
				return ret;
		break;
	}

	return ret;
}