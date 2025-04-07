#include <iostream>
using namespace std;

typedef uint8_t byte;

byte encode( byte in );
byte noise( byte in );
byte ecc_n_decode( byte in );

byte enc_LUT[16] = {
	0x00,0x07,0x19,0x1e,0x2a,0x2d,0x33,0x34,0x4b,0x4c,0x52,0x55,0x61,0x66,0x78,0x7f
};

byte dec_LUT[128] = {
	0,0,0,1,0,1,1,1,0,2,4,8,9,5,3,1,0,2,10,6,7,11,3,1,2,2,3,2,3,2,3,3,
	0,12,4,6,7,5,13,1,4,5,4,4,5,5,4,5,7,6,6,6,7,7,7,6,14,2,4,6,7,5,3,15,
	0,12,10,8,9,11,13,1,9,8,8,8,9,9,9,8,10,11,10,10,11,11,10,11,14,2,10,8,9,11,3,15,
	12,12,13,12,13,12,13,13,14,12,4,8,9,5,13,15,14,12,10,6,7,11,13,15,14,14,14,15,14,15,15,15
};

byte encode( byte in )			{  return enc_LUT[in]; }
byte ecc_n_decode( byte in )		{  return dec_LUT[in]; }

byte noise( byte in )
{
	int bit_nr = rand() % 7;
	return (1 << bit_nr) ^ in;
}

int main(int argc, char *argv[]) 
{
	srand( time(NULL) );
	
	byte msg = rand() % 16;
	byte enc = encode( msg );
	byte bad = noise( enc );
	byte dec = ecc_n_decode( bad );
	
	cout << "msg:    " << bitset<4>(msg) << endl;
	cout << "enc: "    << bitset<7>(enc) << endl;
	cout << "bad: "    << bitset<7>(bad) << endl;
	cout << "dec:    " << bitset<4>(dec) << endl;
	cout << ( msg == dec ? "success" : "failure" ) << endl;
}