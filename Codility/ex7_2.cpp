#include "header.h"
using namespace std;

int remaining_fish(vector<int> &A, vector<int> &B){
    if(A.empty() || B.empty())
        return 0;
    vector<int> stack;
    int total = 0;
    for(unsigned int i = 0; i < A.size(); ++i){
        if(B[i] == 1){
            stack.push_back(A[i]);
        }
        else{
            while(!stack.empty()){
                if (stack[stack.size()-1] < A[i])
                    stack.pop_back();
                else break;
            }
            if (stack.empty())
                ++total;
        }
    }
    return total + stack.size();
}

void do_ex7_2(){
    vector<string> input = readfile_s("testcases/testcase7_2.txt");
    if (input.size() < 2){
        cout << "Missing arrays in input." << endl;
        return;
    }
    vector<int> A, B;
    string word;
    for (unsigned int i = 0; i < input[0].size(); ++i){
        if (input[0][i] != ' ')
            word += input[0][i];
        else{
            A.push_back(stoi(word));
            word.clear();
        }
    }
    word.clear();
    for (unsigned int i = 0; i < input[1].size(); ++i){
        if (input[1][i] != ' ')
            word += input[1][i];
        else {
            B.push_back(stoi(word));
            word.clear();
        }
    }
    cout << "Input:" << endl 
        << "A = ";
    display_vector_i(A);
    cout << "B = ";
    display_vector_i(B);
    if (A.size() != B.size()){
        cout << "Number of elements in A and B are not equal." << endl;
        return;
    }
    int x = remaining_fish(A, B);
    cout << "Number of remaining fish = " << x << endl; 
}