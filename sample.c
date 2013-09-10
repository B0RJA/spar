#include "spar.h"

main()
{
	// Declare sparse 3D integer matrix
	sparInt *data;

	// Init data size, block size and default value
	data = sparIntInit( 100, 100, 100, 4, 0 );

	// Set element
	sparIntSet( data, 99, 99, 99, 123456 );
	
	// Get element
	printf("data(99,99,99) = %d\n", sparIntGet( data, 99, 99, 99 ));

	// Memory usage
	printf("Memory use of data() = %.1fMB\n", sparIntMemory( data ) / 1024. / 1024. );

	// Resize
	//sparIntResize( data, 200, 200, 200 );

	// Change block size
	//sparIntChangeBs( data, 8 );

	// Optimize block size
	//sparIntOptimizeBs( data );
	
	// Duplicate
	//sparInt *data2;
	//data2 = sparIntDuplicate( data );

	// Free memory
	sparIntFree( data );
}
