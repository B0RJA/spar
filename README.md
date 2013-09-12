 spar
=================================================

spar is 3D sparse matrix implementation desgined for fast memory read/write. Data is divided in blocks and compressed if the block is uniform. Maximum compression ratio using blocks of (4x4x4) elements is 32x with approx 50M read/write access per second.


Use
----------------------

```C
#include "spar.h"

main()
{
	// Declare sparse 3D integer matrix
	sparInt *data;
	// Accepted data types: char int long float double

	// Init data size, block size and default value
	data = sparIntInit( 1000, 1000, 1000, 4, 0 );

	// Set element
	sparIntSet( data, 999, 999, 999, 123456 );
	
	// Get element
	printf("data(999,999,999) = %d\n", sparIntGet( data, 999, 999, 999 ));

	// Memory usage
	printf("Memory usage of data() = %.1fMB\n", sparIntMemory( data ) / 1024. / 1024. );

	// Resize
	sparIntResize( data, 2000, 2000, 2000 );

	// Change block size
	sparIntChangeBs( data, 8 );

	// Optimize block size
	sparIntOptimizeBs( data );
	
	// Duplicate
	sparInt *data2;
	data2 = sparIntDuplicate( data );

	// Free memory
	sparIntFree( data );
	sparIntFree( data2 );
}
```

Contribute
----------------------

The source file `build\sparTemplate.h` is based on an arbitrary data type. Run the script `build\build.pl` to obtain the library containing the functions for all data types.


Any problems?
-------------
Feel free to [write an issue](https://github.com/B0RJA/sparMatrix/issues) if you have any questions or problems.


Copyright and license
---------------------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
