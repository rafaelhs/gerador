//#include <stdio.h>
//#include <stdlib.h>

int main()
{
    int a, b, c;
    
    printf("=== Determinar Tipo de Triangulo ===\n\n");
    printf("Entre com o tamanho do lado A: ");
    scanf("%d",&a);
    printf("Entre com o tamanho do lado B: ");
    scanf("%d",&b);
    printf("Entre com o tamanho do lado C: ");
    scanf("%d",&c);

    printf("\n");
    
    if((a<b+c) && (b<a+c) && (c<a+b))
    {
        if((a==b)&&(b==c))
        {
            printf("Triangulo Equilatero.");
        }
        else
        {
            if((a==b)||(a==c)||(c==b))
            {
                printf("Triangulo Isosceles.");
            }
            else
            {
                printf("Triangulo Escaleno.");
            }
        }
    }
    else
    {
        printf("Nao eh Triangulo!!!\n");
    }
    return 0;
}

