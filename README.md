# AI_experiment
This is a repository of experiment project  for our AI course.

# 开发流程和原则：

     - pull下最新的代码

     - cpp文件夹已经弃用，改用source文件夹

     - 如何新增自己的源文件：

       - 写好gitignore文件（正常应该会自动pull下来）

       - source/include和source/src文件夹下分别放入自己的头文件和cpp文件

       - source/include 下加入一个config.h文件，写入以下代码：

         ```cpp
         //#define RUN_ONE  //当只想运行自己的进化算法，而不想跑别人的算法，定义这个宏RUN_ONE
         #ifndef RUN_ONE
             #define RUN_ALL
         #else
             //define 自己的进化算法宏 ，只从这几个注释里放一个define出来
             //如你开发的是GA算法，就 #define __GA__
         	#define __GA__
             //#define __PSO__
             //#define __DE__
             //#define __CC__
         #endif
         ```

         

       - 上述文件config.h不会被push和pull，所以记得自己做，并且按注释改好

       - 不会cpp用C写进化算法也可以，就是在调用fx函数的时候用一下cpp的vector语法即可

     - 如何进行代码维护：

       - 每次修改和commit前pull一下最新的修改
       - 每个人只添加自己负责的进化算法源文件，都只在demo.cpp加上调用自己的进化算法代码，不可删减修改别人宏下的东西。
       - demo.cpp中加自己代码的时候，加到自己的宏区间中