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

using namespace std;

bool createNewUser(Password&);
bool verifyPass(Password&);
bool rainbowAttack(Password&);

int main() {
    
    Password passData;
    //RainbowTable rainTab = new RainbowTable()
    passData.readPasswordFile("passwords.csv");

    int input = 0;
    while (input != 5) {
        string tempIn;

        cout << "1. Add User " << endl;
        cout << "2. Verify Password " << endl;
        cout << "3. Create Rainbow Table" << endl;
        cout << "4. Perform Rainbow Attack " << endl;
        cout << "5. Exit" << endl;
        cout << "Input: ";

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
                //createTable();
                break;
            case 4:
                rainbowAttack(passData);
                break;
            default:
                passData.writePasswordFile("passwords.csv");
                return 0;
        };
    }
}

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

bool verifyPass(Password& pData) {
    string username, password;
    cout << endl << "Enter Username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    if (pData.verifyPassword(username, password)) {
        cout << "Successfully verified password" << endl << endl;
        return true;
    }
    else {
        cout << "Failed to verify password" << endl << endl;
        return false;
    }

}

bool rainbowAttack(Password& pData){
    
    return true;
}