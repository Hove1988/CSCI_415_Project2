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
	for (int i = 1; i < y - 1; i++) {
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

string md5::decToHexa(unsigned long n)
{
    // ans string to store hexadecimal number
    string ans = "";
    
    while (n != 0) {
        // remainder variable to store remainder
        int rem = 0;
          
        // ch variable to store each character
        char ch;
        // storing remainder in rem variable.
        rem = n % 16;
  
        // check if temp < 10
        if (rem < 10) {
            ch = rem + 48;
        }
        else {
            ch = rem + 55;
        }
          
        // updating the ans string with the character variable
        ans += ch;
        n = n / 16;
    }
      
    // reversing the ans string to get the final result
    int i = 0, j = ans.size() - 1;
    while(i <= j)
    {
      swap(ans[i], ans[j]);
      i++;
      j--;
    }
    return ans;
}

unsigned long md5::lcs(unsigned long base, unsigned long shift) {

	unsigned long temp = (base << shift) | (base >> (64 - shift));
	return temp;
}

bool md5::hash(string msg, string& hashCode) {
	//cout << "Entering hash!\n";
	unsigned long A = 0x67425301; //j
	unsigned long B = 0xefcdab89; //k
	unsigned long C = 0x98badcfe; //l
	unsigned long D = 0x10325476; //m

	string bitString = toBitString(msg);
	vector<string> chunks;

	string tempChunk;
	vector<int> fragments;
	for (int i = 0; i < bitString.length() / 512; i++){
		tempChunk = "";
		for (int j = 0; j < 512; j++) {
			tempChunk += bitString[(i * 512) + j];
		}
		chunks.push_back(tempChunk);

		for (int j = 0; j < 15; j++) {
			int tempFrag = 0;
			for (int k = 0; k < 32; k++) {
				int temp = tempChunk[(k + (32 * j))] - '0';
				tempFrag += temp * pow(2, k);
			}
			fragments.push_back(tempFrag);
		}

		unsigned long a = A;
		unsigned long b = B;
		unsigned long c = C;
		unsigned long d = D;
		for (int j = 0; j < 64; j++) {
			unsigned long f, g;
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
			b = b + lcs(f, x[i]);
		}
		A += a;
		B += b;
		C += c;
		D += d;
	}
	//cout << A << endl << B << endl << C << endl << D << endl;
	hashCode.append(decToHexa(A));
	hashCode.append(decToHexa(B));
	hashCode.append(decToHexa(C));
	hashCode.append(decToHexa(D));

	//cout << "Appended hash code\n";

	return true;
}

