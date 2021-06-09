#include "header.h"
using namespace std;

int main(){
    cout << "Please write input in testcase<ex_name>.txt in directory testcases beforehand." <<endl;
    int choice = 1, subchoice = 1;
    while (choice != 0){
        cout<< endl << "Enter the exercise's number to view that exercise, or 0 to exit: ";
        cin >> choice;
        if (!cin.good()){
            cout<<"Invalid input. Please try again."<<endl;
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 1;
            continue;
        }
        if (choice == 0){
            cout<<"Exit now."<<endl;
            return 0;
        }
        switch (choice){
        case 1:
            do_ex1();
            break;
        case 2:
            cout<< "There are 2 sub-exercises, enter 1 or 2: ";
            cin >> subchoice;
            if (!cin.good()){
                cout<< "Invalid input" <<endl;
                cin.clear();
                cin.ignore(1000, '\n');
                break;
            }
            if (subchoice == 1)
                do_ex2_1();
            else do_ex2_2();
            break;
        case 3:
            cout<< "There are 3 sub-exercises, enter 1, or 2, or 3: ";
            cin >> subchoice;
            if (!cin.good()){
                cout<< "Invalid input" <<endl;
                cin.clear();
                cin.ignore(1000, '\n');
                break;
            }
            if (subchoice == 1)
                do_ex3_1();
            else if (subchoice == 2)
                do_ex3_2();
            else 
                do_ex3_3();
            break;
        case 6:
            cout<< "There are 4 sub-exercises, enter 1, or 2, or 3, or 4: ";
            cin >> subchoice;
            if (!cin.good()){
                cout<< "Invalid input" <<endl;
                cin.clear();
                cin.ignore(1000, '\n');
                break;
            }
            if (subchoice == 1)
                do_ex6_1();
            else if (subchoice == 2)
                do_ex6_2();  
            else if (subchoice == 3)
                do_ex6_3();
            else
                do_ex6_4();
            break;
        case 7:
            cout<< "There are 4 sub-exercises, enter 1, or 2, or 3, or 4: ";
            cin >> subchoice;
            if (!cin.good()){
                cout<< "Invalid input" <<endl;
                cin.clear();
                cin.ignore(1000, '\n');
                break;
            }
            if (subchoice == 1)
                do_ex7_1();
            else if (subchoice == 2)
                do_ex7_2();  
            else if (subchoice == 3)
                do_ex7_3();
            else
                do_ex7_4();
            break;
        default:
            break;
        }
    }
    
    return 0;
}