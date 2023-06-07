#ifndef SOURCE_DG2_H
#define SOURCE_DG2_H

#include "../include/Header.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>


class DG2 {

public:

    DG2(Benchmarks *func);
    ~DG2();
    void ism (void);
    void dsm (void);
    void dg (double);
    vector<vector<int>>& getGroups();

private:

    std::vector<vector<int>> g; //邻接矩阵
    static constexpr int dim =1000;
    Benchmarks *func;
    double **delta1;
    double **delta2;
    double **lambda;
    double **theta;
    double **epsilon;
    int eval_count;
    double **archive_f;
    double *archive_fhat;
    double archive_base;
    std::vector< std::vector<int> > groups;

    void arrayCopy (double *dest, double *src, int size);
    void dfs(vector<bool>&,vector<int>&,int);
    double gammaFunc (double d);

};

#endif 
