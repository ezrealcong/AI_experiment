#ifndef SOURCE_GA_H
#define SOURCE_GA_H

#include "Header.h"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

class GeneticAlgorithm
{
private:
    static constexpr int Dimension=1000;    //自变量维度
    int Population_size;                    //种群大小
    int x_max,x_min;                        //自变量范围
    int Iteration_times;                    //迭代次数
    int Iteration_terminate_times;          //迭代终止次数
    Benchmarks *fp;                         //f函数指针
    double Cross_rate;                      //交叉概率
    double Mutation_rate;                    //变异概率
    vector<double> fitness;                 //种群适应度(不等同于函数值)
    double best_fitness;                    //最优适应度
    vector<vector<double>> Population;      //种群
    vector<int> index;
    vector<int> x_group;                    //自变量分组(内部关联的自变量分为一组)

    void Init_population();
    void Compute_fitness();                 //计算种群适应度
    int Random_int(int,int);                //生成随机整数
    double Random_double(double,double);    //生成随机浮点数
    vector<double>& Select();               //选择(轮盘赌)
    void Iterative_Evolution();             //迭代进化
public:
    GeneticAlgorithm(Benchmarks*fp);
    ~GeneticAlgorithm();
    void Init_GA();
    void Set_group(vector<int>& group);
    vector<double>& Get_Solutions();
};

#endif //SOURCE_GA_H