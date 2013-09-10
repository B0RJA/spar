#include "spar.h"

main()
{
	// Declare sparse 3D matrix of integers
	sparInt *data;

	// Init data size, block size and default value
	data = sparIntInit( 1000, 1000, 1000, 4, 0 );

	// Set some elements
	sparIntSet( data,   0,   0,   0, 123 );
	sparIntSet( data, 999, 999, 999, 456 );

	// Get some elements
	printf("data(  0,  0,  0) = %d\n", sparIntGet( data,   0,   0,   0 ));
	printf("data(499,499,499) = %d\n", sparIntGet( data, 499, 499, 499 ));
	printf("data(999,999,999) = %d\n", sparIntGet( data, 999, 999, 999 ));

	// Free memory
	sparIntFree( data );
}
