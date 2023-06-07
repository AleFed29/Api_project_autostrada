#include<stdio.h>
#include<stdlib.h>

/*
copia di peso da "rbtree_str.c" purtroppo per il progetto va tutto su un file
*/
/*
typedef struct rbhead vettura;
typedef struct rbhead autostrada;

typedef struct stazione{
    rbelement * self;
    vettura * auto;
}

typedef struct percorso{
    rbelement * tappa;
    struct percorso * next;
}path;
*/
typedef struct 
{
    int kms;
    int level;
    int index;
    //rbhead * vetture;
}stazione;
typedef struct 
{
    stazione * AUTOSTRADA;
    int len;
    int lastindex;
}route;
int IndexOf(stazione * s){
    return 8*(s->level) + s->index + 1;
}
route * InitializeAUTOSTRADA(int kms1/*,rbhead * autos*/){
    route * r = (route *) malloc(sizeof(route));
    r->len = 4;
    r-> AUTOSTRADA = (stazione *) malloc(sizeof(stazione)*4);
    r-> AUTOSTRADA[0] = {0, 0, -1};
    r-> AUTOSTRADA[1] -> kms = kms1;
    r->AUTOSTRADA[1] -> level = 0;
    r->AUTOSTRADA[1] -> index = 0;
    //r->AUTOSTRADA[1] -> vetture = autos;
    r->AUTOSTRADA[2] = NULL;
    r->AUTOSTRADA[3] = NULL;
    r->lastindex = 1;
    return r;
}
stazione * initializestazione(int km, int lev, int ind/*,rbhead * autos*/){
    stazione * s = (stazione * s) malloc(sizeof(stazione));
    s->kms = km;
    s->level = lev;
    s->index = ind;
    //s->vetture = autos;
    return s; 
}
void Check(route * r){
    if(r->len - r-> lastindex < 2)
        r->AUTOSTRADA = (stazione *) realloc(sizeof(stazione), r->len * 2);
}
void inserimento(route * r, int km/*,rbhead * autos*/){
    r->lastindex++;
    AUTOSTRADA[r->lastindex] = initializestazione(km,int((r->lastindex)/8),(r->lastindex) % 8/*,autos*/);
    Check(r);
}

void inseriscistazione(route * r, int km, /*,rbhead * autos*/){
    if(km > r->AUTOSTRADA[r->lastindex]->kms)
        inserimento(r,km);
    else
    {
        //provare a far bidirezionale? con k = r->lastindex - j
        //condizione || km > r -> AUTOSTRADA[r->lastindex - j] -> kms 
        int j = r->lastindex;
        while (km < r ->AUTOSTRADA[j]->kms /* && r->lastindex - j < j*/)
            j--;
        /*controlla questa condizione*/
        if(!(km > r ->AUTOSTRADA[j]->kms)) //se non si è verificato questo caso. 
            j = r->lastindex - j; //l'ho raggiunto dal basso. (km >r -> AUTOSTRADA[r->lastindex - j] -> kms)
        //bisogna cambiare level e index, perché km e vetture devono restare abbinati.
        //da qua in poi con qualsiasi indice il programma funziona così.

        j++;
        int tempindex = j;
        stazione * tmp = (stazione *) malloc(sizeof(stazione));
        stazione * tmp1 = (stazione *) malloc(sizeof(stazione));
        tmp1 = r->AUTOSTRADA[j];
        while (j <= r->lastindex) 
        { 
            //l'idea è: trovo indice, cambio coordinate del punto e indice del riferimento. Devo toccare solo array.
            tmp = r-> AUTOSTRADA[j+1];
            r->AUTOSTRADA[j+1] = tmp1; //tmp1 ha ref alla posizione j
            r->AUTOSTRADA[j+1]->level = (int)((j+1)/8);
            r->AUTOSTRADA[j+1]->index = (j+1)%8; 
            tmp1 = tmp;//tmp1 ha ref alla posizione j+1 che diventerà nuovo j
            j++;//qui
        }
        r->AUTOSTRADA[tempindex] = initializestazione(km,(int)(tempindex/8), (tempindex)%8/*,autos*/);
        free(tmp);
        free(tmp1);
        r->lastindex ++;
        Check(r);
    }
}
void cancellazione(route * r, int km){
    //farai
}





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

