#include "header.h"
using namespace std;

int tape_equilibrium(vector<int> &A){
    if(A.size() == 1)
        return A[0];
    unsigned int len = A.size(), i = 0; 
    int sum = 0;
    for (i = 0; i < len; ++i)
        sum += A[i];
    int sum1 = A[0], sum2 = sum - sum1, dif = abs(sum2 - sum1), min_dif = dif; 
    for (i = 1; i < len - 1; ++i){
        sum1 += A[i];
        sum2 = sum - sum1;
        dif = abs(sum2 - sum1); 
        if (dif < min_dif)
            min_dif = dif;
    }
    cout << "Split at position: " << i <<endl;
    return min_dif;
}

void do_ex3_3(){
    vector<int> input = readfile_i("testcases/testcase3_3.txt");
    cout << "Input array: ";
    display_vector_i(input);
    int x = tape_equilibrium(input);
    cout << "Answer = " << x << endl;
}


