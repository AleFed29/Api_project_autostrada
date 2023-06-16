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
int autolen(route * r){
    return (int)(log2((r->len))) + 1; //numero nodi per fila
}
int lastline(route *r){
    return (int)(r->lastindex/autolen(r)); //ultimo indice array con qualcosa dentro.
}
int cellbyindex(int index){
    return (int)(index/autolen(r))
}
int indexbycell(int cell){
    return cell*autolen(r);
}
route * InitializeAUTOSTRADA(int kms1/*,rbhead * autos*/){
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
stazione * initializestazione(int km/*,rbhead * autos*/){
    stazione * s = (stazione *) malloc(sizeof(stazione));
    s->kms = km;
    //s->vetture = autos;
    return s; 
}
void Check(route * r){
    if(indexbycell(r->len) - r-> lastindex < 2){
        stazione ** new = (stazione **)realloc(r->AUTOSTRADA,sizeof(stazione*)* (r->len * 2)); //raddoppio lunghezza vettore.
        free(r->AUTOSTRADA);
        r->AUTOSTRADA = new;
        int i,j;
        int cells = autolen(r);
        for(i = 1; i < cells-1; i++) //elemento i-esimo array deve puntare a elemento i posti dopo.
            for(j = 0; j < i; j++) //n-1+1 elementi in [0], [1] si sposta di 1 in avanti. E così via...
                r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;
        r->len *= 2;
    }
}
int cercaprof(route * r, int km, int cell){
    int i;
    int nodes = autolen(r);
    stazione * curr = r->AUTOSTRADA[cell];
    for(i = 0; i < nodes; i++)
    {
        if(curr->kms == km)
            return indexbycell(cell) + i;
        curr = curr ->next;
    }
    return -1; //non c'è
}
int binarysearch(route * r, int km, int start, int stop){
    int mid = (int)(ceil((start+stop)/2)); 
    if(km == r->AUTOSTRADA[mid]->kms) //caso ottimo, trovata.
        return indexbycell(mid);
    if(km > r->AUTOSTRADA[mid] ->kms) //è nella seconda parte.
    {
        if(km > r->AUTOSTRADA[mid+1]->kms) //[mid] è molto lontano da elemento.
            return binarysearch(r,km,mid,stop);
        else if(km == r->AUTOSTRADA[mid+1]->kms)
            return indexbycell(mid+1); 
        else //è tra [mid] e [mid+1]
            cercaprof(r,km,mid);
    }
    else if(km < r->AUTOSTRADA[mid]->kms) //prima parte.
    {
        if(km < r->AUTOSTRADA[mid-1]->kms)//[mid] è molto lontano da elemento.
            return binarysearch(r,km,start,mid);
        else if(km == r->AUTOSTRADA[mid-1]->kms)
            return indexbycell(mid-1);
        else //in profondità
            cercaprof(r,km,mid-1);
    }
}
stazione * profonditainserimento(route * r, int km, int index){
    int i;
    stazione * curr = r->AUTOSTRADA[index];
    while(curr -> kms <= km && curr -> next != NULL){
        if(curr->kms == km){
            printf("\n Già presente");
            return NULL;
        }
        else
            curr = curr ->next;
    }
    if(curr->next!= NULL) //se non era ultimo riferimento.
        curr = curr ->prev;//torno ad ultimo riferimento più piccolo "A" e metto tra A e B.
    return curr;
}
int seqsearch(route* r, int km){
    int i = 0;
    int cells = lastline(r);
    while (r->AUTOSTRADA[i]->kms < km && i < cells)
        i++;
    i--;
    if(r->AUTOSTRADA[cells] == km)
        return indexbycell(cells);
    else
        cercaprof(r,km,i); //cerco nell'ultimo indice che mi dà minore.
}
int cercaindice(route* r, int km){
    if(r->lastindex < 9)
        return seqsearch(r,km);
    return binarysearch(r,km, 0, lastline(r));
}
//se inserisco elemento, controlla che io abbia al più autolen(r) elementi e se no aggancia primo elemento di troppo ad indice successivo.
void fixline(route * r, int km){
    int len = autolen(r);
    int cell = lastline(r);
    stazione * curr = r->AUTOSTRADA[cell];
    int i = 0;
    while (i < len)
    {
        if(curr ->next == NULL)
            return;
        curr = curr -> next;
        i++;
    }
    if(curr->next != NULL) //cell non è ultima cella, perché altrimenti Check avrebbe raddoppiato tutto.
        r->AUTOSTRADA[cell + 1] = curr;
}
void insertion(route * r, int km, int index){
    stazione * curr = profonditainserimento(r,km,index);
    if(curr == NULL)
        return;
    stazione * ref = curr -> next; //C <- B
    curr -> next = initializestazione(km);//B<-new
    curr ->next->next = ref;//B->next = C;
    curr ->next->prev = curr;//B->prev = A; mentre C->prev ora è A e C->next resta D;
    if(ref != NULL) ref ->prev = curr->next;//C->prev = B.
    r->lastindex++;
    if(r->AUTOSTRADA[lastline(r)]->kms < km) //potrei aver inserito nuovo max.
        fixmax(r);
    if(indexbycell(lastline(r))+autolen(r)- r-> lastindex < 3) //controllo lunghezza linea
        fixline(r);//questo controllo va fatto sulla linea dove inserisci, non l'ultima.
    Check(r);
}
void littleinsert(route * r, int km){
    int cell = lastline(r);
    int i = 0;
    while (i<cell && r->AUTOSTRADA[i]->kms < km)
        i++;
    if(r->AUTOSTRADA[i]->kms > km)
        i--;
    insertion(r,km,i);
}
void inserimento(route * r, int km){
    if(r->lastindex < 9) //qualche fila vuota.
        littleinsert(r,km);
    int mid; 
    int start = 0;
    int stop = lastline(r);
    if(r->AUTOSTRADA[stop]->kms < km)
        insertion(r,km,stop);
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
stazione * cerca(route* r, int km){
    int index = cercaindice(r,km);
    if(index == -1)
        return NULL;
    int cell = cellbyindex(index);
    int prof = index - indexbycell(cell);
    int i;
    stazione * curr = r->AUTOSTRADA[cell];
    for(i = 0; i <= prof; i++)
        curr = curr -> next;
    return curr;
}
void cancella(route * r, int km){
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
    int cell = cellbyindex(index);
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
    int line = cell + 1; //da cancellato, faccio scalare di 1 tutti quanti.
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

