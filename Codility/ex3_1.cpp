#include "header.h"
using namespace std;

int min_step(int X, int Y, int D){
    int ret = (Y - X)/D;
    if (X + ret*D < Y)
        return ret + 1;
    return ret;
}

void do_ex3_1(){
    vector<int> input = readfile_i("testcases/testcase3_1.txt");
    for (unsigned int i = 0; i + 2 < input.size(); i+= 3){
        cout << (i+3)/3 << "." << endl
            << "X = " << input[i] <<endl
            << "Y = " << input[i+1] <<endl
            << "D = " << input[i+2] <<endl
            << "Answer = " << min_step (input[i], input[i+1], input[i+2]) <<endl; 
    }
}