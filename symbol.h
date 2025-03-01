#ifndef SYMBOL_H
#define SYMBOL_H

#include "parser.h"

enum symbol_types {
	SYMB_TYPE_INTEGER = 1,
	SYMB_TYPE_FLOAT,
	SYMB_TYPE_STRING,
	SYMB_TYPE_FUNCTION
};

enum symbol_scopes {
	SYMB_SCOPE_GLOBAL = 1,
	SYMB_SCOPE_LOCAL
};

struct symbol {
	char *name;
	enum symbol_types type;
	enum symbol_scopes scope;

	struct symbol *next;
	struct symbol *prev;
};

struct symbol *symbol_create_table_entry();
struct symbol *symbol_insert(struct symbol *parent, struct ast_node_t *var);
struct symbol *symbol_lookup(struct symbol *entry, struct ast_node_t *var);
void symbol_print_table(struct symbol *head);

#endif //SYMBOL_H