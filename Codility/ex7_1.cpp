#include "header.h"
using namespace std;

int is_properly_nested(string &S){
    if(S.empty())
        return 1;
    vector<char> stack;
    for(unsigned int i = 0; i < S.size(); ++i){
        if (S[i] == '(' || S[i] == '[' || S[i] == '{')
            stack.push_back(S[i]);
        else if (S[i] == ')' || S[i] == ']' || S[i] == '}')
            if (stack.empty())
                return 0;
            if ( 
                    (S[i] == ')' && stack[stack.size()-1] == '(') ||
                    (S[i] == ']' && stack[stack.size()-1] == '[') ||
                    (S[i] == '}' && stack[stack.size()-1] == '{')
                )
                stack.pop_back();
    }
    if (stack.empty())
        return 1;
    return 0;
}

void do_ex7_1(){
    vector<string> input = readfile_s("testcases/testcase7_1.txt");
    for (unsigned int i = 0; i < input.size(); ++i){
        cout << i + 1 << "." << endl << input[i] << endl;
        if(is_properly_nested(input[i]))
            cout << "This string is properly nested." <<endl;
        else cout << "This string is not properly nested."<<endl;
    }
}
