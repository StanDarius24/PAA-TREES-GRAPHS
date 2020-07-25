#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct dictionar{
	char caractere[5];
	int biti:4;
}dictionar;
int n;



void afisdictionar(dictionar *q)
{	printf("index |diction| biti \n");
	for(int i=0;i<n-1;i++)
	{
	printf("%4d  |%-7s|%4d\n",i,q[i].caractere,q[i].biti);
}
printf("\n");
}

int existindichar (char q, dictionar *p)
{
	for(int i=0;i<n;i++)
	{
		if(q==p[i].caractere[0])
			return 1;
	}
	return 0;
}
int existindic(char *q, dictionar *p)
{
	for(int i=0;i<n;i++)
	{
		if(!strcmp(q,p[i].caractere))
			return 1;
	}
	return 0;
}

dictionar *codificalzw(dictionar *p,char *s)
{	int i;
	for(i=0;i<strlen(s)-1;i++)
	{
		if(!existindichar(s[i],p)&& s[i]!='\n')
		{	
			
			p=realloc(p,sizeof(dictionar)*n);
			p[n-1].caractere[0]=s[i];
			p[n-1].caractere[1]=0;
			p[n-1].biti=n;
			n++;
		}
		else
		{
			
			char q[5];
			q[0]=s[i];
			int x=2;
			if(s[i+1]!='\n')
			{	i++;
				q[1]=s[i];
				int sw=1;
				while(existindic(q,p))
				{
					i++;
					if(s[i]!='\n')
					{
						q[x]=s[i];
						x++;
					}
					else
						{sw=0; break;}
				}
				if(sw)
				{	
					
					p=realloc(p,sizeof(dictionar)*n);
					strcpy(p[n-1].caractere,q);
					p[n-1].biti=n;
					n++;
				}
			}


		}
	}
	return p;

}
char *si;
void codificaresir(char *s, dictionar *p)
{
	int i;
	char sir[30]="",q[4];



	for(i=0;i<strlen(s);i++)
	{
		if(existindichar(s[i],p)&& s[i+1]=='\n')
		{sir[strlen(sir)]='<';
			for(int k=0;k<n-1;k++)
							if(s[i]==p[k].caractere[0])
								{sir[strlen(sir)]='0'+p[k].biti;}
								sir[strlen(sir)]='>';
		 break;}
		if(existindichar(s[i],p))
		{	int x=2;
			q[0]=s[i];
			
			if(s[i+1]!='\n')
			{
				i++;
				q[1]=s[i];
				
				while(existindic(q,p))
				{
					if(s[i+1]!='\n')
					{
						i++;
						q[x]=s[i];
						x++;
						
					}
					else
					{	
						char wes[4]="<";
						for(int k=0;k<n-1;k++)
							if(!strcmp(q,p[k].caractere))
								{wes[1]='0'+p[k].biti;}
								wes[2]='>';
						strcat(sir,wes);
					}
				}
				
				i--; 
				q[x-1]=0;
				
				char we[4]="<";
						for(int k=0;k<n-1;k++)
							if(!strcmp(q,p[k].caractere))
								{we[1]='0'+p[k].biti;}
								we[2]='>';
						strcat(sir,we);

			}
		}

		

	}
	strcat(sir,"<0>");
	si=malloc(sizeof(char)*strlen(sir));
	strcpy(si,sir);
}

void decripteaza(char *s,dictionar *p)
{
	int i;
	char *decript="";
	int size=1;
	decript=malloc(sizeof(char));
	for(i=0;i<strlen(s);i++)
	{
		if(s[i]>='1' && s[i]<='9')
		{	size++;
			int x=s[i]-'0';
			decript=realloc(decript,sizeof(char)*size);
			strcat(decript,p[x-1].caractere);
		}
	}
	printf("sir decriptat : %s\n",decript);


}

int main()
{
	dictionar *p;
	n=1;
	p=malloc(sizeof(dictionar));
	FILE *f;
	f=fopen("LZWTEXT.md","r");
	char *s=malloc(sizeof(char)*16);
	fgets(s,15,f);
	printf("%s",s);
	p=codificalzw(p,s);
	afisdictionar(p);
	codificaresir(s,p);
	printf("sir codificat: %s\n",si);
	decripteaza(si,p);
	free(p);
}