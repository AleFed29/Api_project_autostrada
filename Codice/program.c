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
#pragma region ParametriStazioni
//array lungo m, ho log2(m) + 1 nodi per fila.
//n = mlog2(m) + m => n = m*ln(2m) => e^n = (2m)^m.
pint autolen(route * r){
    return Log2(r->len) + 1; //numero nodi per fila
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
pint firstofthelist(route*r){
    pint cell = cellbyindex(r, r->lastindex);
    return r->lastindex == indexbycell(r, cell) && r->lastindex > 0; //serve per controllare allocamento.
}
pint firstofthelistbyindex(route * r, pint index){
    pint cell = cellbyindex(r, index);
    return index == indexbycell(r,cell) && index > 0;
}
#pragma endregion
#pragma region Metodigestionestrutturadati
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
        for(i = 1; i < cells; i++) //elemento i-esimo array deve puntare a elemento i posti dopo. Si svuota [cells], ma [cells-1] assorbe tutti i L-1 elementi di [cells] e aggiunge l'unico che gli resta.
            for(j = 0; j < i; j++) //[0] L-1 + 1,[1] si sposta di 1, [1] L -1+2, [2] si sposta di 2 ecc... 
                r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;
        r->AUTOSTRADA[cells] = NULL; //r->AUTOSTRADA[i]->next; la riga L all'inizio ha L elementi, alla fine ne avrà 0. 
        r->len *= 2;
    }
}
#pragma endregion
#pragma region Ricercastazioni
void plotline(route *r, pint line){
    stazione * curr = r->AUTOSTRADA[line];
    pint end = autolen(r);
    pint i = 0; 
    while(i < end){
        printf("\t %d", curr->kms);
        curr = curr->next;
        i++;
    }
    printf("\n");
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
    else if(i > 0)
        return cercaprof(r,km,i-1); //cerco nell'ultimo indice che mi dà minore.
    else
        return cercaprof(r,km,0);
}
int cercaindice(route* r, pint km){
    if(r->lastindex < 9) //se lastindex è 9, ho 10 elementi e almeno le prime 4 righe piene
        return seqsearch(r,km);
    return binarysearch(r,km, 0, lastline(r));
}
stazione * cerca(route* r, pint km){
    int index = cercaindice(r,km);
    if(index == -1)
        return NULL;
    pint cell = cellbyindex(r, index);
    pint prof = index - indexbycell(r, cell);
    int i;
    stazione * curr = r->AUTOSTRADA[cell];
    for(i = 0; i < prof; i++)
        curr = curr -> next;
    return curr;
}
#pragma endregion
#pragma region Inserimentostazioni
stazione * profonditainserimento(route * r, pint km, pint index){
    stazione * curr = r->AUTOSTRADA[index];
    pint len = autolen(r);
    if(index == 0){
        if(r->AUTOSTRADA[0]->kms == km){
                printf("\n Già presente");
                return NULL;
        }
        else if(r->AUTOSTRADA[0]->kms > km){
            stazione * ref = r->AUTOSTRADA[0];
            r->AUTOSTRADA[0] = initializestazione(km);
            r->AUTOSTRADA[0]->prev = ref -> prev;
            ref ->prev = r->AUTOSTRADA[0];
            r->AUTOSTRADA[0]->next = ref;
            return NULL;
        }
    }
    if(index != lastline(r)){
        pint i;
        for(i = 0;i < len; i++){
            if(curr->kms == km){
                printf("\n Già presente");
                return NULL;
            }
            else if(curr -> kms < km)
                curr = curr ->next;
            else
                return curr -> prev; //torno ad ultimo riferimento più piccolo "A" e metto tra A e B.
        }
    }
    else // può non essere completa la lista
    {
        while (curr->next != NULL && curr->kms < km)
            if(curr->kms == km){
                printf("\n Già presente");
                return NULL;
            }
            else if(curr -> kms < km)
                curr = curr ->next;
            else
                return curr->prev;
        if(curr -> kms < km)
            return curr; //è il max.
        else if(curr->kms == km){
                printf("\n Già presente");
                return NULL;
        }
        else
            return curr -> prev;
    }
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
    //sto inserendo in mezzo a due puntatori.

    //mi assicuro che il riferimento alla cella sia ok.    
    if(index > 0)
        if(r->AUTOSTRADA[index]->kms > km && r->AUTOSTRADA[index]->prev->kms < km)
            r->AUTOSTRADA[index] = curr ->next; //fino a qui il mio array punta ancora a vecchio posto se sono nel caso in cui tocca una cella.
    pint line = index + 1; //riga successiva ad inserito, faccio scalare di 1 tutti quanti.
    pint last = lastline(r);
    while (line <= last){
        r->AUTOSTRADA[line] = r->AUTOSTRADA[line]->prev; //arrivo con uno di anticipo a indice significativo.
        line++;   
    }
    if(r->AUTOSTRADA[last] != NULL)
        if(r->AUTOSTRADA[last]->kms < km) //potrei aver inserito nuovo max.
                fixmax(r);
    //aggiusto linee
    r->lastindex++;
    if(firstofthelist(r)) //ho inserito nuova testa di lista nell'ultima riga, ed è il massimo.
    {
        pint linea = lastline(r);
        r->AUTOSTRADA[linea] = (stazione*) malloc(sizeof(stazione));
        r->AUTOSTRADA[linea] = max_stazione(r);
    }
    Check(r);
}
void littleinsert(route * r, pint km){
    pint cell = lastline(r);
    pint i = 0;
    while (i <= cell && r->AUTOSTRADA[i]->kms < km)
        i++;
    if(r->AUTOSTRADA[i]->kms == km)
        printf("\n Già presente");
            return;
    else if(i > 0)
        i--;
    insertion(r,km,i);
}
void inserimento(route * r, pint km){
    if(r->lastindex < 9){ //qualche fila vuota.
        littleinsert(r,km);
        return;
    }
    pint mid; 
    pint start = 0;
    pint stop = lastline(r);
    if(stop == 0)
        insertion(r,km,stop);
    if(r->AUTOSTRADA[stop]->kms < km)
        insertion(r,km,stop);
    while (start <= stop)
    {
        mid = (pint)((start+stop)/2); 
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
        else if(r->AUTOSTRADA[mid]->kms > km && mid > 0){
            if(r->AUTOSTRADA[mid-1]->kms < km){
                insertion(r,km,mid-1); //tra [mid-1] e [mid]
                return;
            }
            stop = mid;
        } else if(mid == 0 && stop == 1){
            if(r->AUTOSTRADA[1]->kms > km && r->AUTOSTRADA[0]->kms < km)
                insertion(r,km,0); //caso limite se non si verifica questo, già gestito prima del while.
        }
    }
}
#pragma endregion 
#pragma region Cancellazionestazioni
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
    while (s->next != NULL)
        if(s->next->kms < km)
            s = s->next;
        else
            break;
    stazione * A = s->prev;
    stazione * B = s->next;
    A->next = B;
    if(B!=NULL) B->prev = A;
    if(km == r->AUTOSTRADA[cell]->kms)
        r->AUTOSTRADA[cell] = r->AUTOSTRADA[cell]->next;
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
#pragma endregion
int main(){ //aggiusta inserimenti, devo mantenere ordinato. Aggiusta cancellazioni, non le conta, aggiusta indicizzazioni.
    route * sixtysix = InitializeAUTOSTRADA(100);
    printf("%d", autolen(sixtysix));
    inserimento(sixtysix, 66);
    inserimento(sixtysix, 100000);
    inserimento(sixtysix, 2);
    inserimento(sixtysix, 200);
    cancella(sixtysix, 100000);
    plotline(sixtysix,0);
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

