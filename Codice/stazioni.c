#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAXVETTURE 512 
typedef unsigned int pint; //type defined to save space (int) numbers
int EDGECASE = 0;
int begin = 1;//global variable defined to avoid \n at the beginning and at the end of the stdout file.
//#pragma region vetture
///root of vehicles' data structure
typedef struct head
{
    int * first; //pointer to the first vehicle of the array of vehicles
    int lastindexv; //last index of the used part of the array: useful to reduce binary search input length.
    int len; //len of first, taking in count void space.
    int countzero;//number of vehicle which have just 0 Kms of autonomy. It helps to reduce binary search input length.
} head_vetture;
int numberofelement(head_vetture * h){
    return h->lastindexv + 1 + h->countzero;
}
head_vetture * Initialize(){
    head_vetture * h = (head_vetture *) malloc(sizeof(head_vetture)); //creation and allocation.
    h->len = 16; //default minimum length.
    h->lastindexv = -1; //no elements in. When a creation is performed, last index becomes 0.
    h->first = (int *)malloc(sizeof(int)*16);//vehicles allocation.
    h->countzero = 0;//no elements -> no elements with 0 kms.
    return h;
}
int vehicle_search_index_selection(head_vetture *h, int key, int mid, int forinsertion){
    if(h->first[mid] != key){
        if(forinsertion == 1)
            return mid; //insert in mid position.
        else
            return -1; //not found.
    }
    return mid; //found with mid == stop or mid == start.
}
///recursive binary search algorithm. tries to search a vehicle with kms key from array[start] to array[stop].
int search_index(head_vetture*h, int key,int start, int stop, int forinsertion){
    int mid = 0;
    //two base cases to avoid that bisection and sum of mid index doesn't give 0 or len - 1.
    if(h->first[start] == key)
        return start;
    if(h->first[stop] == key)
        return stop;
    if(start == stop) //last step of a binary search: if a search is required to avoid sequential search of the poisition in which data are inserted, manage the case.
    {
        if(forinsertion == 1) //insertion
        {
            if(h->first[start] > key) //here insertion is performed between first[start] and first[start+1].
                return start;
        }
        else if(h->first[start] == key) //searched key = first[start].
            return start;
        else
            return -1; //not found.
    }
    if(start < stop) //induction case.
    {
        mid = ceil((start+stop)/2); //here it is not possible to have 0, that is the reason why there is the first base case.
        if(mid == start)
            return vehicle_search_index_selection(h,key,start,forinsertion);
        if(mid == stop)
            return vehicle_search_index_selection(h,key,stop,forinsertion);
        if(h->first[mid] == key)
            return mid; //found    
        if(h->first[mid] < key)//nella seconda metà.
            return search_index(h,key,mid,stop,forinsertion);
        if(h->first[mid] > key)//nella prima metà.
            return search_index(h,key,start,mid,forinsertion); 
    }
    if(forinsertion == 1) //case start greater than stop.
        return (int)((start+stop)/2); //indice più vicino.
    return -1;//not found.
}
int cerca_vetture(head_vetture*h, int key){ ///search method of vehicles. key is the searched element.
    if(key == 0)
        return (int)(h->countzero > 0) - 1; //if are there 0 kms vehicles and key == 0, index is 0.
    if(h->lastindexv < 0) //no vehicles.
        return -1;
    return search_index(h,key,0,h->lastindexv,0); //binary search.
}
int max_vetture(head_vetture*h){///max element of a vehicles' array
    if(h->lastindexv < 0) //no vehicles.
        return 0; //conceptually, it should be -1 if h->countzero == 0 and 0 otherwise, but for calculations 0 is always necessary.
    return h->first[h->lastindexv];//last valued vehicle position.
}
int min_vetture(head_vetture*h){///min element of a vehicles' array
    if(h->countzero > 0) //there are 0 kms vehicles.
        return 0;
    if(h->lastindexv >= 0) //there are no 0 kms vehicles.
        return h->first[0];
    return -1; //no vehicles.
}
void Check_vetture(head_vetture*h){///checks if the vehicles' array is almost full and reallocs space (double of previous, if it is the case).
    if(h ->len - h->lastindexv < 2) //realloc if the array is almost full.
    {
        h->first = (int*)realloc(h->first, sizeof(int)*h->len*2);
        h->len = h->len*2;
    }
}
void Swap(int *a, int *b){ ///basic swapping values function.
    int c = *a;
    *a = *b;
    *b = c;
}
void slidingfor(head_vetture*h, int key, int firstvalue){ ///insertion of a vehicle in the position searched by the binary search.
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
    {    //positioning in the right position of the array to inserti the element.
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
int cancella_vetture(head_vetture*h, int key){ //remove method
    if(key == 0){
        if(h->countzero > 0)
        {
            h->countzero--;
            return 1;
        }
        else //printf("\n Non c'è una vettura con autonomia: %d", key);
            return 0;
    }
    int i = cerca_vetture(h,key); //if there isn't the vehicle, you don't remove it.
    if(i == -1) //printf("\n Non c'è una vettura con autonomia: %d", key);
        return 0;
    if(key == 0 && i == 0)
    {
        h->lastindexv--;
        return 1;
    }
    while (i < h->lastindexv){
        h->first[i] = h->first[i+1];
        i++;
    }
    h->first[h->lastindexv] = 0; //"initialization"
    h->lastindexv--;
    return 1;
}
//#pragma endregion vetture

//#pragma region stazioni
typedef struct station 
{
    pint kms;//each station has a distance from an ideal point (the user position). they are sorted on this distance.
    struct station * next; //next and previous station.
    struct station * prev;
    head_vetture * vetture;//vehicles available in a station.
}stazione;
typedef struct 
{
    stazione ** AUTOSTRADA; //array of stations: Autostrada means Highway.It does not contain all the stations, see following code.
    pint lastindex; //as for vehicles, takes note to the last index. Note: here it is not possible to have two stations with same km.
    pint len;//len of the array: it is not the number of stations, but it is about m = n/log2(n), with n as number of station.
}route;
pint Log2( pint x ) //it would be possible to replace this function with math.h function int(Natural Log(x)/Natural Log(2)), but it didn't work when the code was developed.
{
  pint ans = 0;
  while(x > 1){
    x = (pint)(x/2);
    ans++;
  } 
  return ans;
}
//#pragma region ParametriStazioni
//Array length is m, log2(m) + 1 bound nodes (double pointer list) per cell (the sublists are called lines from now).
//n = mlog2(m) + m => n = m*ln(2m) => e^n = (2m)^m.
pint autolen(route * r){
    return Log2(r->len) + 1; //number of nodes per cell list (line).
}
pint lastline(route *r){
    return (pint)(r->lastindex/autolen(r)); //last line in which there are real values.
}
pint cellbyindex(route*r, int index){ //number of cell -> index / (line length)
    return (pint)(index/autolen(r));
}
pint indexbycell(route*r, pint cell){ //inverse formula of the previous one.
    return cell*autolen(r);
}
stazione * max(route *r){
    return r->AUTOSTRADA[0]->prev; //here there is circular binding: min connected with max, but not viceversa.
}
stazione * min(route * r){
    return r->AUTOSTRADA[0]; //first element in sorted list.
}

//#pragma endregion
//#pragma region MetodiGestioneStrutturaDati
route * InitializeAUTOSTRADA(pint km){ //setup of the structure.
    route * r = (route *) malloc(sizeof(route));
    r->len = 4;
    r->AUTOSTRADA = (stazione **) malloc(sizeof(stazione*)*4);
    r->AUTOSTRADA[0] = (stazione *)malloc(sizeof(stazione));
    if(r->AUTOSTRADA[0] != NULL){
    r-> AUTOSTRADA[0] ->kms = km;
    r->AUTOSTRADA[0]->next = NULL;//the next of the max is null to easily understand when the structure is finished.
    r->AUTOSTRADA[0]->prev = r->AUTOSTRADA[0];
    r->AUTOSTRADA[0]->vetture = Initialize();
    }
    //r->AUTOSTRADA[0] -> vetture = autos;
    r->lastindex = 0;
    return r;
}

stazione * initializestazione(pint km, head_vetture* h){ //station setup.
    stazione * s = (stazione *) malloc(sizeof(stazione));
    s->kms = km;
    s->vetture = h;
    return s; 
}
stazione * EXNOVOstation(pint km){ //station setup without vehicles.
    head_vetture * h = Initialize();
    return initializestazione(km,h);
}
//#pragma endregion
//#pragma region MetodiScrollingRef
///here the MOST IMPORTANT PART OF THE MANAGEMENT CODE. FOR EACH INSERTION AND DELETION PERFORMED, IT IS REQUIRED TO RESET THE INDEX OF THE AFFECTED PART TO
///MANTAIN THE LENGTH OF LOG2(m) + 1 FOR EACH LINE. 
void scrolling_forward(route * r, pint start, pint stop){  ///after insertion
    pint i = start+1;//avoid i = 0
    while (i <= stop)
    {
        r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->prev; //in this way, the last line will have length increasing (+1) the others will have the same as usual.
        //the last element of r->AUTOSTRADA[i-1] is now the first of r-> AUTOSTRADA[i]. 
        i++;
    } 
    //lastline is the last used line. if the maximum goes to a new line, update all.
    if(cellbyindex(r, r->lastindex+1) > lastline(r)) // [lastline(r)+1] is still NULL, so fix.
        r->AUTOSTRADA[lastline(r)+1] = max(r); //max
}
///after delete.
void scrolling_back(route * r, pint start, pint stop){
    pint i = start+1;
    while (i <= stop)
    {
        r->AUTOSTRADA[i] = r->AUTOSTRADA[i]->next;//all the elements go back, they have always the space to do that.
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
    //A line is full
    if(r->lastindex % autolen(r) == 0)
        r->AUTOSTRADA[lastline(r)] = max(r); //now it occupies the new line.
    //initialize the new line.
    if(indexbycell(r, r->len) - r-> lastindex < 2){
        r->AUTOSTRADA = (stazione **)realloc(r->AUTOSTRADA,sizeof(stazione*)* (r->len * 2));
        pint oldlast = r->len;
        r->len *= 2;
        //scrolling for check.
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
/* NOT USED, but available.
void Stampasequenziale(route * r){
    stazione * curr = r->AUTOSTRADA[0];
    while (curr->next != NULL)
    {
        printf("\n %d \n", curr->kms);
        curr = curr->next;
    }
    printf("\n %d \n", curr->kms);
}*/

//#pragma endregion
//#pragma region Ricercacella
int binarysearch(route * r, pint km, pint start, pint stop){
    pint mid;
    if(km > r->AUTOSTRADA[stop]->kms) //last line
        return stop;
    if(km < r->AUTOSTRADA[start]->kms) //before the fist line.
        return start;
    if(start < stop){
        mid = (pint)(ceil((start+stop)/2));
        if(km == r->AUTOSTRADA[mid]->kms) //optimal case, found.
            return mid;
        if(km > r->AUTOSTRADA[mid] ->kms)//it is in the second half.
        {
            if(km > r->AUTOSTRADA[mid+1]->kms) //[mid] is very far from the element.
                return binarysearch(r,km,mid,stop);
            else if(km == r->AUTOSTRADA[mid+1]->kms)
                return mid+1; 
            else //between [mid] e [mid+1]
                return mid;
        }
        else if(km < r->AUTOSTRADA[mid]->kms) //it is in the first half.
        {   if(km < r->AUTOSTRADA[mid-1]->kms)//[mid] is very far from the element.
                return binarysearch(r,km,start,mid);
            else
                return mid-1;
        }
    } 
    return -1;
}
int seqsearch(route * r, pint km){ ///when the data structure is small (length < 9) it is more powerful sequential search.
    int i = 0;
    int cells = lastline(r);
    while ( i <= cells && r->AUTOSTRADA[i]->kms < km)
        i++;
    if(i <= cells){
        if(r->AUTOSTRADA[i]->kms == km) ///if it is in the lastline. 
            return i;
        else if(i > 0)
            return i-1; //last index of a less-valued element.
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
    if(EDGECASE == 1 && r->AUTOSTRADA[0]->kms == km && km == 0)//0 is not really present in this case.
        return NULL;
    int cell = cellofelement(r,km); //search of the cell.
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
void cancella_testa(route * r, pint cell){ ///here if the head is to be removed.
    stazione * old = r->AUTOSTRADA[cell];
    r->AUTOSTRADA[cell] = r->AUTOSTRADA[cell]->next;
    if(r->AUTOSTRADA[cell] != NULL)
        r->AUTOSTRADA[cell] ->prev = old ->prev;
    else
        r->AUTOSTRADA[0]->prev = old ->prev; //last line now is void.
    if(cell != 0)
        old->prev->next = r->AUTOSTRADA[cell];//avoid circularity.
    free(old);
}
void cancella_coda(route * r, stazione * ref){ ///tail removing method.
    r->AUTOSTRADA[0]->prev = ref ->prev;
    ref->prev->next = NULL;
    free(ref);
}
/// @brief Cancels the station passed as parameter from r->AUTOSTRADA
/// @param r route in which there is the element to be canceled
/// @param elemento station to be canceled
int cancellazione(route * r, stazione * elemento, pint cell, pint posizione){
    //Cancellation code
    if(elemento ->next == NULL)
        cancella_coda(r,elemento); //cancel max.
    else if(posizione > 1){ //in this case, is like a double pointer list cancellation.
        stazione * prima = elemento -> prev;
        stazione * dopo = elemento -> next;
        prima ->next = dopo;
        dopo ->prev = prima;
        free(elemento);
    }
    else if(posizione == 1){ //if the position in the line is 1.
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
void Azzera(route * r){ ///reset of the route.
    r->AUTOSTRADA[0]->kms = 0;
    head_vetture * h = r->AUTOSTRADA[0]->vetture;
    free(h);
    r->AUTOSTRADA[0]->vetture = Initialize();
    EDGECASE = 1;
}
int cancella_stazione(route * r, pint km){///looks for a station and cancel just if it is present.
    if(r->lastindex == 0 && r->AUTOSTRADA[0]->kms == km) //not convenient to cancel the structure.
    {
        Azzera(r);
        return 1;
    }
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
/// @brief Insert element in the data structure and returns the pointer of the inserted station.
/// @param r Route in which insert the station.
/// @param km Kms of the new station.
/// @param ref pointer to the next predecessor of the inserted station.
/// @param cell insertion line.
stazione * inserimento(route * r, pint km, stazione * ref, pint cell){
    stazione * new = EXNOVOstation(km);
    inserisci_tra_due(new, ref, ref->next); 
    //fase di scrolling-checking
    scrolling_forward(r,cell,lastline(r));
    r->lastindex++;
    Check(r);
    return new;
}
/// @brief Insert element in the data structure given the position.
/// @param r Route in which insert the station.
/// @param km Kms of the new station.
/// @param cell insertion line. Now r->AUTOSTRADA[cell] becomes the new predecessor.
stazione * inserimento_testa(route * r, pint km, pint cell){
    stazione * ref = r->AUTOSTRADA[cell];
    stazione * new = EXNOVOstation(km);
    r->AUTOSTRADA[cell] = new;
    r->AUTOSTRADA[cell] -> next = ref;
    if(ref != NULL)
    {
        if(ref -> prev != NULL)
            if(cell != 0) //avoid circularity with max double bound with min.
                ref -> prev -> next = r->AUTOSTRADA[cell];
        r->AUTOSTRADA[cell] -> prev = ref -> prev;
        ref->prev = r->AUTOSTRADA[cell];
    }
    scrolling_forward(r,cell,lastline(r));
    r->lastindex++;
    Check(r);
    return new;
}
///Insertion of the maximum element.
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
stazione * inserisci_stazione(route * r, int km){ ///station insertion
    if(r->lastindex == 0 && r->AUTOSTRADA[0]->kms == km && km == 0)//If the route is reset, already done. 
    {
        if(numberofelement(r->AUTOSTRADA[0]->vetture) == 0 && EDGECASE == 1)//aggiungi-stazione 0 n
        {   
            EDGECASE = 0;//EDGE CASE is managed. 
            return r->AUTOSTRADA[0]; //in this case, it was already there.
        }
        else
            return NULL; //0 inserted again...
    }
    if(km < r->AUTOSTRADA[0]->kms) //minimum inserted
        return inserimento_testa(r,km,0);
    if(km > r->AUTOSTRADA[0]->prev->kms)//maximum inserted
        return inserimento_coda(r,km,r->AUTOSTRADA[0]->prev);
    //insertion in the mid.
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
    //IF curr->next == NULL, insert max => already done.
    if(i < nodes){// allora curr->next == NULL
        if(curr->kms > km)
            return inserimento(r,km,curr->prev, cell);
        else if(curr ->kms < km)
            return inserimento(r,km,curr, cell); //max inserted now.
        else
            return NULL; //already inserted element.
    }
    else if(i == nodes)//here holds the condition cell < lastline(r), otherwise all would be full.
        return inserimento_testa(r,km,cell+1);//end of the line, but new line'station is there.
    //intermediate line.
    return NULL;
}
//#pragma endregion
//#pragma endregion stazioni

//#pragma region metodicomuni
int autonomia_max_destra(stazione * curr){
    return max_vetture(curr->vetture) + curr ->kms;
}
int autonomia_max_sinistra(stazione * curr){
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
        printf("\nnon aggiunta");
        return;
    }
    pint i;
    if(macchine != NULL)
        for(i = 0; i < numeromacchine; i++)
            insert_vetture(inserita->vetture,macchine[i]);
    printf("\naggiunta");
}
void demolisci_stazione(route * r, int km){
    int feedback = cancella_stazione(r,km);
    if(feedback == 0)
        printf("\nnon demolita");
    else printf("\ndemolita");
}
void aggiungi_auto(route *r, int km, int vettura){
    stazione * s = cerca_stazione(r,km);
    if(s == NULL){
        if(begin == 1)
        {
            printf("non aggiunta");
            begin = 0;
        }
        else
        {
            printf("\nnon aggiunta");
        }
    }
    else
    {
        if(numberofelement(s->vetture) < MAXVETTURE){
            if(insert_vetture(s->vetture, vettura) == 1){
                if(begin == 1)
                {
                    begin = 0;
                    printf("aggiunta");
                }
                else
                    printf("\naggiunta");
            }
            else
            {
                if(begin == 1)
                {
                    begin = 0;
                    printf("non aggiunta");
                }
                else
                    printf("\nnon aggiunta");
            }
        }
        else
        {
            if(begin == 1)
            {
                begin = 0;
                printf("non aggiunta");
            }
            else
                printf("\nnon aggiunta");
        }
    }
}
void rottama_auto(route * r, int km, int vettura){
    stazione * s = cerca_stazione(r,km);
    if(s == NULL) {
    if(begin == 1)
        {
            begin = 0;
            printf("non rottamata");
        }
        else
            printf("\nnon rottamata");
    }
    else
    {
        int feedback = cancella_vetture(s->vetture,vettura);
        if(feedback == 1){
            if(begin == 1)
            {
                begin = 0;
                printf("rottamata");
            }
            else
                printf("\nrottamata");
        }
        else
        {
            if(begin == 1)
            {
                begin = 0;
                printf("non rottamata");
            }
            else
                printf("\nnon rottamata");
        }
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
    struct node * raggiuntoda; //nearest node which reaches this node.
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
void FreeSxNode(nodosinistro * s)
{
    nodosinistro * r = s;
    while (s->next != NULL)
    {
        s = s->next;
        free(r);
        r = s;
    }
    free(s);
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

    nodosinistro * currn = n->prev;
    
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
    FreeSxNode(cucco);
    return per;
}
void plot_path(path * p){
    if(p == NULL)
    {
        printf("\nnessun percorso");
        return;
    }
    if(p->next == NULL)
    {
        printf("%d", p->km);
        free(p);
        return;
    }
    path * dacanc = p;
    int i = 0;
    while (p->next != NULL){
        if( i == 0){
            printf("\n%d", p->km);
            i++;
        }
        else
        {
            printf(" %d", p->km);
        }
        p = p->next;
    }
    printf(" %d", p->km);
    //printf("\n");
    CancellaPath(dacanc);
}
//#pragma endregion
int main(){
    char command[20];
    route * r = NULL;
    while(!feof(stdin))
    {
        if(scanf("%s ", command))
        {
            if(strcmp(command, "aggiungi-stazione") == 0)
            {
                int kmstazione=0;
                int numeromacchine=0;
                if(scanf("%d ", &kmstazione) && scanf("%d ", &numeromacchine)){
                int i;
                int * ptr = (int *) malloc(sizeof(int)*numeromacchine);
                for(i = 0; i < numeromacchine; i++)
                    if(!scanf("%d ", &ptr[i]))
                        break;
                if(i == numeromacchine)
                {
                    if(r != NULL)
                        aggiungi_stazione(r,kmstazione,numeromacchine, ptr);
                    else
                    {
                        r = InitializeAUTOSTRADA(kmstazione);
                        for(i = 0; i < numeromacchine; i++)
                            insert_vetture(r->AUTOSTRADA[0]->vetture, ptr[i]);
                        if(begin == 1){
                            printf("aggiunta");
                            begin = 0; 
                        }
                        else    
                            printf("\naggiunta");
                    }
                }
                else
                {
                    if(begin == 1){
                        printf("non aggiunta");
                        begin = 0;
                    }
                    else
                        printf("\nnon aggiunta");
                }
                free(ptr);
                }
            }
            else if (strcmp(command,"aggiungi-auto") == 0)
            {
                int kmstazione = 0;
                int kmauto = 0;
                if(scanf("%d" , &kmstazione) && scanf("%d " , &kmauto)){
                if(r != NULL)
                    aggiungi_auto(r,kmstazione,kmauto);
                }
            }
            else if (strcmp(command,"demolisci-stazione") == 0){
                int kmstazione = 0;
                if(scanf("%d", &kmstazione)){
                    if(r != NULL)
                        demolisci_stazione(r, kmstazione);
                }
            }
            else if (strcmp(command,"rottama-auto") == 0)
            {
                if(r != NULL)
                {
                    int kmstazione = 0;
                    int kmauto = 0;
                    if(scanf("%d " , &kmstazione) && scanf("%d " , &kmauto))
                        rottama_auto(r,kmstazione,kmauto);
                }   
            }
            else if(strcmp(command, "pianifica-percorso") == 0)            
            {
                if(r != NULL){
                    int a = 0, p = 0;
                    if(scanf("%d " , &p) && scanf("%d " , &a)){
                        if(p <= a){
                            plot_path(pianifica_percorso_destra(r,p,a));
                        }
                        else if(p > a){
                            plot_path(pianifica_percorso_sinistra(r,a,p));
                        }
                    }
                }
                else
                {
                    printf("\nnessun percorso");
                }
            }
        }
    }
    printf("\n");
}