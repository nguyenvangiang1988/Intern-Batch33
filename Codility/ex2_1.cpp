#include "header.h"
using namespace std;

vector<int> rotate_array(vector<int> &A, int K){
    unsigned int len = A.size();
    if (len == 0 || len == 1)
        return A;
    K = K % len;
    for (unsigned int i = 0; i < K; ++i){
        int x = A[len-1];
        A.pop_back();
        A.insert(A.begin(), x);
    }
    return A;
}

void do_ex2_1(){
    vector<int> input = readfile_i("testcases/testcase2_1.txt");
    cout<<"Input array: ";
    display_vector_i(input);
    cout << "Rotate how many units? ";
    int unit;
    cin >> unit;
    if (!cin.good()){
        cout<<"Invalid input. Exit ex2_1."<<endl;
        return;
    }
    input = rotate_array(input, unit);
    cout << "Answer: ";
    display_vector_i(input);
}