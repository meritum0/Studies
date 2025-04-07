#include <iostream>
#include <iomanip>
#include <ctime>
using namespace std;

typedef uint8_t byte;

byte enc_LUT[16] = {
	0x00,0x07,0x19,0x1e,0x2a,0x2d,0x33,0x34,0x4b,0x4c,0x52,0x55,0x61,0x66,0x78,0x7f
};

byte dec_LUT[128] = {
	0,0,0,1,0,1,1,1,0,2,4,8,9,5,3,1,0,2,10,6,7,11,3,1,2,2,3,2,3,2,3,3,
	0,12,4,6,7,5,13,1,4,5,4,4,5,5,4,5,7,6,6,6,7,7,7,6,14,2,4,6,7,5,3,15,
	0,12,10,8,9,11,13,1,9,8,8,8,9,9,9,8,10,11,10,10,11,11,10,11,14,2,10,8,9,11,3,15,
	12,12,13,12,13,12,13,13,14,12,4,8,9,5,13,15,14,12,10,6,7,11,13,15,14,14,14,15,14,15,15,15
};

bitset<70> encode( __int128 in );
bitset<70> noise( bitset<70> in );
bitset<40> ecc_n_decode( bitset<70> in );

bitset<70> encode( __int128 in )
{
	bitset<70> e, enc = 0;
	
	for( int i=0; i<=9; i++ )
	{
		e = enc_LUT[ in & 0b1111 ];
		enc |= (e << 7*i);
		in = in >> 4;
	}
	return enc;
}

bitset<70> noise( bitset<70> in )
{
	bitset<70>  out = in;
	for( int i=0; i<=9; i++ )
	{
		int bit_nr = rand() % 7;
		bit_nr += 7*i;
		out.flip(bit_nr);
	}
	return out;
}

bitset<40>  ecc_n_decode( bitset<70> in )
{
	byte cut;
	bitset<40> out = 0, d;
	for( int i = 0; i <= 9; i++ )
	{
		cut =  (in & bitset<70>(0b1111111)).to_ulong();
		d = dec_LUT[cut];
		d <<= 4*i;
		out |= d;
		in = in >> 7;
	}
	return out;
}

int main(int argc, char *argv[])
{
	srand( time(NULL) );

	__int128_t msg = rand() % 0x200;
	msg = (msg << 31 ) | rand();
	
	bitset<70> enc = encode( msg );
	bitset<70> bad = noise( enc );
	bitset<40> dec = ecc_n_decode( bad );
	cout << "  msg: " << bitset<40>( msg ) << endl;
	cout << "  enc: " << enc << endl;
	cout << "noise: " << (enc ^ bad) << endl;
	cout << "  bad: " << bad << endl;
	cout << "  dec: " << dec << endl;
}