#ifndef IOH
#define IOH
int getNumPoints(FILE * file);
void readFile(FILE * file, double * x1, double * y1, double * x2, double * y2, int nPoints);
void writeFile(FILE * file, double * values, int nCells);

#endif
