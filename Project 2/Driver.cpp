/*******************************************************************************
*
*	Driver.cpp
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	� 2023
********************************************************************************/
#include "MD5.h"
#include "Password.h"
#include "RainbowTable.h"
#include <chrono>
#include <time.h>

using namespace std;

//Functions Prototypes
bool createNewUser(Password&);
bool verifyPass(Password&);
bool rainbowAttack(Password&, RainbowTable&);

int main() {
    
    //Variables and Declarations
    Password passData;
    int input = 0;

    //Calling Constructor
    RainbowTable rt(1000, 10);

    //Opening passwords.csv file 
    passData.readPasswordFile("Passwords.csv");

    
    while (input != 6) {

        //Variables
        string tempIn;

        //Displays Menu for program
        cout << "1. Add User " << endl;
        cout << "2. Verify Password " << endl;
        cout << "3. Create Rainbow Table" << endl;
        cout << "4. Load Rainbow Table" << endl;
        cout << "5. Perform Rainbow Attack " << endl;
        cout << "6. Exit" << endl;
        cout << "Input: ";

        //Gets input from user
        cin >> tempIn;
        input = stoi(tempIn);

        switch (input) {
            case 1:
                createNewUser(passData);
                break;
            case 2:
                verifyPass(passData);
                break;
            case 3:
                cout << endl << "Creating Rainbow Table..." << endl;
                rt.createTable(passData);
                cout << endl;
                break;
            case 4:
                rt.readTable("RainbowTable.txt");
                cout << endl;
                break;
            case 5:
                rainbowAttack(passData, rt);
                break;
            default:
                cout << endl;
                break;
        };
    }
   passData.writePasswordFile("Passwords.csv");
    return 0;
}

/*
* @brief    User enter's username and password, displays result and checks for errors
* @param    Password: pData passed by reference
* @pre      none
* @post     Creates and adds a user to pData and returns false, otherwise error and returns false
*/
bool createNewUser(Password& pData){
    string username, password;
    cout << endl << "Enter Username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (pData.addUser(username, password)){
        cout << endl << "Your user ID is: " << username << endl << endl;
        return true;
    }
    cout << endl << "There was an error creating your user ID. returning to menu." << endl << endl;
    return false;
}

/*
* @brief    verify that a username and associated password exist; print the accompaning MD5 hash
* @param    Password: pData passed by reference
* @pre      none
* @post     returns true if the hash is found in the rainbow table, otherwise false
*/
bool verifyPass(Password& pData) {
    //Variables
    string username, password;

    //Displays prompt and input of username and password
    cout << endl << "Enter Username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    //Determines if the password has been successfully verified or not
    if (pData.verifyPassword(username, password)) {
        cout << "Successfully verified password" << endl << endl;
        return true;
    }
    else {
        cout << "Failed to verify password" << endl << endl;
        return false;
    }
}

/*
* @brief    perform a rainbow attack with a precomputed rainbow table; asks user for hash to crack
* @param    Password Pdata
* @param    RainbowTable rt
* @pre      rainbow table must be precomputed
* @post     returns true if the attack hits
*/
bool rainbowAttack(Password& pData, RainbowTable& rt){
    //Variable
    string hash;

    //Display prompt and has user input hash
    cout << endl << "Enter Hash to crack: ";
    cin >> hash;

    auto start = std::chrono::steady_clock::now();
    bool returnVal = rt.attack(hash, pData);
    auto end = std::chrono::steady_clock::now();

    double crackTime = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    cout << endl << "Time to crack hash: " << crackTime << " ms" << endl;
    if (returnVal) {
        return true;
    }

    //Displays prompt if not password not found
    cout << "Password Not Found\n\n";
    return false;
}