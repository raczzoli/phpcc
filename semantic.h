#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"
#include "symbol.h"

int semantic_analize_ast(struct ast_node_t *head, struct symbol **symbol_table_head);

#endif //SEMANTIC_H