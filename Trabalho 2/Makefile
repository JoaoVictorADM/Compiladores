tradutor: tradutor.l tradutor.y tradutor.h
	bison -d tradutor.y
	flex -o tradutor.lex.c tradutor.l
	gcc -o $@ tradutor.tab.c tradutor.lex.c tradutor.c -lm
	@echo Parser do tradutor estah pronto!

