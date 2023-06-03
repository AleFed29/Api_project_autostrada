#include <stdio.h> 
#include <stdlib.h>
typedef unsigned char byte;
typedef struct autonotree * autochildren;
//devo definire T e di conseguenza T.root separato?
typedef struct autonotree
{
    int value;
    struct autonotree * father;
    struct autonotree * root;
    autochildren children[2]; //[0] left, [1] right;
    byte color;
} autoelement;
 
autoelement * successore(autoelement * ref, int x){
    autoelement * start_from = search(ref, x);
    if(start_from ->children[1] != NULL)
        return start_from ->children[1];
    autoelement * ancestor = start_from ->father;
    while (ancestor != NULL && ancestor ->children[1] == start_from)
    {
        start_from = ancestor;
        ancestor = ancestor ->father;
    }
    return ancestor;
}
autoelement * root(autoelement * ref)
{
    autoelement * curr = ref;
    while (curr->father != NULL)
        curr = curr->father;
    return curr;
}
//0 for left, 1 for right
void rootupdatemethod(autoelement * ref, autoelement * child){
    if(child ->children[0] == NULL && child->children[1] == NULL)
        return;
    child -> root = ref ->root;
    ref = child;
    rootupdatemethod(ref, ref ->children[0]);
    rootupdatemethod(ref, ref ->children[1]);
}
void rootupdate(autoelement * ref){
    if(ref ->children[0] != NULL) 
        rootupdatemethod(ref, ref ->children[0]);
    if(ref->children[1] != NULL)
        rootupdatemethod(ref, ref ->children[1]);
}
//ricerca il nodo con il valore x
autoelement * search(autoelement * ref,int x){
    if(ref == NULL || ref->value == x)
        return ref;
    return search(ref ->children[int(ref->value < x)], x); //se ho elemento minore cerco a destra, indice 1, altrim a sinistra. 
}
//ritorna min se minzero_maxone è zero, max se è uno. 
autoelement * extreme_value(autoelement * ref, int minzero_maxone){
    autoelement * cur = ref;
    while (cur -> children[minzero_maxone] != NULL)
        cur = cur -> children[minzero_maxone];
    return cur;
}

//se verso = 1 prendo successore, se verso è -1 prendo predecessore.
autoelement * adiacente(autoelement * ref, int verso){
    verso = (verso + 1)/2; //0 se -1, 1 se 1.
    if(ref ->children[verso] != NULL)
        return extreme_value(ref->children[verso], 1-verso); //minimo di right se successore, massimo di left se predecessore.
    autoelement * cur = ref->father;
    while(cur != NULL && cur -> children[verso] == ref)
    {
        ref = cur;
        cur = cur->father;
    }
    return cur;
}  

// se verso è -1, il senso è orario (right); se verso è 1, il senso è antiorario (left)
autoelement * rotate(autoelement * ref, autoelement * ruotato, int verso){
    verso = (verso + 1)/2; //0 se -1, 1 se 1.
    autoelement * cur = ruotato ->children[verso];
    ruotato->children[verso] = cur->children[1-verso];
    if(cur->children[1-verso] != NULL)
        cur->children[1-verso]->father = ruotato;
    cur->father = ruotato->father;
    if(ruotato->father == NULL)
        ref->root = cur; //root popola ref.root = curr;
    else if(ruotato == ruotato->father->children[1-verso])
       ruotato->father->children[1-verso] = cur;
    else ruotato->father->children[verso] = cur;
    cur->children[1-verso] = ruotato;
    ruotato->father = cur;
}
//fixing figlio sinistro se isright è 0, destro se isright è 1.
void fix(autoelement*ref, autoelement * inserito,int isright){
    autoelement * y = inserito ->father->father->children[isright];
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
void RiparaRbInserisci(autoelement*ref, autoelement * inserito){
    while (inserito->father->color == 1) // finché papà è rosso
    {
        if(inserito -> father == inserito ->father->father ->children[0])
            fix(inserito,1);
        else
            fix(inserito,0);
    }
    ref->root->color = 0;
}

void inseriscielemento(autoelement*ref, autoelement*insert){
    autoelement* precedente = NULL;
    autoelement* corrente = ref->root;
    while (corrente != NULL)
    {
        precedente = corrente;
        if(insert->value == corrente -> value)
            corrente = corrente ->children[0];
        else 
            corrente = corrente -> children[1];
    }
    insert->father = precedente;
    if(precedente == NULL)
        ref->root = insert;
    else if(insert->value < precedente -> value)
        precedente -> children[0] = insert;
    else precedente -> children[1] = insert;
    //insert -> root = ref -> root; riferimento alla radice, in quel caso mettere ref a parametro RBinserisci non serve...
    RiparaRbInserisci(ref,insert);
}

void cancellaelemento(autoelement * ref, autoelement *cancel){
    autoelement * da_canc;
    autoelement * sottoa;
    //individuo nodo da cancellare.
    if(cancel -> children[0] == NULL || cancel ->children[1] == NULL)
        da_canc = cancel;
    else da_canc = successore(cancel);
    //individuano sottoalbero da spostare e spostano riferimento a padre.
    if(da_canc->children[0] != NULL)
        sottoa = da_canc->children[0];
    else
        sottoa = da_canc->children[1];
    if(sottoa != NULL)
        sottoa->father = da_canc ->father;
    //correzione riferimento a padre.
    if(da_canc ->father == NULL){
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
