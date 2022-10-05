
前端中 calc_c.y，calc.l，lex.yy.c，calc_c.tab.h，calc_c.tab.c

这几个是语法分析和词法分析的，其中后三个是bison和flex生成的

ast.c抽象语法树的，其中基本上是类似生成节点createnode()这样的函数。 头文件基本在def.h

semantic_analysis我用来语义分析和生成中间代码的。其中的函数基本都定义在def.h

table.c 我用来操作符号表的头文件在symtable.h

Tac.c 生成三地址代码的，也大都是createnode()这样的函数，然后还有个打印函数pinrt_IR 头文件也在def.h

liveness_analysis.c 计算变量的liveness的。头文件在liveness.h

graph_coloring.c图着色计算寄存器分配的,头文件在reg.h

list.c从老师那边拿来的一个模板容器，基本可以实现C++中List的功能，但是用起来比较麻烦，头文件在list.h

tArm.h，定义一些ARM翻译的节点。

