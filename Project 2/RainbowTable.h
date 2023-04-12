/*******************************************************************************
*
*	RainbowTable.h
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	� 2023
********************************************************************************/



#pragma once
#include "MD5.h"
#include "Password.h"
#include <fstream>
#include <vector>
#include <algorithm>

#define maxChainLength 1000

using namespace std;

struct values {
    string pWord;
    string hash;
};

#ifndef RAINBOWTABLE_H
#define RAINBOWTABLE_H

class RainbowTable {

    vector <values> table;

    const char alphaNumeric[62] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                           'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                           'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
                           'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 
                           'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                           'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                           'Y', 'Z'};
    //Constructor
    MD5 md5;
    int chainNum;
    int chainLen;

    public:
    RainbowTable(int chainNum, int chainLength);
    RainbowTable(int chainNum, int chainLength, string fileName);
    RainbowTable(RainbowTable &oldTable);
    
    //Member Functions
    void createTable(Password& passData);  //Create a rainbow table
    void readTable(string fileName);    //read to file
    void writeTable(string fileName);   //write to file
    string reduce(string hash, int index);
    bool attack(string hash, Password& passData);

    void readInputSpace(vector<string> &inputs, string filename);
    void writeHashTable(string filename);

};
#endif
