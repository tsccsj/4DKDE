#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void KDECalc(double * x1, double * y1, double * x2, double * y2, int nPoints, double x1Min, double y1Min, double x2Min, double y2Min, int nX1, int nY1, int nX2, int nY2, double cellSize, double bandwidth, double ** grids)
{
	double x1Diff, y1Diff, x2Diff, y2Diff;
	int x1MinI, x1MaxI, y1MinI, y1MaxI, x2MinI, x2MaxI, y2MinI, y2MaxI;

//	int total = nX1 * nX2 * nY1 * nY2;
//	int x1I, x2I, y1I, y2I;

	double distance2_2D;
	double bandwidth2 = bandwidth * bandwidth;
	double weight_2D;

//	printf("x1Min = %lf\t, x2Min = %lf\n", x1Min, x2Min);
//	printf("y1Min = %lf\t, y2Min = %lf\n", y1Min, y2Min);

	for(int i = 0; i < nPoints; i++)
	{
		x1MinI = ceil((x1[i] - bandwidth - x1Min) / cellSize);
		x1MaxI = (x1[i] + bandwidth - x1Min) / cellSize + 1;
		y1MinI = ceil((y1[i] - bandwidth - y1Min) / cellSize);
		y1MaxI = (y1[i] + bandwidth - y1Min) / cellSize + 1;
		x2MinI = ceil((x2[i] - bandwidth - x2Min) / cellSize);
		x2MaxI = (x2[i] + bandwidth - x2Min) / cellSize + 1;
		y2MinI = ceil((y2[i] - bandwidth - y2Min) / cellSize);
		y2MaxI = (y2[i] + bandwidth - y2Min) / cellSize + 1;
/*
		x1I = (x1[i] - x1Min) / cellSize + 1;
		x2I = (x2[i] - x2Min) / cellSize + 1;
		y1I = (y1[i] - y1Min) / cellSize + 1;
		y2I = (y2[i] - y2Min) / cellSize + 1;

		printf("%3d,%3d,%3d,%3d\n", x1I, y1I, x2I, y2I);
		printf("%3d,%3d,%3d,%3d\n", x1MinI, y1MinI, x2MinI, y2MinI);
		printf("%3d,%3d,%3d,%3d\n\n", x1MaxI, y1MaxI, x2MaxI, y2MaxI);
*/
		if(x1MinI < 0)
			x1MinI = 0;
		if(y1MinI < 0)
			y1MinI = 0;
		if(x2MinI < 0)
			x2MinI = 0;
		if(y2MinI < 0)
			y2MinI = 0;
		if(x1MaxI > nX1)
			x1MaxI = nX1;
		if(y1MaxI > nY1)
			y1MaxI = nY1;
		if(x2MaxI > nX2)
			x2MaxI = nX2;
		if(y2MaxI > nY2)
			y2MaxI = nY2;

		for(int iY2 = y2MinI; iY2 < y2MaxI; iY2 ++)
		{
			y2Diff = y2Min + cellSize * iY2 - y2[i];

			for(int iX2 = x2MinI; iX2 < x2MaxI; iX2 ++)
			{
				x2Diff = x2Min + cellSize * iX2 - x2[i];

				distance2_2D = x2Diff * x2Diff + y2Diff * y2Diff;
				if(distance2_2D >= bandwidth2)
				{
					continue;
				}
				weight_2D = 1 - distance2_2D / bandwidth2;

				for(int iY1 = y1MinI; iY1 < y1MaxI; iY1 ++)
				{
					y1Diff = y1Min + cellSize * iY1 - y1[i];
					for(int iX1 = x1MinI; iX1 < x1MaxI; iX1 ++)
					{
						x1Diff = x1Min + cellSize * iX1 - x1[i];
						distance2_2D = x1Diff * x1Diff + y1Diff * y1Diff;
						if(distance2_2D >= bandwidth2)
						{
							continue;
						}
						////
//						int index = iX1 + iY1 * nX1 + iX2 * X2Stride + iY2 * Y2Stride;
//						if(index >= total)
//							printf("\t%d,\t%d,\t%d,\t%d\n", iX1, iY1, iX2, iY2);
//						else
						grids[iX2 + iY2 * nX2][iX1 + iY1 * nX1] += weight_2D * (1 - distance2_2D / bandwidth2);
					}
				}
			}
		}
	}

	return;
}
