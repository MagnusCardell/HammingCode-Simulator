/** Hamming code generator and checker. Can  generate hamming codes and check if inputed hamming codes
	are OK.
											-Magnus Cardell 10/13/2016

*/
#include <iostream>
	using namespace std;

	int INPUTLENGTH = 16;
	int OUTPUTLENGTH = 21;

/** Function: Paritygenerator
	Purpose: calculate even or odd bits by incrementing 'p' each time
	a '1' is encountered
	Return: 1 or 0.
*/
	int paritygenerator(long int paritycode) {
	int p = 0; 									//sum of 1's in paritycode
	for (int i = 0; i< INPUTLENGTH; i++) {		//16 bits
		if (paritycode & 1) p++;
		paritycode >>= 1;
	}
	return p%2;
}
/** Function: Paritygenerator2
	Purpose: calculate even or odd bits
	Return: 1 or 0.
*/
	int paritygenerator2(long int paritycode) {
	int p = 0; 									//sum of 1's in paritycode
	for (int i = 0; i< OUTPUTLENGTH; i++) { 	//DIFFERENCE, I have to use 21 bits
		if (paritycode & 1) p++;
		paritycode >>= 1;
	}
	return p%2;
}

/**	Function Hammingencode
	Purpose: Calculate representing hamming code
	Input: 16 bit number
	Output: 21 bit number
*/
	long int hammingencode(long int n) {

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
			output |= paritygenerator(n & outputmask); 	//OR
			output<<=1;
			outputmask>>=1;
		} 
	}
	output >>=1;
	return output;
}
//Helper function to return num to the power of pow
int pow(int num, int pow) {
	int result = 1;
	for (int i = 0; i < pow; i++) {
		result *= num;
	}
	return result;
}
long int hammingcodecheck(long int n) {
	//create parity checks
	long int checkP1 = 0x100000; //100000000000000000000;
	long int checkP2 = 0x80000; //010000000000000000000;
	long int checkP3 = 0x20000; //000100000000000000000;
	long int checkP4 = 0x2000; //000000010000000000000;
	long int checkP5 = 0x20; //000000000000000100000;
	long int output = 0;	//imaginary 16 bits
	long int outputmask = 0x100000; //10000000000000000000

	int p1, p2, p3, p4, p5; //storing parity values
	// assigning parity values
	// extracting original value bit by bit
	for (int i=1; i<= OUTPUTLENGTH; i++) {
		if (i == 1) {
			p1 = n & checkP1;
		}
		else if (i == 2) {
			p2 = n & checkP2;
		}
		else if (i == 4) {
			p3 = n & checkP3;
		}
		else if (i == 8) {
			p4 = n & checkP4;
		}
		else if (i == 16) {
			p5 = n & checkP5;
		}
		else {
			output |= paritygenerator2(n & outputmask); 	//AND
			output<<=1;
		} 
		outputmask>>=1;
	}
	output >>=1;

	long int testsubject = hammingencode(output);
	if (testsubject == n) { //IF equal THEN no error
		cout.setf(ios::hex, ios::basefield);
		cout<<"This input looks correct"<<endl;
		cout<<"Your pure data: "<<output<< " matches its hammingcode: "<<n<<endl;
		return 0;
	}
	cout<<"Error detected... trying single bit disparrancies..."<<endl;
	long int differencemask = (testsubject ^ n);			//XOR
	int parityposition = 0;

	//Find parity position that is wrong
	if (differencemask & checkP1)
		parityposition += 1;
	if (differencemask & checkP2)
		parityposition += 2;
	if (differencemask & checkP3)
		parityposition += 4;
	if (differencemask & checkP4)
		parityposition += 8;
	if (differencemask & checkP5)
		parityposition += 16;

	//make mask that will target the bit that will be flipped
	long int modoutput = (n ^ pow(2, 21-parityposition));				//XOR
	return modoutput;	
}

int main() {
	//input
	long int choice, n;
	cout.setf(ios::hex, ios::basefield);
	cout<<"Do you want to encode or check hammingbits? (1 for encode, 2 for check)"<<endl;
	cin>>choice;
	if (choice ==1) {
		cout<< "Enter a four digit hexadecimal value: ";
		cin.setf(ios::hex, ios::basefield);
		cin >> n;
		if (!n) {
			cout<< "Expecting a computable value... try again"<<endl;
			return 0;
		}
		cout<<"The hamming code is "<< hammingencode(n)<<endl;
	}
	else if (choice == 2) {
		cout<< "Enter the 21 bit hexadecimal value to be checked: ";
		cin.setf(ios::hex, ios::basefield);
		cin >> n;
		if (!n) {
			cout<< "Expecting a computable value... try again"<<endl;
			return 0;
		}
		cout<<"Your value: "<<n<<" was corrected to: "<<hammingcodecheck(n)<<endl;
	}
	return 0;
}