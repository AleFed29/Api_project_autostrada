#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
copia di peso da "rbtree_str.c" purtroppo per il progetto va tutto su un file
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
    stazione ** try = (stazione **) malloc(sizeof(stazione*)*4);
    r->AUTOSTRADA = try;
    r-> AUTOSTRADA[0] ->kms = kms1;
    r->AUTOSTRADA[0]->next = NULL;
    r->AUTOSTRADA[0]->prev = NULL;
    //r->AUTOSTRADA[0] -> vetture = autos;
    r->lastindex = 0;
    return r;
}
void fixmax(route*r){
    int lastcell = (int)(log2(r->lastindex + 1));
    stazione* curr = r->AUTOSTRADA[lastcell];
    while (curr->next != NULL)
        curr = curr->next;
    r->AUTOSTRADA[0]->prev = curr;
}
stazione * min(route * r){
    return r->AUTOSTRADA[0];
}
stazione * max(route * r){
    return r->AUTOSTRADA[0]->prev;
}
stazione * initializestazione(int km/*,rbhead * autos*/){
    stazione * s = (stazione *) malloc(sizeof(stazione));
    s->kms = km;
    //s->vetture = autos;
    return s; 
}
void Check(route * r){
    if(r->len - r-> lastindex < 2){
        stazione ** new = (stazione **)realloc(r->AUTOSTRADA,sizeof(stazione*)* (r->len * 2)); //raddoppio lunghezza vettore.
        free(r->AUTOSTRADA);
        r->AUTOSTRADA = new;
        int i,j;
        int cells = (int)(log2(r->lastindex + 1));
        for(i = 1; i < cells-1; i++) //elemento i-esimo array deve puntare a elemento i posti dopo.
            for(j = 0; j < i; j++) //n-1+1 elementi in [0], [1] si sposta di 1 in avanti. E così via...
                r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;
        r->len *= 2;
    }
}
int binarysearch(route * r, int km, int start, int stop){
    int mid = (int)(ceil((start+stop)/2)); 
    if(km == r->AUTOSTRADA[mid]->kms) //caso ottimo, trovata.
        return (int)(pow(2,mid));
    if(km > r->AUTOSTRADA[mid] ->kms) //è nella seconda parte.
    {
        if(km > r->AUTOSTRADA[mid+1]->kms) //[mid] è molto lontano da elemento.
            return binarysearch(r,km,mid,stop);
        else if(km == r->AUTOSTRADA[mid+1]->kms)
            return (int)(pow(2,mid+1)); 
        else //è tra [mid] e [mid+1]
        {
            int i;
            int nodes = (int)(log2(r->lastindex + 1));
            stazione * curr = r->AUTOSTRADA[mid];
            for(i = 0; i < nodes; i++)
            {
                if(curr->kms == km)
                    return (int)(pow(2,mid)) + i;
                curr = curr ->next;
            }
            return -1; //non c'è
        }
    }
    else if(km < r->AUTOSTRADA[mid]->kms) //prima parte.
    {
        if(km < r->AUTOSTRADA[mid-1]->kms)//[mid] è molto lontano da elemento.
            return binarysearch(r,km,start,mid);
        else if(km == r->AUTOSTRADA[mid-1]->kms)
            return (int)(pow(2,mid-1));
        else //in profondità
        {
            int i;
            int nodes = (int)(log2(r->lastindex + 1));
            stazione * curr = r->AUTOSTRADA[mid-1];
            for(i = 0; i < nodes; i++)
            {
                if(curr->kms == km)
                    return (int)(pow(2,mid)) + i;
                curr = curr ->next;
            }
            return -1; //non c'è
        }
    }
}
void insertion(route * r, int km, int index){
    int i;
    stazione * curr = r->AUTOSTRADA[index];
    while(curr -> kms <= km && curr -> next != NULL){
        if(curr->kms == km){
            printf("\n Già presente");
            return;
        }
        else
            curr = curr ->next;
    }
    curr = curr ->prev;//torno ad ultimo riferimento più piccolo "A" e metto tra A e B.
    stazione * ref = curr -> next; //C <- B
    curr -> next = initializestazione(km);//B<-new
    curr ->next->next = ref;//B->next = C;
    curr ->next->prev = curr;//B->prev = A; mentre C->prev ora è A e C->next resta D;
    if(ref != NULL) ref ->prev = curr->next;//C->prev = B.
    r->lastindex++;
    if(r->AUTOSTRADA[(int)(log2(r->lastindex + 1))]->kms < km) //potrei aver inserito nuovo max.
        fixmax(r);
    Check(r);
}
void inserimento(route * r, int km){
    int stop = (int)(log2(r->lastindex+1));
    int mid; 
    int start = 0;
    if(r->AUTOSTRADA[stop]->kms < km){
        insertion(r,km,stop);
    }
    while (start <= stop)
    {
        mid = (int)((start+stop)/2);
        if(r->AUTOSTRADA[mid]->kms == km){
            printf("\n Già presente");
            return;
        }
        if(r->AUTOSTRADA[mid]->kms < km)
        {
            if(r->AUTOSTRADA[mid+1]->kms > km){
                insertion(r,km,mid); //tra [mid] e [mid+1]
                return;
            }
            start = mid;
        }
        else if(r->AUTOSTRADA[mid]->kms > km){
            if(r->AUTOSTRADA[mid-1]->kms < km){
                insertion(r,km,mid-1); //tra [mid-1] e [mid]
                return;
            }
            stop = mid;
        }
    }
}

int cercaindice(route* r, int km){
    return binarysearch(r,km, 0, (int)(log2(r->lastindex + 1)));
}
stazione * cerca(route* r, int km){
    int index = cercaindice(r,km);
    if(index == -1)
        return NULL;
    int cell = (int)(log2(index));
    int prof = index - (int)(pow(2,cell));
    int i;
    stazione * curr = r->AUTOSTRADA[cell];
    for(i = 0; i <= prof; i++)
        curr = curr -> next;
    return curr;
}

void cancella(route * r, int km){
    int index = binarysearch(r,km,0,(int)(log2(r->lastindex + 1)));
    if(index == -1)
    {
        printf("\n Non c'è.");
        return;
    }
    int cell = (int)(log2(index));
    stazione * s = r->AUTOSTRADA[cell];
    while (s->next->kms < km)
        s = s->next;
    stazione * A = s->prev;
    stazione * B = s->next;
    A->next = B;
    if(B!=NULL) B->prev = A;
    free(s);
    printf("\n Cancellato"); 
}

int main(){
    route * sixtysix = InitializeAUTOSTRADA(100);
    inserimento(sixtysix, 66);
    inserimento(sixtysix, 100000);
    inserimento(sixtysix, 2);
    inserimento(sixtysix, 200);
    cancella(sixtysix, 100000);
    stazione * s = cerca(sixtysix,2);
    printf("\n %d", s->kms);
    inserimento(sixtysix, 200);
    cancella(sixtysix, 3);
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

