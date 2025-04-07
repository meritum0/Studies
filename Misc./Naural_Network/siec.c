#include <stdio.h>
#include <stdlib.h>
#include <math.h>

const int nr_inputs  = 25;			// 5x5 bitmap
const int nr_hidden  = 5;
const int nr_outputs = 10;			// 10 cyfr

double dot( double *x, double *y, int length, int step )			// iloczyn skalarny
{
	double dot = 0;
	for( int i = 0; i < length; i++ )
		dot += (x[i] * *(y + i*step));
	return dot;
}

double sigmoid( double x )  { return 1.0 / (1.0 + exp(-x) ); }		// funkcja aktywacji

double neuron( double input[], double weights[], int size )
{
	return sigmoid( dot( input, weights, size+1, 1 ) );
}

void feed_forward( double hidden_weights[][nr_inputs+1], double output_weights[][nr_hidden+1], 
				   double input[], double output[], double *hidden_output )
{
	double	*hidden_out = hidden_output != NULL ? hidden_output : (double *)alloca(sizeof(double)*(nr_hidden+1));
	input[nr_inputs] = hidden_out[nr_hidden] = 1;		// bias
	for( int i = 0; i < nr_hidden; i++)
		hidden_out[i] = neuron( input, hidden_weights[i], nr_inputs );
	for( int i = 0; i < nr_outputs; i++ )
		output[i] = neuron( hidden_out, output_weights[i], nr_hidden );
}

void back_propagate( double hidden_weights[][nr_inputs+1], double output_weights[][nr_hidden+1], 
					 double input[], double result[] )
{
	double h_output[nr_hidden+1],  output[nr_outputs];
	feed_forward( hidden_weights, output_weights, input, output, h_output );
	
	double out_deltas[nr_outputs];
	for( int i = 0; i < nr_outputs; i++ )
		out_deltas[i] = output[i] * (1 - output[i]) * (output[i] - result[i]);
	
	for( int i = 0; i < nr_outputs; i++ )
		for( int n = 0; n <= nr_hidden; n++ )
			output_weights[i][n] -= out_deltas[i] * h_output[n];
		
	double h_deltas[nr_hidden];
	for( int i = 0; i < nr_hidden; i++ )
		h_deltas[i] = h_output[i] * (1 - h_output[i]) * dot( out_deltas, output_weights[0]+i, nr_outputs, nr_hidden+1 );
	
	for( int i = 0; i < nr_hidden; i++ )
		for( int n = 0; n <= nr_inputs; n++ )
			hidden_weights[i][n] -= h_deltas[i] * input[n];
}

int main()
{
	double	hidden_weights[nr_hidden][nr_inputs+1], output_weights[nr_outputs][nr_hidden+1];
	
	for( int i = 0; i < nr_hidden*(nr_inputs+1); i++ )			// wagi inicjowane losowo
		((double *)hidden_weights)[i] = ((double)rand())/(double)RAND_MAX;
	for( int i = 0; i < nr_outputs*(nr_hidden+1); i++ )			// wagi inicjowane losowo
		*((double *)output_weights + i) = ((double)rand())/(double)RAND_MAX;
	
	double digits[11][nr_inputs+1] = {							//5x5bitmaps + bias
		1,1,1,1,1,1,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,1,	// 0 bitmap
		0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,1,	// 1 bitmap
		1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,	// 2 bitmap
		1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,	// 3 bitmap
		1,0,0,0,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,1,	// 4 bitmap
		1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,	// 5 bitmao
		1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,	// 6 bitmap
		1,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,1,	// 7 bitmap
		1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,	// 8 bitmap
		1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,	// 9 bitmap
		0,0,1,1,0,												// 3 - test bitmap
		0,0,0,0,1,
		0,0,1,1,0,
		0,0,0,0,1,
		0,0,1,1,0
	};
	double results[10][10] = {
		1,0,0,0,0,0,0,0,0,0,  0,1,0,0,0,0,0,0,0,0,				// 0 & 1
		0,0,1,0,0,0,0,0,0,0,  0,0,0,1,0,0,0,0,0,0,				// 2 & 3
		0,0,0,0,1,0,0,0,0,0,  0,0,0,0,0,1,0,0,0,0,				// 4 & 5
		0,0,0,0,0,0,1,0,0,0,  0,0,0,0,0,0,0,1,0,0,				// 6 & 7
		0,0,0,0,0,0,0,0,1,0,  0,0,0,0,0,0,0,0,0,1				// 8 & 9
	};
	
	for( int i = 0; i < 10000; i++ )		// proces uczenia przy pomocy propagacji wstecznej
		for( int n = 0; n < 10; n++ )
			back_propagate( hidden_weights, output_weights, digits[n], results[n] );
		
	double	output[nr_outputs];
	for( int i = 0; i < 11; i++ )
	{
		feed_forward( hidden_weights, output_weights, digits[i], output, NULL );
		for( int n = 0; n < nr_outputs; n++ )  printf("%.2f ", output[n] );
		printf("\n");
	}
}