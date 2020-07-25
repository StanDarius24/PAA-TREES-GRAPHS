#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE* f;
typedef struct NodArboreDeRegasire {
	struct NodArboreDeRegasire* alfabet[27]; //alfabetul A B ... Z [
}NodArboreDeRegasire;

void Initializare(NodArboreDeRegasire* Nod)
{
	//ATENTIE, aceasta functie trebuie apelata cu *Nod deja alocat
	char c;
	for (c = 'A'; c <= '['; c++)
		Nod->alfabet[c - 'A'] = NULL;
}

void Atribuie(NodArboreDeRegasire* Nod, char c, NodArboreDeRegasire* p)
{
	Nod->alfabet[c - 'A'] = p;
}

NodArboreDeRegasire* Valoare(NodArboreDeRegasire* Nod, char c)
{
	return Nod->alfabet[c - 'A'];
}

void NodNou(NodArboreDeRegasire* Nod, char c)
{
	Nod->alfabet[c - 'A'] = (NodArboreDeRegasire*)malloc(sizeof(struct NodArboreDeRegasire));
	Initializare(Nod->alfabet[c - 'A']);
}

void Adauga(char* x, NodArboreDeRegasire* cuvinte)  //x=cuvantul, cuvinte=radacina arborelui de regasire

{

	unsigned i;

	NodArboreDeRegasire* t;

	t = cuvinte;

	for (i = 0; i < strlen(x); i++)
	{

		if (Valoare(t, x[i]) == NULL) //nodul curent nu are fiu pentru caracterul x[i], deci se creaza unul nou

			NodNou(t, x[i]);

		t = Valoare(t, x[i]); //avansez in arborele de regasire

	}
	Atribuie(t, '[', t); //se face o bucla pentru '[', pentru a marca un nod terminal

}
void Parcurge(NodArboreDeRegasire* Nod, char cuv[], int niv) //nodul curent, un buffer pentru construirea cuvantului, nivelul curent
{
	char c;
	for (c = 'A'; c <= 'Z'; c++)		//pentru toate literele considerate
	{
		if (Valoare(Nod, c))			//daca exista legatura in dictionar
		{
			cuv[niv] = c;				//adaug litera gasita in buffer
			Parcurge(Valoare(Nod, c), cuv, niv + 1); //avansez 
		}
	}
	if (Valoare(Nod, '['))			//exista terminator=>am gasit un cuvant intreg
	{
		cuv[niv] = '\0';
		printf("%s\n", cuv);		//prelucrez datele
	}
}

NodArboreDeRegasire* dictionar_nou(char* sir)
{
	NodArboreDeRegasire* radacina = NULL;
	radacina = malloc(sizeof(NodArboreDeRegasire));
	Initializare(radacina);
	f = fopen(sir, "r");
	if (f == NULL)
		return NULL;
	char s[30];
	while (!feof(f)) {
		fscanf(f, "%s\n", s);
		s[strlen(s)] = '\0';
		Adauga(s, radacina);
	}
	return radacina;
}

void cuv_ortografiate_corect(NodArboreDeRegasire* nod,char *sir)
{
	char c;
	FILE* g = fopen(sir, "r");
	char s[20];
	int contor = 0, i, j = 0, k, sw = 0,o;
	NodArboreDeRegasire* t;
	
	while (!feof(g))
	{
		t = nod;
		fscanf(g, "%s\n", s);
		strcat(s, "[");
		sw = 0;
		for (i = 0; i < strlen(s); i++)
		{
			if (Valoare(t, s[i]))
			{
				t = Valoare(t, s[i]);
				if (i == strlen(s) - 1)
					contor++;
			}
			else
			{
				s[strlen(s) - 1] = '\0';
				printf("cuvant gresit : %s la linia %d\n", s, j);
				strcat(s, "[");
				for (k = i + 1; k < strlen(s); k++)
				{
					if (Valoare(t, s[k]))
						t = Valoare(t, s[k]);
					else
						break;
					if (k == strlen(s) - 1)
					{
						sw = 1;
						printf("se poate corecta prin stergerea carac %c de la poz %d\n", s[i],i);
					}

				}
				if (sw == 0)
				{
					o = i;
					while (sw == 0 && o != 0)
					{
						c = s[o];
						s[o] = s[o + 1];
						s[o + 1] = c;
						t = nod;
						for (k = 0; k < strlen(s); k++)
						{
							if (Valoare(t, s[k]))
								t = Valoare(t, s[k]);
							else
								break;
							if (k == strlen(s) - 1)
							{
								sw = 1;
								o == 0;
								printf("se poate corecta prin inversarea lui %c %c poz %d %d\n", s[i], s[i + 1], i, i + 1);
							}
						}
						if (sw == 0)
						{
							c = s[o];
							s[o] = s[o + 1];
							s[o + 1] = c;
							o--;
						}
					}

				}
				if (sw == 0)
				{
					
				
					
						t = nod;
						for (k = 0; k < strlen(s); k++)
						{
							if (Valoare(t, s[k]))
								t = Valoare(t, s[k]);
							else
							{
								if(sw==0)
								{
									o = k;
									sw = 1;
									for (c = 'A'; c <= 'Z'; c++)
										if (Valoare(t, c))
										{
											t = Valoare(t, c);
											break;
										}
								}
							}
							if (k == strlen(s) - 1)
							{
								printf("lipseste litera %c de la poz %d\n",c,o);
							}
						}
						

					
				}
				i = strlen(s);
			}
		}

		j++;
	
	}
	printf("cuvinte corecte:%d", contor);


	fclose(g);
}

int main()
{
	
	NodArboreDeRegasire* radacina = NULL;
	radacina = malloc(sizeof(NodArboreDeRegasire));
	Initializare(radacina);
	radacina = dictionar_nou("wordlist.txt");
		//char q[20];
	//Parcurge(radacina, q, 0);
	cuv_ortografiate_corect(radacina, "deprelucrat.txt");
	printf("\n");
	

	fclose(f);
}