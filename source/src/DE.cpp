#include"../include/DE.h"

using namespace std;


// 定义目标函数
double target_func(double x, double y) {
    return sin(x) + cos(y);
}

// 定义个体类
    Individual::Individual(Benchmarks *fp_, vector<int> indexList_, int gene_size,double max_v,double min_v) {
        genes.resize(gene_size);
        fitness = 0.0;
        max_value=max_v;
        min_value=min_v;
        fp=fp_;
        indexList=indexList_;
    }

    void Individual::setIndexList(vector<int> indexList_){
        indexList=indexList_;
    }

    // 随机初始化基因
    void Individual::random_init() {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(min_value, max_value);
        for (int i = 0; i < (int)genes.size(); i++) {
            genes[i] = dis(gen);
        }
    }

    // 计算适应度
    void Individual::calc_fitness() {
        
        fitness = fp->compute(genes.data());
    }

    // 变异操作
    void Individual::mutation(const vector<Individual>& population, double F) {
        //随机数
        random_device rd;
        mt19937 gen(rd());

        uniform_int_distribution<> dis(0, population.size() - 1);
        int r1 = dis(gen);
        int r2 = dis(gen);
        int r3 = dis(gen);
        while (r1 == r2 || r1 == r3 || r2 == r3) {
            r1 = dis(gen);
            r2 = dis(gen);
            r3 = dis(gen);
        }
        for (int i : indexList)
        {
            genes[i] = population[r1].genes[i] + F * (population[r2].genes[i] - population[r3].genes[i]);
        }
        //  (int i = 0; i < genes.size(); i++) {
        //     genes[i] = population[r1].genes[i] + F * (population[r2].genes[i] - population[r3].genes[i]);
        // }
    }

    // 交叉操作
    void Individual::crossover(const Individual& parent, double CR) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);
        int j_rand = rand() % indexList.size();
        for (int j = 0; j < (int)indexList.size(); j++) {
            if (dis(gen) < CR || j == j_rand) {
                genes[indexList[j]] = parent.genes[indexList[j]];
            }
        }
    }


DE::DE(int function_id_,vector<int> S_,int mode_,int pop_size_, int gene_size_, double F_, double CR_, int max_iter_){
    function_id=function_id_;
    pop_size=pop_size_;
    gene_size=gene_size_;
    F=F_;
    CR=CR_;
    max_iter=max_iter_;
    S=S_;
    mode=mode_;

    indexList=vector<int>();

     if (function_id==1){
        fp = new F1();
        max_value=100;min_value=-100;
    }else if (function_id==2){
        fp = new F2();
        max_value=5;min_value=-5;
    }else if (function_id==3){
        fp = new F3();
        max_value=32;min_value=-32;
    }else if (function_id==4){
        fp = new F4();
        max_value=100;min_value=-100;
    }else if (function_id==5){
        fp = new F5();
        max_value=5;min_value=-5;
    }else if (function_id==6){
        fp = new F6();
        max_value=32;min_value=-32;
    }else if (function_id==7){
        fp = new F7();
        max_value=100;min_value=-100;
    }else if (function_id==8){
        fp = new F8();
        max_value=100;min_value=-100;
    }else if (function_id==9){
        fp = new F9();
        max_value=100;min_value=-100;
    }else if (function_id==10){
        fp = new F10();
        max_value=5;min_value=-5;
    }else if (function_id==11){
        fp = new F11();
        max_value=32;min_value=-32;
    }else if (function_id==12){
        fp = new F12();
        max_value=100;min_value=-100;
    }else if (function_id==13){
        fp = new F13();
        max_value=100;min_value=-100;
    }else if (function_id==14){
        fp = new F14();
        max_value=100;min_value=-100;
    }else if (function_id==15){
        fp = new F15();
        max_value=100;min_value=-100;
    }

    //设置最初始的S和indexList

    if(mode==0){
        S.clear();
        for(int i=0;i<10;i++){
            S.push_back(100);
        }
        for(int i=0;i<S[0];i++){
            indexList.push_back(i);
        }
    }else if(mode==1){
        for(int i=0;i<S[0];i++){
            indexList.push_back(i);
        }
    }else if(mode==2){
        if (S.empty()){
            for(int i=0;i<10;i++){
            S.push_back(100);
            }
            for(int i=0;i<S[0];i++){
                indexList.push_back(i);
            }
        }else{
            for(int i=0;i<S[0];i++){
                indexList.push_back(i);
            }
        }
    }


    population=vector<Individual>(pop_size, Individual(fp,indexList,gene_size,max_value,min_value));
    for (int i = 0; i < pop_size; i++) {
        population[i].random_init();
        population[i].calc_fitness();
    }
}

void DE::run(){
    int iter = 0;
    double best_fitness = population[0].fitness;
    double past_fitness= best_fitness;
    int best_index = 0;
    
    double threhold=best_fitness;
    int sequence=0;
    //挑出随机初始的群中最优的解
    for(int i=0;i<pop_size;i++){
        if (population[i].fitness<=best_fitness){
                best_fitness=population[i].fitness;
                best_index=i;
            }
    }

    past_fitness=best_fitness;

    //优化
    while (iter < max_iter) {
        for (int i = 0; i < pop_size; i++) {
            Individual mutant(fp,indexList,gene_size,max_value,min_value);
            mutant.mutation(population, F);
            mutant.calc_fitness();
            if (mutant.fitness <= population[i].fitness) {
                Individual child(fp,indexList,gene_size,max_value,min_value);
                child.crossover(mutant, CR);
                child.calc_fitness();
                if (child.fitness <= population[i].fitness) {
                    population[i] = child;
                } else {
                    population[i] = mutant;
                }
            }
            //优化后的适应值小于最好的，存储
            if (population[i].fitness<=best_fitness){
                best_fitness=population[i].fitness;
                best_index=i;
            }

        }
        iter++;



        if(past_fitness-best_fitness<=threhold){
            sequence=(sequence+1)%S.size();
            if(mode==0 || mode==1){
                int sum=0;
                for(int i=0;i<sequence;i++){
                    sum+=S[i];
                }
                indexList.clear();
                for(int i=sum;i<sum+S[sequence];i++){
                    indexList.push_back(i);
                }

                for(int i=0;i<pop_size;i++){
                    population[i].setIndexList(indexList);
                }


            }else if(mode==2){

                if(S.size()!=1){
                    //这里是S还没有完全合并成1000的情况。
                    if(sequence==0){
                        vector<int> tmpS(S);
                        S.clear();

                        for(int i=0;i<(int)tmpS.size();i+=2){
                            S.push_back(tmpS[i]+tmpS[i+1]);
                        }
                        if(tmpS.size()%2 !=0){
                            S.push_back(tmpS[tmpS.size()-1]);
                        }   
                    }

                    int sum=0;
                    for(int i=0;i<sequence;i++){
                        sum+=S[i];
                    }
                    indexList.clear();
                    for(int i=sum;i<sum+S[sequence];i++){
                        indexList.push_back(i);
                    }
                    
                    for(int i=0;i<pop_size;i++){
                        population[i].setIndexList(indexList);
                    }

                }

            }
        }


        threhold=0.5 * threhold + 0.5 *(best_fitness-past_fitness);
        past_fitness=best_fitness;

        //这里加入一个当适应值上升不了后，则对小组进行调换。
    }


    // 输出最优解
    best_fitness = population[0].fitness;
    best_index = 0;
    for (int i = 1; i < pop_size; i++) {
        if (population[i].fitness < best_fitness) {
            best_fitness = population[i].fitness;
            best_index = i;
        }
    }


    for(int i=0;i<gene_size;i++){
        cout<<population[best_index].genes[i];
    }
    cout<endl;
    population[best_index].calc_fitness();
    cout << "Best fitness: " << (int)population[best_index].fitness<< endl;
}


// // DE算法主函数
// void de_algorithm(int pop_size, int gene_size, double F, double CR, int max_iter) {
//     vector<Individual> population(pop_size, Individual(gene_size));  // 初始化种群
//     double min_value = -10.0, max_value = 10.0;  // 定义基因的取值范围
//     for (int i = 0; i < pop_size; i++) {
//         population[i].random_init(min_value, max_value);
//         population[i].calc_fitness();
//     }
//     int iter = 0;
//     while (iter < max_iter) {
//         for (int i = 0; i < pop_size; i++) {
//             Individual mutant(gene_size);
//             mutant.mutation(population, F);
//             mutant.calc_fitness();
//             if (mutant.fitness <= population[i].fitness) {
//                 Individual child(gene_size);
//                 child.crossover(mutant, CR);
//                 child.calc_fitness();
//                 if (child.fitness <= population[i].fitness) {
//                     population[i] = child;
//                 } else {
//                     population[i] = mutant;
//                 }
//             }
//         }
//         iter++;
//     }
//     // 输出最优解
//     double best_fitness = population[0].fitness;
//     int best_index = 0;
//     for (int i = 1; i < pop_size; i++) {
//         if (population[i].fitness < best_fitness) {
//             best_fitness = population[i].fitness;
//             best_index = i;
//         }
//     }
//     cout << "Best solution: (" << population[best_index].genes[0] << ", " << population[best_index].genes[1] << ")" << endl;
//     cout << "Best fitness: " << best_fitness << endl;
// }

// int main() {
//     int pop_size = 50;  // 种群大小
//     int gene_size = 2;  // 基因长度
//     double F = 0.8;  // 缩放因子
//     double CR = 0.5;  // 交叉概率
//     int max_iter = 100;  // 最大迭代次数
//     de_algorithm(pop_size, gene_size, F, CR, max_iter);
//     return 0;
// }