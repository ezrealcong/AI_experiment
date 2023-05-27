/*
为遗传算法设计一个类，包含以下成员函数：
1. 构造函数，初始化种群
2. 选择函数，根据适应度函数选择个体
3. 交叉函数，根据交叉概率交叉个体
4. 变异函数，根据变异概率变异个体
5. 适应度函数，计算个体适应度（调用具体的f）
6. 最优个体函数，返回最优个体
7. 迭代函数，迭代一次
8. 迭代次数函数，返回迭代次数
9. 迭代终止函数，判断是否终止迭代
10. 迭代终止次数函数，返回迭代终止次数
11. 迭代终止最优个体函数，返回迭代终止时的最优个体
12. 迭代终止最优个体适应度函数，返回迭代终止时的最优个体适应度
13. 迭代终止最优个体位置函数，返回迭代终止时的最优个体位置

成员包括：
1. 种群(个体:一个唯独为D的向量)
2. 种群大小
3. 交叉概率
4. 变异概率
5. 迭代次数
6. 迭代终止条件（限定迭代次数、或者适应度fit到0）
7. 迭代终止次数
8. 迭代终止最优个体
9. 迭代终止最优个体适应度
10. 迭代终止最优个体位置
11. 适应度函数指针
*/
#include "Header.h"
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>

using namespace std;

class GeneticAlgorithm
{
private:
    static constexpr int Population_size=50;
    static constexpr int Dimension=1000;
    int x_max,x_min;

    int Iteration_times;
    int Iteration_terminate_times=100;

    int function_id;
    Benchmarks *fp;

    double Cross_rate;
    double Mutation_rate;
    double best_fitness;
    double* best_idividual;


    vector<double*> Population;
    vector<double> fitness;
    vector<int> index;
    vector<double> temp;
    
    void init_population();
    void compute_fitness();
    double* select();
    double* Individuals_Copy();
    double* cross();
    double* mutation();
    void Iterative_Evolution();

    
public:
    GeneticAlgorithm(int, double , double , int);
    ~GeneticAlgorithm();

    void run();
};