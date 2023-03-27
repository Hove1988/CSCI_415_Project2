/*******************************************************************************
*
*	MD5.cpp
*
*	Authors: Jake Haapoja, Joseph Hoversten, Nathan O'Connor, Zach Sawicki
*
*	© 2023
********************************************************************************/

#include "MD5.h"
#include <string>

void md5::padding(string& msg) {
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

	length %= pow(2, 64);

	for (int i = 64; i >= 1; i--) {
		if (length % pow(2, i)) {
			lengthStr.push_back('1');
		}
		else {
			lengthStr.push_back('0');
		}
	}
	msg.append(lengthStr);
}

string toBitString(string msg) {
	char temp = '';
	string bitStr = "";

	for (temp : msg) {
		for (int i = 8; i >= 1; i--) {
			if ((int)temp % pow(2, i)) {
				bitStr.push_back('1');
			}
			else {
				bitStr.push_back('0');
			}
		}
	}

	padding(bitStr);

	return bitStr;
}




bool md5::hash(string msg, string& hashCode) {

	int A = 0x67425301; //j
	int B = 0xefcdab89; //k
	int C = 0x98badcfe; //l
	int D = 0x10325476; //m

    string bitString = toBitString(msg);
    vector<string> chunks;

	string tempChunk;
	vector<int> fragments;

	for (int i = 0; i < bitString.length() / 512; i++) {
		tempChunk = "";
		for (int j = 0; j < 512; j++) {
			tempChunk += bitString[(i * 512) + j];
		}
		chunks.push_back(tempChunk);

        for(int j = 0; j < 16; j++){
            int tempFrag = 0;
            for(int k = 0; k < 32; k++){
                tempFrag += atoi(tempChunk[(k + (32 * j))]) * pow(2, k);
            }
            fragments.push_back(tempFrag);
        }

		int a = A;
		int b = B;
		int c = C;
		int d = D;
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
				g = (y * i) % 16;
			}
			f = f + a + y[i] + fragments[g];
			a = d;
			d = c;
			c = b;
			b + LCS(f, x[i]);
		}
		A += a;
		B += b;
		C += c;
        D += d;
	}
	hashCode.append(A);
	hashCode.append(B);
	hashCode.append(C);
	hashCode.append(D);

	return true;
}

