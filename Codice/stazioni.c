#include <stdio.h>
#include <stdlib.h>
#include <math.h>
typedef unsigned int pint;
#pragma region vetture
typedef struct head
{
    int * first;
    int lastindex;
    int len;
    int countzero;
} head_vetture;
int numberofelement(head_vetture * h){
    return h->lastindex+1+h->countzero;
}
head_vetture * Initialize(){
    head_vetture * h = (head_vetture *) malloc(sizeof(head_vetture));
    int * ptr = (int *) malloc(sizeof(int)*16);
    h->len = 16;
    h->lastindex = -1;
    h->first = ptr;
    h->countzero = 0;
    return h;
}
int search_index(head_vetture*h, int key,int start, int stop, int forinsertion){
    int mid = 0;
    if(start == stop)
        if(forinsertion == 1 || h->first[start] == key)
            return start;
        else
            return -1;
    if(start < stop)
    {
        mid = ceil((start+stop)/2);
        if(mid == start && h->first[mid] != key)
            if(forinsertion == 1)
                return start;
            else
                return -1;
        if(mid == stop && h->first[mid] != key)
            if(forinsertion == 1)
                return stop;
            else
                return -1;
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
    if(h->lastindex < 0)
        return -1;
    return search_index(h,key,0,h->lastindex,0);
}
int max_vetture(head_vetture*h){
    if(h->lastindex < 0)
        if(h->countzero == 0)
            return -1;
        else
            return 0;
    return h->first[h->lastindex];
}
int min_vetture(head_vetture*h){
    if(h->countzero > 0)
        return 0;
    if(h->lastindex >= 0)
        return h->first[0];
    return -1;
}
void Check_vetture(head_vetture*h){
    if(h ->len - h->lastindex < 2)
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
    for(i=firstvalue; i<= h->lastindex; i++)
        Swap(&(h->first[i+1]), &temp);
}
void insert_vetture(head_vetture*h, int key){
    if(key == 0){
        h->countzero++;
        return;
    }
    if(h->lastindex == -1)
    {
        h->first[0] = key;
        h->lastindex++;
        return;
    }
    if(key > max_vetture(h))
        h->first[h->lastindex+1] = key;
    else 
    {    
        int i = search_index(h,key,0,h->lastindex,1);
        while (h->first[i]<= key && i <= h->lastindex)
            i++;
        while (h->first[i] > key && i <= h->lastindex)
            i--;
        if(h->first[i] < key && i <= h->lastindex)
            i++;
        slidingfor(h,key,i);
    }
    h->lastindex++;
    Check_vetture(h);
}
void cancella_vetture(head_vetture*h, int key){
    if(key == 0)
        if(h->countzero > 0)
        {
            h->countzero--;
            return;
        }
        else
        {
            printf("\n Non c'è una vettura con autonomia: %d", key);
        }
    int i = search_index(h,key,0,h->lastindex,0);
    if(i == -1)
    {
        printf("\n Non c'è una vettura con autonomia: %d", key);
        return;
    }
    while (i < h->lastindex){
        h->first[i] = h->first[i+1];
        i++;
    }
    h->lastindex--;
}
#pragma endregion vetture


#pragma region stazioni
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

stazione * initializestazione(pint km, head_vetture h/*,rbhead * autos*/){
    stazione * s = (stazione *) malloc(sizeof(stazione));
    s->kms = km;
    s->vetture = h;
    return s; 
}
stazione * EXNOVOstation(pint km/*,head_vetture * autos*/){
    head_vetture h = Initialize();
    return initializestazione(km/*,h*/);
}
#pragma endregion
#pragma region MetodiScrollingRef
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

void scrollingresizing(route * r, pint lastcell){
    pint nodes = autolen(r);
    pint i, j; 
    pint quanti = 0;
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
        int i;
        int j;
        pint cells = autolen(r); //devo spostare per L = log2(m) + 1 file.
        pint oldlast = r->len;
        r->len *= 2;
        //scrolling per check.
        scrollingresizing(r,oldlast);
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
void Stampasequenziale(route * r){
    stazione * curr = r->AUTOSTRADA[0];
    while (curr->next != NULL)
    {
        printf("\n %d \n", curr->kms);
        curr = curr->next;
    }
    printf("\n %d \n", curr->kms);
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
void cancellazione(route * r, stazione * elemento, pint cell, pint posizione){
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
}
void cancella_stazione(route * r, pint km){
    if(r->lastindex < 0) return;
    if(km > max(r)->kms)
    {
        printf("\n Non c'è una stazione al km %d.\n", km);
        return;
    }
    if(km < min(r)->kms)
    {
        printf("\n Non c'è una stazione al km %d.\n", km);
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
    inserisci_tra_due(new, ref, ref->next); 
    //fase di scrolling-checking
    scrolling_forward(r,cell,lastline(r));
    r->lastindex++;
    Check(r);
}
/// @brief Inserisce elemento nella struttura dati.
/// @param r Autostrada alla quale inserire la stazione.
/// @param km Chilometri della nuova stazione.
/// @param cell Cella nella quale inserire la nuova stazione. L'elemento di cella vecchio è nuovo successore.
void inserimento_testa(route * r, pint km, pint cell){
    stazione * ref = r->AUTOSTRADA[cell];
    r->AUTOSTRADA[cell] = EXNOVOstation(km);
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
}
//inserimento del massimo.
void inserimento_coda(route * r, pint km, stazione * ref){
    stazione* new = EXNOVOstation(km);
    new -> prev = ref;
    ref -> next = new;
    new -> next = NULL;
    r->AUTOSTRADA[0]->prev = new;
    r->lastindex++;
    Check(r);
}
void inserisci_stazione(route * r, int km){
    if(km < r->AUTOSTRADA[0]->kms) //inserisco minimo
    {
        inserimento_testa(r,km,0);
        return;
    }
    if(km > r->AUTOSTRADA[0]->prev->kms)//inserisco massimo
    {
        inserimento_coda(r,km,r->AUTOSTRADA[0]->prev);
        return;
    }
    //inserisco in mezzo.
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
    //SE curr->next == NULL, inserisco massimo => già fatto.
    if(i < nodes)// allora curr->next == NULL
        if(curr->kms > km)
            inserimento(r,km,curr->prev, cell);
        else
            inserimento(r,km,curr, cell); //sto inserendo massimo.
    else if(i == nodes)//qui sono certo che cell < lastline(r), altrimenti avrei tutto pieno.
        inserimento_testa(r,km,cell+1);//sono a fine riga, ma la stazione della nuova riga esiste.
    //sono in una riga intermedia.
    
}
#pragma endregion
#pragma endregion stazioni

#pragma region metodisupporto
    void vetturaperstazione(route *r, int km, int vettura){
        stazione * s = cerca_stazione(r,km);
        insert_vetture(s->vetture, vettura);
    }
#pragma endregion

#pragma region metodirichiesti
    void aggiungi_stazione(route * r, int km, int * macchine){
        inserisci_stazione(r,km);

    }
#pragma endregion

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