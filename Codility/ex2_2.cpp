#include "header.h"
using namespace std;

int single_in_array(vector<int> &A){
    vector<int> A_tmp = A;
    sort(A_tmp.begin(), A_tmp.end());
    unsigned int len = A_tmp.size();
    
    while (!A_tmp.empty()){
        if (A_tmp[A_tmp.size()-1] != A_tmp[A_tmp.size()-2]){
            return A_tmp[A_tmp.size()-1];
        }
        else{
            A_tmp.pop_back();
            A_tmp.pop_back();
        }
    }
    return -1;
}

void do_ex2_2(){
    vector<int> input = readfile_i("testcases/testcase2_2.txt");
    cout<<"Input array: ";
    display_vector_i(input);
    int x = single_in_array(input);
    cout << "Answer: " << x << endl;
}