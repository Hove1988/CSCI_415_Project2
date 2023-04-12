/*******************************************************************************
*
*	Password.h
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	� 2023
********************************************************************************/

#include <string>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include "MD5.h"

#ifndef PASSWORD_H
#define PASSWORD_H

using namespace std;

struct UserData {
	UserData(string uid, string slt, string md) {
		userID = uid;
		salt = slt;
		hash = md;
	};

	string toString() {
		return userID + ',' + salt + ',' + hash + '\n';
	};

	string userID;
	string salt;
	string hash;
};

class Password {
public:

    //Construnctor
	Password();

    //Member Functions
	bool readPasswordFile(string);
	bool writePasswordFile(string);
	bool verifyPassword(string, string);
	bool addUser(string, string);

	vector<UserData> getUserData() { return records; }
    

private:

	vector<UserData> records;
	string makeSalt();
};

#endif