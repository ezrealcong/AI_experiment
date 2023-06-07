#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <algorithm>
#include "../include/DG2.h"

using namespace std;

DG2::DG2 (Benchmarks *f)
{
    this->func = f;
    g.resize(dim);
    delta1    = new double* [dim];
    delta2    = new double* [dim];
    lambda    = new double* [dim];
    theta     = new double* [dim];
    epsilon   = new double* [dim];
    archive_f = new double* [dim];
    for (int i = 0; i < dim; i++){
        delta1[i]    = new double [dim];
        delta2[i]    = new double [dim];
        lambda[i]    = new double [dim];
        theta[i]     = new double [dim];
        epsilon[i]   = new double [dim];
        archive_f[i] = new double [dim];
    }

    archive_fhat = new double [dim];

    eval_count = 0;
    archive_base = std::numeric_limits<double>::quiet_NaN();
    for (int i = 0; i < dim; i++){
        archive_fhat[i] = std::numeric_limits<double>::quiet_NaN();
        for (int j = 0; j < dim; j++){
            delta1[i][j] = std::numeric_limits<double>::quiet_NaN();
            delta2[i][j] = std::numeric_limits<double>::quiet_NaN();
            lambda[i][j] = std::numeric_limits<double>::quiet_NaN();
            epsilon[i][j] = std::numeric_limits<double>::quiet_NaN();
            archive_f[i][j] = std::numeric_limits<double>::quiet_NaN();
            if (i == j){
                theta[i][j] = 1;
            }
            else{
                theta[i][j] = std::numeric_limits<double>::quiet_NaN();
            }
        }
    }
}

void DG2::arrayCopy(double *dest, double *src, int size){
    for (int i = 0; i < size; i++){
        dest[i] = src[i];
    }
}

void DG2::ism (void){

    double fp1, fp2, fp3, fp4, d1, d2;
    double temp = (func->getMinX()+func->getMaxX())/2.0;


    double *p1 = new double [dim];
    double *p2 = new double [dim];
    double *p3 = new double [dim];
    double *p4 = new double [dim];

    for (int i = 0; i < dim; i++){
        p1[i] = func->getMinX();
    }

    fp1 = func->compute(p1);
    archive_base = fp1;
    eval_count += 1;

    int counter = 0;
    int prev = 0;
    int prog = 0;
    for (int i = 0; i < dim - 1; i++){
        if (!std::isnan(archive_fhat[i])){
            fp2 = archive_fhat[i];
        }else{
            arrayCopy(p2, p1, dim);
            p2[i] = temp;
            fp2 = func->compute(p2);
            eval_count += 1;
            archive_fhat[i] = fp2;

        }

        for (int j = i + 1; j < dim; j++){
            counter++;
            prev = prog;
            prog = (int)(counter/(float)(dim*(dim-1))*2*100);
//            if (prog % 5 == 0 && prev != prog){
//                printf("Progress = %02d\r", prog);
//            }

            if (!std::isnan(archive_fhat[j])){
                fp3 = archive_fhat[j];
            }else {
                arrayCopy(p3, p1, dim);
                p3[j] = temp;
                fp3 = func->compute(p3);
                eval_count += 1;
                archive_fhat[j] = fp3;
            }

            arrayCopy(p4, p1, dim);
            p4[i] = temp;
            p4[j] = temp;
            fp4 = func->compute(p4);
            eval_count += 1;
            archive_f[i][j] = fp4;
            archive_f[j][i] = fp4;

            d1 = fp2 - fp1;
            d2 = fp4 - fp3;

            delta1[i][j] = d1;
            delta2[i][j] = d2;
            lambda[i][j] = fabs(d1 - d2);
            lambda[j][i] = fabs(d1 - d2);
        }
    }

    delete [] p1;
    delete [] p2;
    delete [] p3;
    delete [] p4;

}

double DG2::gammaFunc(double d){
    double muM = (std::numeric_limits<double>::epsilon())/2.0;
    return (d * muM)/(1 - (d * muM));

}

void DG2::dg(double epsilon){
    for (int i = 0 ; i < dim - 1; i++){
        for (int j = i + 1; j < dim; j++){
            if (lambda[i][j] < epsilon){
                theta[i][j] = 0;
            }else if (lambda[i][j] > epsilon){
                theta[i][j] = 1;
            }
        }
    }
}


vector<vector<int>>& DG2::getGroups()
{
    return groups;
}


void DG2::dfs(vector<bool>& f,vector<int>& group,int cur)
{
    if(f[cur])
        return;
    f[cur]=true;
    group.push_back(cur);
    for(auto next:g[cur])
    {
        dfs(f,group,next);
    }
    return;
}

void DG2::dsm (void)
{

    double array [4] = {};
    double fMax = 0;
    double eInf = 0, eSup = 0;
    double etha0 = 0, etha1 = 0;
    double eps = 0;

    for (int i = 0 ; i < dim - 1; i++){
        for (int j = i + 1; j < dim; j++){
            array[0] = archive_base;
            array[1] = archive_f[i][j];
            array[2] = archive_fhat[i];
            array[3] = archive_fhat[j];
            fMax = *max_element(array, array + 4);
            eInf = gammaFunc(2) * std::max(array[0] + array[1], array[2] + array[3]);
            //eSup = gammaFunc(std::sqrt(func->getDim())) * fMax;
            eSup = gammaFunc(pow((double)dim, (double)0.5)) * fMax;
            if (lambda[i][j] <= eInf){
                theta[i][j] = 0;
                theta[j][i] = 0;
                etha0 += 1;
            }
            else if (lambda[i][j] >= eSup){
                theta[i][j] = 1;
                theta[j][i] = 1;
                g[i].push_back(j);
                g[j].push_back(i);
                etha1 += 1;
            }
        }
    }

    for (int i = 0 ; i < dim - 1; i++){
        for (int j = i + 1; j < dim; j++){
            array[0] = archive_base;
            array[1] = archive_f[i][j];
            array[2] = archive_fhat[i];
            array[3] = archive_fhat[j];
            fMax = *std::max_element(array, array + 4);
            eInf = gammaFunc(2) * std::max(array[0] + array[1], array[2] + array[3]);
            //eSup = gammaFunc(std::sqrt(func->getDim())) * fMax;
            eSup = gammaFunc(pow((double)dim, (double)0.5)) * fMax;
            if (std::isnan(theta[i][j])){
                eps = (etha0 / (etha0 + etha1)) * eInf + (etha1 / (etha0 + etha1)) * eSup;
                epsilon[i][j] = eps;
                epsilon[j][i] = eps;
                if (lambda[i][j] > eps){
                    theta[i][j] = 1;
                    theta[j][i] = 1;
                    g[i].push_back(j);
                    g[j].push_back(i);
                }
                else{
                    theta[i][j] = 0;
                    theta[j][i] = 0;
                }
            }

        }
    }


    vector<bool> f(dim,false);
    for(int i=0;i<dim;i++)
    {
        if(!f[i])
        {
            vector<int> group;
            dfs(f,group,i);
            groups.push_back(group);
        }
    }


}



DG2::~DG2(void)
{
    for (int i = 0; i < dim; i++){
        delete [] delta1[i]    ;
        delete [] delta2[i]    ;
        delete [] lambda[i]    ;
        delete [] theta[i]     ;
        delete [] epsilon[i]   ;
        delete [] archive_f[i] ;
    }
    delete [] delta1    ;
    delete [] delta2    ;
    delete [] lambda    ;
    delete [] theta     ;
    delete [] epsilon   ;
    delete [] archive_f ;
    delete [] archive_fhat;
}
