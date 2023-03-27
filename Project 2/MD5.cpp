/*******************************************************************************
*
*	MD5.cpp
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	� 2023
********************************************************************************/

#include "MD5.h"
#include <string>
#include <iostream>

using namespace std;



void md5::padding(string& msg) {
	//cout << "Entering padding\n";
	
	unsigned long length = msg.length(); //liong // loing
	string lengthStr = "";
	bool first = true;
	
	while (msg.length() % 512 != 448) {
		if (first) {
			msg.push_back('1');
			first = false;
		}
		else {
			msg.push_back('0');
		}
	}

	//cout << "Entering second for loop\n";
	for (int i = 64; i >= 1; i--) {
		//cout << "In second for loop: " << i << endl;
		if (length & power(2, i)) {
			//cout << "1\n";
			lengthStr.push_back('1');
		}
		else {
			//cout << "0\n";
			lengthStr.push_back('0');
		}
	}
	//cout << "Appending\n";
	msg.append(lengthStr);
	//cout << "Exiting padding\n";
}

unsigned long md5::power(int x, int y) {
	//cout << "x: " << x << "y: " << y << endl;
	unsigned long long z = x;
	//cout << "z original: "<< z << endl;
	for (int i = 1; i < y-1; i++) {
		//cout << i << ' ';
		z *= x;
		//cout << i << "th z: " << z << endl;
	}
	return z;
}

string md5::toBitString(string msg) {
	string bitStr = "";

	for (char temp : msg) {
		for (int i = 8; i >= 1; i--) {
			if ((int)temp % (unsigned long)std::pow(2, i)) {
				bitStr.push_back('1');
			}
			else {
				bitStr.push_back('0');
			}
		}
	}

	padding(bitStr);
	//cout << "Exiting toBitString\n";
	return bitStr;
}

bool md5::hash(string msg, string& hashCode) {
	cout << "Entering hash!\n";
	int A = 0x67425301; //j
	int B = 0xefcdab89; //k
	int C = 0x98badcfe; //l
	int D = 0x10325476; //m

    string bitString = toBitString(msg);
    vector<string> chunks;

	string tempChunk;
	vector<int> fragments;

	cout << "Entering big for loop\n";
	cout << bitString << endl;
	for (int i = 0; i < bitString.length() / 512; i++) {
		tempChunk = "";
		for (int j = 0; j < 512; j++) {
			tempChunk += bitString[(i * 512) + j];
		}
		chunks.push_back(tempChunk);

        for(int j = 0; j < 15; j++){
            int tempFrag = 0;
            for(int k = 0; k < 32; k++){
                tempFrag += atoi(&tempChunk[(k + (32 * j))]) * pow(2, k);
            }
            fragments.push_back(tempFrag);
        }

		unsigned long a = A;
		unsigned long b = B;
		unsigned long c = C;
		unsigned long d = D;
		cout << "Entering main Loop!\n";
		for (int j = 0; j < 64; j++) {	//"Main Loop"
			int f, g;
			if (0 <= j && j <= 15) {
				f = F(b, c, d);
				g = i;
			}
			else if (16 <= j && j <= 31) {
				f = G(b, c, d);
				g = (5 * i + 1) % 16;
			}
			else if (32 <= j && j <= 47) {
				f = H(b, c, d);
				g = (3 * i + 5) % 16;
			}
			else {
				f = I(b, c, d);
				g = 7 * i % 16;
			}
			f = f + a + y[i] + fragments[g];
			a = d;
			d = c;
			c = b;
			b = b + LCS(f, x[i]);
		}
		A += a;
		B += b;
		C += c;
        D += d;
	}
	cout << A << endl << B << endl << C << endl << D << endl;
	hashCode.append(to_string(A));
	hashCode.append(to_string(B));
	hashCode.append(to_string(C));
	hashCode.append(to_string(D));

	cout << "Appended hash code\n";

	return true;
}

