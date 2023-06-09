#include"../include/DECCFR.h"
using namespace std;



DECCFR::DECCFR(int function_id_,vector<vector<int>> indexList_,int mode_,int pop_size_, int gene_size_, double F_, double CR_, int max_iter_){
    function_id=function_id_;
    pop_size=pop_size_;
    gene_size=gene_size_;
    F=F_;
    CR=CR_;
    max_iter=max_iter_; //这里max_iter指的是每一组子问题进化一次加一，所有组总共能迭代的最大次数
    S=vector<int>();
    mode=mode_;
    expect_value = EXPECT_VALUE;
    evolution_step = EVOLUTION_STEP;
    

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
        indexList=vector<vector<int>>();
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
        indexList= indexList_;
    }else if(mode==2){
        indexList=vector<vector<int>>();
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
    //init the contribution value for every group
    for (int i = 0; i < indexList.size(); i++){
        contribution.push_back(0.0);//每个子问题的贡献度
        pre_stagnant.push_back(0);//每个子问题的伪停滞数
        stagnant.push_back(false);//每个子问题的停滞标志位
    }
    for(int i = 0; i < gene_size; i++){
        mean.push_back(0.0);//每个维度的均值
        standard_deviation.push_back(0.0);//每个维度的标准差
    }

    max_iter = max_iter*indexList.size();
}

void DECCFR::run(int* iter_res,double* best_fitness_res){
    
    int iter = 0;
    double best_fitness = population[0].fitness;
    int best_index = 0;
    double last_best_fitness;

    //挑出随机初始的群中最优的解
    for(int i=0;i<pop_size;i++){
        if (population[i].fitness<=best_fitness){
            best_fitness=population[i].fitness;
            best_index=i;
        }
    }


    int group_index_to_evolve = 0;
    double sum, mean_tmp , variance, standard_deviation_tmp  ;//暂存一个维度的和，均值,方差及标准差
    int d_same,d_size;//用来统计一个组中一次进化后不变的维度数
    double max_contribution,min_contribution;

    

    //优化 // 在迭代次数没有达到最大 并且 优化效果没有达到预期 之前都一直优化
    while (iter < max_iter ) {
        group_index_to_evolve = 0;
        //分别对不同的组别进行优化
        for(vector<int> &indexList_ : indexList){
           // printf("index %d\r\n",group_index_to_evolve);
            d_size=indexList_.size();
            //每个子问题去共同进化，evolution_step为每次审计贡献度之间进化的步数
            for(int j = 0;j<evolution_step;j++){
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
                
                d_same=0;
                //计算更新每个维度的均值和方差，并统计均值和方差与前一次相同的维度数量
                
                for (int gen_i : indexList_)
                {
                    //提取该维度所有种群的基因值
                    vector<double> gen_in_i;
                    for(Individual & individual : population){
                        gen_in_i.push_back(individual.genes[gen_i] );
                    }
                    //计算该维度这次的均值和标准差
                    
                    sum = std::accumulate(std::begin(gen_in_i), std::end(gen_in_i), 0.0);
                    mean_tmp =  sum /pop_size;
                    variance  = 0.0;
                    for (int k = 0 ; k < pop_size ; k++)
                    {
                        variance = variance + pow(gen_in_i[k]-mean_tmp,2);
                    }     
                    standard_deviation_tmp = sqrt(variance /pop_size);    
                    //统计均值和方差与前一次相同的维度数量
                    if(mean_tmp ==mean[gen_i] && standard_deviation_tmp ==standard_deviation[gen_i]){
                        d_same++;
                    }
                    //更新每个维度的均值和方差
                    mean[gen_i]= mean_tmp ;
                    standard_deviation[gen_i]=  standard_deviation_tmp;
                    
                }
                
                //如果所有维度都不变，则子问题伪停滞次数+1，否则reset为0
                if(d_same == d_size){
                    pre_stagnant[group_index_to_evolve]++;
                }else{
                    pre_stagnant[group_index_to_evolve]=0;
                }
                //若停滞次数大于维度数(这个数可以自己设，人家推荐是这个)，则标记该种群停滞不前
                if(pre_stagnant[group_index_to_evolve]>=d_size){
                    stagnant[group_index_to_evolve]=true;
                }else{
                    stagnant[group_index_to_evolve]=false;
                }
                //如果子问题停滞不前则提前终止对此子问题的优化
                if(stagnant[group_index_to_evolve]){
                    break;
                }
                
                iter++;

            }
            //暂存旧的最优解，用于计算贡献度
            last_best_fitness = best_fitness;
            //扫描是否有新的最优解
            for (int i = 0; i < pop_size; i++) {
                if (population[i].fitness < best_fitness) {
                    best_fitness = population[i].fitness;
                    best_index = i;
                }
            }
            //看看是不是达到了终止进化条件：
            if(iter%100==0){
                printf("function %d  iter :%d ",function_id,iter);
            }
            if(best_fitness <= expect_value || iter >= max_iter){
                goto end_evolution;
            }
            //更新每个子问题的贡献度
            //printf("cao\r\n");
            contribution[group_index_to_evolve]= (contribution[group_index_to_evolve] + fabs(last_best_fitness-best_fitness) )/2;
            //如果总群进化停滞不前，则贡献度归0
            //printf("cao\r\n");
            if(stagnant[group_index_to_evolve]){
                contribution[group_index_to_evolve]=0;
            }
            //printf("cao\r\n");
            group_index_to_evolve ++;
        }

        
        max_contribution = *(max_element(contribution.begin(),contribution.end()));
        min_contribution =*(min_element(contribution.begin(),contribution.end()));
        //如果不是每个子问题贡献率都一致，就一直循环，每轮只有贡献度最高的会被计算
        while(max_contribution != min_contribution){
            //取贡献度最高的子问题去进化，evolution_step为每次审计贡献度之间进化的步数
            //找到贡献度最高的子问题分组
            group_index_to_evolve = max_element(contribution.begin(),contribution.end()) - contribution.begin();
            vector<int> indexList_=indexList[group_index_to_evolve];
            d_size=indexList_.size();
            //该分组拿去进化
            for(int j = 0;j<evolution_step;j++){
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
                d_same=0;
                //计算更新每个维度的均值和方差，并统计均值和方差与前一次相同的维度数量
                for (int gen_i : indexList_)
                {
                    //提取该维度所有种群的基因值
                    vector<double> gen_in_i;
                    for(Individual individual : population){
                        gen_in_i.push_back(individual.genes[gen_i] );
                    }
                    //计算该维度这次的均值和标准差
                    
                    sum = std::accumulate(std::begin(gen_in_i), std::end(gen_in_i), 0.0);
                    mean_tmp =  sum /pop_size;
                    variance  = 0.0;
                    for (int k = 0 ; k < pop_size ; k++)
                    {
                        variance = variance + pow(gen_in_i[k]-mean_tmp,2);
                    }
                    standard_deviation_tmp = sqrt(variance /pop_size);
                    //统计均值和方差与前一次相同的维度数量
                    if(mean_tmp ==mean[gen_i] && standard_deviation_tmp ==standard_deviation[gen_i]){
                        d_same++;
                    }
                    //更新每个维度的均值和方差
                    mean[gen_i]= mean_tmp ;
                    standard_deviation[gen_i]=  standard_deviation_tmp;
                    
                }
                
                //如果所有维度都不变，则子问题伪停滞次数+1，否则reset为0
                if(d_same == d_size){
                    pre_stagnant[group_index_to_evolve]++;
                }else{
                    pre_stagnant[group_index_to_evolve]=0;
                }
                //若停滞次数大于维度数(这个数可以自己设，人家推荐是这个)，则标记该种群停滞不前
                if(pre_stagnant[group_index_to_evolve]>=d_size){
                    stagnant[group_index_to_evolve]=true;
                }else{
                    stagnant[group_index_to_evolve]=false;
                }
                //如果子问题停滞不前则提前终止对此子问题的优化
                if(stagnant[group_index_to_evolve]){
                    break;
                }
                iter++;

            }
            //暂存旧的最优解，用于计算贡献度
            last_best_fitness = best_fitness;
            //扫描是否有新的最优解
            for (int i = 0; i < pop_size; i++) {
                if (population[i].fitness < best_fitness) {
                    best_fitness = population[i].fitness;
                    best_index = i;
                }
            }
            //看看是不是达到了终止进化条件：
            if(iter%100==0){
                printf("function %d  iter :%d ",function_id,iter);
            }
            if(best_fitness <= expect_value || iter >= max_iter){
                goto end_evolution;
            }

            //更新每个子问题的贡献度
            contribution[group_index_to_evolve]= (contribution[group_index_to_evolve] + fabs(last_best_fitness-best_fitness) )/2;
            //如果总群进化停滞不前，则贡献度归0
            if(stagnant[group_index_to_evolve]){
                contribution[group_index_to_evolve]=0;
            }
            //重新计算最大和最小贡献值
            max_contribution = *(max_element(contribution.begin(),contribution.end()));
            min_contribution =*(min_element(contribution.begin(),contribution.end()));
        }
        
    end_evolution:        
        if(best_fitness < expect_value){
            break;
        }
        //cout<<iter<<endl;
       
    }
    *iter_res=iter;
    *best_fitness_res=best_fitness;
}

