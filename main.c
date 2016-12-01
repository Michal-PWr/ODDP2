/*
 * main.c
 *
 *  Created on: 20 pa� 2016
 *      Author: Micha�
 */
#include <stdio.h>			// T - nast�pnicy technologiczni

#define MAX_ZADAN 100			// P - czasy wykonania operacji
#define MAX_STANOWISK 20

void ZamienKolejnosc(int *A, int *Pm, int a, int b)
{
	int c = Pm[a];
	int d = A[b];
	A[c] = b;
	A[a] = d;
	A[b] = a;
	Pm[a] = b;
	Pm[b] = c;
	Pm[d] = a;
}
int WczytajZPliku(FILE *fp, int *iloscZadan, int *iloscStanowisk, int czasyZadan[MAX_ZADAN][MAX_STANOWISK])
{
	int i;
	fscanf(fp, "%d", iloscZadan);
	fscanf(fp, "%d", iloscStanowisk);
	for (i = 0; i < *iloscZadan; i++)
	{
		for (int j = 0; j < *iloscStanowisk; j++)
		{
			fscanf(fp, "%d", &czasyZadan[i][j]);
		}
	}
	return 0;
}

int main()
{
	FILE *fp;
	int czasyZadan[MAX_ZADAN][MAX_STANOWISK] = {{0}};
	int iloscZadan, iloscStanowisk;

	fp = fopen("NEH9.DAT", "r");
	WczytajZPliku(fp, &iloscZadan, &iloscStanowisk, czasyZadan);
	for (int i=0; i<iloscZadan; i++)
	{
		for (int j = 0; j<iloscStanowisk; j++)
		{
			printf("%d ", czasyZadan[i][j]);
		}
		printf("\n");
	}

	return 0;
}