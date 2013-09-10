#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Do not edit! Automatically-generated file

// Matrix struct
typedef struct sparCharMatrix
{
	int nx, ny, nz;       // Matrix size (nx,ny,nz)
	int bs, bs3;          // Block size (bs,bs,bs)
	int mx, my, mz;       // Block matrix size (mx,my,mz)
	char *blockValue; // Uniform block data
	char **blockData; // Heterogeneous block data
	char def;         // Default value
} sparCharMatrix;

// Matrix constructor
sparCharMatrix* sparCharInit( int nx, int ny, int nz, int bs, char def )
{
	// Check matrix size
	if( !( nx > 0 && ny > 0 && nz > 0 ) )
	{
		fprintf(stderr, "sparInit error: Matrix size must be positive\n");
		return NULL;
	}

	// Check block size
	if( !( bs > 1 ) )
	{
		fprintf(stderr, "sparInit error: Block size must be greater than 1\n");
		return NULL;
	}

	// Declare struct and allocate space
	sparCharMatrix *matrix;
	matrix = (sparCharMatrix*) malloc(sizeof(sparCharMatrix));

	// Set matrix size (nx,ny,nz)
	matrix->nx = nx;
	matrix->ny = ny;
	matrix->nz = nz;

	// Set block size (bs,bs,bs)
	matrix->bs  = bs;
	matrix->bs3 = bs * bs * bs;

	// Set block matrix size (mx,my,mz)
	matrix->mx = (int)( ( nx + bs - 1 ) / bs );
	matrix->my = (int)( ( ny + bs - 1 ) / bs );
	matrix->mz = (int)( ( nz + bs - 1 ) / bs );

	// Number of blocks
	int blocks = matrix->mx * matrix->my * matrix->mz;

	// Allocate space for block uniform data
	matrix->blockValue = (char*) calloc( blocks, sizeof(char) );

	// Allocate space for block heterogeneous data arrays
	matrix->blockData = (char**) calloc( blocks, sizeof(char*) );

	// Set default value
	matrix->def = def;

	// Set matrix elemets to default value
	int i;
	for( i = 0 ; i < blocks ; i++ )
	{
		matrix->blockData[i] = NULL; // Flag for uniform block
		matrix->blockValue[i] = def;
	}

	// Return pointer
	return matrix;
}

// Matrix destructor
void sparCharFree( sparCharMatrix *matrix )
{
	// Number of blocks
	int blocks;
	blocks = matrix->mx * matrix->my * matrix->mz;

	// Free heterogeneous blocks
	int i;
	for( i = 0 ; i < blocks ; i++ )
	{
		if( matrix->blockData[i] != NULL ) // Flag for uniform block
		{
			free( matrix->blockData[i] );
		}
	}

	// Free block uniform data
	free(matrix->blockValue);

	// Free block heterogeneous data array
	free(matrix->blockData);

	// Free matrix instance
	free(matrix);
}

// Reset matrix values
void sparCharReset( sparCharMatrix *matrix )
{
	// Number of blocks
	int blocks;
	blocks = matrix->mx * matrix->my * matrix->mz;

	// Reduce blocks and set to default value
	int i;
	for( i = 0 ; i < blocks ; i++ )
	{
		// Heterogeneous block
		if( matrix->blockData[i] != NULL )
		{
			free( matrix->blockData[i] );
			matrix->blockData[i] = NULL;
		}
		matrix->blockValue[i] = matrix->def;
	}
}

// Get matrix memory usage in bytes
double sparCharMemory( sparCharMatrix *matrix )
{
	// Number of blocks
	int blocks;
	blocks = matrix->mx * matrix->my * matrix->mz;

	// Matrix instance
	double size;
	size = (double)( sizeof(sparCharMatrix) );

	// Uniform block data
	size = size + (double)( blocks * sizeof(char) );

	// Heterogeneous block data arrays
	size = size + (double)( blocks * sizeof(char*) );

	// Heterogeneous block data
	int i;
	for( i = 0 ; i < blocks ; i++ )
	{
		// Heterogeneous block
		if( matrix->blockData[i] != NULL )
		{
			size = size + (double)( sizeof(char) * matrix->bs3 );
		}
	}

	return size;
}

// Check if block is uniform
int sparCharUniformBlock( sparCharMatrix *matrix, int x, int y, int z )
{
	// Block size
	int bs, bs3;
	bs = matrix->bs;
	bs3 = matrix->bs3;

	// Block matrix size (mx,my,mz)
	int mx, my, mz;
	mx = matrix->mx;
	my = matrix->my;
	mz = matrix->mz;

	// Linear block index (n) <-> (x,y,z)
	int n;
	n = x + mx * ( y + my * z );

	// Block data array
	char *blockData;
	blockData = matrix->blockData[n];

	// Uniform block
	if( blockData == NULL )
	{
		return 1;
	}

	int isUniform;
	isUniform = 1;

	// Inner block
	if( x < mx - 1 && y < my - 1 && z < mz - 1)
	{
		// Start with last element
		char value;
		value = blockData[ bs3 - 1 ];

		// Read block elements
		int i;
		for( i = 0 ; i < bs3 ; i++ )
		{
			if( blockData[i] != value )
			{
				isUniform = 0;
				i = bs3;
			}
		}
	}
	// Boundary block, with outside elements?
	else
	{
		// Start with first element
		char value;
		value = blockData[0];

		// Read block elements
		int i, j, k;
		for( k = 0 ; k < bs ; k++ )
		{
			if( z * bs + k < matrix->nz ) // Avoid outside elements
			for( j = 0 ; j < bs ; j++ )
			{
				if( y * bs + j < matrix->ny ) // Idem
				for( i = 0 ; i < bs ; i++ )
				{
					if( x * bs + i < matrix->nx ) // Idem
					if( blockData[ i + bs * ( j + bs * k ) ] != value )
					{
						isUniform = 0;
						i = j = k = bs;
					}
				}
			}
		}
	}

	return isUniform;
}

// Set matrix element (x,y,z)
void sparCharSet( sparCharMatrix *matrix, int x, int y, int z, char value )
{
	// Block size
	int bs, bs3;
	bs = matrix->bs;
	bs3 = matrix->bs3;

	// Block (i1,j1,k1) contains the element (x,y,z)
	int i1, j1, k1;
	i1 = x / bs;
	j1 = y / bs;
	k1 = z / bs;

	// Element indices in the block
	int i2, j2, k2;
	i2 = x % bs;
	j2 = y % bs;
	k2 = z % bs;

	// Linear block index (n) <-> (i1,j1,k1)
	int n;
	n = i1 + matrix->mx * ( j1 + matrix->my * k1 );

	// Block uniform value
	char blockValue;
	blockValue = matrix->blockValue[n];

	// Block data array
	char *blockData;
	blockData = matrix->blockData[n];

	// Uniform block
	if( blockData == NULL )
	{
		// Input value is different
		if( value != blockValue )
		{
			// Expand block
			blockData = (char*) calloc( bs3, sizeof(char) );
			matrix->blockData[n] = blockData;

			// Set previous value
			int i;
			for( i = 0 ; i < bs3 ; i++ )
			{
				blockData[i] = blockValue;
			}

			// Set input value
			blockData[ i2 + bs * ( j2 + bs * k2 ) ] = value;
		}
		// Else, do nothing
	}
	// Heterogeneous block
	else
	{
		// Set input value
		blockData[ i2 + bs * ( j2 + bs * k2 ) ] = value;

		// Reduce block
		if( sparCharUniformBlock( matrix, i1, j1, k1 ) )
		{
			matrix->blockValue[n] = value;
			free(matrix->blockData[n]);
			matrix->blockData[n] = NULL;
		}
	}
}

// Get matrix element (x,y,z)
char sparCharGet( sparCharMatrix *matrix, int x, int y, int z )
{
	// Block size
	int bs;
	bs = matrix->bs;

	// Block (i1,j1,k1) contains the element (x,y,z)
	int i1, j1, k1;
	i1 = x / bs;
	j1 = y / bs;
	k1 = z / bs;

	// Linear block index (n) <-> (i1,j1,k1)
	int n;
	n = i1 + matrix->mx * ( j1 + matrix->my * k1 );

	char *blockData;
	blockData = matrix->blockData[n];

	// Uniform block
	if( blockData == NULL )
	{
		return matrix->blockValue[n];
	}
	// Heterogeneous block
	else
	{
		// Element indices in the block
		int i2, j2, k2;
		i2 = x % bs;
		j2 = y % bs;
		k2 = z % bs;
		// Return element value
		return blockData[ i2 + bs * ( j2 + bs * k2 ) ];
	}
}

// Duplicate matrix
sparCharMatrix* sparCharDuplicate( sparCharMatrix *matrix )
{
	// Declare matrix and init
	sparCharMatrix *matrix2;
	matrix2 = sparCharInit( matrix->nx, matrix->ny, matrix->nz,
						matrix->bs, matrix->def );

	// Number of blocks
	int blocks;
	blocks = matrix2->mx * matrix2->my * matrix2->mz;

	// Copy blocks
	int i;
	for( i = 0 ; i < blocks ; i++ )
	{
		// Heterogeneous block
		if( matrix->blockData[i] != NULL )
		{
			// Allocate space for block data
			matrix2->blockData[i] = (char*) calloc( matrix2->bs3, sizeof(char) );

			// Copy block data
			memcpy( matrix2->blockData[i], matrix->blockData[i], matrix2->bs3 * sizeof(char) );
		}
		// Uniform block
		else
		{
			matrix2->blockValue[i] = matrix->blockValue[i];
			matrix2->blockData[i] = NULL;
		}
	}

	return matrix2;
}

// Get matrix memory usage in bytes under certain block size
double sparCharMemoryBs( sparCharMatrix *matrix, int bs )
{
	if( bs == matrix->bs )
	{
		return sparCharMemory( matrix );
	}

	// Matrix size (nx,ny,nz)
	int nx, ny, nz;
	nx = matrix->nx;
	ny = matrix->ny;
	nz = matrix->nz;

	// Block size (bs,bs,bs)
	int bs3;
	bs3 = bs * bs * bs;

	// Block matrix size (mx,my,mz)
	int mx, my, mz;
	mx = (int)( ( nx + bs - 1 ) / bs );
	my = (int)( ( ny + bs - 1 ) / bs );
	mz = (int)( ( nz + bs - 1 ) / bs );

	// Number of blocks
	int blocks;
	blocks = mx * my * mz;

	// Size of matrix instance
	double size;
	size = (double)( sizeof(sparCharMatrix) );

	// Size of uniform block data
	size = size + (double)( blocks * sizeof(char) );

	// Size of heterogeneous block data arrays
	size = size + (double)( blocks * sizeof(char*) );

	int i, j, k;
	int i1, j1, k1;
	int isUniform;
	char value;

	// For each virtual block
	for( k1 = 0 ; k1 < mz ; k1++ )
	{
		for( j1 = 0 ; j1 < my ; j1++ )
		{
			for( i1 = 0 ; i1 < mx ; i1++ )
			{
				// Check if block is uniform 
				isUniform = 1;
				value = sparCharGet( matrix, i1 * bs, j1 * bs, k1 * bs );
				for( k = k1 * bs ; k < ( k1 + 1 ) * bs ; k++ )
				{
					if( k < nz ) // Avoid outside elements
					for( j = j1 * bs ; j < ( j1 + 1 ) * bs ; j++ )
					{
						if( j < ny ) // Idem
						for( i = i1 * bs ; i < ( i1 + 1 ) * bs ; i++ )
						{
							if( i < nx ) // Idem
							if( sparCharGet( matrix, i, j, k ) != value )
							{
								isUniform = 0;
								i = ( i1 + 1 ) * bs;
								j = ( j1 + 1 ) * bs;
								k = ( k1 + 1 ) * bs;
							}
						}
					}
				}
				if( isUniform == 0 )
				{
					size = size + (double)( sizeof(char) * bs3 );
				}
			}
		}
	}

	return size;
}

// Change matrix block size
void sparCharChangeBs( sparCharMatrix *matrix, int bs )
{
	// Matrix size (nx,ny,nz)
	int nx, ny, nz;
	nx = matrix->nx;
	ny = matrix->ny;
	nz = matrix->nz;

	// Default value
	char def;
	def = matrix->def;

	// Declare temporal matrix and init
	sparCharMatrix *matrix2;
	matrix2 = sparCharInit( nx, ny, nz, bs, def );

	// Copy values and clear original matrix
	int i, j, k;
	for( k=0 ; k<nz ; k++ )
	{
		for( j=0 ; j<ny ; j++ )
		{
			for( i=0 ; i<nx ; i++ )
			{
				// Copy element
				sparCharSet( matrix2, i, j, k, sparCharGet( matrix, i, j, k ) );

				// Set uniform to free memory
				sparCharSet( matrix, i, j, k, def);
			}
		}
	}

	// Set new block size and grid
	matrix->bs  = matrix2->bs;
	matrix->bs3 = matrix2->bs3;
	matrix->mx  = matrix2->mx;
	matrix->my  = matrix2->my;
	matrix->mz  = matrix2->mz;

	// Free old blocks
	free(matrix->blockValue);
	free(matrix->blockData);

	// Copy new blocks
	matrix->blockValue = matrix2->blockValue;
	matrix->blockData = matrix2->blockData;

	// Free temporal matrix
	free(matrix2);
}

// Optimize matrix block size
void sparCharOptimizeBs( sparCharMatrix *matrix )
{
	// Test block size and store optimal
	int bestBs;
	double memory, bestMemory;
	int bs[10] = { 2, 3, 4, 6, 8, 10 };

	bestBs = bs[0];
	bestMemory = sparCharMemoryBs( matrix, bs[0] );

	int i;
	for( i=1 ; i<6 ; i++ )
	{
		memory = sparCharMemoryBs( matrix, bs[i] );
		if( memory < bestMemory )
		{
			bestBs = bs[i];
			bestMemory = memory;
		}
	}

	// Change matrix block size
	sparCharChangeBs( matrix, bestBs );
}

// Resize matrix
void sparCharResize( sparCharMatrix *matrix, int nx, int ny, int nz )
{
	// Check matrix size
	if( !( nx > 0 && ny > 0 && nz > 0 ) )
	{
		fprintf(stderr, "sparResize error: Matrix size must be positive\n");
		return;
	}

	// Block size
	int bs;
	bs = matrix->bs;

	// Default value
	char def;
	def = matrix->def;

	// Block grid size
	int mx, my, mz;

	// New block data
	char *blockValue;
	char **blockData;

	int i, j, k;
	int blocks;

	// Expand x
	if( nx > matrix->nx )
	{
		// New block grid size
		mx = (int)( ( nx + bs - 1 ) / bs );
		my = matrix->my;
		mz = matrix->mz;

		// Number of blocks
		blocks = mx * my * mz;
		blockValue = (char*) calloc( blocks, sizeof(char) ); // Sets to 0s
		blockData = (char**) calloc( blocks, sizeof(char*) ); // Sets to NULLs

		// Copy existing blocks
		for( k = 0 ; k < mz ; k++ )
		{
			for( j = 0 ; j < my ; j++ )
			{
				for( i = 0 ; i < matrix->mx ; i++ )
				{
					blockValue[ i + mx * ( j + my * k ) ]
						= matrix->blockValue[ i + matrix->mx * ( j + matrix->my * k ) ];
					blockData[ i + mx * ( j + my * k ) ]
						= matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ];
				}
			}
		}
		// Create new blocks
		for( k = 0 ; k < mz ; k++ )
		{
			for( j = 0 ; j < my ; j++ )
			{
				for( i = matrix->mx ; i < mx ; i++ )
				{
					blockValue[ i + mx * ( j +  my * k ) ] = def;
					blockData[ i + mx * ( j +  my * k ) ] = NULL;
				}
			}
		}

		int xi, xf;
		xi = matrix->nx;
		xf = matrix->bs * matrix->mx;
		if( xf >= nx )
		{
			xf = nx - 1;
		}

		matrix->nx = nx;
		matrix->mx = mx;

		free(matrix->blockData);
		free(matrix->blockValue);

		matrix->blockData = blockData;
		matrix->blockValue = blockValue;

		// Set expanded elements to default
		for( k = 0 ; k < matrix->nz ; k++ )
		{
			for( j = 0 ; j < matrix->ny ; j++ )
			{
				for( i = xi ; i < xf ; i++ )
				{
					sparCharSet( matrix, i, j, k, def );
				}
			}
		}
	}
	// Shrink x
	else if( nx < matrix->nx )
	{
		// New block grid size
		mx = (int)( ( nx + bs - 1 ) / bs );
		my = matrix->my;
		mz = matrix->mz;

		// Number of blocks
		blocks = mx * my * mz;
		blockValue = (char*) calloc( blocks, sizeof(char) ); // Sets to 0s
		blockData = (char**) calloc( blocks, sizeof(char*) ); // Sets to NULLs

		// Copy existing blocks
		for( k = 0 ; k < mz ; k++ )
		{
			for( j = 0 ; j < my ; j++ )
			{
				for( i = 0 ; i < mx ; i++ )
				{
					blockValue[ i + mx * ( j + my * k ) ]
						= matrix->blockValue[ i + matrix->mx * ( j + matrix->my * k ) ];
					blockData[ i + mx * ( j + my * k ) ]
						= matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ];
				}
			}
		}

		// Free excedent blocks
		for( k = 0 ; k < mz ; k++ )
		{
			for( j = 0 ; j < my ; j++ )
			{
				for( i = mx ; i < matrix->mx ; i++ )
				{
					if( matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ] != NULL )
					{
						free( matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ] );
					}
				}
			}
		}

		matrix->nx = nx;
		matrix->mx = mx;

		free(matrix->blockData);
		free(matrix->blockValue);

		matrix->blockData = blockData;
		matrix->blockValue = blockValue;
	}

	// Expand y
	if( ny > matrix->ny )
	{
		// New block grid size
		mx = matrix->mx;
		my = (int)( ( ny + bs - 1 ) / bs );
		mz = matrix->mz;

		// Number of blocks
		blocks = mx * my * mz;
		blockValue = (char*) calloc( blocks, sizeof(char) ); // Sets to 0s
		blockData = (char**) calloc( blocks, sizeof(char*) ); // Sets to NULLs

		// Copy existing blocks
		for( k = 0 ; k < mz ; k++ )
		{
			for( j = 0 ; j < matrix->my ; j++ )
			{
				for( i = 0 ; i < mx ; i++ )
				{
					blockValue[ i + mx * ( j + my * k ) ]
						= matrix->blockValue[ i + matrix->mx * ( j + matrix->my * k ) ];
					blockData[ i + mx * ( j + my * k ) ]
						= matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ];
				}
			}
		}
		// Create new blocks
		for( k = 0 ; k < mz ; k++ )
		{
			for( j = matrix->my ; j < my ; j++ )
			{
				for( i = 0 ; i < mx ; i++ )
				{
					blockValue[ i + mx * ( j +  my * k ) ] = def;
					blockData[ i + mx * ( j +  my * k ) ] = NULL;
				}
			}
		}

		int yi, yf;
		yi = matrix->ny;
		yf = matrix->bs * matrix->my;
		if( yf >= ny )
		{
			yf = ny - 1;
		}

		matrix->ny = ny;
		matrix->my = my;

		free(matrix->blockData);
		free(matrix->blockValue);

		matrix->blockData = blockData;
		matrix->blockValue = blockValue;

		// Set expanded elements to default
		for( k = 0 ; k < matrix->nz ; k++ )
		{
			for( j = yi ; j < yf ; j++ )
			{
				for( i = 0 ; i < matrix->nx ; i++ )
				{
					sparCharSet( matrix, i, j, k, def );
				}
			}
		}
	}
	// Shrink y
	else if( ny < matrix->ny )
	{
		// New block grid size
		mx = matrix->mx;
		my = (int)( ( ny + bs - 1 ) / bs );
		mz = matrix->mz;

		// Number of blocks
		blocks = mx * my * mz;
		blockValue = (char*) calloc( blocks, sizeof(char) ); // Sets to 0s
		blockData = (char**) calloc( blocks, sizeof(char*) ); // Sets to NULLs

		// Copy existing blocks
		for( k = 0 ; k < mz ; k++ )
		{
			for( j = 0 ; j < my ; j++ )
			{
				for( i = 0 ; i < mx ; i++ )
				{
					blockValue[ i + mx * ( j + my * k ) ]
						= matrix->blockValue[ i + matrix->mx * ( j + matrix->my * k ) ];
					blockData[ i + mx * ( j + my * k ) ]
						= matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ];
				}
			}
		}

		// Free excedent blocks
		for( k = 0 ; k < mz ; k++ )
		{
			for( j = my ; j < matrix->my ; j++ )
			{
				for( i = 0 ; i < mx ; i++ )
				{
					if( matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ] != NULL )
					{
						free( matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ] );
					}
				}
			}
		}

		matrix->ny = ny;
		matrix->my = my;

		free(matrix->blockData);
		free(matrix->blockValue);

		matrix->blockData = blockData;
		matrix->blockValue = blockValue;
	}

	// Expand z
	if( nz > matrix->nz )
	{
		// New block grid size
		mx = matrix->mx;
		my = matrix->my;
		mz = (int)( ( nz + bs - 1 ) / bs );

		// Number of blocks
		blocks = mx * my * mz;
		blockValue = (char*) calloc( blocks, sizeof(char) ); // Sets to 0s
		blockData = (char**) calloc( blocks, sizeof(char*) ); // Sets to NULLs

		// Copy existing blocks
		for( k = 0 ; k < matrix->mz ; k++ )
		{
			for( j = 0 ; j < my ; j++ )
			{
				for( i = 0 ; i < mx ; i++ )
				{
					blockValue[ i + mx * ( j + my * k ) ]
						= matrix->blockValue[ i + matrix->mx * ( j + matrix->my * k ) ];
					blockData[ i + mx * ( j + my * k ) ]
						= matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ];
				}
			}
		}
		// Create new blocks
		for( k = matrix->mz ; k < mz ; k++ )
		{
			for( j = 0 ; j < my ; j++ )
			{
				for( i = 0 ; i < mx ; i++ )
				{
					blockValue[ i + mx * ( j +  my * k ) ] = def;
					blockData[ i + mx * ( j +  my * k ) ] = NULL;
				}
			}
		}

		int zi, zf;
		zi = matrix->nz;
		zf = matrix->bs * matrix->mz;
		if( zf >= nz )
		{
			zf = nz - 1;
		}

		matrix->nz = nz;
		matrix->mz = mz;

		free(matrix->blockData);
		free(matrix->blockValue);

		matrix->blockData = blockData;
		matrix->blockValue = blockValue;

		// Set expanded elements to default
		for( k = zi ; k < zf ; k++ )
		{
			for( j = 0 ; j < matrix->ny ; j++ )
			{
				for( i = 0 ; i < matrix->nx ; i++ )
				{
					sparCharSet( matrix, i, j, k, def );
				}
			}
		}
	}
	// Shrink z
	else if( nz < matrix->nz )
	{
		// New block grid size
		mx = matrix->mx;
		my = matrix->my;
		mz = (int)( ( nz + bs - 1 ) / bs );

		// Number of blocks
		blocks = mx * my * mz;
		blockValue = (char*) calloc( blocks, sizeof(char) ); // Sets to 0s
		blockData = (char**) calloc( blocks, sizeof(char*) ); // Sets to NULLs

		// Copy existing blocks
		for( k = 0 ; k < mz ; k++ )
		{
			for( j = 0 ; j < my ; j++ )
			{
				for( i = 0 ; i < mx ; i++ )
				{
					blockValue[ i + mx * ( j + my * k ) ]
						= matrix->blockValue[ i + matrix->mx * ( j + matrix->my * k ) ];
					blockData[ i + mx * ( j + my * k ) ]
						= matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ];
				}
			}
		}

		// Free excedent blocks
		for( k = mz ; k < matrix->mz ; k++ )
		{
			for( j = 0 ; j < my ; j++ )
			{
				for( i = 0 ; i < mx ; i++ )
				{
					if( matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ] != NULL )
					{
						free( matrix->blockData[ i + matrix->mx * ( j + matrix->my * k ) ] );
					}
				}
			}
		}

		matrix->nz = nz;
		matrix->mz = mz;

		free(matrix->blockData);
		free(matrix->blockValue);

		matrix->blockData = blockData;
		matrix->blockValue = blockValue;
	}
}
