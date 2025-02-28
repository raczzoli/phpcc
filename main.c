#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

char *source = "$a = 10;\n$b = 20;\n$x = $a + $b;\n$y = $b - $a;\n$z = $a + $b + 10;\n$z1 =$a;\n$str = \"hello world\";$oo = ;$xxx = 15;";

int main()
{
	struct ast_node_t *ast_head = NULL;
	struct token_t *token_head = lexer_parse(source);
	
	
	struct token_t *token = token_head;

	printf("\n******* LEXER INFO *******\n");
	while (token) {
		printf("token: \"%s\" (type: %s)\n", token->value, lexer_token_type_str(token->type));
		token = token->next;
	}
	
	
	printf("\n******* PARSE INFO *******\n");
	ast_head = parser_parse_program(token_head);

	if (ast_head)
		parser_print_ast(ast_head, 0);

	return 0;
}
