#include <stdlib.h>
#include <stdio.h>

#include "symbol.h"
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "codegen.h"

char *source = "$a = 10;\n$b = 20;\n$x = $a + $b;\n$y = $b - $a;\n$z = $a + $b + 10;\n$z1 =$a;\n$str = \"hello world\";$oo = ;$xxx = 15.99;";

int main()
{
	struct ast_node_t *ast_head = NULL;
	struct codegen_result *res = NULL;
	struct token_t *token_head = lexer_parse(source);
	struct token_t *token = token_head;
	struct symbol *symbol_table_head = NULL;
	int ret = 0;

	printf("\n******* LEXER INFO *******\n");
	while (token) {
		printf("token: \"%s\" (type: %s)\n", token->value, lexer_token_type_str(token->type));
		token = token->next;
	}
	
	
	printf("\n******* PARSE INFO *******\n");
	ast_head = parser_parse_program(token_head);

	if (ast_head)
		parser_print_ast(ast_head, 0);


	ret = semantic_analize_ast(ast_head, &symbol_table_head);

	if (ret != 0) 
		return -1;

	res = codegen_generate(ast_head, symbol_table_head);

	if (res) {
		printf("Codegen result :::: %s\n", res->error ? "ERROR!" : "OK");

		if (!res->error) {
			printf("\n******* START OF GENERATED C CODE *******\n\n");
			printf("%s\n", res->buffer);
			printf("\n******* END OF GENERATED C CODE *******\n\n");
		}
	}

	return 0;
}
