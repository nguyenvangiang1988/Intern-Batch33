#include "header.h"
using namespace std;

int no_block(vector<int> &H){
    unsigned int i, len = H.size();
    if (len == 0 || len ==1)
        return len;
    int count = 0; 
    vector<int> stack;
    for (i = 0; i < len; ++i){
        if (stack.empty()){
            stack.push_back(H[i]);
            ++count;
        }
        else if (H[i] > stack[stack.size()-1]){
            stack.push_back(H[i]);
            ++count;
        }
        else if (H[i] < stack[stack.size()-1]){
            while (H[i] < stack[stack.size()-1])
                stack.pop_back();
            if (stack.empty() || H[i] > stack[stack.size()-1]){
                stack.push_back(H[i]);
                ++count;
            }
        }
    }
    return count;
}

void do_ex7_4(){
    vector<int> input = readfile_i("testcases/testcase7_4.txt");
    cout << "Input: ";
    display_vector_i(input);
    int x = no_block(input);
    cout << "Minimum number of blocks = " << x << endl;
}