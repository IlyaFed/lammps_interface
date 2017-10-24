#ifndef __AREAS_CPP__
#define __AREAS_CPP__
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>
#include "classes.h"
#include <stdlib.h>


using namespace std;

areas::areas(atom* _atoms, int _N_electron){
  atoms = _atoms;
  N_electron = _N_electron;
  N = 0;
  _gistogram = new int [_max_gist];
  for (int i = 0; i < _max_gist; i++) _gistogram[i] = 0;
  last = new area_tree();
  last->count = 1;
  start = last;
  last->next = NULL;
  }

areas::~areas(){
    while (start){
      last = start->next;
      delete start;
      start = last;
    }
    start = NULL;
    last = NULL;
  }

int areas::analysis(double rho_cut){
	//cerr << "start area work" << endl;
	if (start == NULL) return 0;
	mass += last->mass;
	_to_print = new int [N];
	_to_print_last = 0;
 	double coeff = mass / N_electron;
	area_tree* buf = start;
	N_electron_real = 0;
	while (buf != NULL){
		buf->N_electron = round(buf->mass/coeff);

		N_electron_real += buf->N_electron;

		for (int i = 0; i < atoms->N; i ++)
			if (atoms->flag[i] == buf->count) buf->N_atom++;

		gist_add(buf->N_atom, buf->N_electron);
		smth_useful(rho_cut, buf);

		buf = buf->next;
	}

	return 0;
}


int areas::smth_useful(double rho_cut, area_tree* buf){
  for (int i = 0; i < atoms->N; i++)
    if (atoms->flag[i] == buf->count) {
      atoms->neigbours_atom[i] = buf->N_atom;
      atoms->neigbours_electron[i] = buf->N_electron;
    }
	// if (rho_cut != 96 || buf->N_atom != 0)  return 0;
	// cerr << "empty" << endl;
	// _to_print[_to_print_last] = buf->count;
	// _to_print_last++;
	return 0;
}

int areas::gist_analysis(){
	for (int i = 0; i < atoms->N; i ++) if (atoms->flag[i] == 0) _gistogram[0]++;
	return 0;
}

int areas::gist_add(int n_atoms, int n_electrons){
// max 10 electrons per atom
  int cell;
  if (n_electrons > 9) return 1;
	cell = n_atoms*10 + n_electrons;

	switch (cell) {
		case 01:
			_gistogram[0]++;
			break;
		case 10:
			_gistogram[1]++;
			break;
		case 11:
			_gistogram[2]++;
			break;
		case 20:
			_gistogram[3]++;
			break;
		case 21:
			_gistogram[4]++;
			break;
		case 22:
			_gistogram[5]++;
			break;
		case 30:
			_gistogram[6]++;
			break;
		case 31:
			_gistogram[7]++;
			break;
		case 32:
			_gistogram[8]++;
			break;
		case 33:
			_gistogram[9]++;
			break;
		default:
  //    cerr << "at: " << n_atoms << "\tel: " << n_electrons << endl;
			_gistogram[10]++;
	}
	return 0;
}

int areas::gist_get(int n_atoms, int n_electrons){
// max 10 electrons per atom
  int cell;
  if (n_electrons > 9) return 1;
	cell = n_atoms*10 + n_electrons;

	switch (cell) {
		case 1:
			return _gistogram[0];
		case 10:
			return _gistogram[1];
		case 11:
			return _gistogram[2];
		case 20:
			return _gistogram[3];
		case 21:
			return _gistogram[4];
		case 22:
			return _gistogram[5];
		case 30:
			return _gistogram[6];
		case 31:
			return _gistogram[7];
		case 32:
			return _gistogram[8];
		case 33:
			return _gistogram[9];
	}
	return 0;
}

char* areas::gist_to_print(){
	char *buf = new char [100];
	buf[0] = '\0';
	for (int i = 0; i < _max_gist; i++)
		sprintf(buf,"%s\t%d", buf, _gistogram[i]);

	return buf;
}



int areas::add(){
  N++;
  if (N == 1)
    return 0;
  mass += last->mass;

  last->next = new area_tree();
  last = last->next;
  last->count = N;
  last->next = NULL;
  return 0;
}


area_tree::~area_tree(){
	//if (next != NULL){
	//	delete next;
	//}
}

area_tree::area_tree(){}

#endif // __AREAS_CPP__
