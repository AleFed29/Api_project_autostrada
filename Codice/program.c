#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef unsigned int pint;
/*
copia di peso da "rbtree_str.c" purtroppo per il progetto va tutto su un file
*/
pint Log2( pint x )
{
  pint ans = 0;
  while(x > 1){
    x = (pint)(x/2);
    ans++;
  } 
  return ans;
}
typedef struct station 
{
    pint kms;
    struct station * next;
    struct station * prev;
    //rbhead * vetture;
}stazione;
typedef struct 
{
    stazione ** AUTOSTRADA;
    pint lastindex;
    pint len;
}route;
//array lungo m, ho log2(m) + 1 nodi per fila.
//n = mlog2(m) + m => n = m*ln(2m) => e^n = (2m)^m.
pint autolen(route * r){
    return Log2((r->len)) + 1; //numero nodi per fila
}
pint lastline(route *r){
    return (pint)(r->lastindex/autolen(r)); //ultimo indice array con qualcosa dentro.
}
pint cellbyindex(route*r, int index){ //posso avere index -1
    return (pint)(index/autolen(r));
}
pint indexbycell(route*r, pint cell){
    return cell*autolen(r);
}
route * InitializeAUTOSTRADA(pint kms1/*,rbhead * autos*/){
    route * r = (route *) malloc(sizeof(route));
    r->len = 4;
    stazione ** try = (stazione **) malloc(sizeof(stazione*)*4);
    r->AUTOSTRADA = try;
    int i;
    for(i=0; i<4; i++)
        r->AUTOSTRADA[i] = (stazione *)malloc(sizeof(stazione*));
    r-> AUTOSTRADA[0] ->kms = kms1;
    r->AUTOSTRADA[0]->next = NULL;
    r->AUTOSTRADA[0]->prev = NULL;
    //r->AUTOSTRADA[0] -> vetture = autos;
    r->lastindex = 0;
    return r;
}
void fixmax(route*r){
    int lastcell = lastline(r);
    stazione* curr = r->AUTOSTRADA[lastcell];
    while (curr->next != NULL)
        curr = curr->next;
    r->AUTOSTRADA[0]->prev = curr;
}
stazione * min_stazione(route * r){
    return r->AUTOSTRADA[0];
}
stazione * max_stazione(route * r){
    return r->AUTOSTRADA[0]->prev;
}
stazione * initializestazione(pint km/*,rbhead * autos*/){
    stazione * s = (stazione *) malloc(sizeof(stazione));
    s->kms = km;
    //s->vetture = autos;
    return s; 
}
void Check(route * r){
    if(indexbycell(r, r->len) - r-> lastindex < 2){
        stazione ** new = (stazione **)realloc(r->AUTOSTRADA,sizeof(stazione*)* (r->len * 2)); //raddoppio lunghezza vettore.
        free(r->AUTOSTRADA);
        r->AUTOSTRADA = new;
        int i,j;
        pint cells = autolen(r); //devo spostare per L = log2(m) + 1 file.
        for(i = 1; i < cells-1; i++) //elemento i-esimo array deve puntare a elemento i posti dopo.
            for(j = 0; j < i; j++) //[0] L-1 + 1,[1] si sposta di 1, [1] L -1+2, [2] si sposta di 2 ecc... 
                r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;
        r->AUTOSTRADA[cells] = NULL; //r->AUTOSTRADA[i]->next; la riga L all'inizio ha L elementi, alla fine ne avrà 0. 
        r->len *= 2;
    }
}
int cercaprof(route * r, pint km, pint cell){
    int i;
    pint nodes = autolen(r);
    stazione * curr = r->AUTOSTRADA[cell];
    for(i = 0; i < nodes; i++)
    {
        if(curr->kms == km)
            return indexbycell(r, cell) + i;
        curr = curr ->next;
    }
    return -1; //non c'è
}
int binarysearch(route * r, pint km, pint start, pint stop){
    pint mid;  
    while (start <= stop)
    {
        mid = (pint)(ceil((start+stop)/2));
        if(km == r->AUTOSTRADA[mid]->kms) //caso ottimo, trovata.
            return indexbycell(r,mid);
        if(km > r->AUTOSTRADA[mid] ->kms) //è nella seconda parte.
        {
            if(km > r->AUTOSTRADA[mid+1]->kms) //[mid] è molto lontano da elemento.
                return binarysearch(r,km,mid,stop);
            else if(km == r->AUTOSTRADA[mid+1]->kms)
                return indexbycell(r, mid+1); 
            else //è tra [mid] e [mid+1]
                return cercaprof(r,km,mid);
        }
        else if(km < r->AUTOSTRADA[mid]->kms) //prima parte.
        {
            if(km < r->AUTOSTRADA[mid-1]->kms)//[mid] è molto lontano da elemento.
                return binarysearch(r,km,start,mid);
            else if(km == r->AUTOSTRADA[mid-1]->kms)
                return indexbycell(r, mid-1);
            else //in profondità
                return cercaprof(r,km,mid-1);
        }
    }
    return -1;
}
int seqsearch(route* r, pint km){
    int i = 0;
    int cells = lastline(r);
    while (r->AUTOSTRADA[i]->kms < km && i < cells)
        i++;
    if(r->AUTOSTRADA[i]->kms == km)
        return indexbycell(r, i);
    else
        return cercaprof(r,km,i-1); //cerco nell'ultimo indice che mi dà minore.
}
int cercaindice(route* r, pint km){
    if(r->lastindex < 9) //se lastindex è 9, ho 10 elementi e almeno le prime 4 righe piene
        return seqsearch(r,km);
    return binarysearch(r,km, 0, lastline(r));
}
stazione * profonditainserimento(route * r, pint km, pint index){
    stazione * curr = r->AUTOSTRADA[index];
    while(curr -> kms <= km && curr -> next != NULL){
        if(curr->kms == km){
            printf("\n Già presente");
            return NULL;
        }
        else
            curr = curr ->next;
    }
    if(curr->next != NULL || curr -> kms > km) //se non era ultimo riferimento o lo era ed è più grande.
        curr = curr ->prev;//torno ad ultimo riferimento più piccolo "A" e metto tra A e B.            
    return curr;
}

void insertion(route * r, pint km, pint index){
    stazione * curr = profonditainserimento(r,km,index);
    if(curr == NULL)
        return;
    stazione * ref = curr -> next; //C <- B
    curr -> next = initializestazione(km);//B<-new
    curr ->next->next = ref;//B->next = C;
    curr ->next->prev = curr;//B->prev = A; mentre C->prev ora è A e C->next resta D;
    if(ref != NULL) ref ->prev = curr->next;//C->prev = B.
    if(r->AUTOSTRADA[lastline(r)]->kms < km) //potrei aver inserito nuovo max.
        fixmax(r);
       //aggiusto linee
    pint line = cellbyindex(r, index) + 1; //da successivo inserito, faccio scalare di 1 tutti quanti.
    pint last = lastline(r);
    while (line <= last){
        r->AUTOSTRADA[line] = r->AUTOSTRADA[line]->prev; //arrivo con uno di anticipo a indice significativo.
        line++;
    }
    r->lastindex++;
    Check(r);
}
void littleinsert(route * r, pint km){
    pint cell = lastline(r);
    int i = 0;
    while (i<cell && r->AUTOSTRADA[i]->kms < km)
        i++;
    if(r->AUTOSTRADA[i]->kms > km)
        i--;
    insertion(r,km,i);
}
void inserimento(route * r, pint km){
    if(r->lastindex < 9) //qualche fila vuota.
        littleinsert(r,km);
    pint mid; 
    pint start = 0;
    pint stop = lastline(r);
    if(r->AUTOSTRADA[stop]->kms < km)
        insertion(r,km,stop);
    while (start <= stop)
    {
        mid = (pint)(ceil((start+stop)/2));
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
stazione * cerca(route* r, pint km){
    int index = cercaindice(r,km);
    if(index == -1)
        return NULL;
    pint cell = cellbyindex(r, index);
    pint prof = index - indexbycell(r, cell);
    int i;
    stazione * curr = r->AUTOSTRADA[cell];
    for(i = 0; i <= prof; i++)
        curr = curr -> next;
    return curr;
}
void cancella(route * r, pint km){
    /*stazione * this = cerca(r,km);
    if(this == NULL)
    {
        printf("\n Non c'è.");
        return;
    }
    this ->prev ->next = this ->next;
    this ->next->prev = this ->prev;
    free(this);*/
    int index = cercaindice(r,km);
    if(index == -1)
    {
        printf("\n Non c'è.");
        return;
    }
    pint cell = cellbyindex(r,index);
    stazione * s = r->AUTOSTRADA[cell];
    while (s->next->kms < km)
        s = s->next;
    stazione * A = s->prev;
    stazione * B = s->next;
    A->next = B;
    if(B!=NULL) B->prev = A;
    free(s);
    printf("\n Cancellato");
    //aggiusto linee
    int line = cellbyindex(r, index) + 1; //da successivo di cancellato, faccio scalare di 1 tutti quanti.
    int last = lastline(r);
    while (line <= last){
        r->AUTOSTRADA[line] = r->AUTOSTRADA[line]->next;
        line++;
    }
    r->lastindex--;
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

