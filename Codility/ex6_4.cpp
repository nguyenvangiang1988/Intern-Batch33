#include "header.h"
using namespace std;

int check_triangle(vector<int> &A){
    if (A.empty())
        return 0;
    sort(A.begin(), A.end());
    unsigned int i, len = A.size();
    for(i = len - 1; i >= 2; --i){
        if (A[i] - A[i-1] < A[i-2])
            return 1;
    }
    return 0;
}

void do_ex6_4(){
    vector<int> input = readfile_i("testcases/testcase6_4.txt");
    cout << "Input array: ";
    display_vector_i(input);
    int x = check_triangle(input);
    if (x != 0)
        cout << "There is at least 1 triangle." << endl;
    else cout << "There is no triangle." << endl;
}