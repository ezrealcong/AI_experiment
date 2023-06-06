#include "../include/Header.h"
#include"../include/GA.h"
#include"../include/DE.h"
#include"../include/DECCFR.h"
#include"../include/DG2.h"
#include"../include/config.h"
#include <sys/time.h>
#include <cstdio>
#include <unistd.h>

#include <vector>
#include <ctime>

#ifdef RUN_ALL  //每个人给自己开发的进化算法定一个宏，加到这里
    #define __GA__
    #define __DG2__
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
    //调用举例
    Benchmarks* fp=new F1();
    vector<int> group;        //自变量分组,如果想要把所有变量分一组，就在vector中填入0-999

    GeneticAlgorithm ga=GeneticAlgorithm(fp); 
    ga.Set_group(group);      //将分组传入GA算法中
    ga.Init_GA();             //初始化GA算法。传入分组之后必须初始化，这样设计的目的是，一个GA对象可以多次使用
    vector<double> result=ga.Local_Solutions(); //调用GA算法，返回最优解


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



  
  clock_t start,end;
  DECCFR deccfr=DECCFR(1,vector<int>(),0,100,1000,0.6,0.8,150);
  
  start=time(0);
  deccfr.run();
  end=time(0);

  cout<<"总时间"<<(end-start)<<"s"<<endl;

#endif
/*----------------------- CC算法调用结束--------------------------*/


/*----------------------- DG2算法调用-----------------------------*/
#ifdef __DG2__

  //调用举例
  DG2 dg2 = DG2(new F1());
  dg2.ism();
  dg2.dsm();
  vector<vector<int>> groups=dg2.getGroups();//返回的是分组情况（每一组，存储的是下标）

#endif
/*----------------------- DG2算法调用结束--------------------------*/




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