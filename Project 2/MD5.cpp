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

/*
* @brief	padding function to add padding bits to a message bitstring
* @param	string: message
* @pre		msg is a string holding a series of 1s and 0s
* @post		msg has been padded as per the MD5 hashing algorithm
*/
void MD5::padding(string& msg) {
    string lenStr = bitset<64>(msg.length()/8).to_string(); //Length to be appended at end

    msg.push_back('1');                     //append first 1 bit

    while (msg.length() % 512 != 448) {     //append until length % 512 == 448
		msg.push_back('0');
	}

    msg.append(lenStr);
}

/*
* @brief	conversion function to change ascii text into a bitstring
* @param	string: message
* @pre		msg is a string of ascii characters that make up the message to hash
* @post		returns a string of bits as 1 and 0 characters
*/
string MD5::toBitString(string msg) { //return string of bits in littleEndian
    string bitStr;

  	for (int i = 0; i < msg.size(); ++i) {
      bitStr.append(bitset<8>(msg[i]).to_string());     //gather the 8 bits of each char of string
  	}

    reverse(bitStr.begin(), bitStr.end());     //to make little endian
	return bitStr;
}

/*
* @brief	conversion function between decimal and hexadecimal values
* @param	unsigned: number
* @pre		n is an unsigned integer 4 bytes in size
* @post		returns a string of ascii characters that correspond to the hex representation of n
*/
string MD5::decToHexa(unsigned n){

    // ans string to store hexadecimal number
    string ans = "";
    
    while (n != 0) {
        int rem = 0;
        char ch;
        rem = n % 16;
  
        if (rem < 10) {
            ch = rem + 48;
        }
        else {
            ch = rem + 55;
        }
          
        ans += ch;
        n = n / 16;
    }
      
    int i = 0, j = ans.size() - 1;
    while(i <= j)
    {
      swap(ans[i], ans[j]);
      i++;
      j--;
    }
    return ans;
}

/*
* @brief	left circular shift
* @param	unsigned: number to shift ; unsigned: shift count
* @pre		base and shift are unsigned integers 4 bytes in size
* @post		returns an unsigned integer that corresponds to the value of a left circular shift operation on the base
*/
unsigned MD5::lcs(unsigned base, unsigned shift) {

	return (base << shift) | (base >> (32 - shift));
}

/*
* @brief	the MD5 hash algorithm
* @param	string: msg
* @pre		string must be a bit string
* @post		returns a bit string
*/
string MD5::hash(string msg) {	//msg as a string of bits
	//Declare MD5 buffers
	unsigned a0 = 0x67452301;
	unsigned b0 = 0xefcdab89;
	unsigned c0 = 0x98badcfe;
	unsigned d0 = 0x10325476;

	const int x[64] = { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12,
	17, 22, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14,
	20, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };

	const unsigned k[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
	};

	padding(msg);	//pad bit string

	//Variables and Declarations
	vector<string> chunks;
	string tempChunk;
	vector<string> fragments;
	string tempFrag;
	vector<unsigned> intFrags;
	unsigned tempIntFrag = 0;

	//create 512-bit chunks
	for (int i = 0; i < msg.length()/512; i++) {
		tempChunk = "";
		for (int j = 0; j < 512; j++) {
			tempChunk.push_back(msg[(i * 512) + j]);
		}
		chunks.push_back(tempChunk);
		//cout << chunks[i] << endl;		//to test
	}

	//break each 512-bit chunk into 16 32-bit words
	for (int i = 0; i < chunks.size(); i++) {
		for (int j = 0; j < 16; j++) {
			tempFrag = "";
			for (int k = 0; k < 32; k++) {
				tempFrag.push_back(chunks[i][(j * 16) + k]);
			}
			fragments.push_back(tempFrag);
		} 
	}

	//convert bitstring fragments into integers
	for (int i = 0; i < fragments.size(); i++) {				
		tempIntFrag = 0;
		for (int j = 0; j < 32; j++) {
			tempIntFrag += (fragments[i][j] - '0') * pow(2, j);
		}
		intFrags.push_back(tempIntFrag);
	}
	
	//for each 512-bit chunk
	for (int i = 0; i < chunks.size(); i++) {
		unsigned A = a0;
		unsigned B = b0;
		unsigned C = c0;
		unsigned D = d0;

		for (int j = 0; j < 64; j++) {				//"Main loop"
			unsigned f, g;
			if (0 <= j && j <= 15) {
				f = ((B & C) | (~B & D));
				g = j;
			}
			else if (16 <= j && j <= 31) {
				f = ((B & D) | (C & ~D));
				g = (5 * j + 1) % 16;
			}
			else if (32 <= j && j <= 47) {
				f = B ^ C ^ D;
				g = (3 * j + 5) % 16;
			}
			else {
				f = C ^ (B | ~D);
				g = (7 * j) % 16;
			}

			f = f + A + k[j] + intFrags[(16 * i) + g];
			A = D;
			D = C;
			C = B;
			B = B + lcs(f, x[j]);
		}
		a0 += A;
		b0 += B;
		c0 += C;
		d0 += D;
	}
	return ((UIntToBitString(a0) + UIntToBitString(b0) + UIntToBitString(c0) + UIntToBitString(d0)));	//return the bitString of the hash result
}

/*
* @brief	gets the bit representation of an unsigned int
* @param	unsigned int: x
* @pre		parameter is of type unsigned int
* @post		returns a string of the bit representation of an unsigned int
*/
string MD5::UIntToBitString(unsigned x) {
	return (bitset<32>(x).to_string());
}

/*
* @brief	convert a binary string into hex
* @param	string: binary
* @pre		paramenter must be a binary string
* @post		returns a hexadecimal representation of the given argument
*/
string MD5::binToHex(string binary) {
    unordered_map<string, char> hex_dict = {
        {"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
        {"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
        {"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
        {"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}
    };
    string hexadecimal;
    for (int i = 0; i < binary.length(); i += 4) {
        string group = binary.substr(i, 4);
        hexadecimal += hex_dict[group];
    }
    return hexadecimal;
}

/*
* @brief	runs the hash algorithm 1000 times
* @param	string: msg
* @pre		parameter is a plaintext string
* @post		returns a 16 digit hexadecimal
*/
string MD5::crypt(string msg) {
	string hashString = toBitString(msg);
	hashString = hash(hashString);
	return binToHex(hashString);
}