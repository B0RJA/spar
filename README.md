 spar
=================================================

3D sparse matrix implementation in C


Use
----------------------
TODO

```C
// spar for integer data
#include "sparInt.c"

main()
{
	// Declare int sparse matrix
	sparInt *data;

	// Init size, block size and default value
	data = sparIntInit( 1000, 1000, 1000, 4, 0 );

	// Set element
	sparIntSet( data, 999, 999, 999, 123456789 );

	// Get element
	printf("data(999,999,999) = %d\n", sparIntGet( data, 999, 999, 999 ));

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
