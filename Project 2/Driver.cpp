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
    md.hash("password", digest);

    cout << "Hash code: " << digest << endl;
}