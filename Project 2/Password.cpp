/*******************************************************************************
*
*	Password.cpp
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	� 2023
********************************************************************************/

#include "Password.h"

using namespace std;

Password::Password() {};

bool Password::readPasswordFile(string file){
    
    //Local Variables
    ifstream inFile;

    //Opening the file
    inFile.open(file);

    //Checking if file can open 
    if (!inFile.is_open()) {
        return false;
    }

    string line;
    
    //Reading file by line in the inFile stream
    while (getline(inFile, line)) {

        //Variables and Declarations
        string tempID;
        string tempSalt;
        string tempDigest;
        int commaCount = 0;

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ',') {
                commaCount++;
            }
            else if (commaCount == 0) {
                tempID.push_back(line[i]);
            }
            else if (commaCount == 1) {
                tempSalt.push_back(line[i]);
            }
            else {
                tempDigest.push_back(line[i]);
            }
        }
        records.push_back(Password::UserData(tempID, tempSalt, tempDigest));
    }

    //Close the file
    inFile.close();

    //Returns true once done
    return true;
}


bool Password::writePasswordFile(string file){
    //Variable
    ofstream outFile;

    //Opening the File
    outFile.open(file);
    
    //Determine if file can open, returns false if not
    if(!outFile.is_open()){
        return false;
    }

    //Writes to a csv file, line by line
    for(int i = 0; i < records.size(); i++){
        outFile << records[i].toString();
    }
    
    //Close the file
    outFile.close();

    //Returns true once done
    return true;
}


bool Password::verifyPassword(string uid, string pWord){

    //Variables
    string salt;
    string oldHash, newHash;
    
    for(int i = 0; i < records.size(); i++) {
        if (records[i].userID == uid){
            oldHash = records[i].hash;
            salt = records[i].salt;
        }
    }

    if (salt != ""){
        pWord += salt;
        MD5::hash(pWord, newHash);
        if (newHash == oldHash){
            return true;
        }
    }
    return false;
}


string Password::makeSalt(){
    srand(time(0));
    string salt;

    for (int i = 0; i < 6; i++) {
        char temp = ',';
        while (temp == ',') {
            temp = (char)(rand() % 256);
        }
        
        salt.push_back(temp);
    }

    return salt;
}


bool Password::addUser(string uid, string pWord){
    string digest;
    
    string salt = makeSalt();
    pWord += salt;
    
    if (MD5::hash(pWord, digest)){
        Password::UserData newUser = Password::UserData(uid, salt, digest);
        records.push_back(newUser);
        return true;
    }
    
    return false;
}