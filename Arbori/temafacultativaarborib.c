#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define n 2
#define nn (n*2)

typedef struct tip
{
	char id[5];
	int ziua;
	int luna;
	int an;
}tip;


typedef struct Nod{
	struct tip cheie;
	struct Pagina *p;
	int contor;
}Nod;

typedef struct Pagina{
	int m;
	struct Pagina *p0;
	struct Nod e[nn+1];
}Pagina;

Nod v;
	
Pagina *adauga(Pagina *pag,tip x,Nod *nod)
{
	int mij,i,s,d;
	Pagina *r,*q;

	if(nod==NULL)
	{
		v.cheie=x;
		v.p=NULL;
		v.contor=1;
	}
	else
	v= *nod;

	if(pag==NULL)
	{
		pag=(Pagina *)calloc(sizeof(Pagina),1);
		pag->e[++pag->m]=v;
		return pag;
	}

	s=1;
	d=pag->m;
	while(s<=d)
	{
		mij=(s+d)/2;
		if(!strcmp(pag->e[mij].cheie.id,v.cheie.id))
			{pag->e[mij].contor++;
				return pag;}
		if(strcmp(pag->e[mij].cheie.id,v.cheie.id)>0)
			d=mij-1;
		else
			s=mij+1;
	}

	if(pag->p0==NULL || nod)
	{

		if(pag->m<nn)
		{
			pag->m++;
			for(i=pag->m;i>d+1;i--)
				pag->e[i]=pag->e[i-1];
			pag->e[i]=v;
		}
		else
		{
			Pagina *a=pag;
			Pagina *b=calloc(sizeof(Pagina),1);
			pag=calloc(sizeof(Pagina),1);
			for(i=1;i<=n;i++)
				b->e[i]=a->e[i+n];
			a->m=b->m=n;
			if(strcmp(v.cheie.id,a->e[n].cheie.id)>0 && strcmp(b->e[1].cheie.id,v.cheie.id)>0)
				pag->e[++pag->m]=v;
			else
			{
				if(strcmp(a->e[n].cheie.id,v.cheie.id)>0)
				{
					pag->e[++pag->m] = a->e[n];
					for (i = a->m; i > 1 && strcmp(a->e[i - 1].cheie.id,v.cheie.id)>0; i--)
						a->e[i] = a->e[i - 1];
					a->e[i] = v;
				}
				else
				{

					pag->e[++pag->m] = b->e[1];
					for (i = 1; i < b->m && strcmp(b->e[i + 1].cheie.id,v.cheie.id)<0; i++)
						b->e[i] = b->e[i + 1];
					b->e[i] = v;
				}
			}

			b->p0=pag->e[1].p;
			pag->p0=a;
			pag->e[1].p=b;

			
		}



	}
	else
	{
		if( d==0)
			q=pag->p0;
		else
			q=pag->e[d].p;
		r=adauga(q,x,NULL);
		if(q!=r)
		{
			pag=adauga(pag,r->e[1].cheie,&r->e[1]);
			free(r);
		}
	}
return pag;
}

void afisare(Pagina *arbore, int nivel)
{
	int i;

	if (!arbore)
		return;
	afisare(arbore->p0, nivel + 1);
	for (i = 1; i <= arbore->m; i++)
		afisare(arbore->e[i].p, nivel + 1);

	printf("Nivel %d: ", nivel);
	for (i = 1; i <= arbore->m; i++)
		printf("%s  \t ", arbore->e[i].cheie.id);
	printf("\n");
}



void vecinStang(Pagina *pag, Pagina *st, Pagina *r, int d)
{
	int i;

	if (st->m == n) //daca pagina din stanga are fix N elemente => contopire
	{
		st->e[n + 1] = pag->e[d];
		st->e[n + 1].p = r->p0;
		st->m = nn;
		for (i = n + 2; i <= n; i++)
			st->e[i] = r->e[i - n - 1];

		for (i = d; i < pag->m; i++)
			pag->e[i] = pag->e[i + 1];
		pag->m--;
		free(r);
	}
	else //imprumut
	{
		r->m = n;
		for (i = n; i > 1; i--)
			r->e[i] = r->e[i - 1];
		r->e[1] = pag->e[d];
		r->e[1].p = r->p0;
		r->p0 = st->e[st->m].p;

		pag->e[d] = st->e[st->m];
		pag->e[d].p = r;
		st->m--;
	}
}

void vecinDrept(Pagina *pag, Pagina *dr, Pagina *r, int d)
{
	int i;

	r->e[n] = pag->e[d + 1];
	r->e[n].p = dr->p0;
	r->m = n;

	if (dr->m == n) //daca pagina din dreapta are fix N elemente => contopire
	{
		r->m = nn;
		for (i = n + 1; i <= nn; i++)
			r->e[i] = dr->e[i - n];

		for (i = 1; i < pag->m; i++)
			pag->e[i] = pag->e[i + 1];
		pag->m--;
		free(dr);
	}
	else //contopire
	{
		pag->e[d + 1] = dr->e[1];
		pag->e[d + 1].p = dr;

		dr->p0 = dr->e[1].p;
		for (i = 1; i < dr->m; i++)
			dr->e[i] = dr->e[i + 1];
		dr->m--;
	}
}

Pagina* suprima(Pagina *pag, tip x, Nod *nod)
{
	int i, s, d, mij;
	Pagina *q, *r;

	//daca nu exista cheia in arbore
	if (pag == NULL || pag->m == 0)
		return NULL;

	s = 1;
	d = pag->m;
	while (s <= d) //cautare binara
	{
		mij = (s + d) / 2;
		if (!strcmp(x.id,pag->e[mij].cheie.id))
			break;
		if (strcmp(pag->e[mij].cheie.id,x.id)>0)
			d = mij - 1;
		else
			s = mij + 1;
	}

	/*se suprima cu ajutorul nodului, cu cheie maxima,
	mai mica decat cheia nodului curent*/
	if (!strcmp(x.id,pag->e[mij].cheie.id))
	{
		if (pag->p0 == NULL) //pagina curenta este pagina terminala
		{
			/*daca se elimina un nod dintr-o pagina neterminala,
			se schimba continutul nodurilor, dar se pastreaza legatura*/
			if (nod)
			{
				q = nod->p;
				*nod = pag->e[pag->m];
				nod->p = q;
			}
			for (i = mij; i < pag->m; i++)
				pag->e[i] = pag->e[i + 1];
			pag->m--;

			//pagina terminala curenta s-a golit => arborele scade in inaltime
			//posibil doar pentru N == 1
			for (q = pag; pag && pag->m == 0; free(q), q = pag)
				pag = pag->p0;

			return pag;
		}
		/*dupa mutarea cheii intr-o pagina terminala,
		se trece la eliminarea ei, prin repetarea pasului curent;
		se presupune ca cheia de sters este mai mica cu o unitate*/
		return suprima(pag, x , &pag->e[mij]);
	}

	if (d == 0)
		q = pag->p0;
	else
		q = pag->e[d].p;
	r = suprima(q, x, nod); //se incearca eliminarea cheii din subarborele corespunzator

	//conditia (r == NULL) garanteaza ca este pagina terminala
	/*daca nu exista cheia aleasa sa inlocuiasca cheia de sters
	se repeta pasul de stergere folosind cheia cu cea mai mare valoare*/
	if (r == NULL)
	{
		if (nod)
			return suprima(pag, pag->e[d].cheie, nod);
		else //daca nu exista cheia in subarbore
			return pag;
	}

	if (r->m < n) //subdepasire => imprumut sau contopire
	{
		Pagina *st, *dr;
		if (d == 1)
			st = pag->p0;
		else
			st = pag->e[d - 1].p;
		dr = pag->e[d + 1].p;

		if (r == pag->p0) //cel mai din stanga nod
			vecinDrept(pag, dr, r, d);
		else if (d == pag->m) //cel mai din dreapta nod
			vecinStang(pag, st, r, d);
		else //are 2 vecini
		{
			/*
			se imprumuta de la vecinul cu nr maxim de noduri in pagina sau se contopesc 2 pagini;
			cum nu pot exista simultan 2 pagini cu mai putin de N elemente =>
			oricare pagina poate fi aleasa pentru contopire, daca este cazul
			*/

			//se imprumuta de la vecinul drept
			if (dr->m > st->m)
				vecinDrept(pag, dr, r, d);
			//se imprumuta (sau contopeste) de la (cu) vecinul stang
			else
				vecinStang(pag, st, r, d);
		}
	}

	//radacina subarborelui curent s-a golit => arborele scade in inaltime
	for (q = pag; pag && pag->m == 0; free(q), q = pag)
		pag = pag->p0;

	return pag;
}
tip *celmaidindreapta(Pagina *pag)
{
	if(pag->e[pag->m].p==NULL)
		return &pag->e[pag->m].cheie;
	else
		celmaidindreapta(pag->e[pag->m].p);

}

Pagina *stergeMaxim(Pagina *radacina)
{
	return suprima(radacina,*celmaidindreapta(radacina),NULL);
}

Pagina *stergedupaid(Pagina *radacina,char *s)
{
	tip x;
	strcpy(x.id,s);
	return suprima(radacina,x,NULL);
}

tip cautadupaid(Pagina *pag,char *sir)
{
	int s,d,mij;
	s=1;
	d=pag->m;
	while(s<=d)
	{
		mij=(s+d)/2;
		if(!strcmp(pag->e[mij].cheie.id,sir))
			return pag->e[mij].cheie;
		if (strcmp(sir, pag->e[mij].cheie.id)>0)
			s = mij + 1;
		else
			d = mij - 1;
			
	}
		
		if(mij==1)
			return cautadupaid(pag->p0,sir);
		else
			return cautadupaid(pag->e[mij].p,sir);
}


int etata(Pagina *radacina,char *sir)
{
	int i,j;
	if (radacina != NULL)
	{
		for (i = 1; i <= radacina->p0->m; i++)
			if (!strcmp(radacina->p0->e[i].cheie.id, sir))
				return 1;
		for (i = 1; i <= radacina->m; i++)
		{	
			
			for (j = 1; j <= radacina->e[i].p->m; j++)
				if (!strcmp(sir, radacina->e[i].p->e[j].cheie.id))
					return 1;
		}
	}
		return 0;
}
		
Pagina* returntata(Pagina* pag, char* sir)
{
	if (pag != NULL)
	{
		if (etata(pag, sir))
			return pag;
		else
		{
			int s, d, mij;
			s = 1;
			d = pag->m;
			while (s <= d)
			{
				mij = (s + d) / 2;
				if (strcmp(sir, pag->e[mij].cheie.id) > 0)
					s = mij + 1;
				else
					d = mij - 1;

			}
			if (d == 0)
				return returntata(pag->p0, sir);
			else
				return returntata(pag->e[mij].p, sir);
		}

	}
	return NULL;
}

tip urmatorul(Pagina* pag, char* sir)	//nu stiu daca e cea mai eficienta metoda.Am adaugat comentarii pt ca e mai usor de corectat
{										//daca exista o modalitate mai usoara de trimis urmatorul caracter va rog sa mi scrieti
	int i, j;
	//1
	for(i=1;i<=pag->m;i++)				// de la 1-2 am cautat pagina tata, adica acea pagina care contine respectiva cheie in p0 sau in e[i].p
		if (!strcmp(sir, pag->e[i].cheie.id))	//am facut acest lucru pt ca exista 3 cazuri mari si late in returnarea urmatorului caracter
		{										//primul si cel mai simplu este atunci cand caracterul de indice i(cautat) nu e egal cu m se returneaza urmatorul dar daca 
			if (pag->e[i].p != NULL)			//e la capatul paginii apar alte 2 cazuri.
				pag = pag->e[i].p;				//primul este ca acea cheie sa aiba o alta pagina adica e[i].p (sa fie dif de null)
			while (pag->p0 != NULL)				//si daca avem asa ceva trebuie sa coboram in jos pe p0 (ideea de la arbori binari ordonati urmatorul mai mare decat val e val de dr si stanga pana la null)
				pag = pag->p0;					//daca nu trebuie returnat valoarea din pagina de la care am plecat 
			return pag->e[1].cheie;				//(sau tatal)
		}//2
	pag = returntata(pag, sir);
	
	for(i= 1 ; i<=pag->p0->m;i++)
		if (!strcmp(sir, pag->p0->e[i].cheie.id))
		{
			if (i + 1 <= pag->p0->m)
				return pag->p0->e[i + 1].cheie;
			else
				if (pag->p0->e[i].p != NULL)
					return pag->p0->e[i].p->e[1].cheie;
				else
					return pag->e[1].cheie;
		}

	for (i = 1; i <= pag->m; i++)
	{
		for(j=1;j<pag->e[i].p->m;j++)
			if (!strcmp(sir, pag->e[i].p->e[j].cheie.id))
			{
				if (j + 1 <= pag->e[i].p->m)
					return pag->e[i].p->e[j + 1].cheie;
				else
					if (pag->e[i].p->e[j].p != NULL)
						return pag->e[i].p->e[j].p->e[1].cheie;
					else
						return pag->e[i].cheie;
			}
	}
}



tip x[21];
Pagina *radacina=NULL;
int main()
{
	strcpy(x[0].id,"a");
	strcpy(x[1].id,"b");
	strcpy(x[2].id,"c");
	strcpy(x[3].id,"d");
	strcpy(x[4].id,"e");
	strcpy(x[5].id,"f");
	strcpy(x[6].id,"g");
	strcpy(x[7].id,"h");
	strcpy(x[8].id,"i");
	strcpy(x[9].id,"j");
	strcpy(x[10].id,"k");
	strcpy(x[11].id,"l");
	strcpy(x[12].id,"m");
	strcpy(x[13].id,"n");
	strcpy(x[14].id,"o");
	strcpy(x[15].id,"p");
	strcpy(x[16].id,"q");
	strcpy(x[17].id,"r");
	strcpy(x[18].id,"s");
	strcpy(x[19].id,"t");
	strcpy(x[20].id,"u");
	for(int i=0;i<21;i++)
		radacina=adauga(radacina,x[i],NULL);
	afisare(radacina,1);
	
	radacina=stergeMaxim(radacina);
	radacina=stergedupaid(radacina,"b");
	printf("\n");
	afisare(radacina,1);
	printf("\n");
	printf("aici trebuie sa returneze cheia trimisa ca parametru %s\n",(cautadupaid(radacina,"a").id));
	tip x;
	x = urmatorul(radacina, "i");
	printf("%s\n",x.id);
	
	
free(radacina);
}