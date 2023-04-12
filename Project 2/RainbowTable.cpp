 /*******************************************************************************
*
*	RainbowTable.cpp
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	� 2023
********************************************************************************/

#include "RainbowTable.h"

/**
 * @brief   Constructor that initializes the chainNum and chainLength
 * @param   chainNum the number of chains in the rainbow table
 * @param   chainLength the length of each chain in the rainbow table
 */
RainbowTable::RainbowTable(int chainNum, int chainLength) {
    this->chainNum = chainNum;
    this->chainLen = chainLength;
}

/*
* @brief    Creates a new rainbow table with the values stored in the Password class
* @param    passData the storage place for all user data
* @pre      a filled out password class with user data
*/
void RainbowTable::createTable(Password& passData) {
    // Choose a hash function and input space
    vector<string> inputSpace;
    readInputSpace(inputSpace, "PotentialPasswords.txt");

    // Generate a starting set of plaintext/hashed pairs
    vector<values> startingPairs;
    vector<UserData> userData = passData.getUserData();
    for (int i = 0; i < inputSpace.size(); i++) {
        int index;
        bool unique = true;

        if (unique) {
            for (int j = 0; j < userData.size(); j++) {
                string hash = MD5::crypt(inputSpace[i] + userData[j].salt);
                values temp;
                temp.pWord = inputSpace[i];
                temp.hash = hash;
                startingPairs.push_back(temp);
            }
        }
    }

    // Generate hash chains
    for (int i = 0; i < startingPairs.size(); i++) {
        string plaintext = startingPairs[i].pWord;
        string hash = startingPairs[i].hash;
        values temp; //Declare temp variable
        temp.hash = hash;
        temp.pWord = plaintext;
        table.push_back(temp);
        for (int j = 0; j < chainLen; j++) {
            plaintext = reduce(hash, (j + i)); //reduce hash to get new plaintext
            hash = MD5::crypt(plaintext); //hash plaintext
            // Check if hash is already in the table

            bool found = false;
            for (int k = 0; k < table.size(); k++) {
                if (hash == table[k].hash) {
                    found = true;
                    break;
                }
            }

            if (found) {
                continue;
            }

            temp.hash = hash;
            temp.pWord = plaintext;
            table.push_back(temp);
        }
    }

    // Save the rainbow table to disk
    writeTable("RainbowTable.txt");

}

/*
* @brief    The rainbow table reduction function
* @param    hash the hash value that is going to be reduced
* @param    the index to reduce the hash by
* @post     returns a reduced hash as the new plaintext
*/
string RainbowTable::reduce(string hash, int index) {
    // Split the hash value into 4-byte chunks
    int chunkSize = 8;
    int numChunks = 2;
    unsigned long chunks[2];
    char temp1[9] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
    char temp2[9] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };

    for (int i = 0; i < 8; i++) {
        temp1[i] = hash[i];
        temp2[i] = hash[i + 8];
    }
    char reduced[9] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
    for (int i = 0; i < 8; i++) {
        reduced[i] = alphaNumeric[((temp1[i] ^ temp2[i])* index) % 62];
    }
    
    return string(reduced);
}

/*
* @brief    The attack function to crack a password
* @param    hash the hash to try and find in the rainbow table
* @param    passData is the class containing all the user information
* @pre      Requires an already build rainbow table
* @post     returns true if it was found or false if it wasn't
*/
bool RainbowTable::attack(string hash, Password& passData){

    for (int i = 0; i < table.size(); i++) {
        if (table[i].hash == hash) {
            cout << "Your password is " << table[i].pWord << endl << endl;
            return true;
        }
    }
    cout << endl << "Password is not in rainbow table." << endl;
    cout << "Attempting crack with different password variations..." << endl;

    vector<UserData> userData = passData.getUserData();
    for (int i = 0; i < table.size(); i++) {
        string temp = table[i].pWord;
        reverse(temp.begin(), temp.end());
        string original = temp;
        for (int k = 0; k < userData.size(); k++) {
            temp += userData[k].salt;
            string tempHash = MD5::crypt(temp);
            if (tempHash == hash) {
                cout << "Your password is " << original << endl << endl;
            }
        }
    }
    return false;
}

/*
* @brief    Reads the file with the potential passwords
* @param    inputs a vector to store all of the potential passwords into
* @param    fileName the filename containing all the potential passwords
      requires a file exist with potential passwords
\
    //*/
void RainbowTable::readInputSpace(vector<string> &inputs, string fileName){
    ifstream inFile;
    inFile.open(fileName);
    string line;

    while (getline(inFile, line)){
        inputs.push_back(line);
    }
    inFile.close();
}

/*
* @brief    Reads a rainbow table from a file
* @param    fileName the filename of the rainbow table 
* @pre      requires a file containing a rainbow table
*/
void RainbowTable::readTable(string fileName) {
    ifstream inFile;
    inFile.open(fileName);
    string line;

    while (getline(inFile, line)) {
        int commaCount = 0;
        string tempPassword;
        string tempHash;

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

/*
* @brief    Writes an existing rainbow table to a file
* @param    filename the name of the file to write the rainbow table to
* @pre      requires that there are entries in the rainbow to be written
*/
void RainbowTable::writeTable(string filename){
    ofstream outFile;
    outFile.open(filename);

    for (int i = 0; i < table.size(); i++) {
        outFile << table[i].pWord << "," << table[i].hash << "\n";
    }
    outFile.close();
}
