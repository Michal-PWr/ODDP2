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

int ObliczNastepnikowTechnologicznych(int *T, int iloscStanowisk, int iloscZadan, int *kolejnoscZadan)
{
	for (int i = 1; i <= iloscZadan * iloscStanowisk + 2 * iloscStanowisk; i++)
	{
		if ((i % iloscStanowisk) == 0)
			T[i] = 0;
		else
			T[i] = i + 1;
	}

	return 0;
}
int ObliczLiczbePoprzenikow(int *LP, int *A, int *T, int n)
{
	int i;
	for (i = 1; i <= n; i++)
	{
		LP[A[i]]++; //OBLICZANIE liczby poprzeników
		LP[T[i]]++;
	}
	return 0;
}
int WyznaczKolejke(int *Q, int *LP, int *A, int *T, int n)
{
	int b = 0, e = 0, i, ns, o;
	for (i = 1; i <= n; i++)
	{
		if (LP[i] == 0) //WRZUCENIE DO KOLEJKI TYCH KTÓRE NIE MAJ¥ POPRZEDNIKÓW
			Q[++e] = i;
	}
	while (b != e) // DOPISANIE DO KOLEJKI
	{
		o = Q[++b];
		ns = T[o];
		if (ns != 0)
		{
			LP[ns]--;
			if (LP[ns] == 0)
				Q[++e] = ns;
		}
		ns = A[o];
		if (ns != 0)
		{
			LP[ns]--;
			if (LP[ns] == 0)
				Q[++e] = ns;
		}
	}
	return 0;
}

int ObliczCzasyZakonczeniaiPh(int *C, int *Ph, int *Q, int *Pm, int *Pt, int *T, int *A, int *P, int n)
{
	int i,j;
	for (j = 1; j <= n; j++)	//Obliczenie czasu zakoñczenia procesów
	{
		i = Q[j];
		Pt[T[i]] = i;
		Pm[A[i]] = i;
		C[i] = fmax(C[Pm[i]], C[Pt[i]]) + P[i];
		if (C[Pm[i]] > C[Pt[i]])
			Ph[i] = Pm[i];
		else
			Ph[i] = Pt[i];
	}
	return 0;
}

int ZnajdzCMax(int *C, int n)
{
	int max = 0, indexCmax = 0;
	for (int i = 1; i <= n; i++) //Znajdowanie procesu o maksymalnym czasie zakoñczenia
	{
		if (C[i] > max)
		{
			max = C[i];
			indexCmax = i;
		}
	}
	return indexCmax;
}

int ObliczCmaxZPermutacji(int *a, int *t, int *p, int *indexCmax, int *ph, int *pm, int *pt, int n)
{
	int LP[MAX_ZADAN*MAX_STANOWISK+1] = { 0 }, C[MAX_ZADAN*MAX_STANOWISK+1] = {0}, Q[MAX_ZADAN*MAX_STANOWISK+1] = {0};
	ObliczLiczbePoprzenikow(LP, a, t, n);
	WyznaczKolejke(Q, LP, a, t, n);
	ObliczCzasyZakonczeniaiPh(C, ph, Q, pm, pt, t, a, p, n);

	int index = ZnajdzCMax(C, n);
	*indexCmax = index;
	return C[*indexCmax];
}

int ZnajdzSziezkeKrytyczna(int *Sk, int *Ph, int index_Cmax)
{
	int i, iloscWSciezce;
	for (i = 1, iloscWSciezce = 0; index_Cmax != 0; i++, iloscWSciezce++) //Znajdowanie œcie¿ki krytycznej
	{
		Sk[i] = index_Cmax;
		index_Cmax = Ph[index_Cmax];
	}
	for (i = 1; i <= iloscWSciezce / 2; i++) // Odwrocenie sciezki krytycznej, aby bylo wygodniej
	{
		int temp = Sk[i];
		Sk[i] = Sk[iloscWSciezce - i + 1];
		Sk[iloscWSciezce - i + 1] = temp;
	}
	return iloscWSciezce;
}

int ObliczPoczatkiIKonceBlokow(int *PiKBloku, int *Pt, int *Sk, int *Pm, int *A, int *T, int iloscWSciezce)
{
	for (int i = 2; i < iloscWSciezce; i++) // Obliczanie poczatkow i koncow blokow
	{
		if ((Pt[Sk[i]] == Sk[i - 1]) && (A[Sk[i]] == Sk[i + 1]))
			PiKBloku[i] = 1;
		else if ((Pm[Sk[i]] == Sk[i - 1]) && (T[Sk[i]] == Sk[i + 1]))
			PiKBloku[i] = 2;
		else
			PiKBloku[i] = 0;
	}
	return 0;
}

int main()
{
	FILE *fp;
	int A[MAX_ZADAN*MAX_STANOWISK+1] = {0}; // nastepnicy maszynowi
	int T[MAX_ZADAN*MAX_STANOWISK+1] = {0}; // nastepnicy technologiczni
	int czasyZadan[MAX_ZADAN*MAX_STANOWISK+1] = {0};
	int kolejnoscZadan[MAX_ZADAN*MAX_STANOWISK+2*MAX_STANOWISK] = {0}; // jest to tablica zawieraj¹ca kolejnosc wykonywania zadan na maszynach. '-1' oddziela maszyny
	int Pt[MAX_ZADAN*MAX_STANOWISK+1] = { 0 }, Pm[MAX_ZADAN*MAX_STANOWISK+1] = { 0 }, Ph[MAX_ZADAN*MAX_STANOWISK+1] = { 0 };
	int Sk[MAX_ZADAN*MAX_STANOWISK+1];
	int PiKBloku[MAX_ZADAN*MAX_STANOWISK+1] = { 0 };
	int iloscZadan, iloscStanowisk, Cmax_min, indexCmax, iloscWSciezce;

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
	for (int i = 0; i<= iloscStanowisk*iloscZadan; i++ )
	{
		printf("%d ", A[i]);
	}
	printf("\n");
	ObliczNastepnikowTechnologicznych(T, iloscStanowisk, iloscZadan, kolejnoscZadan);
	for (int i = 0; i<= iloscStanowisk*iloscZadan; i++ )
	{
		printf("%d ", T[i]);
	}
	printf("\n");

	Cmax_min = ObliczCmaxZPermutacji(A, T, czasyZadan, &indexCmax, Ph, Pm, Pt, iloscStanowisk * iloscZadan);
	printf("%d\n", Cmax_min);
	iloscWSciezce = ZnajdzSziezkeKrytyczna(Sk, Ph, indexCmax);
	printf("%d\n", iloscWSciezce);
	ObliczPoczatkiIKonceBlokow(PiKBloku, Pt, Sk, Pm, A, T, iloscWSciezce);
	for (int i=0; i<iloscWSciezce; i++)
		printf("%d", PiKBloku[i]);
	return 0;
}
