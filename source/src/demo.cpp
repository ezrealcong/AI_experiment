#include "../include/Header.h"
#include"../include/GA.h"
#include"../include/DE.h"
#include"../include/config.h"
#include <sys/time.h>
#include <cstdio>
#include <unistd.h>

#include <vector>
#include <ctime>

#ifdef RUN_ALL  //每个人给自己开发的进化算法定一个宏，加到这里
    #define __GA__
    #define __PSO__
    #define __DE__=true
    #define __CC__
    
#endif

int main()
{
  //每个人只在这里加代码来调用自己的算法，不可删减修改别人宏下的东西
  //加自己代码的时候，加到自己的宏区间中


/*----------------------- GA算法调用-----------------------------*/
#ifdef __GA__  
  //参数依次为：函数编号、交叉概率、变异概率、迭代次数
  GeneticAlgorithm GA=GeneticAlgorithm(10,0.9,0.001,20);
  GA.run();

#endif
/*----------------------- GA算法调用结束--------------------------*/


  


/*-----------------------PSO算法调用-----------------------------*/
#ifdef __PSO__

#endif
/*-----------------------PSO算法调用结束--------------------------*/






/*----------------------- DE算法调用-----------------------------*/
#ifdef __DE__
  clock_t start,end;


  DE de=DE(1,vector<int>(),0,100,1000,0.6,0.8,150);
  
  start=time(0);
  de.run();
  end=time(0);

  cout<<"总时间"<<(end-start)<<"s"<<endl;

#endif
/*----------------------- DE算法调用结束--------------------------*/





/*----------------------- CC算法调用-----------------------------*/
#ifdef __CC__


#endif
/*----------------------- CC算法调用结束--------------------------*/



  return 0;
}


//以下为之前的demo代码

/*int main(){
  //  Test the basic benchmark function 
  double* X;
  Benchmarks* fp=NULL;
  unsigned dim = 1000;
  unsigned funToRun[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  unsigned funToRun[] = {1};
  unsigned funToRun[] = {15};
  unsigned funNum = 15;
  unsigned run = 1;

  vector<double> runTimeVec;
  struct timeval start, end;
  long seconds, useconds;    
  double mtime;
        
  X = new double[dim];
  for (unsigned i=0; i<dim; i++){
    X[i]=0;
  }

  for (unsigned i=0; i<funNum; i++){
    fp = generateFuncObj(funToRun[i]); 
    printf("F %d value = %1.20E\n", fp->getID(), fp->compute(X));
    gettimeofday(&start, NULL);
    for (unsigned j=0; j < run; j++){
      fp->compute(X);
    }
    gettimeofday(&end, NULL);
    
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = (((seconds) * 1000 + useconds/1000.0) + 0.5)/1000;

    runTimeVec.push_back(mtime);
    printf ( "F %d, Running Time = %f s\n\n", fp->getID(), mtime);
    
    delete fp;
  }

  delete []X;

  // for (unsigned i=0; i<runTimeVec.size(); i++){
  // 	printf ( "%f\n", runTimeVec[i] );
  // }

  return 0;
}

// create new object of class with default setting
Benchmarks* generateFuncObj(int funcID){
  Benchmarks *fp;
  // run each of specified function in "configure.ini"
  if (funcID==1){
    fp = new F1();
  }else if (funcID==2){
    fp = new F2();
  }else if (funcID==3){
    fp = new F3();
  }else if (funcID==4){
    fp = new F4();
  }else if (funcID==5){
    fp = new F5();
  }else if (funcID==6){
    fp = new F6();
  }else if (funcID==7){
    fp = new F7();
  }else if (funcID==8){
    fp = new F8();
  }else if (funcID==9){
    fp = new F9();
  }else if (funcID==10){
    fp = new F10();
  }else if (funcID==11){
    fp = new F11();
  }else if (funcID==12){
    fp = new F12();
  }else if (funcID==13){
    fp = new F13();
  }else if (funcID==14){
    fp = new F14();
  }else if (funcID==15){
    fp = new F15();
  }else{
    cerr<<"Fail to locate Specified Function Index"<<endl;
    exit(-1);
  }
  return fp;
}
*/