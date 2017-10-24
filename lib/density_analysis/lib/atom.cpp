#ifndef __ATOM_CPP__
#define __ATOM_CPP__
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>
#include "classes.h"
#include <stdlib.h>


using namespace std;

atom::atom(int _N){
  N = _N;
  _x = new double [N*3];
  flag = new int [N];
  _fix_area = new int [N];
  _fix_area_flag = new int [N];
  for (int i = 0; i < N; i++) {
    _fix_area[i] = 0;
    _fix_area_flag[i] = 0;
  }
  neigbours_atom = new int [N];
  neigbours_electron = new int [N];

  clear();
}

atom::~atom(){
  delete[] _x;
  delete[] flag;
  delete[] neigbours_atom;
  delete[] neigbours_electron;
  delete[] _fix_area;
  delete[] _fix_area_flag;
}

int atom::clear(){
  for (int i = 0; i < N; i++) {
    flag[i] = 0;
    //_fix_area_flag[i] = 0;
    //_fix_area[i] = 0;
  }
  _last_fix = 1;
  return 0;
}

double* atom::x(int i){
  return &_x[i*3];
}

char* atom::print_coeff(int i, double* lat, double coeff){
  char *buf = new char [100];
  sprintf(buf, "%lf\t%lf\t%lf", _x[i*3]/lat[0]/coeff, _x[i*3+1]/lat[1]/coeff, _x[i*3+2]/lat[2]/coeff);
  return buf;
}

char* atom::print(int i){
  char *buf = new char [100];
  //cerr << "start_printing\n";
  sprintf(buf, "%lf\t%lf\t%lf", _x[i*3], _x[i*3+1], _x[i*3+2]);
  return buf;
}

int atom::add(int i, double* coord){
  for(int l = 0; l < 3; l++)
    _x[i*3+l] = coord[l];
  return 0;
}

int atom::fix_area(int count, int neig_at, int neig_el){
	for( int i = 0; i < N; i ++)
		if(flag[i] == count) {
      _fix_area[i] = _last_fix;
      _fix_area_flag[i] = neig_at*10+neig_el;
    }
	_last_fix++;
	return 0;
}

int atom::types_count(int type_flag){
  int number = 0;
  for( int i = 0; i < N; i ++) if (_fix_area_flag[i] == type_flag) number++;
  return number;
}
#endif // __ATOM_CPP__
