#include <stdio.h> 
#include <stdlib.h>
typedef unsigned char byte;
typedef struct rbtree * rbchildren;
typedef struct rbtree
{
    int value;
    struct rbtree * father;
    rbchildren children[2]; //[0] left, [1] right;
    byte color;
} rbelement;
 
typedef struct head
{
    struct tail * father;
    struct rbelement * root;
} rbhead;
typedef struct tail
{
    (void*) NIL = NULL;
    struct rbhead * child;
}rbtail;

#pragma region metodi non utilizzati da utente (private)
//ricerca il nodo con il valore x
rbelement * searchelem(rbelement * ref,int x, rbtail * end){
    if(ref == end -> NIL || ref->value == x)
        return ref;
    return searchelem(ref ->children[int(ref->value < x)], x); //se ho elemento minore cerco a destra, indice 1, altrim a sinistra. 
}
//ritorna min se minzero_maxone è zero, max se è uno. 
rbelement * extreme_value_byroot(rbelement * ref, int minzero_maxone, rbtail* end){
    rbelement * cur = ref;
    while (cur -> children[minzero_maxone] != end -> NIL)
        cur = cur -> children[minzero_maxone];
    return cur;
}
//ritorna min se minzero_maxone è zero, max se è uno.
rbelement *extreme_value(rbhead * ref, int minzero_maxone, rbtail* end){
    rbelement * newref = ref ->root;
    return extreme_value_byroot(newref,minzero_maxone,end);
}
//se verso = 1 prendo successore, se verso è -1 prendo predecessore.
rbelement * adiacente(rbelement * ref, int verso, rbtail * end){
    verso = (verso + 1)/2; //0 se -1, 1 se 1.
    if(ref ->children[verso] != end -> NIL)
        return extreme_value(ref->children[verso], 1-verso,end); //minimo di right se successore, massimo di left se predecessore.
    rbelement * cur = ref->father;
    while(cur != end -> NIL && cur -> children[verso] == ref)
    {
        ref = cur;
        cur = cur->father;
    }
    return cur;
} 
#pragma endregion

//ricerca il nodo con il valore x
rbelement * search(rbhead * ref,int x, rbtail * end){
    rbelement * newref = ref ->root;
    return searchelem(newref,x,end);
}
rbelement * max(rbhead * ref, rbtail* end)
{
    return extreme_value(ref,1,end);
}
rbelement * min(rbhead * ref, rbtail* end)
{
    return extreme_value(ref,0,end);
}
rbelement * successore_byref(rbelement*ref, rbtail*end){
    return adiacente(ref,1,end);
}
rbelement * predecessore_byref(rbelement*ref, rbtail*end){
    return adiacente(ref,-1,end);
}

// se verso è -1, il senso è orario (right); se verso è 1, il senso è antiorario (left)
rbelement * rotate(rbhead * ref, rbelement * ruotato, int verso, rbtail * end){
    verso = (verso + 1)/2; //0 se -1, 1 se 1.
    rbelement * cur = ruotato ->children[verso];
    ruotato->children[verso] = cur->children[1-verso];
    if(cur->children[1-verso] != end -> NIL)
        cur->children[1-verso]->father = ruotato;
    cur->father = ruotato->father;
    if(ruotato->father == end -> NIL)
        ref->root = cur; //root popola ref.root = curr;
    else if(ruotato == ruotato->father->children[1-verso])
       ruotato->father->children[1-verso] = cur;
    else ruotato->father->children[verso] = cur;
    cur->children[1-verso] = ruotato;
    ruotato->father = cur;
}
//fixing figlio sinistro se isright è 0, destro se isright è 1.
void fix(rbhead *ref, rbelement * inserito,int isright){
    rbelement * y = inserito ->father->father->children[isright];
    if(y->color == 1)
    {
        inserito->father->color = 0;
        y->color = 0;
        inserito ->father ->father->color = 1;
        inserito = inserito ->father->father;
    }
    else 
    {
        if(inserito == inserito->father->children[isright])
        {
        inserito = inserito->father;
        rotate(ref,inserito,1);
        }
        inserito ->father->color = 0;
        inserito ->father->father ->color = 1;
        rotate(ref,inserito->father->father,-1);
    }
}
void RiparaRbInserisci(rbhead*ref, rbelement * inserito){
    while (inserito->father->color == 1) // finché papà è rosso
    {
        if(inserito -> father == inserito ->father->father ->children[0])
            fix(ref,inserito,1);
        else
            fix(ref,inserito,0);
    }
    ref->root->color = 0;
}

void inseriscielemento(rbhead*ref, rbelement*insert, rbtail*end){
    rbelement* precedente = end -> NIL;
    rbelement* corrente = ref->root;
    while (corrente != end -> NIL)
    {
        precedente = corrente;
        if(insert->value == corrente -> value)
            corrente = corrente ->children[0];
        else 
            corrente = corrente -> children[1];
    }
    insert->father = precedente;
    if(precedente == end -> NIL)
        ref->root = insert;
    else if(insert->value < precedente -> value)
        precedente -> children[0] = insert;
    else precedente -> children[1] = insert;
    RiparaRbInserisci(ref,insert);
}

void cancellaelemento(rbhead * ref, rbelement *cancel, rbtail*end){
    rbelement * da_canc;
    rbelement * sottoa;
    //individuo nodo da cancellare.
    if(cancel -> children[0] == end -> NIL || cancel ->children[1] == end -> NIL)
        da_canc = cancel;
    else da_canc = successore(cancel);
    //individuano sottoalbero da spostare e spostano riferimento a padre.
    if(da_canc->children[0] != end -> NIL)
        sottoa = da_canc->children[0];
    else
        sottoa = da_canc->children[1];
    if(sottoa != end -> NIL)
        sottoa->father = da_canc ->father;
    //correzione riferimento a padre.
    if(da_canc ->father == end -> NIL){
        ref->root = sottoa;
        rootupdate(ref);
    }
    else if(da_canc == da_canc ->father->children[0])
        da_canc ->father->children[0] = sottoa;
    else da_canc ->father->children[1] = sottoa;
    //copiatura valore chiave.
    if(da_canc != cancel)
        cancel ->value = da_canc ->value;
    free(da_canc);
}
//fai rbcancella.
void insert(rbhead*ref, int x, rbtail*end){
    //fai metodo costruttore.
}