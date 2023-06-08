#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
copia di peso da "rbtree_str.c" purtroppo per il progetto va tutto su un file
*/
/*
typedef struct rbhead vettura;
typedef struct rbhead autostrada;

typedef struct stazione{
    rbelement * self;
    vettura * auto;
}

typedef struct percorso{
    rbelement * tappa;
    struct percorso * next;
}path;
*/
typedef struct station 
{
    int kms;
    struct station * next;
    struct station * prev;
    //rbhead * vetture;
}stazione;
typedef struct 
{
    stazione ** AUTOSTRADA;
    int lastindex;
    int len;
}route;
route * InitializeAUTOSTRADA(int kms1/*,rbhead * autos*/){
    route * r = (route *) malloc(sizeof(route));
    r->len = 4;
    r-> AUTOSTRADA = (stazione **) malloc(sizeof(stazione*)*4);
    r-> AUTOSTRADA[0] ->kms = kms1;
    r->AUTOSTRADA[0]->next = NULL;
    r->AUTOSTRADA[0]->prev = NULL;
    //r->AUTOSTRADA[0] -> vetture = autos;
    r->lastindex = 0;
    return r;
}
stazione * initializestazione(int km/*,rbhead * autos*/){
    stazione * s = (stazione * s) malloc(sizeof(stazione));
    s->kms = km;
    //s->vetture = autos;
    return s; 
}
int log2(int n){
    return int(log(n)/log(2));
}
void Check(route * r){
    if(r->len - r-> lastindex < 2){
        r->AUTOSTRADA = (stazione *) realloc(sizeof(stazione), r->len * 2);
        int i,j;
        int cells = log2(r->lastindex + 1);
        for(i = 1; i < cells-1; i++)
            for(j = 0; j < i; j++)
                r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;
        r->len *= 2;
    }
}
stazione * binarysearch(route r, int km, int start, int stop){
    int mid = int((start+stop)/2); //ceiling?
    if(km == r->AUTOSTRADA[mid]->kms)
        return pow(2,mid);
    if(km > r->AUTOSTRADA[mid] ->kms){
        if(km > r->AUTOSTRADA[mid-1]->kms)
            return binarysearch(r,km,mid,stop);
        else if(km == r->AUTOSTRADA[mid-1]->kms)
            return pow(2,mid-1); //int per pow?
        else
        {
            int i;
            int nodes = log2(r->lastindex + 1);
            stazione * curr = r->AUTOSTRADA[mid -1];
            for(i = 0; i < nodes; i++)
            {
                if(curr->kms == km)
                    return pow(2,mid-1) + i;
                curr = curr ->next;
            }
        }
    }
    else if(km < r->AUTOSTRADA[mid+1]->kms){
        if(km < r->AUTOSTRADA[mid]->kms)
            return binarysearch(r,km,start,mid);
        else if(km == r->AUTOSTRADA[mid]->kms)
            return pow(2,mid);
        else
        {
            int i;
            int nodes = log2(r->lastindex + 1);
            stazione * curr = r->AUTOSTRADA[mid];
            for(i = 0; i < nodes; i++)
            {
                if(curr->kms == km)
                    return pow(2,mid) + i;
                curr = curr ->next;
            }
        }
    }
}

//inserimento, come binary search, ma nel for vedi while(curr -> kms < km), poi inserisci.
//per cancella simile.

//if max(vetture) -> value > stazione1 -> kilometraggio -  stazione -> kilometraggio; ok.
//alberi rossoneri anche là? può essere idea, perché ti muovi a w sull'albero.
//pensaci, nel caso però devi fare struct RedBlack tree e poi fare due alias con typedef.

/*
//devi trovare modo per spostarti sull'albero delle stazioni e mantenere riferimento alle auto.
con precedente e successivo poi ti sposti.
autostrada * InitializeRoute(int kmstaz){
    return (autostrada *)(Initializetree(kmstaz));
}
path * get_path(autostrada * route, int km1, int km2){
    //se km1 < km2 
    rbelement * p =  (rbelement *) malloc(sizeof(rbelement));
    rbelement * a =  (rbelement *) malloc(sizeof(rbelement));
    p = search(route, km1);
    a = search(route, km2);

}
//parti da arrivo, this va indietro finché la massima autonomia della stazione precedente non è minore della distanza tra arrivo e quella stazione precedente -> ottengo ultima stazione da cui fare ultimo salto.
//esco se la stazione arrivo e quella di partenza coincidono.
// richiamo la funzione
//l -> next = last_of_last(route, p, this, l);
return l; 
la popoli a ritroso. 
path * last_of_last(autostrada * route,stazione * partenza, stazione * arrivo, percorso * l){
    stazione * this = (stazione *) malloc(sizeof(stazione));
    this = arrivo;
    while(this -> self -> value > p -> self -> value && max(predecessore(route, this->self) -> auto) < arrivo -> self -> value - predecessore(this->self) -> value){

    }
}
*/

