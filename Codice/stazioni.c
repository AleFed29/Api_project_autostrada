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
#pragma endregion
#pragma region MetodiGestioneStrutturaDati
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
    if(indexbycell(r, r->len) - r-> lastindex < 2){
        //stazione ** new = (stazione **)realloc(r->AUTOSTRADA,sizeof(stazione*)* (r->len * 2)); //raddoppio lunghezza vettore.
        //r->AUTOSTRADA = new;
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
                for(j = 0; j < i; j++)
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
#pragma endregion


//dai, riprovaci! Ce la puoi fare.
//basta mantenere la calma, usare propriamente la libreria rb, e poi, con cancelazione ed inserimento corretti, hai tutto.
//algo.txt per un percorso.
//l'altro ci penserai.


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
    while (r->AUTOSTRADA[i]->kms < km && i <= cells)
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
    else return binarysearch(r,km,0,lastline(r))
}
#pragma endregion
#pragma region RicercaInserimentoCancellazione
/// @brief Cerca la stazione al chilometro specificato dal parametro
/// @param r Autostrada su cui cercare
/// @param km Chilometro della stazione 
/// @return La stazione, se esiste, altrimenti NULL
stazione * cerca(route * r, pint km){
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
    return NULL;
}
/// @brief Cancella l'elemento stazione passato come parametro da r->AUTOSTRADA
/// @param r Autostrada di cancellazione
/// @param elemento stazione da cancellare
void cancellazione(route * r, stazione * elemento, pint cell){
    //codice di cancellazione elemento
    //codice
    //scrolling
    scrolling_back(r,cell,lastline(r));
    r->lastindex++;
    //se è il firstofthelastlist, bisogna aggiustare [lastline(r)]
    //codice
    Check(r);
}
void cancella(route * r, pint km){
    int cell = cellofelement(r,km);
    if(cell == -1)
        return NULL;
    stazione * curr = r->AUTOSTRADA[cell];
    pint i = 0;
    pint nodes = autolen(r);
    while (i < nodes && curr ->next != NULL)
    {
        if(curr->kms == km){
            cancellazione(r,curr,cell);
            return;
        }
        else if(curr ->kms > km){
            printf("\n Non c'è una stazione al km %d.\n", km);
            return NULL;
        }
        else
        {
            curr = curr->next;
            i++;    
        }
    }
    printf("\n Non c'è una stazione al km %d.\n", km); //in caso in cui km sia più grande di max->kms.
    return NULL;
}
/// @brief Inserisce elemento nella struttura dati.
/// @param r Autostrada alla quale inserire la stazione.
/// @param km Chilometri della nuova stazione.
/// @param ref Riferimento al futuro predecessore della nuova stazione.
void inserimento(route * r, pint km, stazione * ref){

}
/// @brief Inserisce elemento nella struttura dati.
/// @param r Autostrada alla quale inserire la stazione.
/// @param km Chilometri della nuova stazione.
/// @param ref Riferimento al futuro successore della nuova stazione.
void inserimento_testa(route * r, pint km, stazione * ref){

}
void inserisci(route * r, int km){
    int cell = cellofelement(r,km);
    if(cell == -1)
        return NULL;
    stazione * curr = r->AUTOSTRADA[cell];
    pint i = 0;
    pint nodes = autolen(r);
    while (i < nodes && curr ->next != NULL)
    {
        if(curr->kms == km)
        {
            printf("\n Già presente una stazione al km %d.\n", km);
            return NULL;
        }
        else if(curr ->kms > km){
            if(i > 0)
            {
                inserimento(r,km,curr->prev);
                return;
            }
            else
            {
                inserimento_testa(r, km, r->AUTOSTRADA[cell]);
                return;
            }
        }
        else
        {
            curr = curr->next;
            i++;    
        }
    }
    if(i < nodes) // allora curr->next == NULL
        inserimento(r,km,curr);
}
#pragma endregion

#pragma endregion stazioni
