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
pint firstofthelastlist(route*r){
    pint cell = cellbyindex(r, r->lastindex);
    return r->lastindex == indexbycell(r, cell) && r->lastindex > 0; //serve per controllare allocamento.
}
stazione * min_stazione(route * r){
    return r->AUTOSTRADA[0];
}
stazione * max_stazione(route * r){
    return r->AUTOSTRADA[0]->prev;
}
#pragma endregion
#pragma region Metodigestionestrutturadati
route * InitializeAUTOSTRADA(pint km/*, rbhead * autos*/){
    route * r = (route *) malloc(sizeof(route));
    r->len = 4;
    stazione ** try = (stazione **) malloc(sizeof(stazione*)*4);
    r->AUTOSTRADA = try;
    int i;
    for(i=0; i<4; i++)
        r->AUTOSTRADA[i] = (stazione *)malloc(sizeof(stazione*));
    r-> AUTOSTRADA[0] ->kms = km;
    r->AUTOSTRADA[0]->next = NULL;
    r->AUTOSTRADA[0]->prev = NULL;
    //r->AUTOSTRADA[0] -> vetture = autos;
    r->lastindex = 0;
    return r;
}
stazione * initializestazione(pint km/*,rbhead * autos*/){
    stazione * s = (stazione *) malloc(sizeof(stazione));
    s->kms = km;
    //s->vetture = autos;
    return s; 
}
//dopo inserimento
void scrolling_forward(route * r, pint start, pint stop){
    pint i = start;
    while (i <= stop)
    {
        r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->prev;
        i++;
    } //vedi se fixref 
}
//dopo cancellazione
void scrolling_back(route * r, pint start, pint stop){
    pint i = start;
    while (i <= stop)
    {
        r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;
        i++;
    }//vedi se fixref 
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
void riferimentoprimoelemento(route * r, pint km, pint index){
    stazione * ref = r->AUTOSTRADA[0];
    r->AUTOSTRADA[index] = initializestazione(km);
    r->AUTOSTRADA[index]->prev = ref -> prev;
    ref ->prev = r->AUTOSTRADA[index];
    r->AUTOSTRADA[index]->next = ref;
}

/// @brief Devo ricercare l'elemento nella riga "cell". Se devo effetturare ricerca, cerco il riferimento esatto, altrimenti ritorno NULL. Se invece devo inserire, restituisco riferimento a cui attaccare in coda.
/// @param r //autostrada
/// @param km //km stazione da cercare
/// @param cell riferimento riga
/// @param inserimento //1 se inserisci, 0 se cerchi
/// @return //puntatore a stazione
stazione * deepsearch(route * r, pint km, pint cell, int inserimento){
    stazione * curr = r->AUTOSTRADA[cell];
    pint i = 0;
    pint nodes = autolen(r);
    if(cell == 0 && curr->kms > km)//unico caso, inserimento minimo. In altri casi avviene prima la selezione.
    {
        if(inserimento == 1){
        riferimentoprimoelemento(r,km,cell);
        r->lastindex++;
        pint last = lastline(r);
        //scrolling
        scrolling_forward(r,1,last-1);
        if(firstofthelastlist(r)) //può capitare che stia popolando una nuova riga.
        {
            curr = r->AUTOSTRADA[last - 1];
            i = 0;
            while (i<nodes){
                curr = curr->next;
                i++;
            }
            r->AUTOSTRADA[last] = curr;
        }
        else
        {
            r->AUTOSTRADA[last] = r->AUTOSTRADA[last]->prev;
        }
        //end scrolling
        Check(r);
        }
        return NULL;
    }
    while (curr->next != NULL && curr->kms < km && i < nodes)
    {
        curr = curr->next;
        i++;
    }//arrivo a riferimento candidato.
    if(inserimento == 1){
        if(curr->kms == km)
        {
            printf("\n Già presente una stazione al km %d.\n", km);
            return NULL;
        }
        if(curr->kms < km)
            if(i == nodes) //ho massimo di riga
            {
                pint last = lastline(r);
                if(cell < last)
                {
                    stazione *ref = r->AUTOSTRADA[cell + 1];
                    stazione * new = initializestazione(km);
                    new ->next = ref;
                    r->AUTOSTRADA[cell + 1] = new;
                    new ->prev = curr;
                    curr ->next = new;
                    if(ref != NULL) ref->prev = new;
                    r->lastindex++;
                    last = lastline(r);
                    //codice scrolling/checking
                    scrolling_forward(r,cell+1,last-1);
                    if(firstofthelastlist(r)) //può capitare che stia popolando una nuova riga.
                    {
                        curr = r->AUTOSTRADA[last - 1];
                        i = 0;
                        while (i < nodes){
                            curr = curr->next;
                            i++;
                        }
                        r->AUTOSTRADA[last] = curr;
                    }
                    else
                    {
                        r->AUTOSTRADA[last] = r->AUTOSTRADA[last]->prev;
                    }
                    //end scrolling
                    Check(r);
                    return NULL;
                } //già controllato da check se ho lo spazio solo per un ultimo massimo, quindi else non si verifica.
            }
            else if(curr->next == NULL) //ho massimo assoluto.
            {
                stazione * new = initializestazione(km);
                new -> prev = curr;
                curr ->next = new;
                new ->next = NULL;
                r->AUTOSTRADA[0] ->prev = new;
                r->lastindex++;
                //no scrolling, messo in coda.
                Check(r);
                return NULL;
            }
        if(curr->kms > km && i > 0)
            return curr ->prev;
    }
    else if(inserimento == 0)
    {
        if(curr->kms == km)
            return curr;
        else
            return NULL;
    }
}
stazione * binarysearch(route * r, pint km, pint start, pint stop, int inserimento){
    pint mid;
    if(start < stop){  
        mid = (pint)(ceil((start+stop)/2));
        if(km == r->AUTOSTRADA[mid]->kms) //caso ottimo, trovata.
            return r->AUTOSTRADA[mid];
        if(km > r->AUTOSTRADA[mid] ->kms)//è nella seconda parte.
        {
            if(km > r->AUTOSTRADA[mid+1]->kms) //[mid] è molto lontano da elemento.
                return binarysearch(r,km,mid,stop,inserimento);
            else if(km == r->AUTOSTRADA[mid+1]->kms)
                return r->AUTOSTRADA[mid+1]; 
            else //è tra [mid] e [mid+1]
                return deepsearch(r,km,mid,inserimento);
        }
        else if(km < r->AUTOSTRADA[mid]->kms) //prima parte.
        {
            if(km < r->AUTOSTRADA[mid-1]->kms)//[mid] è molto lontano da elemento.
                return binarysearch(r,km,start,mid,inserimento);
            else if(km == r->AUTOSTRADA[mid-1]->kms)
                return r->AUTOSTRADA[mid-1];
            else //in profondità
                return deepsearch(r,km,mid-1,inserimento);
        }
    }
    return NULL;    
}
stazione * seqsearch(route* r, pint km, int inserimento){
    int i = 0;
    int cells = lastline(r);
    while (r->AUTOSTRADA[i]->kms < km && i <= cells)
        i++;
    if(i <= cells){
        if(r->AUTOSTRADA[i]->kms == km) //potrebbe fregarmi i più alto di cells.
            return r->AUTOSTRADA[i];
        else if(i > 0)
            return deepsearch(r,km,i-1,inserimento); //cerco nell'ultimo indice che mi dà minore.
        else
            return deepsearch(r,km,0,inserimento);
    }
    else
    {
        return deepsearch(r,km,cells,inserimento);
    }
}
stazione * cerca(route* r, pint km, int inserimento){
    if(r->lastindex < 9) //se lastindex è 9, ho 10 elementi e almeno le prime 4 righe piene
        return seqsearch(r,km,inserimento);
    return binarysearch(r,km, 0, lastline(r), inserimento);
}
stazione * ricerca(route* r, pint km){
    return cerca(r, km, 0);
}
void plotline(route *r, pint line){
    if(r->AUTOSTRADA[line] != NULL && lastline(r)>=line){
    stazione * curr = r->AUTOSTRADA[line];
    pint end = autolen(r);
    pint i = 0; 
    printf("\n%d: ", line);
    while(i < end && curr -> next != NULL){
        printf("\t %d", curr->kms);
        curr = curr->next;
        i++;
    }
    if(curr->next == NULL && i<end) printf("\t %d", curr->kms); //ultimo
    printf("\n");
    }
    else{
        printf("\n\tRiga %d non esistente.\n", line);
    }
}
#pragma endregion
#pragma region Inserimentostazioni
pint linebyelement(route * r, stazione * element){
    pint start = 0;
    pint stop = lastline(r);
    pint mid = (pint)(ceil(start+stop/2)); 
    while(start < stop){
    if(r->AUTOSTRADA[mid]->kms == element ->kms)
        return mid;
    if(r->AUTOSTRADA[mid]->kms < element ->kms)
        if(r->AUTOSTRADA[mid+1]->kms > element ->kms)
        {
            pint i = 0;
            pint nodes = autolen(r);
            stazione * curr = r->AUTOSTRADA[mid];
            while (i < nodes && curr ->kms < element ->kms)
            {
                curr = curr->next;
                i++;
            }
            if(i < nodes && curr ->kms == element ->kms)
                return mid;
            else
                return NULL;
        }
        else if(r->AUTOSTRADA[mid+1]->kms == element ->kms)
            return mid + 1;
        else
            start = mid;
    else
        if(element->kms < r->AUTOSTRADA[mid-1]->kms)//[mid] è molto lontano da elemento.
            stop = mid;
        else if(element ->kms == r->AUTOSTRADA[mid-1]->kms)
            return mid-1;
        else //in profondità
        {
            pint i = 0;
            pint nodes = autolen(r);
            stazione * curr = r->AUTOSTRADA[mid-1];
            while (i < nodes && curr ->kms < element ->kms)
            {
                curr = curr->next;
                i++;
            }
            if(i < nodes && curr ->kms == element ->kms)
                return mid-1;
            else
                return NULL;
        }
    }
}
void inserimento(route * r, pint km){
    stazione * ref = cerca(r,km, 1);
    if(ref == NULL)
        return; //caso limite già inserito
    stazione * new = initializestazione(km);
    stazione * over = ref->next; //questo non dovrebbe essere NULL, perché altrimenti ho già inserito nuovo massimo.
    if(over != NULL)
        over ->prev = new;
    new ->next = over;
    pint firstcell = linebyelement(new);
    pint last = lastline(r);
    scrolling_forward(r,firstcell,last);
    r->lastindex++;
    if(firstofthelastlist(r))
    {
        //aggiustiref.
    }

}
#pragma endregion

int main(){ //aggiusta inserimenti, devo mantenere ordinato. Aggiusta cancellazioni, non le conta, aggiusta indicizzazioni.
    route * sixtysix = InitializeAUTOSTRADA(0);
    inserimento(sixtysix,100);
    inserimento(sixtysix, 100000);
    inserimento(sixtysix, 2);
    inserimento(sixtysix, 200);
    plotline(sixtysix,0);
    plotline(sixtysix,1);
    //cancella(sixtysix, 0);
}

