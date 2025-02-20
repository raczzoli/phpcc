#include <stdlib.h>
#include <stdio.h>

#include "lexer.h"

char *source = "$a = 10;\n$b = 20;\n$x = $a + $b;\n$y = $b - $a;$str = \"hello world\";";

int main()
{
	struct token_t *token = lexer_parse(source);
	
	while (token) {
		printf("token: \"%s\" (type: %s)\n", token->value, lexer_token_type_str(token->type));
		token = token->next;
	}
	
	return 0;
}
