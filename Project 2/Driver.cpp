/*******************************************************************************
*
*	Driver.cpp
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	© 2023
********************************************************************************/
#include "MD5.h"

using namespace std;

int main(){

    md5 md;

    string digest;
    md.hash("password", digest);

    cout << digest << endl;
}