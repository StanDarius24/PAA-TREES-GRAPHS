#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Nod{
	int cheie;
struct Adj* arce;
struct Nod* urm;
}Nod;

typedef struct Adj {
	int cheie;
	struct Adj* urm;
}Adj;

typedef struct Graf {
	int nr_noduri;
	struct Nod* noduri;
}Graf;

typedef struct Arc {
	Nod* nod1;
	Nod* nod2;
}Arc;

Graf* InitializareGraf()
{
	Graf* g = malloc(sizeof(Graf));
	g->nr_noduri = 0;
	g->noduri = NULL;
	return g;
}
Nod* InitializareNod(int cheie)
{
	Nod* nod = malloc(sizeof(Nod));
	nod->cheie = cheie;
	nod->arce = NULL;
	nod->urm = NULL;
	return nod;
}

Graf * AdaugaNod(Graf* g, Nod* nod)
{
	g->nr_noduri++;
	nod->urm = g->noduri;
	g->noduri = nod;
	return g;
}
Graf* AdaugArc(Graf* g, Arc* arc)
{
	Adj* adc1 = malloc(sizeof(Adj));
	Adj* adc2 = malloc(sizeof(Adj));
	Nod *p = g->noduri;
	for (int i = 0; i < g->nr_noduri; i++)
	{
		if (arc->nod1->cheie == p->cheie)
		{
			
			adc1->cheie = arc->nod2->cheie;
			adc1->urm = p->arce;
			p->arce = adc1;
		}

		if (arc->nod2->cheie == p->cheie)
		{
		
			adc2->cheie = arc->nod1->cheie;
			adc2->urm = p->arce;
			p->arce = adc2;
		}
		p = p->urm;
	}
}

void printNoduri(Graf* g)
{
	Nod* p;
	p = g->noduri;
	for (int i = 0; i < g->nr_noduri; i++)
	{
		printf("%d ", p->cheie);
		p = p->urm;
	}
	printf("\n");
}

void printArce(Graf* g)
{
	Nod* p = g->noduri;
	Adj* q;
	for (int i = 1; i < g->nr_noduri; i++)
	{
		printf("cheie : %d\n", p->cheie);
		q = p->arce;
		if (q != NULL)
		{
			while (q != NULL)
			{
				printf("%d ", q->cheie);
				q = q->urm;
			}

			printf("\n");
		}
		p = p->urm;
	}
}

Graf* citire(Graf* g, char* s)
{
	FILE* f;
	f = fopen(s, "r");
	Nod* nod;
	Nod* nod1;
	int x, y;
	fscanf(f, "%d %d", &x, &y);
	for (int i = 1; i <= x; i++)
	{	
		nod = InitializareNod(i);
		g = AdaugaNod(g, nod);
	}
	int n, m;
	Arc* arc = malloc(sizeof(Arc));

	for (int i = 0; i < y; i++)
	{
		fscanf(f, "%d %d", &n,&m);
		nod = InitializareNod(n);
		nod1 = InitializareNod(m);
		arc->nod1 = nod;
		arc->nod2 = nod1;
		g = AdaugArc(g, arc);

	}
	fclose(f);
	return g;
}
int nrcrt;

void parcA(Graf* g, int index,int *tst, int *svs)
{	
	int i;
	svs[nrcrt++] = index;
	tst[index] = 1;
	Nod* p = g->noduri;
	for (i = 1; i <= g->nr_noduri; i++)
	{
		if (p->cheie == index)
			i = g->nr_noduri+1;
		else
		p = p->urm;
	}
	Adj* adj= p->arce;

	while (adj != NULL )
	{	
		if(!tst[adj->cheie])
		parcA(g, adj->cheie, tst, svs, nrcrt);
		adj = adj->urm;
	}

}


void parcurgereAdancime(Graf* g)
{
	
	int* tst = malloc(sizeof(int) * (g->nr_noduri));
	int* svs = malloc(sizeof(int) * (g->nr_noduri));
	

	for (int i = 1; i < g->nr_noduri; i++) {
		for (int j = 0; j <= g->nr_noduri; j++)
			tst[j] = svs[j] = 0;
		nrcrt = 0;
		printf("Din nodul %d", i);
		parcA(g, i, tst, svs, nrcrt);
		printf("\n");
		for (int k = 0; k < g->nr_noduri; k++)
			printf("%d ", svs[k]);
		printf("\n");
	}
	
}



void cycle(Graf* g, int index, int* trt, int* avz, int father, int nrcrt,int *sw)
{
	if (*sw == 0) {
		trt[nrcrt++] = index;
		avz[index] = 1;
		Nod* n = g->noduri;

		for (int i = 0; i < g->nr_noduri; i++)
		{
			if (index == n->cheie)
				i = g->nr_noduri;
			else
				n = n->urm;
		}

		Adj* adj = n->arce;
		while (adj != NULL)
		{
			
			if (adj->cheie != father)
			{
				if (!avz[adj->cheie])
					cycle(g, adj->cheie, trt, avz, index, nrcrt, sw);
				else
					*sw = 1;
				
			}
			adj = adj->urm;
			if (*sw == 1)
				adj = NULL;
		}
	}
	
}

void ciclesverify(Graf* g)
{
	printf("Punctul 2) \n");
	int *sw = malloc(sizeof(int));
	*sw = 0;
	int nrcrt = 0;
	int* trt = malloc(sizeof(int) * (g->nr_noduri));
	int* avz = malloc(sizeof(int) * (g->nr_noduri));
	for (int i = 0; i <= g->nr_noduri; i++)
		trt[i] = avz[i] = 0;
	for (int i = 1; i < g->nr_noduri && *sw==0; i++)
	{
	
		cycle(g, i, trt, avz, -1, nrcrt,sw);
		if (*sw==1)
			printf("exista ciclu\n");
	}
}

int nrcrt2;
int* aparente2;
int* vizitate2;
void ParcurgereC(Graf* g, int index)
{
	int k = 0;
	int* viz = malloc(sizeof(int) * (g->nr_noduri));

	Nod* p = g->noduri;
	for (int i = 1; i <= g->nr_noduri; i++)
	{
		if (p->cheie == index)
			i = g->nr_noduri + 1;
		else
		p = p->urm;
	}

	Adj* q = p->arce;
	while (q != NULL)
	{
		if (!aparente2[q->cheie]) {
			
			viz[k++] = q->cheie;
			aparente2[q->cheie] = 1;
			vizitate2[nrcrt2++] = q->cheie;
		}
		q = q->urm;
	}

	
	for (int i = 0; i < k; i++)
		ParcurgereC(g, viz[i]);
	free(viz);
}


void parcurgereCuprindere(Graf* g)
{
	aparente2 = malloc(sizeof(int) * (g->nr_noduri));
	vizitate2 = malloc(sizeof(int) * (g->nr_noduri));
	int* tst = malloc(sizeof(int) * (g->nr_noduri));
	int* svs = malloc(sizeof(int) * (g->nr_noduri));

	printf("Cuprindere egala cu adancime : ");
	for (int i = 1; i <= g->nr_noduri; i++) {
		int sw = 1;
		for (int j = 0; j <= g->nr_noduri; j++)
			tst[j] = svs[j] = aparente2[j] = vizitate2[j] = 0;
		nrcrt = 0;
		nrcrt2 = 0;
		aparente2[i] = 1;
		vizitate2[nrcrt2++] = i;
		ParcurgereC(g, i);
		parcA(g, i, tst, svs);

		for (int k = 0; k < g->nr_noduri; k++)
			if (vizitate2[k] != svs[k])
				sw = 0;
		if (sw == 1)
			printf("%d ", i);


	}
}

void parc(Graf* g, int index1, int index2, int k, int* avs, int* trt, int nrcrt)
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

		Nod* p = g->noduri;
		for (int i = 0; i <= g->nr_noduri; i++)
		{
			if (p->cheie == index1)
				i = g->nr_noduri + 1;
			else
				p = p->urm;
		}
		Adj* q = p->arce;
		while (q != NULL)
		{
			if (!avs[q->cheie])
				parc(g, q->cheie, index2, k - 1, avs, trt, nrcrt);
			
			q=q->urm;
		}

	
	}

}

void lanturi(Graf* g, int index1, int index2, int k)
{
	printf("Punctul 3)\n");
	printf("Lantul de la %d la %d cu %d pasi este :", index1, index2, k);
	int nrcrt = 0;
	int* avs = malloc(sizeof(int) * (g->nr_noduri));
	int* trt = malloc(sizeof(int) * (g->nr_noduri));
	for (int i = 0; i <= g->nr_noduri; i++)
		avs[i] = trt[i] = 0;
	parc(g, index1, index2, k, avs, trt, nrcrt);

	
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

	Graf* g = InitializareGraf();
	g=citire(g, "graf.txt");
	char c;
	text();
	while ((c = getchar()) != 'x')
	{
		if (c == '1')
			parcurgereAdancime(g);
		else
			if (c == '2')
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
					if (c == '4')
						parcurgereCuprindere(g);
					else
						text();

		printf("\n");
	}
	
}
