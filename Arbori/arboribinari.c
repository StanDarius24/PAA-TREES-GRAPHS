#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct nod{
	char nume[30];
	int zi;
	int luna;
	int an;
	struct nod *drept,*stang;
}nod;

nod *creeznod(char *s,int z,int l,int a)
{
	nod *q;
	if((q=malloc(sizeof(nod)))==NULL)
	{	printf("Insuf memorie\n");
			exit(1);
		}
		else
		{
			strcpy(q->nume,s);
			q->zi=z;
			q->luna=l;
			q->an=a;
			q->stang=NULL;
			q->drept=NULL;
		}
		return q;

}

nod *adaug(nod *q,char *s,int z,int l,int a)
{
	if(q==NULL)
		{q=creeznod(s,z,l,a); return q;}
	else
	{
		nod *p=q;
		nod *sda=creeznod(s,z,l,a);
		while(p!=NULL)
		{
			if((sda->an>p->an || (sda->an==p->an && sda->luna>p->luna) || (sda->an==p->an && sda->luna==p->luna && sda->zi>p->zi)) && p->drept==NULL)
			{
				p->drept=sda;
				break;
			}
			else
			if((sda->an<p->an || (sda->an==p->an && sda->luna<p->luna) || (sda->an==p->an && sda->luna==p->luna && sda->zi<p->zi)) && p->stang==NULL)
			{
				p->stang=sda;
				break;
			}
			else
			if((sda->an>p->an || (sda->an==p->an && sda->luna>p->luna) || (sda->an==p->an && sda->luna==p->luna && sda->zi>p->zi)) && p->drept!=NULL)
				p=p->drept;
			else
			if((sda->an<p->an || (sda->an==p->an && sda->luna<p->luna) || (sda->an==p->an && sda->luna==p->luna && sda->zi<p->zi)) && p->stang!=NULL)
				p=p->stang;

		}
		return q;
	}
}
void afis(nod *rad)
{
	if(rad!=NULL)
	{
	afis(rad->stang);
	printf("%s %d/%d/%d\n",rad->nume,rad->zi,rad->luna,rad->an);
	afis(rad->drept);
	}

}
void afisstabil(nod *rad,int z,int l,int a)
{
	if(rad!=NULL)
	{
		if(rad->zi==z && rad->luna==l && rad->an==a)
			printf("%s",rad->nume);
		else
		if(rad->an<a || (rad->an==a && rad->luna<l) || (rad->an==a && rad->luna==l &&rad->zi<z))
			afisstabil(rad->drept,z,l,a);
		else
			if(rad->an>a || (rad->an==a && rad->luna>l) || (rad->an==a && rad->luna==l &&rad->zi>z))
			afisstabil(rad->stang,z,l,a);
		
	}
}

void afismaimicide(nod *rad,int z,int l,int a)
{
	if(rad!=NULL)
	{
		if(rad->an<a || (rad->an==a && rad->luna<l) || (rad->an==a && rad->luna==l && rad->zi<z))
			printf("%s\n",rad->nume);
		afismaimicide(rad->stang,z,l,a);
		afismaimicide(rad->drept,z,l,a);
	}
}
void celmaitanar(nod *rad)
{
	while(rad->stang!=NULL)
		rad=rad->stang;
	printf("%s",rad->nume);
}

int inaltimearbore(nod *rad)
{
	if(rad!=NULL)
	{
		int l=inaltimearbore(rad->stang);
		int r=inaltimearbore(rad->drept);
		if(l>r)
			return(l+1);
		else
			return(r+1);
	}
	return 0;
}




void cautazi(nod *rad,char *s,int *z,int *l,int *a)
{
	if(rad!=NULL)
	{	
		if(!strcmp(s,rad->nume))
		{	
			*z=rad->zi;
			*l=rad->luna;
			*a=rad->an;
			

		}
		else{
		cautazi(rad->stang,s,z,l,a);
		cautazi(rad->drept,s,z,l,a);
	}}
}

nod *cauta(nod *rad,char *s,int z,int l,int a)
{	
	
	if(rad!=NULL)
	{ 
		if(!strcmp(rad->nume,s))
			{
			return rad;}
			
		else
			if(rad->an<a || (rad->an==a && rad->luna<l) || (rad->an==a && rad->luna==l &&rad->zi<z))
			
				return cauta(rad->drept,s,z,l,a);
		else
			if(rad->an>a || (rad->an==a && rad->luna>l) || (rad->an==a && rad->luna==l &&rad->zi>z))
			
			return cauta(rad->stang,s,z,l,a);
	}
	return NULL;

}

nod *stergz(nod *rad,char *s,int z,int l,int a)
{
	nod *p=cauta(rad,s,z,l,a);
	if(p->drept && p->stang)
	{	nod *q=p->stang;
		if(q->drept==NULL)
		{
			strcpy(p->nume,q->nume);
			p->zi=q->zi;
			p->luna=q->luna;
			p->an=q->an;
			p->stang=q->stang;
		}
		else
		{
			while(q->drept->drept!=NULL)
				q=q->drept;
			strcpy(p->nume,q->drept->nume);
			p->zi=q->drept->zi;
			p->luna=q->drept->luna;
			p->an=q->drept->an;
			q->drept=NULL;
		}
	}
	else
	
		if(p->drept==NULL && p->stang)
		{
			nod *q=p->stang;
			strcpy(p->nume,q->nume);
			p->zi=q->zi;
			p->luna=q->luna;
			p->an=q->an;
			p->stang=q->stang;
			p->drept=q->drept;
			q=NULL;
		}

	else

		if(p->stang==NULL && p->drept)
		{	
			nod *q=p->drept;
			strcpy(p->nume,q->nume);
			p->zi=q->zi;
			p->luna=q->luna;
			p->an=q->an;
			p->drept=q->drept;
			p->stang=q->stang;
			}
	return rad;
	
}



nod *sterg(nod *radacina,char *s)
{
	int z,l,a;
	cautazi(radacina,s,&z,&l,&a);
	return stergz(radacina,s,z,l,a);
}

int main()
{	nod *radacina=NULL;
	FILE *f;
	f=fopen("FisierTest.txt","r");
	if(!f)
	{
		printf("insuf memorie\n");
		exit(1);
	}
	
	char s[30];
	int z,l,a;
	
	

for(int i=0;i<20;i++){
	fgets(s,29,f);
	s[strlen(s)-2]=NULL;
	fscanf(f,"%d/%d/%d\n",&z,&l,&a);
	radacina=adaug(radacina,s,z,l,a);
}

//afisstabil(radacina,19,3,1997);
//afismaimicide(radacina,19,3,1997);
//celmaitanar(radacina);
	//printf("%d\n",inaltimearbore(radacina));
	//z=l=a=0;
	//cautazi(radacina,"MELHEM SAMER",&z,&l,&a);
	//printf("%d %d %d\n",z,l,a);
	//cauta(radacina,"GALANTON MOROSAN LIVIU",z,l,a);
	//afis(radacina);
	//radacina=sterg(radacina,"PRICA DAN ROMEO");
	//printf("\n");
	//afis(radacina);
	//nod *q=cauta(radacina ,"MELHEM SAMER",z,l,a);
	//printf("%d %d %d %s",q->zi,q->luna,q->an,q->nume);

	//afis(radacina);
		fclose(f);
}