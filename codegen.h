#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"
#include "symbol.h"

#define BUFFER_CHUNK_SIZE 1024

struct codegen_result {
	char *buffer;
	int num_allocated_bytes;
	int error;
};

struct codegen_result *codegen_generate(struct ast_node_t *node, struct symbol *symbol_table_head);

#endif //CODEGEN_H