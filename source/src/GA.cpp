#include"../include/GA.h"


//构造函数
GeneticAlgorithm::GeneticAlgorithm(Benchmarks*fp)
{
    this->Iteration_times = 0;
    this->Iteration_terminate_times=100;
    this->Population_size=50;
    this->Cross_rate=0.9;
    this->Mutation_rate=0.01;
    this->fitness = vector<double>(Population_size);
    this->fp=fp;
    x_min=fp->getMinX();
    x_max=fp->getMaxX();
}


void GeneticAlgorithm::Set_group(vector<int>& group)
{
    this->x_group = group;
    //如果只对一维进行优化，此时没有交叉操作，只需要进行变异操作
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
    best_fitness = *min_element(fitness.begin(), fitness.end());
    best_x = Population[distance(fitness.begin(), min_element(fitness.begin(), fitness.end()))];
}

//析构函数
GeneticAlgorithm::~GeneticAlgorithm()
{
    delete fp;
}


//初始化种群
inline void GeneticAlgorithm::Init_population()
{
    this-> Population = vector < vector < double >> (Population_size, vector<double>(Dimension));
    //仅仅对要分组优化的维度进行初始化赋值（赋一个随机数），其余默认为0.0
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
    best_fitness = y_max;
    int best_index = 0;
    for (int i = 0; i < Population_size; i++)
    {
        //这里记录一下最优个体的下标
        if (fitness[i] < best_fitness) 
        {
            best_fitness = fitness[i];
            best_index = i;
        }
        fitness[i] = y_max - fitness[i];
        sum += fitness[i];
    }
    for (int i = 0; i < Population_size; i++)
    {
        fitness[i] /= sum;
    }

    best_x = Population[best_index];

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


//选择
inline vector<double> &GeneticAlgorithm::Select()
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


//交叉
inline void GeneticAlgorithm::Cross(vector<double> &child)
{
    vector<double> &f1 = Select();
    vector<double> &f2 = Select();
    for (auto i:x_group)
    {
        int r = Random_int(0, 1);
        if (r == 0)
        {
            child[i] = f1[i];
        }
        else
        {
            child[i] = f2[i];
        }
    }

}


//变异
inline void GeneticAlgorithm::Mutation(vector<double> &child)
{
    if(x_group.size()==1)
    {
        child[x_group[0]]=Random_double(x_min,x_max);
        return;
    }
    vector<double> &fa = Select();
    for (auto i:x_group)
    {
        if (Random_int(1, 2) == 1)
        {
            child[i] = Random_double(x_min, x_max);
        }
        else
        {
            child[i] = fa[i];
        }
    }
}


//复制
inline void GeneticAlgorithm::Copy(vector<double> &child)
{
    vector<double> &father = Select();
    copy(father.begin(), father.end(), child.begin());
}


//迭代进化
inline void GeneticAlgorithm::Iterative_Evolution()
{
    vector <vector<double>> newPopulation(Population_size, vector<double>(Dimension));
    copy(best_x.begin(), best_x.end(), newPopulation[0].begin());   //保留最优个体
    for (int i = 1; i < Population_size; i++) {
        double r = Random_double(0, 1);
        if (r <= Mutation_rate)
        {
            Mutation(newPopulation[i]);
        }
        else if (r <= Cross_rate + Mutation_rate)
        {
            Cross(newPopulation[i]);
        }
        else
        {
            Copy(newPopulation[i]);
        }
    }
    Population = newPopulation;
    ++Iteration_times;
}


//运行
vector<double>  GeneticAlgorithm::Local_Solutions()
{
    while (Iteration_times < Iteration_terminate_times)
    {
        Iterative_Evolution();
        Compute_fitness();
    }
    return best_x;
    //Print();
}


//输出结果
void GeneticAlgorithm::Print()
{
    printf("final result:\n");
    printf("best fitness:  %e\n", best_fitness);
    printf("iteration times:  %d\n", Iteration_times);
    printf("Polulation size:  %d\n", Population_size);
    printf("Dimension:  %d\n", Dimension);
    printf("best individual:\n");
    for (int i = 0; i < Dimension; i++)
    {
        printf("%lf  ", best_x[i]);
        if ((i + 1) % 20 == 0) printf("\n");
    }
}
