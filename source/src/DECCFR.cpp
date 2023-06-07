#include"../include/DECCFR.h"
using namespace std;



DECCFR::DECCFR(int function_id_,vector<int> S_,int mode_,int pop_size_, int gene_size_, double F_, double CR_, int max_iter_){
    function_id=function_id_;
    pop_size=pop_size_;
    gene_size=gene_size_;
    F=F_;
    CR=CR_;
    max_iter=max_iter_;
    S=S_;
    mode=mode_;

    indexList=vector<vector<int>>();

//下面是选择调优的函数。
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
        for(int i=0;i<200;i++){
            S.push_back(5);
        }
        int start=0;
        for (int s : S){
            vector<int> tmp=vector<int>();
            for(int i=start;i<start+s;i++){
                tmp.push_back(i);
            }
            indexList.push_back(tmp);
            start+=s;
        }

    }else if(mode==1){
        int start=0;
        for (int s : S){
            vector<int> tmp=vector<int>();
            for(int i=start;i<start+s;i++){
                tmp.push_back(i);
            }
            indexList.push_back(tmp);
            start+=s;
        }
    }else if(mode==2){
        if (S.empty()){
            for(int i=0;i<10;i++){
            S.push_back(100);
            }
        }
        int start=0;
        for (int s : S){
            vector<int> tmp=vector<int>();
            for(int i=start;i<start+s;i++){
                tmp.push_back(i);
            }
            indexList.push_back(tmp);
            start+=s;
        }
    }



    population=vector<Individual>(pop_size, Individual(fp,gene_size,max_value,min_value));
    for (int i = 0; i < pop_size; i++) {
        population[i].random_init();
        population[i].calc_fitness();
    }
}

void DECCFR::run(){
    
    int iter = 0;
    double best_fitness = population[0].fitness;
    int best_index = 0;
    

    //挑出随机初始的群中最优的解
    for(int i=0;i<pop_size;i++){
        if (population[i].fitness<=best_fitness){
                best_fitness=population[i].fitness;
                best_index=i;
            }
    }



    //优化
    while (iter < max_iter) {
//下面是DE算法主要的优化代码。
        //分别对不同的组别进行优化
        //要改成只有贡献度最高的会被计算
        for(vector<int> indexList_ : indexList){

            for (int i = 0; i < pop_size; i++) {
                Individual mutant(population[i]);
                //变异
                mutant.mutation(population, F,indexList_);
                mutant.calc_fitness();
            
                if (mutant.fitness <= population[i].fitness) {
                    //交换
                    Individual child(population[i]);
                    child.crossover(mutant, CR,indexList_);
                    child.calc_fitness();
                    if (child.fitness <= population[i].fitness) {
                        population[i] = child;
                    } else {
                        population[i] = mutant;
                    }
                }
            }

        }
       
        // 比较最优解   
        for (int i = 1; i < pop_size; i++) {
            if (population[i].fitness < best_fitness) {
                best_fitness = population[i].fitness;
                best_index = i;
            }
        }

        //更新贡献度

        //达到预期退出
        if(best_fitness<EXPECT_VALUE){
            break;
        }
        iter++;
        //cout<<iter<<endl;
    }
    cout << "iter : "<<iter<<"Best fitness: " << best_fitness<< endl;
}

