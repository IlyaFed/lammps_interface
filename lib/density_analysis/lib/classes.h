#ifndef __DENSITY_H__
#define __DENSITY_H__

#include <iostream>
#include <fstream>
#include <string>


using namespace std;


class atom{
public:
	double *_x; //coordinates of all atoms
	int *flag;
	int N;
	int *neigbours_atom;
	int *neigbours_electron;
	int *_fix_area = NULL;
	int *_fix_area_flag = NULL;
	int _last_fix = 1;

	atom(int _N);
	~atom();
	int clear();
	double* x(int i);
	char* print_coeff(int i, double* lat, double coeff);
	char* print(int i);

	int add(int i, double* coord);
	int fix_area(int count, int neig_at, int neig_el);
	int types_count(int type_flag);
};

class density{
public:
	double *_in = NULL;
	double *_to_print = NULL;
	int *_fix_area = NULL;
	int *_fix_area_flag = NULL;
	int _last_fix = 1;
	int N;
	double min;
	double max;
	int *_flag = NULL;
	int wall[3];
	int N_electron;
	int *level;

	density(int* _wall, int* get_level);
	~density();
	int clear();
	double* in(int* coord);
	double* in(int a, int b, int c);
  int add(double* x);
  int analysis();
	int* flag(int* x);
	int* flag(int x, int y, int z);
	int fix_area(int count, int neig_at, int neig_el);
	int fix_to_print();
};

class area_tree
{
public:

	area_tree* next = NULL;

	int count;
	double mass = 0;
	int N_atom = 0;
	int N_electron = 0;
	~area_tree();
	area_tree();
};

class areas{
public:
	atom* atoms;
	int *_to_print;
	//int *print_flag;
	int _to_print_last = 0;
	area_tree *start = NULL;
	area_tree *last = NULL;
	int _max_gist = 11;
	int* _gistogram = NULL;
	double mass = 0;
	int N;
	int N_electron = 0;
	int N_electron_real = 0;
	int N_atom = 0;

	areas(atom* _atoms, int _N_electron);
	~areas();
	int add();
	int analysis(double rho_cut);
	int gist_add(int n_atoms, int n_electrons);
	int gist_get(int n_atoms, int n_electrons);
	char* gist_to_print();
	int gist_analysis();

	int smth_useful(double rho_cut, area_tree* buf);

};

class box{
public:
	density *rho = NULL;
	areas *area = NULL;
	atom *atoms = NULL;
	double lat[3];
	int wall[3];
	double rho_cut = 0;
	int write_flag = 0;
	int level = 20; // atoms + electrons more than 20 - unreal
	box(FILE* input, const char* type_reading);
  box(FILE* input, const char* type_reading, double wall);
	~box();

  int read_chg(FILE* input);
	int read_xyz(FILE* input, double wall_max);
	int write(string input_name, int rho_flag);
	int write_cube(string input_name, int rho_flag);
	int cut_unnes(double rho_cut);

	int create_areas(double density_cut);
	int find_all(area_tree* buf, int* x, double density_cut, int flag_point);
	int atom_flaged();
	int print_atoms(string name);
	int atom_connect();
	int print_areas(string input_name, int split_flag);

	int make_molecule(string input_name, int split_flag);

	int* x(int coord);
	int len(int* x);
	int clear();

};



#endif // __DENSITY_H__
