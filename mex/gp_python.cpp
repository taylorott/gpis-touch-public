/*
 * test_gp.cpp: cpp test code for GPShape library
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of any FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License v3 for more details.
 *
 * You should have received a copy of the GNU General Public License v3
 * along with this program; if not, you can access it online at
 * http://www.gnu.org/licenses/gpl-3.0.html.
 *
 * Author: Sudharshan Suresh <suddhu@cmu.edu>
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <memory>

#include "GPShape.hpp"
#include "get_shapes.hpp"
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

static GPShape* gp = 0;
static cnt contour;
static bool has_eval_contour = false;
static std::chrono::high_resolution_clock::time_point t_start;
static std::chrono::high_resolution_clock::time_point t_end;
static double t_dur = 0.0;
// static std::vector<double> out_list = {0.0,0.0,0.0};
static double v = 0.0;
static double dvdx = 0.0;
static double dvdy = 0.0;

void init(double pvar1,double pvar2,double pvar3, double priorvar1, double priorvar2, double priorvar3, double testLim,double testRes,bool isLocal, double priorRad) {

    if (gp !=0) {
        delete gp;
        gp = 0;
        std::cout<<"init(): GP already intialized! Clearing..."<< std::endl;
    }


    int GPcount = 1;
    if (isLocal){
        GPcount = 16;
    }

    std::vector<double> pvar = {pvar1,pvar2,pvar3};
    std::vector<double> priorvar = {priorvar1,priorvar2,priorvar3};

    std::string kernel = "thinplate";
    gp = new GPShape(pvar, priorvar, kernel, testLim, testRes, GPcount, isLocal);
    gp->addPrior(priorRad, 4, Eigen::Vector3d::Zero());
    return;  
}

void update(double x,double y,double nx,double ny) {

    if (gp ==0) {
        std::cout<<"update(): GP not intialized!"<< std::endl;
        return;
    } 

    Eigen::Vector2d contact_point(x, y);
    Eigen::Vector2d contact_normal(nx, ny);

    gp->update(contact_point, contact_normal, Eigen::Vector3d::Zero());
    return;
}

void evalAtPoint_internal(double x,double y){
    gp->evalAtPoint(x,y,&v,&dvdx,&dvdy);
}

double evalAtPoint_value_internal(){
    return v;
}

double evalAtPoint_dvdx_internal(){
    return dvdx;
}

double evalAtPoint_dvdy_internal(){
    return dvdy;
}

void update_contour_internal(){
    if (gp ==0) {
        std::cout<<"update(): GP not intialized!"<< std::endl;
        return;
    }

    Emx fMean, fVar; Evx contourVar;
    gp->test(contour, fMean, fVar, contourVar);
    has_eval_contour = true;
    return;
}


int contour_length_internal(){
    if (!has_eval_contour){
        std::cout<<"contour does not exist!"<< std::endl;
        return -1; 
    }
    return contour.size();
}

double eval_contour_x_internal(int index){
    if (!has_eval_contour){
        std::cout<<"contour does not exist!"<< std::endl;
        return -1; 
    }
    if(index<0 || index>=contour.size()){
        std::cout<<"index out of bounds!"<< std::endl;
        return 0.0;
    }
    return contour[index][0];
}

double eval_contour_y_internal(int index){
    if (!has_eval_contour){
        std::cout<<"contour does not exist!"<< std::endl;
        return -1; 
    }
    if(index<0 || index>=contour.size()){
        std::cout<<"index out of bounds!"<< std::endl;
        return 0.0;
    }
    return contour[index][1];
}


extern "C"
{
    void init_gp(double pvar1,double pvar2,double pvar3, double priorvar1, double priorvar2, double priorvar3, double testLim, double testRes,bool isLocal, double priorRad){
        init(pvar1,pvar2,pvar3, priorvar1, priorvar2, priorvar3, testLim, testRes,isLocal, priorRad);
    }
    void update_gp(double x,double y,double nx,double ny){
        update(x,y,nx,ny);
    }
    void update_contour(){
        update_contour_internal();
    }

    int contour_length(){
        return contour_length_internal();
    }

    double eval_contour_x(int index){
        return eval_contour_x_internal(index);
    }

    double eval_contour_y(int index){
        return eval_contour_y_internal(index);
    }

    void evalAtPoint(double x,double y){
        evalAtPoint_internal(x,y);
    }

    double evalAtPoint_value(){
        return evalAtPoint_value_internal();
    }

    double evalAtPoint_dvdx(){
        return evalAtPoint_dvdx_internal();
    }

    double evalAtPoint_dvdy(){
        return evalAtPoint_dvdy_internal();
    }
}

