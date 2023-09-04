#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXVETTURE 512 
typedef unsigned int pint;
//#pragma region vetture
typedef struct head
{
    int * first;
    int lastindexv;
    int len;
    int countzero;
} head_vetture;
int numberofelement(head_vetture * h){
    return h->lastindexv + 1 + h->countzero;
}
head_vetture * Initialize(){
    head_vetture * h = (head_vetture *) malloc(sizeof(head_vetture));
    //int * ptr = (int *) malloc(sizeof(int)*16);
    //h->first = ptr;
    h->len = 16;
    h->lastindexv = -1;
    h->first = (int *)malloc(sizeof(int)*16);
    h->countzero = 0;
    return h;
}
int search_index(head_vetture*h, int key,int start, int stop, int forinsertion){
    int mid = 0;
    if(start == stop)
    {
        if(forinsertion == 1 || h->first[start] == key)
            return start;
        else
            return -1;
    }
    if(start < stop)
    {
        mid = ceil((start+stop)/2);
        if(mid == start && h->first[mid] != key){
            if(forinsertion == 1)
                return start;
            else
                return -1;
        }
        if(mid == stop && h->first[mid] != key){
            if(forinsertion == 1)
                return stop;
            else
                return -1;
        }
        if(h->first[mid] == key)
            return mid; //trovato    
        if(h->first[mid] < key)//nella seconda metà.
            return search_index(h,key,mid,stop,forinsertion);
        if(h->first[mid] > key)//nella prima metà.
            return search_index(h,key,start,mid,forinsertion); 
    }
    if(forinsertion == 1)
        return (int)((start+stop)/2); //indice più vicino.
    return -1;
}
int cerca_vetture(head_vetture*h, int key){
    if(key == 0)
        return (int)(h->countzero > 0) - 1;
    if(h->lastindexv < 0)
        return -1;
    return search_index(h,key,0,h->lastindexv,0);
}
int max_vetture(head_vetture*h){
    if(h->lastindexv < 0)
    {
        if(h->countzero == 0)
            return 0;//-1, non trovato, non va bene
        else
            return 0;
    }
    return h->first[h->lastindexv];
}
int min_vetture(head_vetture*h){
    if(h->countzero > 0)
        return 0;
    if(h->lastindexv >= 0)
        return h->first[0];
    return -1;
}
void Check_vetture(head_vetture*h){
    if(h ->len - h->lastindexv < 2)
    {
        h->first = (int*)realloc(h->first, sizeof(int)*h->len*2);
        h->len = h->len*2;
    }
}
void Swap(int *a, int *b){
    int c = *a;
    *a = *b;
    *b = c;
}
void slidingfor(head_vetture*h, int key, int firstvalue){
    int i;
    int temp = h->first[firstvalue];
    h->first[firstvalue] = key;
    for(i=firstvalue; i<= h->lastindexv; i++)
        Swap(&(h->first[i+1]), &temp);
}
int insert_vetture(head_vetture*h, int key){
    if(key == 0){
        h->countzero++;
        return 1;
    }
    if(h->lastindexv == -1)
    {
        h->first[0] = key;
        h->lastindexv++;
        return 1;
    }
    if(key > max_vetture(h))
        h->first[h->lastindexv+1] = key;
    else 
    {    
        int i = search_index(h,key,0,h->lastindexv,1);
        while (h->first[i]<= key && i < h->lastindexv)
            i++;
        while (h->first[i] > key && i <= h->lastindexv && i > 0)
            i--;
        if(h->first[i] < key && i < h->lastindexv)
            i++;
        slidingfor(h,key,i);
    }
    h->lastindexv++;
    Check_vetture(h);
    return 1;
}
int cancella_vetture(head_vetture*h, int key){
    if(key == 0){
        if(h->countzero > 0)
        {
            h->countzero--;
            return 1;
        }
        else //printf("\n Non c'è una vettura con autonomia: %d", key);
            return 0;
    }
    int i = search_index(h,key,0,h->lastindexv,0);
    if(i == -1) //printf("\n Non c'è una vettura con autonomia: %d", key);
        return 0;
    while (i < h->lastindexv){
        h->first[i] = h->first[i+1];
        i++;
    }
    h->lastindexv--;
    return 1;
}
//#pragma endregion vetture

//#pragma region stazioni
typedef struct station 
{
    pint kms;
    struct station * next;
    struct station * prev;
    head_vetture * vetture;
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
//#pragma region ParametriStazioni
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

//#pragma endregion
//#pragma region MetodiGestioneStrutturaDati
route * InitializeAUTOSTRADA(pint km/*, rbhead * autos*/){
    route * r = (route *) malloc(sizeof(route));
    r->len = 4;
    r->AUTOSTRADA = (stazione **) malloc(sizeof(stazione*)*4);
    r->AUTOSTRADA[0] = (stazione *)malloc(sizeof(stazione));
    if(r->AUTOSTRADA[0] != NULL){
    r-> AUTOSTRADA[0] ->kms = km;
    r->AUTOSTRADA[0]->next = NULL;
    r->AUTOSTRADA[0]->prev = r->AUTOSTRADA[0];
    r->AUTOSTRADA[0]->vetture = Initialize();
    }
    //r->AUTOSTRADA[0] -> vetture = autos;
    r->lastindex = 0;
    return r;
}

stazione * initializestazione(pint km, head_vetture* h/*,rbhead * autos*/){
    stazione * s = (stazione *) malloc(sizeof(stazione));
    s->kms = km;
    s->vetture = h;
    return s; 
}
stazione * EXNOVOstation(pint km/*,head_vetture * autos*/){
    head_vetture * h = Initialize();
    return initializestazione(km,h);
}
//#pragma endregion
//#pragma region MetodiScrollingRef
//stessa cosa che avviene in "prova per puntatori"
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
        r->AUTOSTRADA[lastline(r)+1] = max(r); //max
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

void scrollingresizing(route * r, pint lastcell){
    pint i, j; 
    for(i = 0; i < lastcell; i++)
        for(j = 0; j < i && r->AUTOSTRADA[i]->next != NULL; j++)
            r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;
}

void Check(route * r){    
    //ho esaurito vecchia linea
    if(r->lastindex % autolen(r) == 0)
        r->AUTOSTRADA[lastline(r)] = max(r); //sto occupando nuova riga, sicuramente era NULL
    //inizializzo nuova linea.
    if(indexbycell(r, r->len) - r-> lastindex < 2){
        r->AUTOSTRADA = (stazione **)realloc(r->AUTOSTRADA,sizeof(stazione*)* (r->len * 2));
        pint oldlast = r->len;
        r->len *= 2;
        //scrolling per check.
        scrollingresizing(r,oldlast);
    }
}
//#pragma endregion
//#pragma region Stampa
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
void Stampasequenziale(route * r){
    stazione * curr = r->AUTOSTRADA[0];
    while (curr->next != NULL)
    {
        printf("\n %d \n", curr->kms);
        curr = curr->next;
    }
    printf("\n %d \n", curr->kms);
}

//#pragma endregion
//#pragma region Ricercacella
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
        {
            if(km > r->AUTOSTRADA[mid+1]->kms) //[mid] è molto lontano da elemento.
                return binarysearch(r,km,mid,stop);
            else if(km == r->AUTOSTRADA[mid+1]->kms)
                return mid+1; 
            else //è tra [mid] e [mid+1]
                return mid;
        }
        else if(km < r->AUTOSTRADA[mid]->kms) //prima parte.
        {   if(km < r->AUTOSTRADA[mid-1]->kms)//[mid] è molto lontano da elemento.
                return binarysearch(r,km,start,mid);
            else
                return mid-1;
        }
    } 
    return -1;
}
int seqsearch(route * r, pint km){
    int i = 0;
    int cells = lastline(r);
    while ( i <= cells && r->AUTOSTRADA[i]->kms < km)
        i++;
    if(i <= cells){
        if(r->AUTOSTRADA[i]->kms == km) //potrebbe fregarmi i più alto di cells.
            return i;
        else if(i > 0)
            return i-1; //cerco nell'ultimo indice che mi dà minore.
        else
            return 0;
    }
    else
        return cells;
}
int cellofelement(route * r, pint km){
    if(r->lastindex < 9)
        return seqsearch(r,km);
    else return binarysearch(r,km,0,lastline(r));
}
//#pragma endregion
//#pragma region RicercaInserimentoCancellazione
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
void cancella_testa(route * r, pint cell){
    stazione * old = r->AUTOSTRADA[cell];
    r->AUTOSTRADA[cell] = r->AUTOSTRADA[cell]->next;
    if(r->AUTOSTRADA[cell] != NULL)
        r->AUTOSTRADA[cell] ->prev = old ->prev;
    else
        r->AUTOSTRADA[0]->prev = old ->prev; //ho svuotato ultima riga.
    old->prev->next = r->AUTOSTRADA[cell];
    free(old);
}
void cancella_coda(route * r, stazione * ref){
    r->AUTOSTRADA[0]->prev = ref ->prev;
    ref->prev->next = NULL;
    free(ref);
}
/// @brief Cancella l'elemento stazione passato come parametro da r->AUTOSTRADA
/// @param r Autostrada di cancellazione
/// @param elemento stazione da cancellare
int cancellazione(route * r, stazione * elemento, pint cell, pint posizione){
    //codice di cancellazione elemento
    //codice
    if(elemento ->next == NULL)
        cancella_coda(r,elemento); //cancello massimo.
    else if(posizione > 1){
        stazione * prima = elemento -> prev;
        stazione * dopo = elemento -> next;
        prima ->next = dopo;
        dopo ->prev = prima;
        free(elemento);
    }
    else if(posizione == 1){
        r->AUTOSTRADA[cell] -> next = elemento -> next;
        elemento->next->prev = r->AUTOSTRADA[cell];
        free(elemento);
    }
    else
        cancella_testa(r,cell);
    //scrolling
    scrolling_back(r,cell,lastline(r));
    r->lastindex--;
    return 1;
}
int cancella_stazione(route * r, pint km){
    if(r->lastindex < 0) 
        return 0;
    if(km > max(r)->kms) //printf("\n Non c'è una stazione al km %d.\n", km);
        return 0;
    if(km < min(r)->kms) //printf("\n Non c'è una stazione al km %d.\n", km);
        return 0;
    int cell = cellofelement(r,km);
    if(cell == -1)
        return 0;
    stazione * curr = r->AUTOSTRADA[cell];
    pint i = 0;
    pint nodes = autolen(r);
    while (i < nodes && curr ->next != NULL)
    {
        if(curr->kms == km)
            return cancellazione(r,curr,cell, i);
        else if(curr ->kms > km) //printf("\n Non c'è una stazione al km %d.\n", km);   
            return 0;
        else
        {
            curr = curr->next;
            i++;    
        }
    }
    if(curr ->kms != km) //printf("\n Non c'è una stazione al km %d.\n", km);
        return 0; //in caso in cui km sia più grande di max->kms.
    else
        return cancellazione(r,curr,cell, i);
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
/// @brief Inserisce elemento nella struttura dati. Ritorna il riferimento alla stazione inserita.
/// @param r Autostrada alla quale inserire la stazione.
/// @param km Chilometri della nuova stazione.
/// @param ref Riferimento al futuro predecessore della nuova stazione.
/// @param cell Linea di inserimento.
stazione * inserimento(route * r, pint km, stazione * ref, pint cell){
    stazione * new = EXNOVOstation(km);
    inserisci_tra_due(new, ref, ref->next); 
    //fase di scrolling-checking
    scrolling_forward(r,cell,lastline(r));
    r->lastindex++;
    Check(r);
    return new;
}
/// @brief Inserisce elemento nella struttura dati.
/// @param r Autostrada alla quale inserire la stazione.
/// @param km Chilometri della nuova stazione.
/// @param cell Cella nella quale inserire la nuova stazione. L'elemento di cella vecchio è nuovo successore.
stazione * inserimento_testa(route * r, pint km, pint cell){
    stazione * ref = r->AUTOSTRADA[cell];
    stazione * new = EXNOVOstation(km);
    r->AUTOSTRADA[cell] = new;
    r->AUTOSTRADA[cell] -> next = ref;
    if(ref != NULL)
    {
        if(ref -> prev != NULL)
            if(cell != 0) //altrimenti la rendo circolare, con max collegato doppiamente a min.
                ref -> prev -> next = r->AUTOSTRADA[cell];
        r->AUTOSTRADA[cell] -> prev = ref -> prev;
        ref->prev = r->AUTOSTRADA[cell];
    }
    scrolling_forward(r,cell,lastline(r));
    r->lastindex++;
    Check(r);
    return new;
}
//inserimento del massimo.
stazione * inserimento_coda(route * r, pint km, stazione * ref){
    stazione * new = EXNOVOstation(km);
    new -> prev = ref;
    ref -> next = new;
    new -> next = NULL;
    r->AUTOSTRADA[0]->prev = new;
    r->lastindex++;
    Check(r);
    return new;
}
stazione * inserisci_stazione(route * r, int km){
    if(km < r->AUTOSTRADA[0]->kms) //inserisco minimo
        return inserimento_testa(r,km,0);
    if(km > r->AUTOSTRADA[0]->prev->kms)//inserisco massimo
        return inserimento_coda(r,km,r->AUTOSTRADA[0]->prev);
    //inserisco in mezzo.
    int cell = cellofelement(r,km);
    if(cell == -1)
        return NULL;
    stazione * curr = r->AUTOSTRADA[cell];
    pint i = 0;
    pint nodes = autolen(r);
    while (i < nodes && curr ->next != NULL)
    {
        if(curr->kms == km){  //printf("\n Già presente una stazione al km %d.\n", km);   
            return NULL;
        }
        else if(curr ->kms > km){
            if(i > 0)
                return inserimento(r,km,curr->prev, cell);        
        }
        else
        {
            curr = curr->next;
            i++;    
        }
    }
    //SE curr->next == NULL, inserisco massimo => già fatto.
    if(i < nodes){// allora curr->next == NULL
        if(curr->kms > km)
            return inserimento(r,km,curr->prev, cell);
        else
            return inserimento(r,km,curr, cell); //sto inserendo massimo.
    }
    else if(i == nodes)//qui sono certo che cell < lastline(r), altrimenti avrei tutto pieno.
        return inserimento_testa(r,km,cell+1);//sono a fine riga, ma la stazione della nuova riga esiste.
    //sono in una riga intermedia.
    return NULL;
}
//#pragma endregion
//#pragma endregion stazioni

//#pragma region metodicomuni
int autonomia_max_destra(stazione * curr){
    //if(curr ->vetture == NULL) return curr->kms;
    return max_vetture(curr->vetture) + curr ->kms;
}
int autonomia_max_sinistra(stazione * curr){
    //if(curr ->vetture == NULL) return curr->kms;
    int soglia = curr ->kms - max_vetture(curr->vetture);
    if(soglia > 0)
        return soglia;
    return 0;
} 
//#pragma endregion

//#pragma region metodirichiesti
void aggiungi_stazione(route * r, int km,int numeromacchine, int * macchine){
    stazione * inserita = inserisci_stazione(r,km);
    if(inserita == NULL){
        printf("\n non aggiunta \n");
        return;
    }
    pint i;
    if(macchine != NULL)
        for(i = 0; i < numeromacchine; i++)
            insert_vetture(inserita->vetture,macchine[i]);
    printf("\n aggiunta \n");
}
void demolisci_stazione(route * r, int km){
    int feedback = cancella_stazione(r,km);
    if(feedback == 0)
        printf("\n non demolita \n");
    else printf("\n demolita \n");
}
void aggiungi_auto(route *r, int km, int vettura){
    stazione * s = cerca_stazione(r,km);
    if(s == NULL) printf("\n non aggiunta \n");
    else
    {
    if(numberofelement(s->vetture) < MAXVETTURE){
        if(insert_vetture(s->vetture, vettura) == 1)
            printf("\n aggiunta \n");
        else
            printf("\n non aggiunta \n");
    }
    else
        printf("\n non aggiunta \n");
    }
}
void rottama_auto(route * r, int km, int vettura){
    stazione * s = cerca_stazione(r,km);
    if(s == NULL) printf("\n non rottamata \n");
    else
    {
    int feedback = cancella_vetture(s->vetture,vettura);
    if(feedback == 1)
        printf("\n rottamata \n");
    else
        printf("\n non rottamata \n");
    }
}
/*
pianifica percorso.

Parto da part.
Caso base.
Altrimenti.
Finché da partenza arrivo a una stazione, la aggiungo nei children.
Registro ultima stazione raggiungibile.
*/
typedef struct percorso
{
    pint km;
    struct percorso * next;
}path;

typedef struct node
{
    stazione * this;
    struct node * raggiuntoda;
    struct node * next;
    struct node * prev;
}nodoarianna;
typedef struct nodoar{
    stazione * this;
    struct nodoar * raggiuntoda;
    pint kmrelativo;
    struct nodoar * next;
    struct nodoar * prev;
}nodosinistro;

path * InitializePath(stazione * s){
    path * per = (path*) malloc(sizeof(path));
    per->km = s ->kms;
    per ->next = NULL;
    return per;
}
nodoarianna * NewNode(stazione * s){
    nodoarianna * new = (nodoarianna *) malloc(sizeof(nodoarianna));
    new ->this = s;
    new ->raggiuntoda = NULL;
    new ->next = NULL;
    return new;
}
nodoarianna * AnnodainCoda(nodoarianna * n, stazione * elemento, nodoarianna * raggiunto){
    nodoarianna * elem = NewNode(elemento);
    elem ->raggiuntoda = raggiunto;
    n ->next = elem;
    elem ->prev = n;
    return elem;
}
path * AggiungiinTestaPath(path * p, stazione * s){
    path * elemento = InitializePath(s);
    elemento ->next = p;
    return elemento;
}
void AggiungiinCodaPath(path * p, stazione * s){
    path * curr = p;
    path * elemento = InitializePath(s);
    while (curr ->next != NULL)
        curr = curr ->next;
    curr ->next = elemento;
}
void CancellaPath(path* p){
    path * curr = p;
    path * dacanc = p;
    while (curr ->next != NULL)
    {
        curr = curr->next;
        free(dacanc);
        dacanc = curr;
    }
    free(curr);
}
void CancellaNodes(nodoarianna * n){
    nodoarianna * m = n;
    nodoarianna * dacanc = n;
    while (m ->next != NULL)
    {
        m = m->next;
        free(dacanc);
        dacanc = m;
    }
    free(m);
}
path * pianifica_percorso_destra(route * r, pint partenza, pint arrivo){
    stazione * part = cerca_stazione(r,partenza);
    stazione * arr = cerca_stazione(r,arrivo);
    if(part == NULL || arr == NULL)
        return NULL;
    path * per = InitializePath(arr);
    if(arr == part)
        return per;
    pint auton = autonomia_max_destra(part);
    if(auton >= arr->kms) //caso base.
    {
        path * elemento = (path*) malloc(sizeof(path));
        elemento ->km = part ->kms;
        elemento ->next = per;
        return elemento;
    }
    stazione * curr = part->next;
    nodoarianna * n = NewNode(part);
    nodoarianna * lastnode = n;
    
    while (curr ->kms < arr->kms)
    {
        if(curr->kms <= auton)
        {
            n = AnnodainCoda(n,curr, lastnode);
            curr = curr ->next;
        }
        else
        {
            if(lastnode ->next == NULL)
                return NULL;
            lastnode = lastnode ->next;
            auton = autonomia_max_destra(lastnode->this);
        }
    }
    while (lastnode ->this->kms < arr->kms)
    {
        if(arr ->kms <= auton)
        {
            n = AnnodainCoda(n,arr, lastnode);
            break;
        }
        else
        {
            if(lastnode ->next == NULL)
                return NULL;
            lastnode = lastnode ->next;
            auton = autonomia_max_destra(lastnode->this);
        }
    }
    if(n->this != arr)
        return NULL;
    n = n->raggiuntoda;//riavvolgo il filo di Arianna
    while (n->raggiuntoda != NULL)
    {
       per = AggiungiinTestaPath(per, n->this);
       n = n->raggiuntoda;
    }
    if(n ->this == part){
        per = AggiungiinTestaPath(per, n->this);
        CancellaNodes(n);
        return per;
    }
    CancellaNodes(n);
    return NULL;
}
nodosinistro* InMezzoMarco(nodosinistro * limiteinf, nodosinistro * limitesup, nodosinistro * currn, stazione * part){
    nodosinistro * questo = limitesup;
    pint opzionepartenza = 1;
    if(currn ->this != part)
        opzionepartenza = 0;
    while(questo != limiteinf){
    if(max_vetture(questo->this->vetture) + questo->kmrelativo >= currn->kmrelativo){
    //etichetto
        currn ->raggiuntoda = questo;
        if(opzionepartenza == 0)//questo serve per non bloccare il programma se cerchi quella che ariva a part. 
        {
        if(currn->this != part) //se posso andare avanti.
            currn = currn ->prev;
        else //altrimenti.
            return currn;//qui sono alla ricerca della stazione che raggiunge partenza, trovata.
        }
    }
    else //altrimenti
    {
        questo = questo -> next;//limiteinf è più avanti, es. arr rispetto alle altre staz.
    }
    }
    //fuori da ciclo
    return currn;
}
path * pianifica_percorso_sinistra(route * r, pint partenza, pint arrivo){
    stazione * part = cerca_stazione(r,partenza);
    stazione * arr = cerca_stazione(r,arrivo);
    if(part == NULL || arr == NULL)
        return NULL;
    path * per = InitializePath(arr);
    if(arr == part)
        return per;
    pint auton = autonomia_max_sinistra(arr);
    if(auton <= part ->kms)//caso base, due tappe...
    {
        path * elemento = (path*) malloc(sizeof(path));
        elemento ->km = part ->kms;
        per ->next = elemento;
        elemento ->next = NULL;
        return per;
    }
    stazione * curr = part->next;
    pint krel = arr->kms;
    nodosinistro * n = (nodosinistro *)malloc(sizeof(nodosinistro));
    n->kmrelativo = krel - part->kms;
    n->this = part;
    n->next  = NULL;
    n->prev = NULL;
    n->raggiuntoda = NULL;
    nodosinistro * cucco = n; //vecchio puntatore, punta dall'inizio...
    nodosinistro * new;
    while (curr ->kms <= arr->kms)
    {
        new = (nodosinistro *)malloc(sizeof(nodosinistro));
        new ->this = curr;
        new ->prev = n;
        n->next = new;
        new->raggiuntoda = NULL;
        new ->next = NULL;
        new ->kmrelativo = krel - curr->kms;
        n = new; 
        if(curr ->kms < arr ->kms)
            curr = curr->next;
        else
            break;
    }
    //ho salvato stazioni con nuovo percorso relativo.
    /*nodosinistro * s = n;
    printf("\n");
    while (s->prev != NULL)
    {
        printf("\t %d", s->this->kms);
        s = s->prev;
    }
    printf("\t %d", s->this->kms);
    printf("\n");*/
    //ora
    nodosinistro * currn = n->prev;
    /*if(n->prev != NULL) 
        currn = n->prev;
    else
        return NULL; //questo è perché se ho un solo elemento o è il caso base (verificato no) oppure non esiste il percorso...
    */
    nodosinistro * limit = n;
    nodosinistro * thisn = n;
    nodosinistro * mem = n;
    auton = max_vetture(thisn->this->vetture) + thisn->kmrelativo;
    while (currn-> this -> kms >= part -> kms)
    {
        if(auton >= currn -> kmrelativo) //cerco di capire se thisn raggiunge currn.
        {
            currn ->raggiuntoda = thisn;
            if(currn-> this -> kms > part -> kms){
                currn = currn ->prev;
            }
            else
            {
                break; //il ciclo, in realtà, termina qua.
            }
        }
        else
        {
            if(limit ->kmrelativo >= thisn->kmrelativo)//serve affinché thisn non "oltrepassi" limit.
            {
                if(mem == currn) return NULL; //se nessuna stazione in mezzo raggiunge currn...
                thisn = currn -> next;//ultima stazione che viene raggiunta.
                limit = mem; //prima o poi dovrò visitare i vecchi nodi che non ho visitato...
                mem = currn; //devo sapere qual è l'ultimo nodo non visitato... sarà nuovo limit...
                auton = max_vetture(thisn->this->vetture) + thisn->kmrelativo;//ogni volta che cambio thisn, cambio autonomia.
            }
            else
            {
                thisn = thisn -> next;//vado nelle stazioni "in mezzo" tra il vecchio thisn e limit.
                auton = max_vetture(thisn->this->vetture) + thisn->kmrelativo;//ogni volta che cambio thisn, cambio autonomia.
            }
        }
    }
    //la visita è al più lineare... stazioni già visitate non vengono consultate.

    /*nodosinistro * s = cucco;
    printf("\n");
    while (s->next != NULL)
    {
        printf("\t %d:%d", s->this->kms, s->raggiuntoda->this->kms);
        s = s->next;
    }
    printf("\t %d", s->this->kms);
    printf("\n");*/
    //currn = cucco;*/

    //o esiste stazione che arriva fino alla fine, oppure ho il problema che non riesco a raggiungere la fine.
    //se una stazione raggiunge la fine, avrà raggiunto tutte quelle prima, quindi poi avrò l'if, non l'else.
    //il cercare la stazione più vicina dalla lontana raggiunta mi fa sì che, a parità di tappe sono nel migliore, altrimenti ne ho uno con meno tappe. 
    //qui devo ottenere partenza etichettata, cosicché poi a ritroso ottengo le stazioni da part ad arr (le aggiungo in testa).
    if(currn != cucco) return NULL; //devo puntare alla partenza.
    while(currn ->raggiuntoda != NULL)//arrivo già dentro.
    {
        //printf("\t %d:%d", currn->this->kms, currn->raggiuntoda->this->kms);
        per = AggiungiinTestaPath(per,currn->this);//qui abbiamo che currn->raggiuntoda è il riferimento alla staizone più vicina dalla partenza che raggiunge currn. Di conseguenza, riavvolgendo il filo di Arianna...
        currn = currn ->raggiuntoda;
    }
    path * pointer = per;
    while(pointer ->next ->next != NULL)
        pointer = pointer ->next;//pointer->next è l'ultimo...
    //path * memoria = pointer ->next;
    pointer ->next = NULL;
    per = AggiungiinTestaPath(per, arr);//la stazione arrivo subisce inserimenti in testa, ma dovrebbe essere lei in testa...
    //cancellanodosinistro.
    return per;
}
void plot_path(path * p){
    printf("\n");
    if(p == NULL)
    {
        printf("\n nessun percorso \n");
        return;
    }
    if(p->next == NULL)
    {
        printf("\t %d\n", p->km);
        free(p);
        return;
    }
    path * dacanc = p;
    while (p->next != NULL){
        printf("\t %d", p->km);
        p = p->next;
    }
    printf("\t %d", p->km);
    printf("\n");
    CancellaPath(dacanc);
}
//#pragma endregion
int main(){
    //char command [400];
    char command[20];
    //char * token1;
    //char *end;
    route * r = NULL;
    while(!feof(stdin))
    {
        //if(fgets(command, 400,stdin) != NULL)
        if(scanf("%s ", command))
        {
            //command = strtok(command, " ");
            //token1 = strtok(command, " ");
            //if(strcmp(token1, "aggiungi-stazione") == 0)
            if(strcmp(command, "aggiungi-stazione") == 0)
            {
                /*token1 = strtok(NULL, " ");
                int kmstazione = (int)strtol(token1, &end, 10);
                token1 = strtok(NULL, " ");
                int numeromacchine = (int)strtol(token1, &end, 10);*/
                int kmstazione=0;
                int numeromacchine=0;
                if(scanf("%d ", &kmstazione) && scanf("%d ", &numeromacchine)){
                int i;
                int * ptr = (int *) malloc(sizeof(int)*numeromacchine);
                for(i = 0; i < numeromacchine; i++)
                {
                    //token1 = strtok(NULL, " ");
                    //if(token1 == NULL) break;//controlla perché potrebbe dar problemi...
                    //ptr[i] = (int)strtol(token1, &end, 10);
                    if(!scanf("%d ", &ptr[i]))
                        break;
                }
                if(r != NULL && i == numeromacchine){
                    aggiungi_stazione(r,kmstazione,numeromacchine, ptr);
                }
                else
                {
                    if(i == numeromacchine)
                    {
                        r = InitializeAUTOSTRADA(kmstazione);
                        i = 0;
                        while (i < numeromacchine)
                        {
                            insert_vetture(r->AUTOSTRADA[0]->vetture,ptr[i]);
                            i++;
                        }
                        printf("\n aggiunta \n");
                    }
                    else
                    {
                        printf("\n non aggiunta \n");
                    }
                }
                free(ptr); //dovrebbe essere sicura.
                }
            }
            //else if (strcmp(token1,"aggiungi-auto") == 0)
            else if (strcmp(command,"aggiungi-auto") == 0)
            {
                //continua...
                //token1 = strtok(NULL, " ");
                //int kmstazione = (int)strtol(token1, &end, 10);
                int kmstazione = 0;
                int kmauto = 0;
                if(scanf("%d" , &kmstazione) && scanf("%d " , &kmauto)){
                //token1 = strtok(NULL, " ");
                //int kmauto = (int)strtol(token1, &end, 10);
                if(r != NULL)
                    aggiungi_auto(r,kmstazione,kmauto);
                }
            }
            //else if (strcmp(token1,"demolisci-stazione") == 0){
            else if (strcmp(command,"demolisci-stazione") == 0){
                //token1 = strtok(NULL, " ");
                int kmstazione = 0;
                //int kmstazione= (int)strtol(token1, &end, 10);
                if(scanf("%d", &kmstazione)){
                if(r != NULL)
                    demolisci_stazione(r, kmstazione);
                }
            }
            //else if (strcmp(token1,"rottama-auto") == 0)
            else if (strcmp(command,"rottama-auto") == 0)
            {
                if(r != NULL)
                {
                    //token1 = strtok(NULL, " ");
                    //int kmstazione = (int)strtol(token1, &end, 10);
                    //token1 = strtok(NULL, " ");
                    //int kmauto = (int)strtol(token1, &end, 10);
                    int kmstazione = 0;
                    int kmauto = 0;
                    if(scanf("%d " , &kmstazione) && scanf("%d " , &kmauto))
                        rottama_auto(r,kmstazione,kmauto);
                }   
            }
            //else if(strcmp(token1, "pianifica-percorso") == 0)
            else if(strcmp(command, "pianifica-percorso") == 0)            
            {
                if(r != NULL){
                    int a = 0, p = 0;
                    //token1 = strtok(NULL, " ");
                    //int p = (int)strtol(token1, &end, 10);
                    //token1 = strtok(NULL, " ");
                    //int a = (int)strtol(token1, &end, 10);
                    if(scanf("%d " , &p) && scanf("%d " , &a)){
                    if(p < a){
                        plot_path(pianifica_percorso_destra(r,p,a));
                    }
                    else if(p > a){
                        plot_path(pianifica_percorso_sinistra(r,a,p));
                    }
                    else{
                        printf("\n nessun percorso \n");
                    }
                    }
                }
                else
                {
                    printf("\n nessun percorso \n");
                }
            }
        }
    }
}