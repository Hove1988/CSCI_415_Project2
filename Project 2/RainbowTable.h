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

    //Constructor
    MD5 md5;
    int chainNum;
    int chainLen;

    public:
    RainbowTable(int chainNum, int chainLength);
    RainbowTable(int chainNum, int chainLength, string fileName);
    RainbowTable(RainbowTable &oldTable);
    
    //Member Functions
    void createTable();  //Create a rainbow table
    void readTable(string fileName);    //read to file
    void writeTable(string fileName);   //write to file
    string reduce(string hash, int index);
    bool attack(string hash);

    void readInputSpace(vector<string> &inputs, string filename);
    void writeHashTable(string filename);

};
#endif
