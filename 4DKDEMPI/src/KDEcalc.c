#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void KDECalc(double * x1, double * y1, double * x2, double * y2, int nPoints, double x1Min, double y1Min, double x2Min, double y2Min, int nX1, int nY1, int nX2, int nY2, double cellSize, double bandwidth, double * grids)
{
	double dataYMin = y2Min - bandwidth;
	double dataYMax = y2Min + (nY2 - 1) * cellSize + bandwidth;

	double x1Diff, y1Diff, x2Diff, y2Diff;
	int x1MinI, x1MaxI, y1MinI, y1MaxI, x2MinI, x2MaxI, y2MinI, y2MaxI;

	int X2Stride = nX1 * nY1;
	int Y2Stride = nX1 * nY1 * nX2;

	double distance2_2D;
	double bandwidth2 = bandwidth * bandwidth;
	double weight_2D;

	for(int i = 0; i < nPoints; i++)
	{
		if(y2[i] > dataYMin && y2[i] < dataYMax)
		{
			x1MinI = ceil((x1[i] - bandwidth - x1Min) / cellSize);
			x1MaxI = (x1[i] + bandwidth - x1Min) / cellSize + 1;
			y1MinI = ceil((y1[i] - bandwidth - y1Min) / cellSize);
			y1MaxI = (y1[i] + bandwidth - y1Min) / cellSize + 1;
			x2MinI = ceil((x2[i] - bandwidth - x2Min) / cellSize);
			x2MaxI = (x2[i] + bandwidth - x2Min) / cellSize + 1;
			y2MinI = ceil((y2[i] - bandwidth - y2Min) / cellSize);
			y2MaxI = (y2[i] + bandwidth - y2Min) / cellSize + 1;

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

			x1Diff = x1Min + cellSize * x1MinI - x1[i];
			y1Diff = y1Min + cellSize * y1MinI - y1[i];
			x2Diff = x2Min + cellSize * x2MinI - x2[i];
			y2Diff = y2Min + cellSize * y2MinI - y2[i];

			for(int iY2 = y2MinI; iY2 < y2MaxI; iY2 ++)
			{
				for(int iX2 = x2MinI; iX2 < x2MaxI; iX2 ++)
				{
					distance2_2D = x2Diff * x2Diff + y2Diff * y2Diff;
					if(distance2_2D < bandwidth2)
					{
						x2Diff += cellSize;
						continue;
					}
					weight_2D = 1 - distance2_2D / bandwidth2;

					for(int iY1 = y1MinI; iY1 < y1MaxI; iY1 ++)
					{
						for(int iX1 = x1MinI; iX1 < x1MaxI; iX1 ++)
						{
							distance2_2D = x1Diff * x1Diff + y1Diff * y1Diff;
							if(distance2_2D < bandwidth2)
							{
								x1Diff += cellSize;
								continue;
							}
							////
							grids[iX1 + iY1 * nX1 + iX2 * X2Stride + iY2 * Y2Stride] += weight_2D * (1 - distance2_2D / bandwidth2);
							x1Diff += cellSize;
						}
						y1Diff += cellSize;
					}
					x2Diff += cellSize;
				}
				y2Diff += cellSize;
			}
		}
	}

	return;
}
