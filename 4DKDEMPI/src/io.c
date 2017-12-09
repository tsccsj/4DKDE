#include <stdio.h>
#include <stdlib.h>

int getNumPoints(FILE * file)
{
	char buffer[100];
	
	int count = 0;
	rewind(file);

	fgets(buffer, 100, file);

	while(0 != fgets(buffer, 100, file))
	{
		count ++;
	}

	return count;
}

void readFile(FILE * file, double * x1, double * y1, double * x2, double * y2, int nPoints)
{
	char buffer[100];

	rewind(file);
	fgets(buffer, 100, file);

	int i;	
	for(i = 0; i < nPoints; i++)
	{
		if(EOF == fscanf(file, "%lf,%lf,%lf,%lf\n", x1 + i, y1 + i, x2 + i, y2 + i))
		{
			printf("Incorrect points number\n");
			exit(1);
		}
	}
	return;
}

void writeFile(FILE * file, double * values, int nCells)
{	
	fwrite(values, sizeof(double), nCells, file);
	return;
} 
