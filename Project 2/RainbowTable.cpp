 /*******************************************************************************
*
*	RainbowTable.cpp
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	� 2023
********************************************************************************/

#include "RainbowTable.h"

RainbowTable::RainbowTable(int chainNum, int chainLength) {
    this->chainNum = chainNum;
    this->chainLen = chainLength;
}

RainbowTable::RainbowTable(int chainNum, int chainLength, string fileName) {
    this->chainNum = chainNum;
    this->chainLen = chainLength;
    readTable(fileName);
}

RainbowTable::RainbowTable(RainbowTable &oldTable) {
    this->chainNum = oldTable.chainNum;
    this->chainLen = oldTable.chainLen;
    this->table = oldTable.table;
}

void RainbowTable::createTable() {

    // Choose a hash function and input space
    vector<string> inputSpace;
    readInputSpace(inputSpace, "PotentialPasswords.txt");


    // Generate a starting set of plaintext/hashed pairs
    //const int numPairs = 10000;
    vector<values> startingPairs;
    vector<int> usedInputs;

    for (int i = 0; i < chainNum; i++) {
        bool unique = false;
        int index;

        while(!unique){
            index = rand() % inputSpace.size();
            unique = true;
            for( int j = 0; j < usedInputs.size(); j++){
                if (index == usedInputs[j]){
                    unique = false;
                    break;
                }
            }
        }

        string plaintext = inputSpace[index];
        string hash;
        MD5::hash(plaintext, hash);
        values temp;
        temp.pWord = plaintext;
        temp.hash = hash;
        startingPairs.push_back(temp);
    }

    // Generate hash chains
    for (int i = 0; i < chainNum; i++) {
        string plaintext = startingPairs[i].pWord;
        string hash = startingPairs[i].hash;
        for (int j = 0; j < maxChainLength; j++) {
            plaintext = reduce(hash, j + i); //reduce hash to get new plaintext
            MD5::hash(plaintext, hash); //hash plaintext
            values temp; //Declare temp variable
            temp.hash = hash;
            temp.pWord = plaintext;
            table.push_back(temp);
        }
    }

    // Save the rainbow table to disk
    writeTable("rainbowTable.txt");
}

string RainbowTable::reduce(string hash, int index) {
    // Split the hash value into 4-byte chunks
    int chunkSize = 8;
    int numChunks = 2;
    unsigned long chunks[2];
    char temp1[8];
    char temp2[8];
    for (int i = 0; i < 8; i++) {
        temp1[i] = hash[i];
        temp2[i] = hash[i + 8];
    }
    char reduced[8];
    for (int i = 0; i < 8; i++) {
        reduced[i] = (((unsigned short)temp1[i] ^ (unsigned short)temp2[i]) * index) % 256;
        //cout << reduced[i] << endl;
    }
    
    return string(reduced);
}


bool RainbowTable::attack(string pWord){

    string hash;
    MD5::hash(pWord, hash);

    for (int i = 0; i < chainNum; i++) {
        if (table[i].hash == hash) {
            cout << "Your password is " << table[i].pWord << endl;
            return true;
        }
    }
    return false;
}

void RainbowTable::readInputSpace(vector<string> &inputs, string fileName){
    ifstream inFile;
    inFile.open(fileName);
    string line;

    while (getline(inFile, line)){
        line.pop_back();
        inputs.push_back(line);
    }
    inFile.close();
}

void RainbowTable::readTable(string fileName) {
    ifstream inFile;
    inFile.open(fileName);
    string line;

    while (getline(inFile, line)) {
        int commaCount = 0;
        string tempPassword;
        string tempHash;

        line.pop_back();
        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') {
                commaCount++;
            }
            else if (commaCount == 0) {
                tempPassword.push_back(line[i]);
            }
            else {
                tempHash.push_back(line[i]);
            }
        }
        values value;
        value.pWord = tempPassword;
        value.hash = tempHash;
        table.push_back(value);
    }
    inFile.close();
}

void RainbowTable::writeTable(string filename){
    ofstream outFile;
    outFile.open(filename);

    for (int i = 0; i < table.size(); i++) {
        outFile << table[i].pWord << "," << table[i].hash << "\n";
    }
    outFile.close();
}
