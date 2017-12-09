#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include "io.h"
#include "KDEcalc.h"

int main(int argc, char ** argv)
{
	int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if(argc != 9)
	{
		if(rank == 0)
		{
			printf("Incorrect number of parameters: 4DKDE [inputFile] [outputFile] [xMin] [xMax] [yMin] [yMax] [cellSize] [bandwidth]\n");
		}
		MPI_Finalize();
		exit(1);
	}

	double xMin, xMax, yMin, yMax, cellSize, bandwidth;
	
	xMin = atof(argv[3]); 
	xMax = atof(argv[4]);
	yMin = atof(argv[5]); 
	yMax = atof(argv[6]); 
	cellSize = atof(argv[7]); 
	bandwidth = atof(argv[8]);

	int nX, nY;
	nX = ceil((xMax - xMin)/cellSize);
	nY = ceil((yMax - yMin)/cellSize);

	xMax = xMin + cellSize * nX;
	yMax = yMin + cellSize * nY;

	double * x1;
	double * y1;
	double * x2;
	double * y2;

	int nPoints;
	FILE * file;

	if(rank == 0)
	{
		
		if(NULL == (file = fopen(argv[1], "r")))
		{
			printf("ERROR: Can't open the input file.\n");
			exit(1);
		}

		nPoints = getNumPoints(file);
	}

	MPI_Bcast(&nPoints, 1, MPI_INT, 0, MPI_COMM_WORLD);

	if(NULL == (x1 = (double *) malloc (nPoints * sizeof(double))))
	{
		printf("ERROR: Out of memory at line %d in file %s\n", __LINE__, __FILE__);
		exit(1);
	}
	if(NULL == (y1 = (double *) malloc (nPoints * sizeof(double))))
	{
		printf("ERROR: Out of memory at line %d in file %s\n", __LINE__, __FILE__);
		exit(1);
	}
	if(NULL == (x2 = (double *) malloc (nPoints * sizeof(double))))
	{
		printf("ERROR: Out of memory at line %d in file %s\n", __LINE__, __FILE__);
		exit(1);
	}
	if(NULL == (y2 = (double *) malloc (nPoints * sizeof(double))))
	{
		printf("ERROR: Out of memory at line %d in file %s\n", __LINE__, __FILE__);
		exit(1);
	}

	if(rank == 0)
	{
		printf("Raster size: %d * %d * %d * %d\n", nX, nY, nX, nY);
		readFile(file, x1, y1, x2, y2, nPoints);
/*
		for(int i = 0; i < 100; i++)
		{
			printf("%f\t%f\t%f\t%f\n", x1[i], y1[i], x2[i], y2[i]);
		}
*/		
		fclose(file);
	}

	MPI_Bcast(x1, nPoints, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(y1, nPoints, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(x2, nPoints, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(y2, nPoints, MPI_DOUBLE, 0, MPI_COMM_WORLD);

	int nY2PP = (nY + size - 1) / size;
	int myY2Start = rank * nY2PP;
	int myY2End = (rank + 1) * nY2PP;

	double * myCells;
	int nMyCells = nX * nY * nX * nY2PP;
	if(NULL == (myCells = (double *) malloc (nMyCells * sizeof(double))))
	{
		printf("ERROR: Out of memory at line %d in file %s\n", __LINE__, __FILE__);
		exit(1);
	}
	for(int i = 0; i < nMyCells; i++)
	{
		myCells[i] = 0.0;
	}

	KDECalc(x1, y1, x2, y2, nPoints, xMin + 0.5 * cellSize, yMin + 0.5 * cellSize, xMin + 0.5 * cellSize, yMin + (myY2Start + 0.5) * cellSize, nX, nY, nX, nY2PP, cellSize, bandwidth, myCells);


//	printf("Rank %d\t: yRange: %lf ~ %lf\n", myY2Min, myY2Max);

	double * totalCells;
	int nTotalCells = size * nMyCells;
	
	if(rank == 0)
	{
		printf("Size : %d, nY2 per process: %d\n", size, nY2PP);
		if(NULL == (totalCells = (double *) malloc (nTotalCells * sizeof(double))))
		{
			printf("ERROR: Out of memory at line %d in file %s\n", __LINE__, __FILE__);
			exit(1);
		}
	}

	MPI_Gather(myCells, nMyCells, MPI_DOUBLE, totalCells, nMyCells, MPI_DOUBLE, 0, MPI_COMM_WORLD);	
	

	free(x1);
	free(y1);
	free(x2);
	free(y2);
	if(rank == 0)
	{
		if(NULL == (file = fopen(argv[2], "wb")))
		{
			printf("ERROR: Can't open the output file.\n");
			exit(1);
		}

		writeFile(file, totalCells, nX * nY * nX * nY);

		fclose(file);

		free(totalCells);
	}

	free(myCells);
	
	MPI_Finalize();
	return 0;
}
