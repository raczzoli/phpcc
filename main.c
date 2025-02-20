#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"

char *source = "$a = 10;\n$b = 20;\n$x = $a + $b;\n$y = $b - $a;$str = \"hello world\";";

int main()
{
	struct token_t *head = lexer_parse(source);
	struct token_t *token = head;
	
	printf("\n******* LEXER INFO *******\n");
	while (token) {
		printf("token: \"%s\" (type: %s)\n", token->value, lexer_token_type_str(token->type));
		token = token->next;
	}
	
	printf("\n******* PARSE INFO *******\n");
	parse_tokens(head);

	return 0;
}
