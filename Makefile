parser:lexical.l syntax.y main.c tree.h linear_link.c linear_link.h syn.c syn.h ir.c ir.h oc.h oc.c
	flex lexical.l
	bison -d syntax.y
	gcc -gstabs+ -o parser main.c linear_link.c syn.c oc.c ir.c syntax.tab.c lex.yy.c -lfl

