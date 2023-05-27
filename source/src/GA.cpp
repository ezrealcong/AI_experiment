#include"../include/GA.h"

GeneticAlgorithm::GeneticAlgorithm(int function_id, double Cross_rate, double Mutation_rate, int Iteration_terminate_times)
{
    this->function_id=function_id;
    this->Cross_rate=Cross_rate;
    this->Mutation_rate=Mutation_rate;
    this->Iteration_terminate_times=Iteration_terminate_times;
    this->Iteration_times=0;

    if (function_id==1){
        fp = new F1();
        x_max=100;x_min=-100;
    }else if (function_id==2){
        fp = new F2();
        x_max=5;x_min=-5;
    }else if (function_id==3){
        fp = new F3();
        x_max=32;x_min=-32;
    }else if (function_id==4){
        fp = new F4();
        x_max=100;x_min=-100;
    }else if (function_id==5){
        fp = new F5();
        x_max=5;x_min=-5;
    }else if (function_id==6){
        fp = new F6();
        x_max=32;x_min=-32;
    }else if (function_id==7){
        fp = new F7();
        x_max=100;x_min=-100;
    }else if (function_id==8){
        fp = new F8();
        x_max=100;x_min=-100;
    }else if (function_id==9){
        fp = new F9();
        x_max=100;x_min=-100;
    }else if (function_id==10){
        fp = new F10();
        x_max=5;x_min=-5;
    }else if (function_id==11){
        fp = new F11();
        x_max=32;x_min=-32;
    }else if (function_id==12){
        fp = new F12();
        x_max=100;x_min=-100;
    }else if (function_id==13){
        fp = new F13();
        x_max=100;x_min=-100;
    }else if (function_id==14){
        fp = new F14();
        x_max=100;x_min=-100;
    }else if (function_id==15){
        fp = new F15();
        x_max=100;x_min=-100;
    }
    this->fitness=vector<double>(Population_size);
    this->index=vector<int>(Population_size);
    iota(index.begin(),index.end(),0);
    this->temp=vector<double>(Population_size);
    
    init_population();
    compute_fitness();
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    delete fp;
    for(int i=0;i<Population_size;i++){
        delete[] Population[i];
    }
}

void GeneticAlgorithm::init_population()
{
    Population=vector<double*>(Population_size);
    random_device seed;
	ranlux48 engine(seed());
    uniform_int_distribution<> distrib(x_min,x_max);
    
    Population[0]=new double[Dimension];
    for(int i=0;i<Dimension;i++){
        Population[0][i]=0.0;
    }
    for(int i=1;i<Population_size;i++){
        Population[i]=new double[Dimension];
        for(int j=0;j<Dimension;j++){
            Population[i][j]=distrib(engine);
        }
    } 
}

void GeneticAlgorithm::compute_fitness()
{
    double sum=0.0;

    for(int i=0;i<Population_size;i++){
        fitness[i]=fp->compute(Population[i]);
        sum+=fitness[i];
    }
    for(int i=0;i<Population_size;i++){
        temp[i]=fitness[i]/sum*100000;
    }

    sort(index.begin(),index.end(),[&](int a, int b)->bool{return fitness[a]>fitness[b];});

    best_fitness=fitness[index[Population_size-1]];
    best_idividual=Population[index[Population_size-1]];

    for(int i=0;i<Population_size/2;++i){
        swap(temp[index[i]],temp[index[Population_size-1-i]]);
    }
}

double* GeneticAlgorithm::select()
{
    random_device seed;
	ranlux48 engine(seed());
    uniform_real_distribution<> distrib(1, 99999);

    double r=distrib(engine);
    double sum=0;
    int p=-1;
    while(sum<=r && p<Population_size){
        sum+=temp[++p];
    }
    return Population[p];
}

double* GeneticAlgorithm::cross()
{
    double* fa1=select();
    double* fa2=select();
    /*double* fa1=Population[index[Population_size-1]];
    double* fa2=Population[index[Population_size-2]];*/
    double* child=new double[Dimension];


    random_device seed;
	ranlux48 engine(seed());
    uniform_int_distribution<> distrib(0, 1);

    for(int i=0;i<Dimension;i++){   
        int r=distrib(engine);
        if(r==0){
            child[i]=fa1[i];
        }else{
            child[i]=fa2[i];
        }
    }

    return child;

}

double* GeneticAlgorithm::mutation()
{
    double*fa=select();
    //double* fa=Population[index[Population_size-1]];
    double* child=new double[Dimension];

    random_device seed;
    ranlux48 engine(seed());
    uniform_int_distribution<int> distrib(0, 1);
    uniform_real_distribution<double> distrib2(x_min, x_max);

    for(int i=0;i<Dimension;i++){
        if(distrib(engine)==0){
            child[i]=distrib2(engine);
        }else{
            child[i]=fa[i];
        }
    }
    return child;
}

double* GeneticAlgorithm::Individuals_Copy()
{
    double*individual =new double[Dimension];
    double*best=select();
    //double* best=Population[index[Population_size-1]];
    for(int i=0;i<Dimension;i++){
        individual[i]=best[i];
    }
    return individual;
}

void GeneticAlgorithm::Iterative_Evolution()
{
    vector<double*> newPopulation(Population_size);
    random_device seed;
    ranlux48 engine(seed());
    uniform_real_distribution<> distrib(0, 1);

    newPopulation[0]=new double[Dimension];
    for(int i=0;i<Dimension;i++){
        newPopulation[0][i]=best_idividual[i];
    }

    for(int i=1;i<Population_size;i++){
        
        double r=distrib(engine);
        if(r<=Mutation_rate){
            newPopulation[i]=mutation();
        }else if(r<=Cross_rate+Mutation_rate){
            newPopulation[i]=cross();
        }else{
            newPopulation[i]=Individuals_Copy();
        }
    }
    
    for(int i=0;i<Population_size;i++){
        delete[] Population[i];
        Population[i]=nullptr;
    }
    Population=newPopulation;
    ++Iteration_times;
}

void GeneticAlgorithm::run()
{

    while(Iteration_times<Iteration_terminate_times){
        Iterative_Evolution();
        compute_fitness();
    }
    cout<<best_fitness<<endl;
}