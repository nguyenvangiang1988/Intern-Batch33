#include "header.h"
using namespace std;

int perm_missing_elem(vector<int> &A){
    if (A.empty())
        return 1;
    sort(A.begin(), A.end());
    if (A[0] != 1)
        return 1;
    for (unsigned int i = 0; i + 1 < A.size(); ++i){
        if (A[i] == A[i+1] - 2)
            return A[i] + 1;
    }
    return A[A.size()-1] +1;
}

void do_ex3_2(){
    vector<int> input = readfile_i("testcases/testcase3_2.txt");
    cout << "Input array: ";
    display_vector_i(input);
    int x = perm_missing_elem(input);
    cout << "Answer = " << x << endl;
}