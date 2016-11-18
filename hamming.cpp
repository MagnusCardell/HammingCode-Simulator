/** Hamming code generator. Modifies a 16 bit input value to 
	implement parity checkbits. The resulting output is 21 bits
	long.							-Magnus Cardell 10/13/2016

*/
#include <iostream>
using namespace std;

int INPUTLENGTH = 16;
int OUTPUTLENGTH = 21;

/** Function: Paritygenerator
	Purpose: calculate even or odd bits
	Return: 1 or 0.
*/
int paritygenerator(long int paritycode) {
	int p = 0; 									//sum of 1's in paritycode
	for (int i = 0; i< INPUTLENGTH; i++) {
		if (paritycode & 1) p++;
		paritycode >>= 1;
	}
	return p%2;
}

/**	Function Hamming
	Purpose: Calculate representing hamming code
	Input: 16 bit number
	Output: 21 bit number
*/
long int hamming(long int n) {

	//create parity masks
	long int maskP1 = 0xDAB5; //1101101010110101;
	long int maskP2 = 0xB66C; //1011011001101100;
	long int maskP3 = 0x71E3; //0111000111100011;
	long int maskP4 = 0xFE0; //0000111111100000;
	long int maskP5 = 0x1F; //0000000000011111;
	long int output = 0;	//imaginary 21 bits

	long int outputmask = 0x8000; //1000000000000000
	// assigning parity values and filling a new
	// result bit by bit
	for (int i=1; i<= OUTPUTLENGTH; i++) {
		if (i == 1) {
			output |= paritygenerator(n & maskP1); //OR
			output<<=1;
		}
		else if (i == 2) {
			output |= paritygenerator(n & maskP2); //OR
			output<<=1;
		}
		else if (i == 4) {
			output |= paritygenerator(n & maskP3); //OR
			output<<=1;
		}
		else if (i == 8) {
			output |= paritygenerator(n & maskP4); //OR
			output<<=1;
		}
		else if (i == 16) {
			output |= paritygenerator(n & maskP5); //OR
			output<<=1;
		}
		else {
			output |= paritygenerator(n & outputmask); 				//OR
			output<<=1;
			outputmask>>=1;
		} 
	}
	output >>=1;
	return output;
}

int main() {
	//input
	cout<< "Enter a four digit hexadecimal value: "<<endl;
	cin.setf(ios::hex, ios::basefield);
	long int n;
	cin >> n;
	//control
	if (!n) {
		cout<< "Expecting a computable value... try again"<<endl;
		return 0;
	}
	//output
	cout.setf(ios::hex, ios::basefield);
	cout<<"The hamming code is "<< hamming(n)<<endl;

}