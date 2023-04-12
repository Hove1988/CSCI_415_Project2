/*******************************************************************************
*
*	MD5.h
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	� 2023
********************************************************************************/

#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <bitset>
#include <algorithm>
#include <unordered_map>

using namespace std;

#ifndef MD5_H
#define MD5_H

class MD5 {
private:

    //void append1(string msg);

    /******** Helper functions ********/
    static string toBitString(string msg); //Convert string to bit string
    static void padding(string& msg); //Append 1, 0s, and original message length
    static string decToHexa(unsigned n);
    static string binToHex(string msg);
    static string UIntToBitString(unsigned x);
    //static void decToBinary(unsigned &a);

public:

    inline MD5() {};
    static string hash(string msg);
    static string crypt(string msg);
    static unsigned lcs(unsigned, unsigned );
};

#endif