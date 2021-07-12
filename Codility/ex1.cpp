#include "header.h"
using namespace std;

string to_binary(int N){
    string ret;
    while (N > 0){
        ret = to_string(N % 2) + ret;
        N /= 2;
    }
    return ret;
}

int longest_binary_gap(int N){
    string bN = to_binary(N);
    int max = 0, count = 0;
    bool check = false;
    for (int i = 0; i < bN.size(); ++i){
        if (bN[i] == '1'){
            if (!check)
                check = true;
            else{
                if (count > max) 
                    max = count;
                count = 0;
            }
        }
        else{
            if (check)
                ++count;
        }
    }
    return max;
}

void do_ex1(){
    vector<int> input = readfile_i("testcases/testcase1.txt");
    for (unsigned int i = 0; i < input.size(); ++i){
        cout << i+1 << ": "<<input[i]<<endl
            << "Binary representation: " << to_binary(input[i]) << endl
            << "Longest binary gap: " << longest_binary_gap(input[i]) <<endl;
    }
}

