#include "header.h"
using namespace std;

int no_distinct(vector<int> &A){
    if (A.empty())
        return 0;
    sort(A.begin(), A.end());
    int count = 1;
    for (unsigned int i = 1; i < A.size(); ++i)
        if (A[i] != A[i-1])
            ++ count;
    return count;
}

void do_ex6_1(){
    vector<int> input = readfile_i("testcases/testcase6_1.txt");
    cout << "Input array: ";
    display_vector_i(input);
    int x = no_distinct(input);
    cout << "Number of distincts = " << x << endl;
}