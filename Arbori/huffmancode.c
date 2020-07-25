#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct cod{
   int a[5];
   int dim;
   char caracter;
}cod;
cod *e;
//Matricea de aparitii
typedef struct matrix{
   int frecventa;
   char caracter;
}matrix;
matrix *p;
int n;
void create_matrix(char *s)
{  int i,sw=0;
   p=malloc(sizeof(matrix)+2);
   while(strlen(s)>1)
   {
      for(i=0;i<n;i++)
      if(p[i].caracter==s[0])
      {p[i].frecventa++;
         sw=1;}

      if(sw!=1)
      {
         p=realloc(p,sizeof(matrix)*(n+2));
         p[n].frecventa=1;
         p[n].caracter=s[0];
         n++;
      }
   
      sw=0;
      s=s+1;
   }
}

void print_matrix()
{  printf("Printare matrice de aparitii\n");
   for(int i=0;i<n;i++)
      printf("(%c - %d)\n",p[i].caracter,p[i].frecventa);
   printf("\n");
}

typedef struct nod{
   char caracter;
   int frecventa;
   struct nod *st,*dr;
}nod;

typedef struct heap{
   int dimensiune;
   int capacitate;
   struct nod **array;
}heap;

nod *createNod(char c, int x)
{
   nod *q=malloc(sizeof(nod));
   q->caracter=c;
   q->frecventa=x;
   q->st=NULL;
   q->dr=NULL;
   return q;
}

heap *createHeap(int capacity)
{
   heap *q=malloc(sizeof(heap));
   q->capacitate=capacity;
   q->dimensiune=0;
   q->array=malloc(q->capacitate * sizeof(nod *));
   return q;
}

void swapnode(nod **a,nod **b)
{
   nod *t=*a;
   *a=*b;
   *b=t;
}

void createMinHeap(heap *q,int index)
{
   int mic,st,dr;
   mic=index;
   st=2*index+1;//fiu stang
   dr=2*index+2;//fiu drept

   if(st<q->dimensiune && q->array[st]->frecventa<q->array[mic]->frecventa)
      mic=st;
   if(dr<q->dimensiune && q->array[dr]->frecventa<q->array[mic]->frecventa)
      mic=dr;
   if(mic!=index)
   {
      swapnode(&q->array[mic],&q->array[index]);
      createMinHeap(q,mic);
   }
}

int gata(heap *q)
{return (q->dimensiune==1);}

nod *extrageHeap(heap *q)
{
   nod *t=q->array[0];
   q->array[0]=q->array[q->dimensiune-1];
   q->dimensiune--;
   createMinHeap(q,0);
   return t;
}

void adaugaheap(heap *q,nod *r)
{
   q->dimensiune++;
   int x=q->dimensiune-1;
   while(x && r->frecventa<q->array[(x-1)/2]->frecventa)
   {
      q->array[x]=q->array[(x-1)/2];
      x=(x-1)/2;
   }
   q->array[x]=r;
}

void creereheap(heap *q)
{
   int n=q->dimensiune-1;
   
   for(int i=(n-1)/2;i>=0;--i)
   {
      createMinHeap(q,i);
   }
}

heap *macel()
{ 
   heap *q=createHeap(n);
   e=malloc(sizeof(cod)*n);

   for(int i=0;i<n;i++)
   {
   q->array[i]=createNod(p[i].caracter,p[i].frecventa);
   }
   q->dimensiune=n;

   creereheap(q);
   return q;
}

nod *Huffmanarbore()
{
   nod *r,*l,*f;
  
   heap *q=macel();
   
   while(!gata(q))
      {
         l=extrageHeap(q);
         r=extrageHeap(q);
         f=createNod('+',l->frecventa+r->frecventa);
         f->st=l;
         f->dr=r;

         adaugaheap(q,f);
      }
      return extrageHeap(q);
}

void printArray(int arr[], int n)
{
  int i;
  for (i = 0; i < n; ++i)
    printf("%d", arr[i]);

  printf("\n");
}

int sizee;
void creerecod(nod *t,int a[],int x)
{  
   if(t->st)
   {a[x]=0;
      creerecod(t->st,a,x+1);
   }
   if(t->dr)
      {a[x]=1;
      creerecod(t->dr,a,x+1);
      }
   if(t->dr==NULL && t->st==NULL)
   {
      e[sizee].dim=x;
      e[sizee].caracter=t->caracter;
      for(int i=0;i<x;i++)
         e[sizee].a[i]=a[i];
      sizee++;
   }
}

void printarecodHuffMan()
{
   for(int i=0;i<sizee;i++)
     { printf("%c \t",e[i].caracter);
   for(int j=0;j<e[i].dim;j++)
      printf("%d",e[i].a[j]);
   printf("\n");}
   printf("\n");
}

void HuffMan()
{
   nod *t=Huffmanarbore();

   int a[50];
   creerecod(t,a,0);
}
int decodif(char *s)
{  int i,j,k;
   int suma=0;
   for(i=0;i<strlen(s);i++)
   {
      for(j=0;j<sizee;j++)
         if(s[i]==e[j].caracter)
         {
           
            for(k=0;k<e[j].dim;k++)
               if(e[j].a[k]==0)
                  {printf("0"); suma++;}
               else
                  {printf("1"); suma++;}
         }
         printf(" ");

   }

return suma;
}
int main()
{  e=malloc(sizeof(cod));
   FILE *f;
   f=fopen("FisTest.txt","r");
   char *s,c[20];
   s=malloc(sizeof(char));
   while(!feof(f))
{
   fscanf(f,"%s",c);
   s=realloc(s,(sizeof(char)*(strlen(s)+strlen(c))));
   strcat(s,c);
   strcat(s," ");
}
   s[strlen(s)-1]='\n';
   create_matrix(s);
  HuffMan();
   printarecodHuffMan();

printf("___________\n");
printf("%s\n",s);
int x,q;
x=decodif(s);
printf("\nrata compresie:\n");
q=strlen(s)*8;
printf("%dprocente",(x*100)/q);
   fclose(f);
   

free(e);
free(p);
}