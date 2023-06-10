#include"../include/GA.h"


//构造函数
GeneticAlgorithm::GeneticAlgorithm(Benchmarks*fp)
{
    this->Iteration_times = 0;
    this->Iteration_terminate_times=1000;
    this->Population_size=100;
    this->Cross_rate=0.9;
    this->Mutation_rate=0.05;
    this->fitness = vector<double>(Population_size);
    this->index = vector<int>(Population_size);
    iota(index.begin(),index.end(),0);
    this->fp=fp;
    x_min=fp->getMinX();
    x_max=fp->getMaxX();
}


void GeneticAlgorithm::Set_group(vector<int>& group)
{
    this->x_group = group;
    if(x_group.size()==1)
    {
        this->Cross_rate=0.0;
        this->Mutation_rate=1.0;
    }
}

void GeneticAlgorithm::Init_GA()
{
    Iteration_times = 0;
    Init_population();
    Compute_fitness();
}

//析构函数(为空即可,没有分配动态分配的资源)
GeneticAlgorithm::~GeneticAlgorithm()
{

}


//初始化种群
inline void GeneticAlgorithm::Init_population()
{
    this-> Population = vector < vector < double >> (Population_size, vector<double>(Dimension));
    //第一个个体为(0,0,0,0,......)
    for (int i = 1; i < Population_size; i++)
    {
        for (auto j: x_group)
        {
            Population[i][j] = Random_double(x_min, x_max);
        }
    }
}


//计算适应度
inline void GeneticAlgorithm::Compute_fitness()
{
    double sum = 0.0;
    double y_max = 0;
    for (int i = 0; i < Population_size; i++)
    {
        fitness[i] = fp->compute(Population[i].data());
        y_max = max(y_max, fitness[i]);
    }
    sort(index.begin(),index.end(),[&](int a,int b){return fitness[a]<fitness[b];});
    best_fitness = fitness[index[0]];      //记录最优函数值

    for (int i = 0; i < Population_size; i++)
    {
        fitness[i] = y_max - fitness[i];   //根据函数值得到适应度
        sum += fitness[i];
    }
    for (int i = 0; i < Population_size; i++)
    {
        fitness[i] /= sum;                 //换算成概率，后面用于轮盘赌
    }
}


//生成随机整数
inline int GeneticAlgorithm::Random_int(int MIN, int MAX)
{
    random_device seed;
    ranlux48 engine(seed());
    uniform_int_distribution<> distrib(MIN, MAX);
    return distrib(engine);
}


//生成随机浮点数
inline double GeneticAlgorithm::Random_double(double MIN, double MAX)
{
    random_device seed;
    ranlux48 engine(seed());
    uniform_real_distribution<> distrib(MIN, MAX);
    return distrib(engine);
}


//使用轮盘赌选择
inline vector<double>& GeneticAlgorithm::Select()
{
    double r = Random_double(0, 1);
    double sum = 0;
    int p = -1;
    while (sum < r && p < Population_size)
    {
        sum += fitness[++p];
    }
    return Population[p];
}



//迭代进化
inline void GeneticAlgorithm::Iterative_Evolution()
{
    //新种群
    vector <vector<double>> newPopulation(Population_size, vector<double>(Dimension));  
    //保留上一代最优个体，避免种群退化
    for(int i=0;i<10 && i<Dimension;++i)
    {
        copy(Population[index[i]].begin(),Population[index[i]].end(),newPopulation[i].begin());
    }
    for (int i = 10; i < Population_size; i++)
    {
        vector<double>& f1=Select();
        vector<double>& f2=Select();
        //以交叉概率进行交叉操作
        if(Random_double(0,1)>Cross_rate){
            copy(f1.begin(),f1.end(),newPopulation[i].begin());
            goto jump;
        }
        for(auto j:x_group)
        {
            //交叉方法为多点交叉中的均匀交叉
            if(Random_double(0,1)<0.5) 
                newPopulation[i][j]=f1[j];
            else
                newPopulation[i][j]=f2[j];
        }
        jump:
        for(auto j:x_group)
        {
            //printf("test%d %d\n",i,j);
            if(Random_double(0,1)>Mutation_rate)
                continue;
            else
            {
                //高斯变异
                double U1 = rand() * 1.0f / RAND_MAX; 
                double U2 = rand() * 1.0f / RAND_MAX; 
                double Z = sqrt(-2 * log(U1))*cos(2 * PI * U2);
                newPopulation[i][j] =min((double)x_max,newPopulation[i][j] + 2*Z);
                newPopulation[i][j] =max((double)x_min,newPopulation[i][j] + 2*Z);
            } 
        }
    }
    Population=newPopulation;
    ++Iteration_times;
}


//运行
vector<double>&  GeneticAlgorithm::Get_Solutions()
{
    while (Iteration_times < Iteration_terminate_times)
    {
        Iterative_Evolution();
        Compute_fitness();
        //printf("%d %lf\n",Iteration_times,best_fitness);
    }
    return Population[index[0]];//返回最优个体
}