/******************************************************************************

Welcome to GDB Online.
  GDB online is an online compiler and debugger tool for C, C++, Python, PHP, Ruby, 
  C#, OCaml, VB, Perl, Swift, Prolog, Javascript, Pascal, COBOL, HTML, CSS, JS
  Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
typedef struct nodo{
    int value;
    struct nodo * next;
    struct nodo * prev;
}lista;



int main()
{
    //voglio vedere se, facendo una free su element e copiando element prima su array[0], che succede
    lista *firstelement = (lista *) malloc(sizeof(lista));
    firstelement -> value = 1;
    firstelement -> next = NULL;
    
    lista *lastelement = (lista *) malloc(sizeof(lista));
    lastelement -> value = 4;
    
    
    lista * element = (lista *) malloc(sizeof(lista));
    element -> value = 3;
    element ->next = firstelement;
    firstelement ->prev = element;
    
    lista ** array = (lista **) malloc(sizeof(lista)*4);
    int i;
    for(i=0; i<4; i++)
        array[i] = (lista *) malloc(sizeof(lista));
    array[0]->value = 2;
    array[0] -> next = element;
    element ->prev = array[0];
    lastelement -> next = array[0];
    array[0]->prev = lastelement;
    
    printf("\n firstelement: %d", firstelement->value);
    printf("\n element: %d", element->value);
    printf("\n array[0]: %d", array[0]->value);
    printf("\n lastelement: %d \n", lastelement->value);
    
    array[0] = element; //ora ho due copie di element.
    if(array[0]->next == element) printf("\n CAZZO?");
    if(array[0]->next == firstelement) printf("\n STOCAZZO!");
    //free(element);
    printf("\n firstelement: %d", firstelement->value);
    printf("\n array[0]->next: %d", array[0]->next->value);
    printf("\n array[0]: %d", array[0]->value);
    printf("\n array[0]->prev: %d", array[0]->prev->value);
    printf("\n array[0]->prev->prev: %d \n", array[0]->prev->prev->value);
    free(firstelement);
    for(i=0; i<4; i++)
        free(array[i]);
    return 0;
}