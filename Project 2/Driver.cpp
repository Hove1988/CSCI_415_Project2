/*******************************************************************************
*
*	Driver.cpp
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	� 2023
********************************************************************************/
#include "MD5.h"

using namespace std;

int main(){
    cout << "Creating md5 object\n";
    md5 md;

    string digest;
    cout << "Calling md5 hash\n";
    md.hash("P", digest);

    string digest2;
    cout << "Calling md5 hash 2\n";
    md.hash("Password", digest2);

    cout << "Hash code: " << digest << endl;
    cout << "Hash code: " << digest2 << endl;
}