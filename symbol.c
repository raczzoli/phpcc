#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symbol.h"

struct symbol *symbol_create_table_entry()
{
	struct symbol *s = (struct symbol *) malloc( sizeof(struct symbol) );

	if (!s) {
		printf("Error allocating memory for symbol table entry");
		return NULL;
	}

	s->name = NULL;
	s->next = NULL;

	return s;
}

struct symbol *symbol_insert(struct symbol *parent, struct ast_node_t *var)
{
	struct symbol *symbol = symbol_create_table_entry();

	if (!symbol)
		return NULL;

	symbol->name = strdup(var->data.variable.name);
	// TODO: temp
	symbol->type = SYMB_TYPE_INTEGER;
	// TODO: temp
	symbol->scope = SYMB_SCOPE_GLOBAL; 

	if (parent) {
		symbol->prev = parent;
		parent->next = symbol;
	}

	return symbol;
}

struct symbol *symbol_lookup(struct symbol *entry, struct ast_node_t *var)
{
	if (!entry)
		return NULL;

	while (entry) {
		if (entry->name) {
			if (strcmp(entry->name, var->data.variable.name) == 0) 
				return entry;
		}
	
		entry = entry->next;
	}

	return NULL;
}

void symbol_print_table(struct symbol *head)
{
	struct symbol *entry = head;

	printf("::: Symbol table :::\n");

	while (entry) {
		printf("Name: %s, type: %d, scope: %d\n", entry->name, entry->type, entry->scope);
		entry = entry->next;
	}
}