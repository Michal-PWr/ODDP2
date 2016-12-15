/*
 * main.c
 *
 *  Created on: 20 paŸ 2016
 *      Author: Micha³
 */
#include <stdio.h>			// T - nastêpnicy technologiczni
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
	int przesuniecie = 1; //Wszystkie zadania po kolei tylko na jednej maszynie ze stanowiska

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
	memset(A, 0, (MAX_ZADAN * MAX_STANOWISK + 1) * sizeof(int) );
	for (int i = 1; i <= iloscZadan * iloscStanowisk + 2 * iloscStanowisk; i++)
	{
		A[kolejnoscZadan[i]] = kolejnoscZadan[i+1];
	}
	return 0;
}

int ObliczNastepnikowTechnologicznych(int *T, int iloscStanowisk, int iloscZadan, int *kolejnoscZadan)
{
	memset(T, 0, (MAX_ZADAN * MAX_STANOWISK + 1) * sizeof(int) );
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
	memset(LP, 0, (MAX_ZADAN * MAX_STANOWISK + 1) * sizeof(int) );
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
	memset(Q, 0, (MAX_ZADAN * MAX_STANOWISK + 1) * sizeof(int) );
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

	memset(Pm, 0, (MAX_ZADAN * MAX_STANOWISK + 1) * sizeof(int) );
	memset(Pt, 0, (MAX_ZADAN * MAX_STANOWISK + 1) * sizeof(int) );
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

int ObliczCmaxZPermutacji(int *A, int *T, int *p, int *indexCmax, int *ph, int *pm, int *pt, int n)
{
	int temp = n;
	int LP[MAX_ZADAN*MAX_STANOWISK+1] = { 0 }, C[MAX_ZADAN*MAX_STANOWISK+1] = {0}, Q[MAX_ZADAN*MAX_STANOWISK+1] = {0};

	ObliczLiczbePoprzenikow(LP, A, T, temp);
	WyznaczKolejke(Q, LP, A, T, temp);
	ObliczCzasyZakonczeniaiPh(C, ph, Q, pm, pt, T, A, p, temp);

	int index = ZnajdzCMax(C, temp);
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

int IloscMiejscaZaBlokiem(int indeks, int *PiKBloku, int *Sk, int *A, int iloscWSciezce)
{
	int i,ilosc =0;

	for (i = indeks; PiKBloku[i] != 2 && i <= iloscWSciezce; i++)
	{
		if (PiKBloku[i] == 1)
			return 0;
	}
	if (i > iloscWSciezce)
		return 0; //nie ma miejsc za blokiem
	i = Sk[i];
	for ( ; A[i] != 0; ilosc++ )
	{
		i = A[i];
	}
	return ilosc+1;
}

int IloscMiejscaPrzedBlokiem(int indeks, int *PiKBloku, int *Sk, int *Pm, int iloscWSciezce, int *kolejnoscZadan)
{
	int i,ilosc =0;

	for (i = indeks; PiKBloku[i] != 1 && i > 0; i--)
	{
		if (PiKBloku[i] == 2)
			return 0;
	}
	if (i == 0)
		return 0; //nie ma miejsc przed blokiem
	for ( i = Sk[i]; Pm[i] != 0; ilosc++ )
	{
		i = Pm[i];
		if (ilosc > 1000)
		{
//			for (int i = 0; i < 10 * 100 + 2 * 10; i++)
//				printf("%d ",kolejnoscZadan[i]);
		}
			;
	}
	return ilosc+1;
}
int IloscMiejscaNaDrugiejMaszynie(int nrZadania, int *kolejnoscZadan, int iloscZadan, int iloscStanowisk)
{
	int ilosc = 0, poczatek;
	int jestNaPierwszej = 0;
	if ((nrZadania % iloscStanowisk) == 0)
		poczatek = (iloscStanowisk - 1) * (iloscZadan + 2) + 1;
	else
		poczatek = ((nrZadania - 1) % iloscStanowisk) * (iloscZadan + 2) + 1;
	for (int i = poczatek ; kolejnoscZadan[i] != 0; i++)
	{
		if (kolejnoscZadan[i] == nrZadania)
			jestNaPierwszej = 1;
		ilosc++;
	}
	if (jestNaPierwszej == 1)
	{
		return iloscZadan - ilosc + 1;
	}
	return ilosc + 1;
}
int PrzesunONZaBlok(int indeks, int *Sk, int *PiKBloku, int n, int *kolejnoscZadan )
{
	int nrZadania = Sk[indeks];
	int ileDoKoncaBloku;
	for (ileDoKoncaBloku = 0; PiKBloku[indeks] != 2; indeks++)
	{
		ileDoKoncaBloku++;
	}
	for (indeks = 0; kolejnoscZadan[indeks] != nrZadania ; indeks++);
	memmove(&kolejnoscZadan[indeks], &kolejnoscZadan[indeks + 1], sizeof(int)*(ileDoKoncaBloku+n));
	kolejnoscZadan[indeks+ileDoKoncaBloku+n] = nrZadania;
	return 0;
}
int PrzesunONPrzedBlok(int indeks, int *Sk, int *PiKBloku, int n, int *kolejnoscZadan )
{
	int nrZadania = Sk[indeks];
	int ileDoPoczatkuBloku;
	for (ileDoPoczatkuBloku = 0; PiKBloku[indeks] != 1; indeks--)
	{
		ileDoPoczatkuBloku++;
	}
	for (indeks = 0; kolejnoscZadan[indeks] != nrZadania ; indeks++);
	memmove(&kolejnoscZadan[indeks-ileDoPoczatkuBloku-n+1], &kolejnoscZadan[indeks-ileDoPoczatkuBloku-n], sizeof(int)*(ileDoPoczatkuBloku+n));
	kolejnoscZadan[indeks-ileDoPoczatkuBloku-n] = nrZadania;
	return 0;
}
int PrzesunNaNNaDrugiejMaszynie (int nrZadania, int *kolejnoscZadan, int n, int iloscZadan, int iloscStanowisk)
{
	int indeksZadania, indeksZera, poczatek;
	if ((nrZadania % iloscStanowisk) == 0)
		poczatek = (iloscStanowisk - 1) * (iloscZadan + 2) + 1;
	else
		poczatek = ((nrZadania - 1) % iloscStanowisk) * (iloscZadan + 2) + 1;
	for (int i = poczatek; i <= poczatek + iloscZadan; i++)
	{
		if (kolejnoscZadan[i] == nrZadania)
			indeksZadania = i;
		if (kolejnoscZadan[i] == 0)
			indeksZera = i;
	}

	if (indeksZadania < indeksZera) //jest na pierwszej maszynie
	{
		memmove(&kolejnoscZadan[indeksZadania], &kolejnoscZadan[indeksZadania+1], sizeof(int) * (indeksZera - indeksZadania + n) );
		kolejnoscZadan[indeksZera + n] = nrZadania;
	}
	else //jest na drugiej maszynie
	{
		memmove(&kolejnoscZadan[poczatek + n + 1], &kolejnoscZadan[poczatek + n], sizeof(int) * (indeksZadania - poczatek - n));
		kolejnoscZadan[poczatek + n] = nrZadania;
	}
	return 0;
}
double generatorLiczbLosowych()
{
	double temp = rand() ;
	return temp / RAND_MAX;
}
int main()
{
	FILE *fp;
	int A[MAX_ZADAN*MAX_STANOWISK+1] = {0}; // nastepnicy maszynowi
	int T[MAX_ZADAN*MAX_STANOWISK+1] = {0}; // nastepnicy technologiczni
	int czasyZadan[MAX_ZADAN*MAX_STANOWISK+1] = {0};
	int kolejnoscZadan[MAX_ZADAN*MAX_STANOWISK+2*MAX_STANOWISK+1] = {0};
	int kolejnoscZadanKopia[MAX_ZADAN*MAX_STANOWISK+2*MAX_STANOWISK+1] = {0};
	int kolejnoscZadanNajlepsza[MAX_ZADAN*MAX_STANOWISK+2*MAX_STANOWISK+1] = {0};
	int Pt[MAX_ZADAN*MAX_STANOWISK+1] = { 0 }, Pm[MAX_ZADAN*MAX_STANOWISK+1] = { 0 }, Ph[MAX_ZADAN*MAX_STANOWISK+1] = { 0 };
	int Sk[MAX_ZADAN*MAX_STANOWISK+1];
	int PiKBloku[MAX_ZADAN*MAX_STANOWISK+1] = { 0 };
	int iloscZadan, iloscStanowisk, Cmax, Cmax_min, Cmax_poprzedni, indexCmax, iloscWSciezce;
	int iloscPrzed, iloscZa, iloscNaDrugiejMaszynie;
	double temperatura = 1000;
	double lambda = 0.99999;
	int delta;

	srand(time(NULL)); //Ziarno generatora liczb losowych

	fp = fopen("NEH9.DAT", "r");
	WczytajZPliku(fp, &iloscZadan, &iloscStanowisk, czasyZadan);

	UstawPoczatkowaPermutacje(kolejnoscZadan, iloscZadan, iloscStanowisk);
	ObliczNastepnikowMaszynowych(A, iloscStanowisk, iloscZadan, kolejnoscZadan);
	ObliczNastepnikowTechnologicznych(T, iloscStanowisk, iloscZadan, kolejnoscZadan);
	Cmax = ObliczCmaxZPermutacji(A, T, czasyZadan, &indexCmax, Ph, Pm, Pt, iloscStanowisk * iloscZadan);
	iloscWSciezce = ZnajdzSziezkeKrytyczna(Sk, Ph, indexCmax);
	ObliczPoczatkiIKonceBlokow(PiKBloku, Pt, Sk, Pm, A, T, iloscWSciezce);
	printf("%d\n", Cmax);
	Cmax_min = Cmax;
	Cmax_poprzedni = Cmax;
	memmove(kolejnoscZadanNajlepsza, kolejnoscZadan, sizeof(int) * (MAX_ZADAN*MAX_STANOWISK+2*MAX_STANOWISK));
	while (temperatura > 0.1)
	{
		memmove(kolejnoscZadanKopia, kolejnoscZadan, sizeof(int) * (MAX_ZADAN*MAX_STANOWISK+2*MAX_STANOWISK));
		int indeks = rand() % iloscWSciezce + 1; //element do zamiany

		if (PiKBloku[indeks] == 0) //ani poczatek ani koniec bloku
		{
			iloscPrzed = IloscMiejscaPrzedBlokiem(indeks, PiKBloku, Sk, Pm, iloscWSciezce, kolejnoscZadan);
			iloscZa = IloscMiejscaZaBlokiem(indeks, PiKBloku, Sk, A, iloscWSciezce);
			iloscNaDrugiejMaszynie = IloscMiejscaNaDrugiejMaszynie(Sk[indeks], kolejnoscZadan, iloscZadan, iloscStanowisk);
			int opcja = rand() % (iloscPrzed + iloscZa + iloscNaDrugiejMaszynie);
			if (opcja < iloscPrzed)
			{
				PrzesunONPrzedBlok(indeks, Sk, PiKBloku, opcja, kolejnoscZadan);
			}
			else if (opcja < iloscZa + iloscPrzed)
			{
				PrzesunONZaBlok(indeks, Sk, PiKBloku, opcja - iloscPrzed, kolejnoscZadan);
			}
			else
			{
				PrzesunNaNNaDrugiejMaszynie(Sk[indeks], kolejnoscZadan, opcja - iloscPrzed - iloscZa, iloscZadan, iloscStanowisk);
			}

		}
		if (PiKBloku[indeks] == 1) // poczatek bloku
		{
			iloscZa = IloscMiejscaZaBlokiem(indeks, PiKBloku, Sk, A, iloscWSciezce);
			iloscNaDrugiejMaszynie = IloscMiejscaNaDrugiejMaszynie(Sk[indeks], kolejnoscZadan, iloscZadan, iloscStanowisk);
			int opcja = rand() % ( iloscZa + iloscNaDrugiejMaszynie);
			if (opcja < iloscZa )
			{
				PrzesunONZaBlok(indeks, Sk, PiKBloku, opcja, kolejnoscZadan);
			}
			else
			{
				PrzesunNaNNaDrugiejMaszynie(Sk[indeks], kolejnoscZadan, opcja - iloscZa, iloscZadan, iloscStanowisk);
			}
		}
		if (PiKBloku[indeks] == 2) //koniec bloku
		{
			iloscPrzed = IloscMiejscaPrzedBlokiem(indeks, PiKBloku, Sk, Pm, iloscWSciezce, kolejnoscZadan);
			iloscNaDrugiejMaszynie = IloscMiejscaNaDrugiejMaszynie(Sk[indeks], kolejnoscZadan, iloscZadan, iloscStanowisk);
			int opcja = rand() % (iloscPrzed + iloscNaDrugiejMaszynie);
			if (opcja < iloscPrzed)
			{
				PrzesunONPrzedBlok(indeks, Sk, PiKBloku, opcja, kolejnoscZadan);
			}
			else
			{
				PrzesunNaNNaDrugiejMaszynie(Sk[indeks], kolejnoscZadan, opcja - iloscPrzed, iloscZadan, iloscStanowisk);
			}
		}


		//Mamy przesuniete teraz liczymy wszystko
		ObliczNastepnikowMaszynowych(A, iloscStanowisk, iloscZadan, kolejnoscZadan);
		ObliczNastepnikowTechnologicznych(T, iloscStanowisk, iloscZadan, kolejnoscZadan);
		Cmax = ObliczCmaxZPermutacji(A, T, czasyZadan, &indexCmax, Ph, Pm, Pt, iloscStanowisk * iloscZadan);
		delta = Cmax - Cmax_poprzedni;
		if (Cmax < Cmax_min)
		{
			Cmax_min = Cmax;
			memmove(kolejnoscZadanNajlepsza, kolejnoscZadan, sizeof(int) * (MAX_ZADAN*MAX_STANOWISK+2*MAX_STANOWISK));
		}
		if (delta <= 0) //nie gorzej - zamieniamy
		{
			iloscWSciezce = ZnajdzSziezkeKrytyczna(Sk, Ph, indexCmax);
			ObliczPoczatkiIKonceBlokow(PiKBloku, Pt, Sk, Pm, A, T, iloscWSciezce);
			Cmax_poprzedni = Cmax;
		}
		else //gorzej - moze zamieniamy
		{
			if (exp(-delta/temperatura) < generatorLiczbLosowych()) //Nie zmieniamy
			{
				memcpy(kolejnoscZadan, kolejnoscZadanKopia, sizeof(int) * (iloscZadan*iloscStanowisk+2*iloscStanowisk));
				ObliczNastepnikowMaszynowych(A, iloscStanowisk, iloscZadan, kolejnoscZadan);
				ObliczNastepnikowTechnologicznych(T, iloscStanowisk, iloscZadan, kolejnoscZadan);
				Cmax_poprzedni = ObliczCmaxZPermutacji(A, T, czasyZadan, &indexCmax, Ph, Pm, Pt, iloscStanowisk * iloscZadan);
			}
			else	//gorzej ale zamieniamy
			{
				iloscWSciezce = ZnajdzSziezkeKrytyczna(Sk, Ph, indexCmax);
				ObliczPoczatkiIKonceBlokow(PiKBloku, Pt, Sk, Pm, A, T, iloscWSciezce);
				Cmax_poprzedni = Cmax;
			}
		}
		printf("Cmax_min: %d temperatura: %f\n", Cmax_min, temperatura);
		temperatura *= lambda;
	}
	printf("Cmax_min: %d \n", Cmax_min);
	for (int i = 0; i < iloscStanowisk * iloscZadan + 2 * iloscStanowisk; i++)
		printf("%d ",kolejnoscZadanNajlepsza[i]);

	return 0;
}
