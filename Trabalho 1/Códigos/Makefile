calc: calc.l calc.y calc.h
	bison -d calc.y
	flex -o calc.lex.c calc.l
	gcc -o $@ calc.tab.c calc.lex.c calc.c -lm
	@echo Parser da Calculadora com Cmds, funcoes, ... estah pronto!

