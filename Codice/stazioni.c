#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef unsigned int pint;

#pragma region stazioni
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

pint Log2( pint x )
{
  pint ans = 0;
  while(x > 1){
    x = (pint)(x/2);
    ans++;
  } 
  return ans;
}
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
stazione * max(route *r){
    return r->AUTOSTRADA[0]->prev;
}
stazione * min(route * r){
    return r->AUTOSTRADA[0];
}

#pragma endregion
#pragma region MetodiGestioneStrutturaDati
route * InitializeAUTOSTRADA(pint km/*, rbhead * autos*/){
    route * r = (route *) malloc(sizeof(route));
    r->len = 4;
    stazione ** try = (stazione **) malloc(sizeof(stazione*)*4);
    r->AUTOSTRADA = try;
    r->AUTOSTRADA[0] = (stazione *)malloc(sizeof(stazione*));
    r-> AUTOSTRADA[0] ->kms = km;
    r->AUTOSTRADA[0]->next = NULL;
    r->AUTOSTRADA[0]->prev = r->AUTOSTRADA[0];
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
stazione * EXNOVOstation(pint km/*,head_vetture * autos*/){
    //head_vetture h = ...
    return initializestazione(km/*,h*/);
}
#pragma endregion
#pragma region MetodiScrollingRef
//stessa cosa che avviene in "prova per puntatori"
//ricordare che start >= 0 (riga di operazione).

//dopo inserimento
void scrolling_forward(route * r, pint start, pint stop){
    pint i = start+1;
    while (i <= stop)
    {
        r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->prev;
        //-1 in catene precedenti bilancia inserimento.
        i++;
    } 
    //avviene solo se precedente checking mi ha dato l'ok.
    if(cellbyindex(r, r->lastindex+1) > lastline(r)) //ho inserito elemento di nuova lista, ora però [lastline(r)+1] è NULL
    {
        r->AUTOSTRADA[lastline(r)+1] = max(r); //max
    }
}
//dopo cancellazione
void scrolling_back(route * r, pint start, pint stop){
    pint i = start+1;
    while (i <= stop)
    {
        r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;//+1 in catene precedenti bilancia compilazione.
        i++;
    }
}
void Check(route * r){
    //ho esaurito vecchia linea
    if(r->lastindex % autolen(r) == 0){
    pint last = lastline(r);
    if(r->AUTOSTRADA[last] == NULL)
        if(indexbycell(r, last) == r->lastindex)
            r->AUTOSTRADA[last] = max(r);
    }
    //inizializzo nuova linea.
    if(indexbycell(r, r->len) - r-> lastindex < 2){
        r->AUTOSTRADA = (stazione **)realloc(r->AUTOSTRADA,sizeof(stazione*)* (r->len * 2));
        int i = 1;
        int j;
        pint cells = autolen(r); //devo spostare per L = log2(m) + 1 file.
        pint oldlast = r->len;
        r->len *= 2;//dopo il calcolo di cells, perché io ho l-1 elementi in m righe.
        //porto i elementi dalla riga i alla riga i-1, fino alla riga intera.
        //elemento i-esimo array deve puntare a elemento i posti dopo. Si svuota [cells], ma [cells-1] assorbe tutti i L-1 elementi di [cells] e aggiunge l'unico che gli resta.
        while (i < oldlast)
        {
            //[0] L-1 + 1,[1] si sposta di 1, [1] L -1+2, [2] si sposta di 2 ecc... 
            for(j = 0; j < i-1; j++)
            { 
                if(r->AUTOSTRADA[i]->next == NULL)
                    return;
                r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;
            }
            i++;
        }
        //si mette già a posto da sola. //r->AUTOSTRADA[cells] = NULL; //r->AUTOSTRADA[i]->next; la riga L all'inizio ha L elementi, alla fine ne avrà 0. 
    }
}
#pragma endregion
#pragma region Stampa
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
void plot(route *r){
    pint i;
    pint last = lastline(r);
    for(i = 0; i <= last; i++)
        plotline(r,i);
}
#pragma endregion
#pragma region Ricercacella
int binarysearch(route * r, pint km, pint start, pint stop){
    pint mid;
    if(km > r->AUTOSTRADA[stop]->kms) //ultima riga
        return stop;
    if(km < r->AUTOSTRADA[start]->kms) //prima di prima riga.
        return start;
    if(start < stop){
        mid = (pint)(ceil((start+stop)/2));
        if(km == r->AUTOSTRADA[mid]->kms) //caso ottimo, trovata.
            return mid;
        if(km > r->AUTOSTRADA[mid] ->kms)//è nella seconda parte.
            if(km > r->AUTOSTRADA[mid+1]->kms) //[mid] è molto lontano da elemento.
                return binarysearch(r,km,mid,stop);
            else if(km == r->AUTOSTRADA[mid+1]->kms)
                return mid+1; 
            else //è tra [mid] e [mid+1]
                return mid;
        else if(km < r->AUTOSTRADA[mid]->kms) //prima parte.
            if(km < r->AUTOSTRADA[mid-1]->kms)//[mid] è molto lontano da elemento.
                return binarysearch(r,km,start,mid);
            else
                return mid-1;
    } 
    return -1;
}
int seqsearch(route * r, pint km){
    int i = 0;
    int cells = lastline(r);
    while ( i <= cells && r->AUTOSTRADA[i]->kms < km)
        i++;
    if(i <= cells)
        if(r->AUTOSTRADA[i]->kms == km) //potrebbe fregarmi i più alto di cells.
            return i;
        else if(i > 0)
            return i-1; //cerco nell'ultimo indice che mi dà minore.
        else
            return 0;
    else
        return cells;
}
int cellofelement(route * r, pint km){
    if(r->lastindex < 9)
        return seqsearch(r,km);
    else return binarysearch(r,km,0,lastline(r));
}
#pragma endregion
#pragma region RicercaInserimentoCancellazione
/// @brief Cerca la stazione al chilometro specificato dal parametro
/// @param r Autostrada su cui cercare
/// @param km Chilometro della stazione 
/// @return La stazione, se esiste, altrimenti NULL
stazione * cerca_stazione(route * r, pint km){
    int cell = cellofelement(r,km);
    if(cell == -1)
        return NULL;
    stazione * curr = r->AUTOSTRADA[cell];
    pint i = 0;
    pint nodes = autolen(r);
    while (i < nodes && curr ->next != NULL)
    {
        if(curr->kms == km)
            return curr;
        else if(curr ->kms > km)
            return NULL;
        else
        {
            curr = curr->next;
            i++;    
        }
    }
    if(curr ->next == NULL)
        if(curr->kms == km)
            return curr;
    return NULL;
}
/// @brief Cancella l'elemento stazione passato come parametro da r->AUTOSTRADA
/// @param r Autostrada di cancellazione
/// @param elemento stazione da cancellare
void cancellazione(route * r, stazione * elemento, pint cell, pint posizione){
    //codice di cancellazione elemento
    //codice
    if(cell != 0 || posizione != 0){
        stazione * prima = elemento -> prev;
        stazione * dopo = elemento -> next;
        prima ->next = dopo;
        if(dopo != NULL) //sto cancellando massimo?
            dopo ->prev = prima;
        else
            r->AUTOSTRADA[0]->prev = prima;
        free(elemento);
    }
    else
    {
        stazione * massimo = max(r);
        stazione * vecchio = r->AUTOSTRADA[0];
        r->AUTOSTRADA[0] = r->AUTOSTRADA[0]->next;   
        if(r->AUTOSTRADA[0] != NULL)
            r->AUTOSTRADA[0]->prev = massimo;
        if(elemento == vecchio) //controlla sia così...
            free(vecchio);
    }
    //scrolling
    scrolling_back(r,cell,lastline(r));
    r->lastindex--;
}
void cancella_stazione(route * r, pint km){
    if(r->lastindex < 0) return;
    int cell = cellofelement(r,km);
    if(cell == -1)
        return;
    stazione * curr = r->AUTOSTRADA[cell];
    pint i = 0;
    pint nodes = autolen(r);
    while (i < nodes && curr ->next != NULL)
    {
        if(curr->kms == km){
            cancellazione(r,curr,cell, i);
            return;
        }
        else if(curr ->kms > km){
            printf("\n Non c'è una stazione al km %d.\n", km);
            return;
        }
        else
        {
            curr = curr->next;
            i++;    
        }
    }
    if(curr ->kms != km)
        printf("\n Non c'è una stazione al km %d.\n", km); //in caso in cui km sia più grande di max->kms.
    else
        cancellazione(r,curr,cell, i);
    return;
}
void inserisci_tra_due(stazione*dainserire, stazione*precedente, stazione*successivo){
    if(precedente ->next == successivo)
    {
        dainserire ->prev = precedente;
        dainserire ->next = successivo;
        precedente ->next = dainserire;
        if(successivo != NULL)
            successivo ->prev = dainserire;
    }
}
/// @brief Inserisce elemento nella struttura dati.
/// @param r Autostrada alla quale inserire la stazione.
/// @param km Chilometri della nuova stazione.
/// @param ref Riferimento al futuro predecessore della nuova stazione.
/// @param cell Linea di inserimento.
void inserimento(route * r, pint km, stazione * ref, pint cell){
    stazione * new = EXNOVOstation(km);
    /*new ->prev = ref;
    new ->next = ref->next;
    if(new->next != NULL)
        new->next->prev = new;
    ref -> next = new;*/
    inserisci_tra_due(new, ref, ref->next);
    if(r->AUTOSTRADA[0] ->prev == ref) //è il massimo.
        r->AUTOSTRADA[0]->prev = new; 
    //fase di scrolling-checking
    scrolling_forward(r,cell,lastline(r));
    r->lastindex++;
    Check(r);
}
/// @brief Inserisce elemento nella struttura dati.
/// @param r Autostrada alla quale inserire la stazione.
/// @param km Chilometri della nuova stazione.
/// @param ref Riferimento al futuro successore della nuova stazione.
void inserimento_testa(route * r, pint km, stazione * ref, pint cell){
    stazione * new = EXNOVOstation(km);
    if(ref == r->AUTOSTRADA[cell])
    {
        new ->next = ref;
        new ->prev = ref ->prev;
        if(r->AUTOSTRADA[cell] != NULL)
            ref ->prev = new;
        r->AUTOSTRADA[cell] = new;
        //fase scrolling-checking
        scrolling_forward(r,cell,lastline(r));
        r->lastindex++;
        Check(r);
    }
}
void inserisci_stazione(route * r, int km){
    if(km < r->AUTOSTRADA[0]->kms) //inserisco minimo
    {
        inserimento_testa(r,km, r->AUTOSTRADA[0], 0);
        return;
    }
    int cell = cellofelement(r,km);
    if(cell == -1)
        return;
    stazione * curr = r->AUTOSTRADA[cell];
    pint i = 0;
    pint nodes = autolen(r);
    while (i < nodes && curr ->next != NULL)
    {
        if(curr->kms == km)
        {
            printf("\n Già presente una stazione al km %d.\n", km);
            return;
        }
        else if(curr ->kms > km){
            if(i > 0)
            {
                inserimento(r,km,curr->prev, cell);        
                return;
            }
            //non finisce mai nell'ELSE perché altrimenti vorrebbe dire che ricerca binaria fallisce.
            /*else
            {
                inserimento_testa(r, km, r->AUTOSTRADA[cell], cell);
                return;
            }*/
        }
        else
        {
            curr = curr->next;
            i++;    
        }
    }
    if(i < nodes)// allora curr->next == NULL
        if(curr->kms > km)
            inserimento(r,km,curr->prev, cell);
        else
            inserimento(r,km,curr, cell); //sto inserendo massimo.
    else //sono a fine riga, ma la stazione della nuova riga esiste.
    {//qui sono certo che cell < lastline(r), altrimenti avrei tutto pieno.
        pint newcell = cell+1;
        inserimento_testa(r,km,r->AUTOSTRADA[newcell], newcell);
        scrolling_forward(r,newcell,lastline(r));
        r->lastindex++;
        Check(r);
    }
}
#pragma endregion

#pragma endregion stazioni

int main(){
    route * sixtysix = InitializeAUTOSTRADA(66);
    pint cont;
    /*for(cont = 1; cont < 120; cont+=2)
        inserisci_stazione(sixtysix,cont);
    */
   //inserimento sequenziale non può essere effettuato riga per riga, ma elemento per elemento. Oppure lo si aggiusta.
   //si aggiusta riferimento quando in una riga inserisco in mezzo a due elementi e devo scrollare.
    inserisci_stazione(sixtysix,100);
    inserisci_stazione(sixtysix,100000);
    plot(sixtysix);
    inserisci_stazione(sixtysix,2);
    inserisci_stazione(sixtysix, 200);
    stazione * nuova = cerca_stazione(sixtysix, 200);
    printf("\n %d \n",nuova -> kms);
    plot(sixtysix);
    cancella_stazione(sixtysix, 3);
    cancella_stazione(sixtysix, 100000);
    plot(sixtysix);
    pint i;
    for(i = 0; i < 100; i++)
        inserisci_stazione(sixtysix, 300 + 3*i);
    plot(sixtysix);
}