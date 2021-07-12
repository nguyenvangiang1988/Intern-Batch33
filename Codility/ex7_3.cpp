#include "header.h"
using namespace std;
//Reuse from ex7_1
void do_ex7_3(){
    vector<string> input = readfile_s("testcases/testcase7_3.txt");
    for (unsigned int i = 0; i < input.size(); ++i){
        cout << i + 1 << "." << endl << input[i] << endl;
        if(is_properly_nested(input[i]))
            cout << "This string is properly nested." <<endl;
        else cout << "This string is not properly nested."<<endl;
    }
}
