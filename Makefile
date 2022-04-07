CFLAGS = -O0 -g

all: calc_c.tab.c lex.yy.c ast.c
	gcc -o calc $(CFLAGS) ast.c block.c calc_c.tab.c lex.yy.c semantic_analysis.c table.c main.c Tac.c graph_coloring.c liveness_analysis.c

calc_c.tab.c: calc_c.y
	bison -d calc_c.y
lex.yy.c:trycalc.l
	flex trycalc.l
clean:
	$(RM) *.o *.exe calc_c.tab.c lex.yy.c calc_c.tab.h calc