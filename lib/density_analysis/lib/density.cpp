#ifndef __DENSITY_CPP__
#define __DENSITY_CPP__
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>
#include "classes.h"
#include <stdlib.h>


using namespace std;

//implication for class box


density::density(int* _wall, int* get_level){
	N = _wall[0]*_wall[1]*_wall[2];
 level = get_level;
	for( int l = 0; l < 3; l++ ) wall[l] = _wall[l];
	_in = new double [N];
	_to_print = new double [N];
	_flag = new int [N];
	_fix_area = new int [N];
	_fix_area_flag = new int[N];
	for (int i = 0; i < N; i++) {
    _in[i] = 0;
		_fix_area[i] = 0;
		_fix_area_flag[i] = 0;
	}
}

int density::add(double* x){
  _in[int(x[2]*wall[2])*wall[0]*wall[1]+int(x[1]*wall[1])*wall[0]+int(x[0]*wall[0])]++;
  //cerr << "add " << int(x[2]*wall[2])*wall[0]*wall[1]+int(x[1]*wall[1])*wall[0]+int(x[0]*wall[0]) << endl;
	return 0;
}

int density::analysis(){
  min = _in[0];
  max = _in[0];
	double total_mass = _in[0];
  for( int i = 1; i < N; i++){
		total_mass += _in[i];
    if (_in[i] < min) min = _in[i];
    if (_in[i] > max) max = _in[i];
    _to_print[i] = _in[i];
  }
	double coeff = wall[0]*wall[1]*wall[2]*N/total_mass;
	for( int i = 0; i < N; i++){
		_in[i] *= coeff;
	}
	return 0;
}

density::~density(){
	delete[] _in;
	delete[] _to_print;
	delete[] _flag;
	delete[] _fix_area;
	delete[] _fix_area_flag;
}

int density::clear(){
	for( int i = 0; i < N; i ++) _flag[i] = 0;
	for( int i = 0; i < *level*11; i++) _fix_area_flag[i] = 0;
	return 0;
}

int density::fix_area(int count, int neig_at, int neig_el){
	for( int i = 0; i < N; i ++)
		if(_flag[i] == count) {
			_fix_area[i] = _last_fix;
			_fix_area_flag[i] = neig_at*10+neig_el;
		}
	_last_fix++;
	return 0;
}

int density::fix_to_print(){
	for( int i = 0; i < N; i++)
		if (_fix_area[i] != 0) _to_print[i] = _in[i]; //(max+min)/2; //*(1+0.01*_fix_area[i]); // make to easy diff it
		else _to_print[i] = 0;
	return 0;
}


double*	density::in(int* x){
	return &_in[x[2]*wall[0]*wall[1]+x[1]*wall[0]+x[0]];
}

double*	density::in(int x, int y, int z){
	return &_in[z*wall[0]*wall[1]+y*wall[0]+x];
}


int*	density::flag(int* x){
	return &_flag[x[2]*wall[0]*wall[1]+x[1]*wall[0]+x[0]];
}

int*	density::flag(int x, int y, int z){
	return &_flag[z*wall[0]*wall[1]+y*wall[0]+x];
}


#endif // __DENSITY_CPP__
