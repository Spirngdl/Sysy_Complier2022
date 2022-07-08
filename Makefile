CFLAGS = -O0 -g

all: calc_c.tab.c lex.yy.c ast.c
	gcc -o calc.exe $(CFLAGS) ast.c block.c calc_c.tab.c lex.yy.c semantic_analysis.c table.c main.c Tac.c graph_coloring.c liveness_analysis.c list.c optimize/cDag.c

clean:
	$(RM) *.o *.exe calc_c.tab.c lex.yy.c calc_c.tab.h calc

dd:ddd.c
	gcc -o dd.exe $(CFLAGS) ddd.c list.c