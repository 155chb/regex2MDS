# regex2MDS
OT1 将正则表达式转化为最小确定有限自动机

该项目为输入一个正则表达式后，输出其对应的MDS

转换过程中得到的NFA和DFA也会输出

简单介绍一下每一个**文件中包含的内容**

- main.cpp

	主函数入口，主要调用其它文件中的函数

- listNode.h

	链表节点的定义，本次实验中NFA使用状态转换表实现，表类型为listNode

- production.h 

	使用递归的方式执行翻译的过程，使用一个函数获得状态和输入对应的应该执行的产生式

	这里实现了生成NFA的函数

- strFormat.h

	正则表达式规范化，为了简化产生式

- stateListNFA.h、stateListDFA.h、stateListMDS.h

	包含了对应状态转换表的定义、初始化和输出等函数

- NFA2DFA.h、DFA2MDS.h

	实现了生成DFA和MDS的函数

再简单介绍一下**文法**，由于正则表达式中主要包含右运算符

为了能比较简单地判断产生式，同时因为现在还只学习了LL(1)类型的文法

只能仿照其规范，设计了**RR(1)文法**，自右向左开始遍历

**能够较简单地判断产生式，同时可以较简单地实现右运算符**

实在懒得起名字，因此直接用了字母加下标给非终结符起名字

- 0：  E0 -> E1 E2
- 1：  E1 -> E0 |
- 2：  E1 -> E0
- 3：  E2 -> E3 \*
- 4：  E2 -> E3 +
- 5：  E2 -> E3 ?
- 6：  E2 -> E3
- 7：  E3 -> ( E0 )
- 8：  E3 -> [ N ]            前面说过简化了文法，这里就将[]以及里面的内容看作一个整体，strFormat的时候，就获取到[]里对应选择的字符，使用了一个bool[128]来标识包含了哪些字符
- 9：  E3 -> .
- 10：E3 -> C                 这个C就是表示一个字符，例如字符0，字符a之类的
- -1： E1 -> ε

