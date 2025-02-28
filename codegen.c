#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "codegen.h"


static int generate(struct codegen_result *res, struct ast_node_t *node)
{
	int buffer_len = res->buffer ? (int) strlen(res->buffer) : 0;
	
	if (buffer_len >= res->num_allocated_bytes) {
		if (!res->buffer) {
			res->buffer = (char *) malloc(BUFFER_CHUNK_SIZE);
			res->buffer[BUFFER_CHUNK_SIZE-1] = '\0';
		}
		else {
			res->buffer = realloc(res->buffer, BUFFER_CHUNK_SIZE);
		}
	}

	if (!res->buffer) {
		printf("Error allocating memory for source code buffer!\n");
		return -1;
	}

	res->num_allocated_bytes += BUFFER_CHUNK_SIZE;

	switch(node->type) {
		case AST_NODE_VARIABLE:
			switch(node->data.variable.type) {
				case INTEGER:
					strcat(res->buffer, "int ");
				break;
				case FLOAT:
					strcat(res->buffer, "float ");
				break;
				case STRING:
					strcat(res->buffer, "char *");
				break;
			}
			strncat(res->buffer, node->data.variable.name, node->data.variable.name_len);
		break;
		case AST_NODE_LITERAL:
			switch(node->data.literal.type) {
				case LIT_NUMBER:
					strncat(res->buffer, node->data.literal.value, node->data.literal.value_len);
				break;
				case LIT_STRING:
					strcat(res->buffer, "\"");
					strncat(res->buffer, node->data.literal.value, node->data.literal.value_len);
					strcat(res->buffer, "\"");
				break;
			}
		break;
		case AST_NODE_ASSIGNMENT:
			generate(res, node->data.assignment.variable);
			strcat(res->buffer, "=");

			if (node->data.assignment.expression) {
				generate(res, node->data.assignment.expression);
				strcat(res->buffer, ";\n");
			}
		break;
		case AST_NODE_OPERATION:
			generate(res, node->data.operation.left);
			strcat(res->buffer, node->data.operation.operator);
			generate(res, node->data.operation.right);
		break;
		case AST_NODE_INVALID:
		break;
	}

	return 0;
}

struct codegen_result *codegen_generate(struct ast_node_t *node)
{
	struct codegen_result *res = NULL;
	int ret = 0;

	if (node->type != AST_NODE_PROGRAM_START) {
		printf("Invalid AST structure (type of PROGRAM_START expected at the beginning of the tree)!\n");
		return NULL;
	}

	res = (struct codegen_result *) malloc(sizeof(struct codegen_result));

	if (!res) {
		printf("Error allocating memory for codegen_result!\n");
		return NULL;
	}

	res->error = 0;

	if (node->children_len > 0) {
		for (int i=0;i<node->children_len;i++) {
			ret = generate(res, node->children[i]);
			if (ret != 0) {
				res->error = 1;
				break;
			}
		}	
	}

	return res;
}

