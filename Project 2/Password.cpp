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

/*
* @brief    constructor
* @pre      none
* @post     returns a password object
*/
Password::Password() {};

/*
* @brief    reads a password record from a file
* @param    string: filepath
* @pre      file is a valid file path to a passwords record file
* @post     returns T/F if the read was successful. password records are stored in the password object
*/
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
        records.push_back(UserData(tempID, tempSalt, tempDigest));
    }

    //Close the file
    inFile.close();

    //Returns true once done
    return true;
}

/*
* @brief    write existing password records to a file
* @param    string: filepath
* @pre      file is a valid filepath
* @post     returns T/F if write was successful. password records are written to the file
*/
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

/*
* @brief    checks a password against a user ID to verify the correct password is entered
* @param    string: user ID ; string: password
* @pre      uid is an existing user, pWord is a candidate password to verify
* @post     returns T/F if the password correctly hashes to the stored hash value
*/
bool Password::verifyPassword(string uid, string pWord){

    //Variables
    string salt;
    string oldHash, newHash;
    
    //loops through the size size of records
    for(int i = 0; i < records.size(); i++) {
        //Determines if the 
        if (records[i].userID == uid){
            //Sets values for hash value and the salt value
            oldHash = records[i].hash;
            salt = records[i].salt;
        }
    }
    
    //Determines if the salt value is an empty string
    if (salt != ""){

        //Increases pWord 
        pWord += salt;
        newHash = MD5::crypt(pWord);
        if (newHash == oldHash){
            cout << "Your hash is: " << newHash << endl;
            return true;
        }
    }
    return false;
}

/*
* @brief    generator to create a random character string to use as a salt value
* @pre      none
* @post     returns a randomly generated 6 character string
*/
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

/*
* @brief    add a new user to the system; uses a user name and a password to be crypted by MD5
* @param    string: uid
* @param    string: pWord
* @pre      uid must be unique; pWord must be plaintext
* @post     adds a new user to records with the uid, salt, and md5 hash
*/
bool Password::addUser(string uid, string pWord){
    
    string salt = makeSalt();
    pWord = pWord + salt;
    
    string digest = MD5::crypt(pWord);
    UserData newUser = UserData(uid, salt, digest);
    records.push_back(newUser);
    return true;
    
    return false;
}