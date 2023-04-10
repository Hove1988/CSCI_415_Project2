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
bool rainbowAttack(Password&, RainbowTable&);
RainbowTable createTable(Password&);

int main() {
    
    Password passData;
    RainbowTable rt(10, 10);
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
                rt = createTable(passData);
                break;
            case 4:
                rainbowAttack(passData, rt);
                break;
            default:
                passData.writePasswordFile("passwords.csv");
                return 0;
        };
    }
}

RainbowTable createTable(Password& pData) {

    RainbowTable rt(10, 10);

    rt.createTable();
    return rt;
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

bool rainbowAttack(Password& pData, RainbowTable& rt){
    if (rt.attack("w8adwY78")) {
        cout << "Password Found\n";
        return true;
    }

    cout << "Password Not Found\n";
    return false;
}