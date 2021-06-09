#include "header.h"
using namespace std;

vector<string> readfile_s (string filename){
    ifstream file;
    vector<string> ret;
    file.open(filename);
    if (!file.is_open()){
        cout << "Failed to open " <<filename<<endl;
        return ret;
    } 
 
    string line;
    while (getline(file, line))
        if(!line.empty() && line != "\n")
            ret.push_back(line);
    file.close();
    return ret;
}

vector<int> readfile_i (string filename){
    ifstream file;
    vector<int> ret;
    file.open(filename);
    if (!file.is_open()){
        cout << "Failed to open " <<filename<<endl;
        return ret;
    }
    string line;
    while (getline(file, line))
        if(!line.empty() && line != "\n")
            ret.push_back(stoi(line));
    file.close();
    return ret;
}

void display_vector_i(vector<int> A){
    for (unsigned int i = 0; i < A.size(); ++i)
        cout << A[i] << " ";
    cout << endl;
}