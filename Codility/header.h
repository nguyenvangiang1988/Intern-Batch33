#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;
#ifndef _header_h_
#define _header_h_

vector<string> readfile_s(string filename);
vector<int> readfile_i(string filename); 
void display_vector_i(vector<int> A);

//Ex1==
string to_binary(int N);
int longest_binary_gap(int N);
void do_ex1();
//=====

//Ex2==
vector<int> rotate_array(vector<int> &A, int K);
void do_ex2_1();
int single_in_array(vector<int> &A);
void do_ex2_2();
//=====

//Ex3==
int min_step(int X, int Y, int D);
void do_ex3_1();
int perm_missing_elem(vector<int> &A);
void do_ex3_2();
int tape_equilibrium(vector<int> &A);
void do_ex3_3();
//=====

//Ex6==
int no_distinct(vector<int> &A);
void do_ex6_1();
int max_proudct_of_3(vector<int> &A);
void do_ex6_2();
int no_intersect2(vector<int> &A);
void do_ex6_3();
int check_triangle(vector<int> &A);
void do_ex6_4();
//=====

//Ex7==
int is_properly_nested(string &S);
void do_ex7_1();
int remaining_fish(vector<int> &A, vector<int> &B);
void do_ex7_2();
void do_ex7_3();
int no_block(vector<int> &H);
void do_ex7_4();
//=====
#endif