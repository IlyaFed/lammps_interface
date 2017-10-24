#ifndef __BOX_CPP__
#define __BOX_CPP__
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <assert.h>

#include "classes.h"

#include <stdlib.h>

#define Ab 0.52917720859

using namespace std;


box::box(FILE* input, const char* type_reading){
  if (!strcmp(type_reading, "chg") )
    read_chg(input);
}

box::box(FILE* input, const char* type_reading, double wall){
  if (!strcmp(type_reading, "xyz_collect") )
    read_xyz(input, wall);
}

int box::read_chg(FILE* input){
	cerr << "start reading box \n" << endl;
	char buf[100];
	//for( int i = 0; i < 6; i++) {
	fgets(buf, 100, input);
	fgets(buf, 100, input);
	double read_x,read_y, read_z;
	fscanf(input, "%lf %lf %lf", &read_x, &read_y, &read_z);
	lat[0] = read_x;
	fscanf(input, "%lf %lf %lf", &read_x, &read_y, &read_z);
	lat[1] = read_y;
	fscanf(input, "%lf %lf %lf", &read_x, &read_y, &read_z);
	lat[2] = read_z;
	cerr << "wall " << lat[0] << " "<< lat[1] << " " << lat[2] << endl;
	fgets(buf, 100, input);
	fgets(buf, 100, input);

  // read atoms coordinates
  int N_atom;
	fscanf(input, "%d\nDirect", &N_atom); //Number of atoms
	cerr << "N_atoms = " << N_atom << endl;

	//fgets(buf, 100, input);

  atoms = new atom(N_atom);
  double x[3];
  for( int i = 0; i < atoms->N; i++) {
    fscanf(input, "%lf\t%lf\t%lf", &x[0], &x[1], &x[2]);
    atoms->add(i, x);
  }
	//fscanf(input, "%s", buf);
 cerr << "read density" << endl;
  // read density in lattice
	fscanf(input, "%d\t%d\t%d", &wall[0], &wall[1], &wall[2]);
	cerr << "wall: " << wall[0] << " " << wall[1] << " " << wall[2] << endl;
	rho = new density(wall, &level);

	fscanf(input, "%lf", &rho->_in[0]);
	rho->min = rho->_in[0];
	rho->max = rho->_in[0];

	double total_mass = rho->_in[0];
	for( int i = 1; i < rho->N; i++) {
		fscanf(input, "%lf", &rho->_in[i]);
		rho->_to_print[i] = rho->_in[i];
		total_mass += rho->_in[i];
		if (rho->_in[i] < rho->min) rho->min = rho->_in[i];
		if (rho->_in[i] > rho->max) rho->max = rho->_in[i];
	}

	rho->N_electron = round( total_mass/rho->wall[0]/rho->wall[1]/rho->wall[2]);
	cerr << "N_electrons = " << rho->N_electron << endl;

	cerr << "succesful reading box" << endl;
	return 0;
}

int box::read_xyz(FILE* input, double wall_max){
	cerr << "start reading box \n" << endl;
	char buf[100];
  int N;
  int step = 1;
  while (1){
    if (fscanf(input, "%d\n", &N) == EOF) break;
    //cerr << "step " << step << " N =" << N << endl;
  	fgets(buf, 100, input);
  	double read_x[3];
    int type;
    int N_atom = 0, N_electron = 0;
    if (step == 1) atoms = new atom(int(N/2));
    int wall[3] = {27,27,27};
    if (step == 1) rho = new density(wall, &level);
    for (int l = 0; l < 3; l ++) lat[l] = wall_max;
    for (int i = 0; i < N; i++){
      assert(N_atom <= N/2);
      assert(N_electron <= N/2);
  	  fscanf(input, "%d %lf %lf %lf", &type, &read_x[0], &read_x[1], &read_x[2]);
      for (int l = 0; l < 3; l++) read_x[l] /= wall_max;
      if (type == 1){
        N_atom++;
        if (step == 1)
          atoms->add(N_atom, read_x);
      }
      if (type == 2){
        N_electron++;
        rho->add(read_x);
      }
    }
    //cerr << "N_atom = " << N_atom << endl;
    assert(N_atom == N/2);
    assert(N_electron == N/2);
    rho->N_electron = N_electron;
    step++;
  }
  cerr << "get " << step-1 << " steps" << endl;
  rho->analysis();

  cerr << "succesful reading box" << endl;
	return 0;
}

int box::write(string input_name, int rho_flag){
	ofstream foutput;
	foutput.open(input_name);
	cerr << "start writing box \n" << endl;

	foutput << "unknown system\n 1" << endl;
	foutput << lat[0] << "    0.000000    0.000000" << endl;
	foutput << "0.000000    " << lat[1] << "    0.000000" << endl;
	foutput << "0.000000    0.000000    " << lat[2] << endl;
//	int N_atom_print = 0;
//	for (int i = 0; i < N_atom; i ++) if(atom_flag == 0) N_atom_print++;
  //cerr << "1\n";

	if (rho_flag){
		foutput << "H\n" << atoms->types_count(rho_flag) << endl;
		foutput << "Direct" << endl;
		for (int i = 0; i < atoms->N; i ++)
			if (atoms->_fix_area_flag[i] == rho_flag) foutput << atoms->print(i)  << endl;
	}
	else {
		foutput << "H\n" << atoms->N << endl;
		foutput << "Direct" << endl;
		for (int i = 0; i < atoms->N; i ++) foutput << atoms->print(i) << endl;

	}
  //cerr << "2\n";
	foutput << "\n" << rho->wall[0] << "\t" << rho->wall[1] << "\t" << rho->wall[1];
  //cerr << "3\n";
	if (rho_flag){
		for( int i = 0; i < rho->N; i++) {
			if (i%5 == 0) foutput << "\n";
			if (rho->_fix_area_flag[i] == rho_flag)
				foutput << rho->_to_print[i] << " ";
			else
			  foutput << 0 << " ";
		}
	} else {
		for( int i = 0; i < rho->N; i++) {
			if (i%5 == 0) foutput << "\n";
			foutput << rho->_to_print[i] << " ";
		}
	}
  //cerr << "4\n";
	foutput.close();
	cerr << "succesful writin box" << endl;
	return 0;
}

int box::write_cube(string input_name, int rho_flag){
	if (atoms->types_count(rho_flag) < 1) return 0;

	ofstream foutput;
	input_name = input_name + to_string(write_flag)+".cube";
	write_flag++;
	foutput.open(input_name);
	cerr << "start writing box \n" << endl;

	foutput << "CUBE FILE CONVERTED FROM VASP format" << endl;
	foutput << "OUTTER LOOP: X, MIDDLE LOOP: Y, INNER LOOP: Z" << endl;
	foutput << atoms->types_count(rho_flag) << "     0.000000     0.000000     0.000000" << endl;
	double lat_print[3];
	for (int l = 0; l < 3; l ++) lat_print[l] = lat[l]/rho->wall[l] / Ab;
	foutput << rho->wall[0] << "	" << lat_print[0] << "    0.000000    0.000000" << endl;
	foutput << rho->wall[1] << "	0.000000    " << lat_print[1] << "    0.000000" << endl;
	foutput << rho->wall[2] << "	0.000000    0.000000    " << lat_print[2] << endl;
//	int N_atom_print = 0;
//	for (int i = 0; i < N_atom; i ++) if(atom_flag == 0) N_atom_print++;
	if (rho_flag){
		for (int i = 0; i < atoms->N; i ++)
			if (atoms->_fix_area_flag[i] == rho_flag) foutput << "1     0.000000     " << atoms->print_coeff(i, lat_print, Ab) << endl;
	}
	else {
		for (int i = 0; i < atoms->N; i ++) foutput << "1     0.000000     " << atoms->print_coeff(i, lat_print, Ab) << endl;

	}

	if (rho_flag){
		for( int ix = 0; ix < rho->wall[0]; ix++)
			for (int iy = 0; iy < rho->wall[1]; iy++)
				for (int iz = 0; iz < rho->wall[2]; iz++){
					if (iz%6 == 5) foutput << "\n";
					if (rho->_fix_area_flag[iz*wall[0]*wall[1]+iy*wall[0]+ix] == rho_flag)
						foutput << rho->_to_print[iz*wall[0]*wall[1]+iy*wall[0]+ix]/100 << " ";
					else
					  foutput << 0 << " ";
				}

	} else {
		for( int ix = 0; ix < rho->wall[0]; ix++)
			for (int iy = 0; iy < rho->wall[1]; iy++)
				for (int iz = 0; iz < rho->wall[2]; iz++){
					if (iz%6 == 5) foutput << "\n";
					foutput << rho->_to_print[iz*wall[0]*wall[1]+iy*wall[0]+ix]/100 << " ";
				}
	}

	foutput.close();
	cerr << "succesful writin box" << endl;
	return 0;
}


int box::cut_unnes(double rho_cut){
	cerr << "empty areas = " << area->_to_print_last + 1 << endl;
	for(int i = 0; i < rho->N; i++){
//		if (flag[i] != 0) new_line[i] = 50;
//		else new_line[i] = 0;
		rho->_to_print[i] = 0;
		for(int j = 0; j < area->_to_print_last+1; j++)
			if (rho->_flag[i] == area->_to_print[j]) rho->_to_print[i] = rho->_in[i];
	}
	cerr << "cut unnes succesful" << endl;
	return 0;
}

box::~box(){
	if (area != NULL) clear();
	delete atoms;
  cerr << "delete atoms" << endl;
	delete rho;
  cerr << "delete rho" << endl;
}

int* box::x(int coord){
	int* x;
	x = new int [3];
	x[2] = coord / (wall[1]*wall[2]);
	x[1] = (coord % (wall[1]*wall[2]) ) / wall[2];
	x[0] = (coord % (wall[1]*wall[2]) ) % wall[2];
	return x;
}

int box::len(int* x){
	return x[2]*wall[0]*wall[1]+x[1]*wall[0]+x[0];
}






int box::create_areas(const double _rho_cut){
	rho_cut = _rho_cut;
	int k = 1;
	area = new areas(atoms, rho->N_electron);
	for( int i = 0; i < rho->N; i++){
		if ( rho->_in[i] < rho_cut || rho->_flag[i] != 0 ) continue;
//		cerr << "add new area" << endl;
		area->add();
		int* x_0 = x(i);
		find_all(area->last, x_0, rho_cut, k);
  	k++;
	}
  atom_flaged();
	area->analysis(rho_cut);
	//cerr << "1" << endl;
	atom_connect();
//	cerr << "2" << endl;
	return 0;
}

int box::atom_connect(){
	cerr << "new" << endl;
	for( int i = 0; i < atoms->N; i++)
		if(atoms->_fix_area[i] == 0 && (atoms->neigbours_atom[i] + atoms->neigbours_electron[i]) < level) {
			cerr << "add atom " << i << endl;
			rho->fix_area(atoms->flag[i], atoms->neigbours_atom[i], atoms->neigbours_electron[i]);

			atoms->fix_area(atoms->flag[i], atoms->neigbours_atom[i], atoms->neigbours_electron[i]);
		}
	return 0;
}

int box::print_areas(string input_name, int split_flag){
	rho->fix_to_print();
	if (!split_flag) {
		write(input_name, 0);
	}else {
		for (int i = 1; i < 50; i++){
			cerr << "start writing" << endl;
			write_cube(input_name, i);
		}
	}
	return 0;
}

int box::make_molecule(string output_name, int split_flag){
	cerr << "split flag " << split_flag << endl;
	double start = rho->min;
  double stop = rho->max;
	double step = (stop-start) / 100.0;
	for( double rho_cut = start; rho_cut < stop; rho_cut+= step){
    cerr << "rho = " << rho_cut << "/ ("  <<stop << " - " << start << ")\n";
  	create_areas(rho_cut);
	  clear();
	}

	cerr << "ready to print" << endl;
	cerr << "split flag " << split_flag << endl;
	print_areas(output_name, split_flag);
	return 0;
}

int box::find_all(area_tree* buf, int* x, double _rho_cut, int flag_point){
	if (*rho->flag(x) != 0 || *rho->in(x) < _rho_cut) return 0;
//	cerr << flag_point << endl;
	*rho->flag(x) = flag_point;
	buf->mass += *(rho->in(x));
	int x_new [3] = {x[0], x[1], x[2]};
	for (int l = 0; l < 3; l++){
		x_new[l] = (x[l]+wall[l] + 1)% wall[l];
		find_all(buf, x_new, _rho_cut, flag_point);
		x_new[l] = (x[l]+wall[l] - 1)% wall[l];
		find_all(buf, x_new, _rho_cut, flag_point);
		x_new[l] = x[l];
	}
	return 0;
}

int box::atom_flaged(){
	int first[3];
	int second[3];
	int a[3];
	for( int i =0 ; i < atoms->N; i ++){
	//	cerr << "atom " << i << endl;
		for( int l =0; l < 3; l++)
			first[l] =  atoms->_x[i*3+l]*wall[l];
			//if (rho_cut < 37) cerr << "rho flag = " << *rho->flag(first) << endl;
		for (a[0] = 0; a[0] < 2; a[0]++)
			for (a[1] = 0; a[1] < 2; a[1]++)
				for (a[2] = 0; a[2] < 2; a[2]++) {
					for(int l = 0; l < 3; l++)
						second[l] = (first[l] + a[l]) % wall[l];

					if (*rho->flag(second) != 0)  {
						atoms->flag[i] = *rho->flag(second);
					}
		}
	}
	return 0;
}

int box::clear(){
	delete area;
  area = NULL;
  atoms->clear();
	rho->clear();
	return 0;
}

int box::print_atoms(string name){
	ofstream dump_output;
	dump_output.open(name);
	int k = 0;
	dump_output << "ITEM: TIMESTEP\n" << 0 << "\nITEM: NUMBER OF ATOMS\n" << atoms->N << "\nITEM: BOX BOUNDS pp pp pp\n" << 0e0  << " " << 1e0 << "\n" << 0e0  << " "  << 1e0 << "\n" << 0e0  << " "  << 1e0 << "\nITEM: ATOMS id type xs ys zs" << endl;
	for( int i = 0; i < atoms->N; i ++) dump_output << i << " " << atoms->flag[i]  << " " << atoms->print(i)<< endl;
	dump_output.close();
	cerr << "print atom succesful" << endl;
	return 0;
}
#endif // __BOX_CPP__
