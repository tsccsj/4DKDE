#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "io.h"
#include "KDEcalc.h"

int main(int argc, char ** argv)
{
	if(argc != 9)
	{
		printf("Incorrect number of parameters: 4DKDE [inputFile] [outputFile] [xMin] [xMax] [yMin] [yMax] [cellSize] [bandwidth]\n");
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

		
	if(NULL == (file = fopen(argv[1], "r")))
	{
		printf("ERROR: Can't open the input file.\n");
		exit(1);
	}

	nPoints = getNumPoints(file);

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

	printf("Raster size: %d * %d * %d * %d\n", nX, nY, nX, nY);
	printf("Number of points: %d\n", nPoints);
	printf("xMax = %lf\t, xMin = %lf\n", xMax, xMin);
	printf("yMax = %lf\t, yMin = %lf\n", yMax, yMin);
	readFile(file, x1, y1, x2, y2, nPoints);
	fclose(file);
/*
	for(int i = 0; i < nPoints; i++)
	{
		printf("%d\t,%d\t,%d\t,%d\n", (int)((x1[i] - xMin) / cellSize), (int)((y1[i] - yMin) / cellSize), (int)((x2[i] - xMin) / cellSize), (int)((y2[i] - yMin) / cellSize));
	}
*/
	double ** totalCells;
	int nTotalCells = nX * nX * nY * nY;
	if(NULL == (totalCells = (double **) malloc (nX * nY * sizeof(double *))))
	{
		printf("ERROR: Out of memory at line %d in file %s\n", __LINE__, __FILE__);
		exit(1);
	}
	for(int i = 0; i < nX * nY; i++)
	{
		if(NULL == (totalCells[i] = (double *) malloc (nX * nY * sizeof(double))))
		{
			printf("ERROR: Out of memory at line %d in file %s\n", __LINE__, __FILE__);
			exit(1);
		}
		for(int j = 0; j < nX * nY; j++)
		{
			totalCells[i][j] = 0.0;
		}
	}

	KDECalc(x1, y1, x2, y2, nPoints, xMin + 0.5 * cellSize, yMin + 0.5 * cellSize, xMin + 0.5 * cellSize, yMin + 0.5 * cellSize, nX, nY, nX, nY, cellSize, bandwidth, totalCells);

	

	free(x1);
	free(y1);
	free(x2);
	free(y2);

	double Max = totalCells[0][0];
	int maxI = 0, maxJ = 0;
	for(int i = 0; i < nX * nY; i++)
	{
		for(int j = 0; j < nX * nY; j++)
		{
			if(totalCells[i][j] > Max)
			{
		//		printf("%d,%d\n", i, j);
				Max = totalCells[i][j];
				maxI = i;
				maxJ = j;
			}
		}
	}

	int cell_X1, cell_X2, cell_Y1, cell_Y2;
	
	cell_X1 = maxJ % nX;
	cell_Y1 = maxJ / nX;	

	cell_X2 = maxI % nX;
	cell_Y2 = maxI / nX;

	printf("Locations with the highest flow density: %lf\n", Max);
	printf("\tOrigin:\n");
	printf("\tCellID: %d\t,%d\n", cell_X1, cell_Y1);
	printf("\tLocation: %lf\t,%lf\n", xMin + (cell_X1 + 0.5) * cellSize, yMin + (cell_Y1 + 0.5) * cellSize);
	printf("\tDestination:\n");
	printf("\tCellID: %d\t,%d\n", cell_X2, cell_Y2);
	printf("\tLocation: %lf\t,%lf\n", xMin + (cell_X2 + 0.5) * cellSize, yMin + (cell_Y2 + 0.5) * cellSize);



	if(NULL == (file = fopen(argv[2], "wb")))
	{
		printf("ERROR: Can't open the output file.\n");
		exit(1);
	}

	for(int i = 0; i < nX * nY; i++)
	{
		writeFile(file, totalCells[i], nX * nY);
		free(totalCells[i]);
	}

	fclose(file);

	free(totalCells);
	printf("Finished.\n");	
	return 0;
}
