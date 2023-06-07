#ifndef  _DECCFR_H
#define  _DECCFR_H
#include"Header.h"
#include"DE.h"
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#define EXPECT_VALUE 10
using namespace std;

/* 
F是缩放因子

GPT:
    DE算法的缩放因子通常设置在0.5到2之间，具体的值可以根据问题的复杂度和实验结果进行调整。
    一般来说，如果问题比较简单，可以设置较小的缩放因子，如0.5或0.6；
    如果问题比较复杂，可以设置较大的缩放因子，如1.5或2。此外，还可以采用自适应缩放因子的方法，
    根据个体的适应度和种群的变化情况来动态地调整缩放因子的值，以提高算法的性能。

CR是杂交概率

S：是一个整数序列，其中整数累加和为1000，如果题目中没有S，则创建一个空的vector输入进去。
mode:0是完全可分，1是部分可分，2是完全不可分
*/



class DECCFR 
{
  public:
    //add a contribution value list for each group
    vector<int> contribution;
    vector<Individual> population;

    vector<int> S;
    int mode;

    double min_value,max_value;
    int pop_size;
    int gene_size;
    int max_iter;
    double F,CR;
    Benchmarks *fp;
    int function_id;
    vector<vector<int>> indexList;
    DECCFR(int function_id_,vector<int> S_,int mode_,int pop_size_, int gene_size_, double F_, double CR_, int max_iter_);
    void run(int* iter_res,double* best_fitness_res);

};

#endif