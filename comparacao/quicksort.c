//#include <stdio.h>
//#include <stdlib.h>

#define MAX 100

int a[MAX];
int max;

void quicksort(int m, int n)
{
    int non_stop = 1;
    
    int i,j;
    int v,x;
    if (n <= m) { return; }
    i = m-1; j = n; v = a[n];
    while(non_stop)
    {
        do{ i=i+1; }while (a[i] < v);
        do{ j=j-1; }while (a[j] > v);
        if (i >= j)
        {
            non_stop = 0;
        }
        else
        {
            x = a[i]; a[i] = a[j]; a[j] = x;
        }
    }
    x = a[i]; a[i] = a[n]; a[n] = x;
    quicksort(m,j);
    quicksort(i+1,n);
}

int main()
{
    int i;
    int j;
    
    max = 0;
    while(max<=0 || max>100)
    {
        printf("Entre com o tamanho do vetor (valor entre 1 e 100): ");
        scanf("%d",&max);
    }
    for(i = 0; i < max; i++)
    {
        printf("Entre com o valor da posicao %d: ",i+1);
        scanf("%d",&j);
        a[i] = j;
    }
    quicksort(0,max-1);
    for(i = 0; i < max; i++)
    {
        printf("%d ",a[i]);        
    }
    printf("\n");    
    return 0;
}
