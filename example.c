#include "spar.h"

main()
{
	// Declare 3D sparse integer matrix
	sparInt *data;
	// Supported data types: char int long float double

	// Init data size, block size and default value
	data = sparIntInit( 1000, 1000, 1000, 4, 0 );

	// Set element
	sparIntSet( data, 999, 999, 999, 123456 );
	
	// Get element
	printf("data(999,999,999) = %d\n", sparIntGet( data, 999, 999, 999 ));

	// Memory usage
	printf("Memory usage of data() = %.1fMB\n", sparIntMemory( data ) / 1024. / 1024. );

	// Free memory
	sparIntFree( data );
}
