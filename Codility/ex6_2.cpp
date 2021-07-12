#include "header.h"
using namespace std;

int max_product_of_3(vector<int> &A) {
    sort(A.begin(), A.end());
    unsigned int len = A.size();
    int candidate1 = A[len-1] * A[len-2] * A[len-3], candidate2 = A[len-1] * A[0] * A[1];
    if (candidate1 > candidate2)
        return candidate1;
    else return candidate2;
}

void do_ex6_2(){
    vector<int> input = readfile_i("testcases/testcase6_2.txt");
    cout << "Input array: ";
    display_vector_i(input);
    int x = max_product_of_3(input);
    cout << "Max triplet\'s product = " << x << endl;
}
