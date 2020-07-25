#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct Nod {
	int cheie;
	int info;
}Nod;

typedef struct Arc {
	int index_1;
	int index_2;
}Arc;

#define Max 100

typedef struct Graf {
	int nr_noduri;
	Nod noduri[Max];
	int Arce[Max][Max];
}Graf;

Graf * initializareGraf()
{
	Graf* g;
	g = malloc(sizeof(Graf));
	g->nr_noduri = 1;
	return g;
}

void AddNod(Graf* g, Nod* nod)
{
	int index = g->nr_noduri;
	g->noduri[index] = *nod;
	g->nr_noduri++;
	int i;
	for (i = 1; i < g->nr_noduri; i++)
		g->Arce[i][index] = g->Arce[index][i] = 0;
}

void AddArce(Graf* g, Arc* arc)
{
	g->Arce[arc->index_1][arc->index_2] = g->Arce[arc->index_2][arc->index_1] = 1;
}



void PrintGraf(Graf* g)
{
	int i, j;
	for (i = 1; i < g->nr_noduri; i++)
	{
		for (j = 1; j < g->nr_noduri; j++)
			printf("%d ", g->Arce[i][j]);
		printf("\n");
	}
}


Graf * citestefisier(char s[20] , Graf *g)
{
	FILE *f;
	f = fopen(s, "r");
	int n, m,x,y;
	fscanf(f, "%d %d", &n, &m);
	Nod* nod=malloc(sizeof(Nod));
	Arc* a=malloc(sizeof(Arc));
	for (int i = 0; i < n; i++)
		AddNod(g,nod);
	for (int i = 0; i < m; i++)
	{
		fscanf(f, "%d %d", &x, &y);
		a->index_1 = x;
		a->index_2 = y;
		AddArce(g, a);
	}

	

	fclose(f);
	free(nod);
	free(a);
	return g;
}

int nrcrt1;
int* aparente1;
int* vizitate1;

void ParcurgereA(Graf* g, int index)
{
	vizitate1[nrcrt1++] = index;
	aparente1[index] = 1;

	for(int i=0;i<g->nr_noduri;i++)
		if (g->Arce[i][index] == 1 && !aparente1[i])
		{
			ParcurgereA(g, i);
		}

	
}

int *ParcurgereAdancime(Graf* g, int index)
{
	aparente1 = malloc(sizeof(int) * (g->nr_noduri));
	vizitate1 = malloc(sizeof(int) * (g->nr_noduri));
	nrcrt1 = 0;
	for (int i = 0; i < g->nr_noduri; i++)
	{aparente1[i]=vizitate1[i]=0; }
	ParcurgereA(g, index);
	return vizitate1;
}
int* aparente2;
int* vizitate2;
int nrcrt2;

void ParcurgereC(Graf* g, int index)
{
	int k=0;
	int* viz = malloc(sizeof(int) * (g->nr_noduri));
	for (int i = 0; i < g->nr_noduri; i++)
		if (g->Arce[index][i] == 1 && !aparente2[i])
		{
			viz[k++] = i;
			aparente2[i] = 1;
			vizitate2[nrcrt2++] = i;
		}
	for (int i = 0; i < k; i++)
		ParcurgereC(g, viz[i]);
	free(viz);
	
}

int * parcurgereCuprindere(Graf* g, int index)
{
	aparente2 = malloc(sizeof(int) * (g->nr_noduri));
	vizitate2 = malloc(sizeof(int) * (g->nr_noduri));
	nrcrt2 = 0;
	for (int i = 0; i < g->nr_noduri; i++)
	{
		aparente2[i] = vizitate2[i] = 0;
	}
	aparente2[index] = 1;
	vizitate2[nrcrt2++] = index;
	ParcurgereC(g, index);

	return vizitate2;	
}

void AdancimeegalCuprindere(Graf* g)
{
	printf("PUNCTUL 4)\n");
	int sw = 1;
	int* adancime = malloc(sizeof(int) * (g->nr_noduri));
	int* cuprindere = malloc(sizeof(int) * (g->nr_noduri));
	printf("adancime = parcurgere : ");
	for (int i = 1; i < g->nr_noduri; i++)
	{
		adancime = ParcurgereAdancime(g, i);
		cuprindere = parcurgereCuprindere(g, i);

		for (int j = 0; j < g->nr_noduri - 1; j++)
			if (adancime[j] != cuprindere[j])
				sw = 0;
		
		if (sw)
		printf("%d", i);
		sw = 1;
	}
	printf("\n");
	free(adancime);
	free(cuprindere);
}
void printaresuccesiuniadancime(Graf* g)
{
	printf("PUNCTUL 1) \n");
	int* x = malloc(sizeof(g->nr_noduri));
	for (int i = 1; i < g->nr_noduri; i++)
	{
		printf("Din punctul %d\n", i);
		x = ParcurgereAdancime(g, i);

		for (int j = 0; j < g->nr_noduri - 1; j++)
			printf("%d ", x[j]);
		printf("\n");

	}
	free(x);
}

int cycle(Graf* g, int index,int *trt,int *avz,int father,int nrcrt)
{
	trt[nrcrt++] = index;
	avz[index] = 1;
	for (int i = 1; i < g->nr_noduri; i++)
		if (g->Arce[index][i] = 1 && avz[i] && i != father)
			return 1;
		else
			if (g->Arce[index][i] && !avz[i])
				return cycle(g, i, trt, avz, index, nrcrt);
	return 0;
}

void ciclesverify(Graf* g)
{
	printf("Punctul 2) \n");
	int sw=0;
	int nrcrt = 0;
	int* trt = malloc(sizeof(int) * (g->nr_noduri));
	int* avz = malloc(sizeof(int) * (g->nr_noduri));
	for (int i = 0; i < g->nr_noduri; i++)
		trt[i] = avz[i] = 0;
	for (int i = 1; i < g->nr_noduri; i++)
	{
		sw = cycle(g, i, trt, avz, -1, nrcrt);
		if (sw)
		{
			printf("exista ciclu\n");
			i = g->nr_noduri;
		}
	}
	free(trt);
	free(avz);
}

void parc(Graf* g, int index1, int index2, int k, int *avs, int *trt,int nrcrt)
{
	if (k >= 0) {
		trt[nrcrt++] = index1;
		avs[index1] = 1;


		if (index1 == index2 && k == 0)
		{
			
			for (int i = 0; i < nrcrt; i++)
				printf("%d ", trt[i]);
			printf("\n");

		}
		for (int i = 1; i < g->nr_noduri; i++)
			if (g->Arce[index1][i] && !avs[i])
				parc(g, i, index2, k - 1, avs, trt, nrcrt);
	}

}

void lanturi(Graf* g, int index1, int index2, int k)
{
	printf("Punctul 3)\n");
	printf("Lantul de la %d la %d cu %d pasi este :",index1,index2,k);
	int nrcrt = 0;
	int* avs = malloc(sizeof(int) * (g->nr_noduri));
	int* trt = malloc(sizeof(int) * (g->nr_noduri));
	for (int i = 0; i < g->nr_noduri; i++)
		avs[i] = trt[i] = 0;
	parc(g, index1, index2, k, avs, trt, nrcrt);

	free(avs);
	free(trt);
}
void text()
{
	printf("Pentru printarea succesiunii in adancime apasati 1)\n");
	printf("Pentru verificarea de cicluri si afisarea acestora apasati tasta 2)\n");
	printf("Pentru verificare de lanturi de la un punct la altul apasati 3)\n");
	printf("Pentru verificarea adancimii prin cuprindere apasati 4)\n");
	printf("Pentru terminarea programului apasti tasta x\n");
	printf("\n");
}

int main()
{
	Graf* g = NULL;
	g = initializareGraf();
	g = citestefisier("graf.txt", g);
	char c;
	text();
	while ((c = getchar()) != 'x')
	{
		if(c=='1')
		printaresuccesiuniadancime(g);
		else
		if(c=='2')
			ciclesverify(g);
		else
			if (c == '3')
			{
				int x, y, z;
				printf("plecare de la ");
				scanf("%d", &x);
				printf("ajunge la ");
				scanf("%d", &y);
				printf("in atatia pasi ");
				scanf("%d", &z);
				lanturi(g, x, y, z);
			}
		else
			if(c=='4')
		AdancimeegalCuprindere(g);
			else
				text();

		printf("\n");
	}
	
	
}