#include <stdio.h> 
#include <stdlib.h>
typedef unsigned char byte; //se puoi cambia con bool color
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
    rbelement * root;
    struct tail * treetail; //come posso mettere rbtail qua?
} rbhead;

typedef struct tail
{
    void* NIL;
    struct head * child; //come posso mettere rbhead qua?
}rbtail;

rbelement * construct_tree(int value, rbelement * father, rbchildren childs[2]){
    rbelement * new = (rbelement *)malloc(sizeof(rbelement));
    new ->value = value;
    new ->father = father;
    new ->children[0] = childs[0];
    new ->children[1] = childs[1];
    new ->color = 0;
    return new;
}
rbhead * construct_head(rbelement *roote){
    rbhead * head = (rbhead *) malloc(sizeof(rbhead));
    head ->root = roote;
    head ->treetail = NULL;
}
rbtail * construct_tail(rbhead * childh){
    rbtail * tail = (rbtail *)malloc(sizeof(rbtail));
    tail ->NIL = NULL;
    childh ->treetail = tail;
    tail->child = childh;
    //tail->child->treetail = tail;
    return tail; 
}
rbhead * Initializetree(int value){
    rbchildren children [2] = {NULL,NULL};
    rbelement * ref = construct_tree(value, NULL, children);
    rbhead * head = construct_head(ref);
    rbtail * tail = construct_tail(head);
    return head;
}
#pragma region metodi non utilizzati da utente (private)
//ricerca il nodo con il valore x
rbelement * searchelem(rbelement * ref,int x, rbtail * end){
    if(ref == end -> NIL || ref->value == x)
        return ref;
    return searchelem(ref ->children[(int)(ref->value < x)], x,end); //se ho elemento minore cerco a destra, indice 1, altrim a sinistra. 
}
//ritorna min se minzero_maxone è zero, max se è uno. 
rbelement * extreme_value_byroot(rbelement * ref, int minzero_maxone, rbtail* end){
    rbelement * cur = ref;
    while (cur -> children[minzero_maxone] != end -> NIL)
        cur = cur -> children[minzero_maxone];
    return cur;
}
//se verso = 1 prendo successore, se verso è -1 prendo predecessore.
rbelement * adiacente(rbhead*h,rbelement * ref, int verso, rbtail * end){
    verso = (verso + 1)/2; //0 se -1, 1 se 1.
    if(ref == extreme_value_byroot(h->root, verso, end))
        return end->NIL;
    if(ref ->children[verso] != end -> NIL)
        return extreme_value_byroot(ref->children[verso], 1-verso,end); //minimo di right se successore, massimo di left se predecessore.
    rbelement * cur = ref->father;
    while(cur != end -> NIL && cur -> children[verso] == ref)
    {
        ref = cur;
        cur = cur->father;
    }
    return cur;
} 
#pragma endregion

rbelement * search(rbhead*ref, int x){
    return searchelem(ref ->root,x, ref->treetail);
}
rbelement * max(rbhead * ref){
    return extreme_value_byroot(ref->root, 1, ref->treetail);
}
rbelement * min(rbhead * ref){
    return extreme_value_byroot(ref->root, 0, ref->treetail);
}
rbelement * successore(rbhead * head, rbelement * elem){
    return adiacente(head,elem,1, head->treetail);
}
rbelement * predecessore(rbhead * head, rbelement * elem){
    return adiacente(head, elem,-1, head->treetail);
}
//da controllare
// se verso è -1, il senso è orario (right); se verso è 1, il senso è antiorario (left)
rbelement * rotate(rbhead * ref, rbelement * ruotato, int verso, rbtail * end){
    verso = (verso + 1)/2; //0 se -1, 1 se 1.
    rbelement * cur = ruotato ->children[verso]; //deve essere scambiato con ruotato.
    ruotato->children[verso] = cur->children[1-verso];//'ruoto' figlio verso opposto.
    if(cur->children[1-verso] != end -> NIL)
        cur->children[1-verso]->father = ruotato;
    cur->father = ruotato->father;
    if(ruotato->father == end -> NIL) //se ruotato era la radice.
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
        rotate(ref,inserito,1, ref ->treetail);
        }
        inserito ->father->color = 0;
        inserito ->father->father ->color = 1;
        rotate(ref,inserito->father->father,-1,ref ->treetail);
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
    while (corrente != end -> NIL) //finché posso scendere
    {
        precedente = corrente; //scendo
        if(insert->value < corrente -> value) //inserisco nel posto giusto. Come foglia.
            corrente = corrente ->children[0];
        else 
            corrente = corrente -> children[1];
    }
    insert->father = precedente;
    if(precedente == end -> NIL)
        ref->root = insert; //albero era vuoto.
    else if(insert->value < precedente -> value) //figlio sinistro ordino come BST.
        precedente -> children[0] = insert;
    else precedente -> children[1] = insert; //figlio destro ordino come BST.
    RiparaRbInserisci(ref,insert);
}
void insert(rbhead*ref, int x, rbtail*end){
    rbchildren rc [2] = {end->NIL,end->NIL};
    rbelement * t = construct_tree(x,NULL, rc);//fai metodo costruttore.
    inseriscielemento(ref, t,end); //prova a vedere
}
void fixcancel(rbhead * ref, rbelement * x, int isright, rbtail *end){
    rbelement * w = x->father->children[isright];
    if(w->color == 1)
    {
        w->color = 0;
        x->father->color = 1;
        rotate(ref,x->father,2*isright-1,end); //left se è destro (1), right se è sx.
        w = x->father->children[isright];
    }
    if(w->children[isright]->color == 0 && w->children[1-isright]->color == 0)
    {
        w->color = 1;
        x = x->father;
    }
    else
    { 
        if(w->children[isright]->color == 0){
            w->children[1-isright] = 0;
            w->color = 1;
            rotate(ref,w,1-(2*isright),end); //right rotate se right, left se left.
            w = x->father->children[isright];
        }
        w->color = x->father->color;
        x->father->color = 0;
        w->children[isright]->color = 0;
        rotate(ref,x->father, 2*isright-1,end); 
        x = ref->root;
    }
}

void RiparaRbCancella(rbhead*ref, rbelement * sostituto, rbtail*end){
    int index;
    while (sostituto != ref->root && sostituto ->color != 0)
    {
        if(sostituto == sostituto ->father->children[0])
            index = 1;
        else
            index = 0;
        fixcancel(ref,sostituto,index,end);
    }
    sostituto ->color = 0;    
}

void cancellaelemento(rbhead * ref, rbelement *cancel, rbtail*end){
    rbelement * da_canc/*=(rbelement*)malloc(sizeof(rbelement))*/;
    rbelement * sottoa/*=(rbelement*)malloc(sizeof(rbelement))*/;
    //individuo nodo da cancellare.
    if(cancel -> children[0] == end -> NIL || cancel ->children[1] == end -> NIL)
        da_canc = cancel;
    else da_canc = successore(ref,cancel);
    //individuano sottoalbero da spostare e spostano riferimento a padre.
    if(da_canc->children[0] != end -> NIL)
        sottoa = da_canc->children[0];
    else if(da_canc->children[1] != end -> NIL)
        sottoa = da_canc->children[1];
    else
        sottoa = da_canc;
    if(sottoa != end -> NIL)
        sottoa->father = da_canc ->father;
    //correzione riferimento a padre.
    if(da_canc ->father == end -> NIL){
        ref->root = sottoa;
    }
    else if(da_canc == da_canc ->father->children[0])
        da_canc ->father->children[0] = sottoa;
    else da_canc ->father->children[1] = sottoa;
    //copiatura valore chiave.
    if(da_canc != cancel)
        cancel ->value = da_canc ->value;
    free(da_canc);
    RiparaRbCancella(ref,sottoa, end); //sottoa ha preso il posto di da_canc, non deve violare norme alberi rossoneri.
}
void cancella(rbhead*h,int key){
    rbelement * element = search(h,key);
    if(element != NULL){
        cancellaelemento(h,element,h->treetail);
        printf("\n%d", element->value);
        element ->value = -1;
        //free(element); SIGABORT capisci perché questo elemento senza padre e senza figli viene intercettato da successore.
    }
}

void plot(rbhead * h){
    rbelement* s = min(h);
    printf("\n");
    while(s != NULL)
    {
        printf("\t%d", s->value);
        s = successore(h,s);
    }
    printf("\n");
}

int main()
{
    printf("Hello World");
    rbhead*h = Initializetree(10);
    insert(h,5,h->treetail);
    insert(h,15,h->treetail);
    rbelement * m = max(h);
    rbelement * i = min(h);
    printf("\n %d", m -> value); //stampa 15
    printf("\n %d", i -> value); //stampa 5
    printf("\n %d", i->father->value); //stampa 10
    insert(h, 7, h->treetail);
    insert(h, 2, h->treetail);
    insert(h, 3, h->treetail);
    insert(h, 11, h->treetail);
    rbelement* nemo = search(h, 7);
    rbelement* pensosianull = search(h,9);
    printf("\n %d", nemo->value);
    if(pensosianull == NULL)
        printf("\n non esiste 9");
    rbelement * dory = successore(h,nemo);
    printf("\n %d", dory -> value); //esce 10.
    insert(h, 9, h->treetail);
    dory = successore(h,nemo);
    printf("\n %d", dory -> value);//esce 9
    rbelement * tre = search(h,3);
    printf("\n %d", tre -> value); //esce 3
    rbelement * due = predecessore(h,tre);
    printf("\n %d", due -> value);//esce 2
    plot(h);
    cancella(h,11);
    rbelement * pensoundicinoncisiapiu = search(h,11);
    if(pensoundicinoncisiapiu == NULL)
        printf("\n 11 cancellato");
    plot(h);
    return 0;
}