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

void MD5::padding(string& msg) {
	//cout << "Entering padding\n";

	unsigned long length = msg.length()/8; //liong // loing
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
	for (int i = 0; i < 64; i++) {
		//cout << "In second for loop: " << i << endl;
		if (length & (unsigned long)pow(2, i)) {		//usnigned
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

unsigned long MD5::power(int x, int y) {
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

string MD5::toBitString(string msg) {
	string bitStr = "";

	for (char& _char : msg) {
        bitStr +=bitset<8>(_char).to_string();
    }

	padding(bitStr);
	//cout << "Exiting toBitString\n";
	//cout << bitStr << endl;
	return bitStr;
}

string MD5::decToHexa(unsigned long n)
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

unsigned long MD5::lcs(unsigned long base, unsigned long shift) {

	unsigned long temp = (base << shift) | (base >> (64 - shift));
	return temp;
}

string MD5::binToASS(unsigned a) {
	string output = "";
	string bin = "";
	char temp = (char)0;
	for (int i = 0; i < 32; i++) {	//Convert unsigned int to binary string
		bin.push_back((a & (unsigned)pow(2, i)) ? '1' : '0');
	}
	
	for (int i = 0; i < 4; i++) { // for each byte (4 chars of binary string), convert to char
		for (int j = 0; j < 8; j++) {
			if (bin[(i*8)+j] == '1'){
				temp = pow(2, j) + (int)temp;
			}
		}
		output.push_back(temp);
	}
	return output;
}

void decToBinary(int &a)
{
    // array to store binary number
    int binaryNum[32];
 
    // counter for binary array
    int i = 0;
    while (a > 0) {
 
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
	
}


bool MD5::hash(string msg, string& hashCode) {
	//cout << "Entering hash!\n";
	for (int z = 0; z < 1000; z++) {
		unsigned A = 0x67452301; //j
		unsigned B = 0xefcdab89; //k
		unsigned C = 0x98badcfe; //l
		unsigned D = 0x10325476; //m

		
		string bitString = toBitString(msg);
		vector<string> chunks;

		string tempChunk;
		vector<string> fragments;
		string tempFrag;

		for (int i = 0; i < bitString.length()/512; i++) {		//Split bitstring into chunks
			tempChunk = "";
			for (int j = 0; j < 512; j++) {
				tempChunk.push_back(bitString[(i * 512) + j]);
			}
			chunks.push_back(tempChunk);
		}


		for (int i = 0; i < chunks.size(); i++) {				//Split chunks into fragments
			for (int j = 0; j < 16; j++){
				tempFrag = "";
				for (int k = 0; k < 32; k++){
					tempFrag.push_back(chunks[i][(j * 16) + k]);
				}
				fragments.push_back(tempFrag);
			}
		}

		vector<unsigned> intFrags;
		unsigned tempIntFrag = 0;

		for (int i = 0; i < fragments.size(); i++) {				//Create intfrags from fragments
			tempIntFrag = 0;
			for (int j = 0; j < 32; j++) {
				tempIntFrag += (fragments[i][j] - '0') * pow(2, j);
			}
			intFrags.push_back(tempIntFrag);
		}

		for (int i = 0; i < chunks.size(); i++) {			
			unsigned a = A; 		//Reset values to defaults
			unsigned b = B;
			unsigned c = C;
			unsigned d = D;

			for (int j = 0; j < 64; j++) {				//"Main loop"
				unsigned f, g;
				if (0 <= j && j <= 15) {
					f = F(b, c, d);
					g = j;
				}
				else if (16 <= j && j <= 31) {
					f = G(b, c, d);
					g = (5 * j + 1) % 16;
				}
				else if (32 <= j && j <= 47) {
					f = H(b, c, d);
					g = (3 * j + 5) % 16;
				}
				else {
					f = I(b, c, d);
					g = (7 * j) % 16;
				}

				f = f + a + y[j] + intFrags[(16*i) + g];
				a = d;
				d = c;
				c = b;
				b = b + lcs(f, x[j]);
			}
			A += a;
			B += b;
			C += c;
			D += d;
		}
		
		//cout << A << endl << B << endl << C << endl << D << endl;
		/*hashCode.append(A);
		hashCode.append(B);
		hashCode.append(C);
		hashCode.append(D);*/

		msg = hashCode;
		if (z != 999) {
			msg = binToASS(A) + binToASS(B) + binToASS(C) + binToASS(D);
		} 
		else {
			hashCode.append(decToHexa(A));
			hashCode.append(decToHexa(B));
			hashCode.append(decToHexa(C));
			hashCode.append(decToHexa(D));
		}
	}
	cout << hashCode;

	return true;
}

