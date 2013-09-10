 spar
=================================================

3D sparse matrix implementation in C


Use
----------------------
TODO

```C
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
```

Contribute
----------------------
TODO



Any problems?
-------------
Feel free to [write an issue](https://github.com/B0RJA/sparMatrix/issues) if you have any questions or problems.



Copyright and license
---------------------

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 2 of the License, or (at your option) any later version.
