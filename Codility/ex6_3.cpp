#include "header.h"
using namespace std;

//Simple but not efficient
int no_intersect(vector<int> &A){
    unsigned int i, j, len = A.size(), count = 0;
    for (i = 0; i < len; ++i)
        for (j = i + 1; j < len; ++j){
            if (j - i < A[j] + A[i])
                ++count;
            if (count > 10000000)
                return -1;
        }
    return count;
}

//Time efficient, reference Stack Overflow
int no_intersect2(vector<int> &A){
    unsigned long i, len = A.size();
    vector<long> left_marks;
    vector<long> right_marks;
    for (i = 0; i < len; ++i){
        left_marks.push_back(i - A[i]);
        right_marks.push_back(i + A[i]);
    }
    sort(left_marks.begin(), left_marks.end());
    sort(right_marks.begin(), right_marks.end());

    int overlaps = 0, total = 0;
    unsigned long j = 0;
    i = 1;
    while (i < left_marks.size()){
        if (left_marks[i] <= right_marks[j]){
            ++total;
            if (total > 0)
                overlaps += total;
            ++i;
        }
        else{
            --total;
            ++j;
        }
        if (overlaps > 10000000)
            return -1;
    }
    return overlaps;
}

void do_ex6_3(){
    vector<int> input = readfile_i("testcases/testcase6_3.txt");
    cout << "Input array: ";
    display_vector_i(input);
    int x = no_intersect2(input);
    cout << "Number of intersections = " << x << endl;
}