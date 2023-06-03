#include<stdio.h>
#include<stdlib.h>
#include<rb_autonomies.h> //(?)

//qui farai i metodi e la struct per le stazioni
typedef struct stazione
{
    int kilometraggio;
    autonotree * vetture;
};

//if max(vetture) -> value > stazione1 -> kilometraggio -  stazione -> kilometraggio; ok.
//alberi rossoneri anche là? può essere idea, perché ti muovi a w sull'albero.
//pensaci, nel caso però devi fare struct RedBlack tree e poi fare due alias con typedef. 