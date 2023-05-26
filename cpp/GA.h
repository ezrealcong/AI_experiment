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
    static constexpr int Population_size=100;
    static constexpr int Dimension=1000;

    int Iteration_times;
    int Iteration_terminate_times=100;

    int function_id;
    Benchmarks *fp;

    double Cross_rate;
    double Mutation_rate;
    double Iteration_terminate_fit;
    
    vector<double*> Population;
    vector<double> fitness;
    
    void init_population();
    void cal_fitness();
    double* select();
    double* cross();
    double* mutation();

    
public:
    GeneticAlgorithm(int function_id, double Cross_rate, double Mutation_rate, double Iteration_terminate_fit);
    ~GeneticAlgorithm();
};

GeneticAlgorithm::GeneticAlgorithm(int function_id, double Cross_rate, double Mutation_rate, double Iteration_terminate_fit)
{
    this->function_id=function_id;
    this->Cross_rate=Cross_rate;
    this->Mutation_rate=Mutation_rate;
    this->Iteration_terminate_fit=Iteration_terminate_fit;
    this->Iteration_times=0;

    if (function_id==1){
        fp = new F1();
    }else if (function_id==2){
        fp = new F2();
    }else if (function_id==3){
        fp = new F3();
    }else if (function_id==4){
        fp = new F4();
    }else if (function_id==5){
        fp = new F5();
    }else if (function_id==6){
        fp = new F6();
    }else if (function_id==7){
        fp = new F7();
    }else if (function_id==8){
        fp = new F8();
    }else if (function_id==9){
        fp = new F9();
    }else if (function_id==10){
        fp = new F10();
    }else if (function_id==11){
        fp = new F11();
    }else if (function_id==12){
        fp = new F12();
    }else if (function_id==13){
        fp = new F13();
    }else if (function_id==14){
        fp = new F14();
    }else if (function_id==15){
        fp = new F15();
    }
    init_population();
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    delete fp;
}

void GeneticAlgorithm::init_population()
{
    Population=vector<double*>(Population_size);
    for(int i=0;i<Population_size;i++){
        Population[i]=new double[Dimension];
        for(int j=0;j<Dimension;j++){
            Population[i][j]=rand()/(RAND_MAX+1.0)*200-100;
        }
    }
       
}

void GeneticAlgorithm::cal_fitness()
{
    fitness=vector<double>(Population_size);
    for(int i=0;i<Population_size;i++){
        fitness[i]=fp->compute(Population[i]);
    }
}

double* GeneticAlgorithm::select()
{
    vector<int> index(Population_size);
    iota(index.begin(),index.end(),0);

    sort(index.begin(),index.end(),[&](int a, int b){return fitness[a]>fitness[b];});
    vector<int> temp(Population_size);

    for(int i=0;i<Population_size;++i){
        temp[index[i]]=i+1;
    }
    random_device seed;
	ranlux48 engine(seed());
    uniform_int_distribution<> distrib(1, 500500);
    
    int r1=distrib(engine);
    int sum=0,p=0;
    while(sum<r1){
        sum+=temp[p++];
    }
    return Population[p];
    
}

double* GeneticAlgorithm::cross()
{
    double*f1=select();
    double*f2=select();
    double*child=new double[Dimension];

    random_device seed;
	ranlux48 engine(seed());
    uniform_int_distribution<> distrib(0, 1);

    for(int i=0;i<Dimension;i++){
        if(distrib(engine)==0){
            child[i]=f1[i];
        }else{
            child[i]=f2[i];
        }
    }
    return child;

}

double* GeneticAlgorithm::mutation()
{
    double*f=select();
    double*child=new double[Dimension];

    random_device seed;
    ranlux48 engine(seed());
    uniform_int_distribution<> distrib(0, 1);
    uniform_real_distribution<> distrib2(-100, 100);

    for(int i=0;i<Dimension;i++){
        if(distrib(engine)==0){
            child[i]=distrib2(engine);
        }else{
            child[i]=f[i];
        }
    }
    return child;
}