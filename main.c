/*
 * main.c
 *
 *  Created on: 20 paŸ 2016
 *      Author: Micha³
 */
#include <stdio.h>			// T - nastêpnicy technologiczni

#define MAX_ZADAN 100			// P - czasy wykonania operacji
#define MAX_STANOWISK 20

int WczytajZPliku(FILE *fp, int *iloscZadan, int *iloscStanowisk, int *czasyZadan)
{
	int i;
	fscanf(fp, "%d", iloscZadan);
	fscanf(fp, "%d", iloscStanowisk);
	for (i = 1; i <= (*iloscZadan) * (*iloscStanowisk); i++)
	{
		fscanf(fp, "%d", &czasyZadan[i]);
	}
	return 0;
}
void UstawPoczatkowaPermutacje(int *kolejnoscZadan, int iloscZadan, int iloscStanowisk)
{
	//Wszystkie zadania po kolei tylko na jednej maszynie ze stanowiska
	int przesuniecie = 1;
	for (int i =0, j = 0; j<iloscStanowisk; ++j)
	{
		for (i = 0; i< iloscZadan; ++i)
		{
			kolejnoscZadan[j*iloscZadan+i+przesuniecie] = (i*iloscStanowisk + j + 1);
		}
		kolejnoscZadan[j*i+i+przesuniecie] = 0;
		przesuniecie++;
		kolejnoscZadan[j*i+i+przesuniecie] = 0;
		przesuniecie++;
	}
}

int ObliczNastepnikowMaszynowych(int *A, int iloscStanowisk, int iloscZadan, int *kolejnoscZadan)
{
	for (int i = 1; i <= iloscZadan * iloscStanowisk + 2 * iloscStanowisk; i++)
	{
		A[kolejnoscZadan[i]] = kolejnoscZadan[i+1];
	}
	return 0;
}
int main()
{
	FILE *fp;
	int A[MAX_ZADAN*MAX_STANOWISK+1] = {0}; // nastepnicy maszynowi
	int czasyZadan[MAX_ZADAN*MAX_STANOWISK+1] = {0};
	int kolejnoscZadan[MAX_ZADAN*MAX_STANOWISK+2*MAX_STANOWISK] = {0}; // jest to tablica zawieraj¹ca kolejnosc wykonywania zadan na maszynach. '-1' oddziela maszyny
	int iloscZadan, iloscStanowisk;

	fp = fopen("NEH1.DAT", "r");
	WczytajZPliku(fp, &iloscZadan, &iloscStanowisk, czasyZadan);
	for (int i=0; i<=(iloscZadan*iloscStanowisk); i++)
	{
		printf("%d ", czasyZadan[i]);
	}
	printf("\n");
	UstawPoczatkowaPermutacje(kolejnoscZadan, iloscZadan, iloscStanowisk);
	for (int i=0; i<iloscStanowisk*iloscZadan+2*iloscStanowisk; i++)
		printf("%d ",kolejnoscZadan[i]);
	ObliczNastepnikowMaszynowych(A, iloscStanowisk, iloscZadan, kolejnoscZadan);
	printf("\n");
	for (int i = 1; i<= iloscStanowisk*iloscZadan; i++ )
	{
		printf("%d ", A[i]);
	}
	return 0;
}
