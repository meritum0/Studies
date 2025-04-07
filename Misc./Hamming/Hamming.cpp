#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

typedef uint8_t byte;

string to_bit4( byte in );
string to_bit7( byte in );
byte encode( byte in );
byte noise( byte in );
byte ecc( byte in );
byte decode( byte in );

string to_bit4( byte in )
{
	bitset<4> b4 = in;
	return b4.to_string();
}

string to_bit7( byte in )
{
	bitset<7> b7 = in;
	return b7.to_string();
}

byte encode( byte in )
{
	byte code[16] = { 
		0b0000000,
		0b0000111,
		0b0011001,
		0b0011110,
		0b0101010,
		0b0101101,
		0b0110011,
		0b0110100,
		0b1001011,
		0b1001100,
		0b1010010,
		0b1010101,
		0b1100001,
		0b1100110,
		0b1111000,
		0b1111111
	};
	
	return code[in];
}

byte noise( byte in )
{
	int bit_nr = rand() % 7;
	return (1 << bit_nr) ^ in;
}

byte ecc( byte in )
{
	byte p1 = (in & 0b0000001);
	byte p2 = (in & 0b0000010) >> 1;
	byte p3 = (in & 0b0001000) >> 3;
	byte d1 = (in & 0b0000100) >> 2;
	byte d2 = (in & 0b0010000) >> 4;
	byte d3 = (in & 0b0100000) >> 5;
	byte d4 = (in & 0b1000000) >> 6;
	
	byte error = 0;
	if( (d1 + d2 + d4 + p1) % 2 != 0 )
		error += 1;
	if( (d1 + d3 + d4 + p2) % 2 != 0 )
		error += 0b10;
	if( (d2 + d3 + d4 + p3) % 2 != 0 )
		error += 0b100;
	if( error == 0 )
		return in;
	
	byte correction = 1 << (error-1);
	return (in ^ correction);
}

byte decode( byte in )
{
	byte d1 = (in & 0b0000100) >> 2;
	return (in >> 3 ) & 0b1110 | d1;
}

int main(int argc, char *argv[]) 
{
	srand( time(NULL) );
	
	byte msg = rand() % 16;
	byte enc = encode( msg );
	byte bad = noise( enc );
	byte corr = ecc( bad );
	byte dec = decode( corr );
	
	cout << "  msg:    " << to_bit4( msg ) << endl;
	cout << "  enc: " << to_bit7( enc ) << endl;
	cout << "  bad: " << to_bit7( bad ) << endl;
	cout << " corr: " << to_bit7( corr ) << endl;
	cout << "  dec:    " << to_bit4( dec ) << endl;
	
	cout << ( msg == dec ? "success" : "failure" ) << endl;
}