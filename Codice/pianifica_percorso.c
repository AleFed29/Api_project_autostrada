#include <stdio.h>
#include <stdlib.h>


void PlotMark(mark * m){
    mark * voto = m;
    printf("\n DEBUG:");
    while (voto->prev != NULL)
    {
        printf("\t %d:%d", voto->km, voto->raggiuntocon);
        voto = voto ->prev;
    }
    printf("\t %d:%d", voto->km, voto->raggiuntocon);
    printf("\n ENDDEBUG.\n");
}





typedef struct marcatura
{
    pint km;
    struct marcatura * next;
    struct marcatura * prev;
    pint code;
    pint raggiuntocon;
}mark;
typedef struct percorso
{
    pint km;
    struct percorso * next;
}path;
mark * InitializeMark(stazione * s){
    mark * m = (mark*) malloc(sizeof(mark));
    m->next = NULL;
    m->km = s->kms;
    m->code = 1;
    m ->raggiuntocon = 0;
    return m;
}
mark * AggiungiinCodaMark(mark * m, stazione * s, pint posiz,  pint marcatura){
    mark * elemento = (mark*) malloc(sizeof(mark));
    elemento ->km = s->kms;
    elemento ->raggiuntocon = marcatura;
    elemento ->prev = m;
    elemento ->code = posiz;
    m ->next = elemento;
    elemento ->next = NULL;
    return elemento;
}


path * AggiungiinTestapath(path* p, mark* percorso){
    path * elemento = (path*) malloc(sizeof(path));
    elemento ->km = percorso->km;
    elemento->next = p;
    return elemento;
}
void AggiungiinCodapath(path* p, mark* marca){
    path * elemento = (path*) malloc(sizeof(path));
    elemento ->km = marca->km;
    elemento ->next = NULL;
    path * curr = p;
    while (curr ->next != NULL)
        curr = curr ->next;
    curr ->next = elemento;
}

void CancellaMark(mark * m){
    mark * curr = m;
    mark * dacanc = m;
    while (curr ->next != NULL)
    {
        curr = curr->next;
        free(dacanc);
        dacanc = curr;
    }
    free(curr);
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
path * InitializePath(stazione * s){
    path * per = (path*) malloc(sizeof(path));
    per->km = s ->kms;
    per ->next = NULL;
    return per;
}
mark * Filo_di_Arianna_dx(stazione * part, stazione * arr){
    mark * m = InitializeMark(part);
    stazione * curr = part ->next;
    stazione * lastref = part;
    pint pos = 2;//per arrivare qui.
    pint statnum = 1;//parto da qui.
    pint auton = autonomia_max_destra(lastref);
    while(curr ->kms < arr->prev->kms)
    {
        while(curr ->kms <= auton){
            m = AggiungiinCodaMark(m, curr,pos, statnum);
            curr = curr->next;
            pos++;
        }
        statnum++;
        lastref = lastref ->next;
        if(lastref == curr)//nessuna stazione precedente raggiunge curr. 
            return NULL;
        auton = autonomia_max_destra(lastref);
    }
    while (curr->kms < arr ->kms)//penultima
    {
        if(curr ->kms <= auton){
            m = AggiungiinCodaMark(m, curr,pos, statnum);
            curr = curr->next;
            pos++;
        }
        else
        {
            statnum++;
            lastref = lastref ->next;
            auton = autonomia_max_destra(lastref);
        }
        if(lastref == curr)//stesso di prima.
            return NULL;
    }
    while (lastref ->kms < arr ->kms && m->km != arr->kms)//o inserisco, o non c'è.
    {
        if(arr->kms <= auton)
            m = AggiungiinCodaMark(m, arr,pos, statnum);
        else
        {
            statnum++;
            lastref = lastref ->next;
            auton = autonomia_max_destra(lastref);
        }
    }
    if(m->km != arr->kms)
        return NULL;
    return m;
}
path * pianifica_percorso_destra(route * r, int partenza, int arrivo){
    stazione * part = cerca_stazione(r,partenza);
    stazione * arr = cerca_stazione(r,arrivo);
    if(part == NULL || arr == NULL)
        return NULL;
    path * per = InitializePath(arr);
    pint auton = autonomia_max_destra(part);
    if(auton >= arr->kms)
    {
        path * elemento = (path*) malloc(sizeof(path));
        elemento ->km = part ->kms;
        elemento ->next = per;
        return elemento;
    }
    mark * m = Filo_di_Arianna_dx(part,arr);
    pint pos = m->code;
    pint raggiuntocon = m ->raggiuntocon;//riavvolgo il filo di Arianna.
    while (pos > 0)
    {
        while (pos > raggiuntocon && pos > 0)
        {
            pos--;
            if(m->km > part ->kms)
                m = m ->prev;
        }
        per = AggiungiinTestapath(per,m);
        raggiuntocon = m->raggiuntocon;
    }
    CancellaMark(m);
    if(per ->km == part ->kms)
        return per;
    return NULL;
}
mark * Filo_di_Arianna_sx(stazione * part, stazione * arr){
    mark * m = InitializeMark(arr);
    pint pos = 2; //arrivo qui.
    pint statnum = 1; //da qui.
    stazione * lastref = arr;
    stazione * curr = arr -> prev;
    int auton = autonomia_max_sinistra(lastref);
    
    while (curr ->kms > part->next->kms)
    {
        while (auton <= curr ->kms)
        {
            AggiungiinCodaMark(m,curr,pos,statnum);
            pos++;
            curr = curr ->prev;
        }
        if(lastref->kms <= curr->kms)//non raggiungo.
            return NULL;
        statnum++;
        lastref = lastref ->prev;
        auton = autonomia_max_sinistra(lastref);
    }
    while (curr ->kms > part ->kms)//penultima
    {
        if(auton <= curr->kms){
            m = AggiungiinCodaMark(m, curr,pos, statnum);
            curr = curr->prev;
            pos++;
        }
        else
        {
            statnum++;
            lastref = lastref ->prev;
            auton = autonomia_max_sinistra(lastref);
        }
    if(lastref == curr)//stesso di prima.
        return NULL;
    }
    while (lastref ->kms > part ->kms && m->km != part->kms)//o raggiungo fine, o inserisco.
    {
        if(auton <= part ->kms)
            m = AggiungiinCodaMark(m, part,pos, statnum);
        else
        {
            statnum++;
            lastref = lastref ->prev;
            auton = autonomia_max_sinistra(lastref);
        }
    }
    if(m->km != part->kms)
        return NULL;
    return m;
}

path * pianifica_percorso_dasinistra(route * r, int partenza, int arrivo){
    stazione * part = cerca_stazione(r,partenza);
    stazione * arr = cerca_stazione(r,arrivo);
    if(part == NULL || arr == NULL)
        return NULL;
    path* per = InitializePath(part);
    if(arr == part)
        return per;
    if(autonomia_max_sinistra(arr) <= part->kms) //caso base.
    {
        path * elemento = (path*) malloc(sizeof(path));
        elemento ->km = part ->kms;
        per ->next = elemento;
        return per;
    }
    mark * m = Filo_di_Arianna_sx(part, arr);
    //trova stazioni che permettono di avere percorso migliore, dentro a m.
    /*Codice di selezione delle stazioni.*/
    if(m == NULL) return NULL;
    pint raggiungo = m->raggiuntocon;
    pint pos = m->code; 
    //c'è corrispondenza biunivoca tra m e curr:
    // ogni stazione ha almeno una stazione che la raggiunge, 
    //se il percorso migliore è in m, altrimenti una stazione sarebbe irraggiungibile a tutte le precedenti. 
    //Inizializzo variabili a successiva della partenza.
    stazione * curr = part -> next;
    m = m->prev;
    while(m->km < arr ->kms)//m è salvato da arrivo a partenza, io ho ultimo riferimento (partenza).
    {
        if(autonomia_max_sinistra(curr) <= per->km && pos > raggiungo)//se esiste stazione più vicina all'inizio rispetto alla certa, aggiungo quella.
        {
            per = AggiungiinTestapath(per,m);
            raggiungo = m->raggiuntocon;
        }
        curr = curr -> next;
        pos--;
        m = m->prev;
    }
    //fine
    CancellaMark(m);
    if(per ->km == arr->kms)
        return per;
    return NULL;
}





/*
typedef struct marcatura
{
    pint km;
    struct marcatura * next;
    struct marcatura * prev;
    pint code;
    pint raggiuntocon;
}mark;

path * InitializePath(stazione * s){
    path * per = (path*) malloc(sizeof(path));
    per->km = s ->kms;
    per ->next = NULL;
    return per;
}
mark * InitializeMark(stazione * s){
    mark * m = (mark*) malloc(sizeof(mark));
    m->next = NULL;
    m->km = s->kms;
    m->code = 1;
    m ->raggiuntocon = 0;
    return m;
}
mark * AggiungiinCodaMark(mark * m, stazione * s, pint posiz,  pint marcatura){
    mark * elemento = (mark*) malloc(sizeof(mark));
    elemento ->km = s->kms;
    elemento ->raggiuntocon = marcatura;
    elemento ->prev = m;
    elemento ->code = posiz;
    m ->next = elemento;
    elemento ->next = NULL;
    return elemento;
}
path * AggiungiinTestapath(path* p, mark* percorso){
    path * elemento = (path*) malloc(sizeof(path));
    elemento ->km = percorso->km;
    elemento->next = p;
    return elemento;
}

path * pianifica_percorso_destra(route * r, pint partenza, pint arrivo)
{
    stazione * part = cerca_stazione(r,partenza);
    stazione * arr = cerca_stazione(r,arrivo);
    if(part == NULL || arr == NULL)
        return NULL;
    path * per = InitializePath(arr);
    if(arr == part)
        return per;
    pint auton = autonomia_max_destra(part);
    if(auton >= arr->kms)
    {
        path * elemento = (path*) malloc(sizeof(path));
        elemento ->km = part ->kms;
        elemento ->next = per;
        return elemento;
    }
    mark * m = InitializeMark(part);
    stazione * lastref = part;
    stazione * curr = part->next;
    pint pos = 2;
    pint statnum = 1;
    while (curr->kms < arr->kms)
    {
        if(curr ->kms <= auton && curr->kms < arr-> kms)
        {
            m = AggiungiinCodaMark(m,curr,pos,statnum);
            curr = curr->next;
            pos++;
        }
        else
        {
            lastref = lastref ->next;
            statnum++;
            auton = autonomia_max_destra(lastref);
        }
        if(lastref == curr)
            return NULL;
    }
    while (lastref ->kms < arr->kms) //ultima stazione.
    {
        if(arr->kms <= auton)
        {
            m = AggiungiinCodaMark(m,curr,pos,statnum);
            break;
        }
        else
        {
            lastref = lastref ->next;
            statnum++;
            auton = autonomia_max_destra(lastref);
        }
    }
    if(m ->km != arr ->kms)
        return NULL;
    pos = m->code;
    pint raggiungo = m->raggiuntocon;
    while (pos > 0)
    {
        while (pos > raggiungo && raggiungo > 0)
        {
            m = m->prev;
            pos--;
        }
        per = AggiungiinTestapath(per, m);
        raggiungo = m->raggiuntocon;
    }
    if(per ->km != part ->kms)
        return NULL;
    return per;
}

path * pianifica_percorso_dasinistra(route * r, int partenza, int arrivo){
    stazione * part = cerca_stazione(r,partenza);
    stazione * arr = cerca_stazione(r,arrivo);
    if(part == NULL || arr == NULL)
        return NULL;
    path* per = InitializePath(part);
    if(arr == part)
        return per;
    if(autonomia_max_sinistra(arr) <= part->kms) //caso base.
    {
        path * elemento = (path*) malloc(sizeof(path));
        elemento ->km = part ->kms;
        per ->next = elemento;
        return per;
    }
    mark * m = InitializeMark(part);
    stazione * lastref = part;
    stazione * curr = part ->next;
    pint statnum = 1;
    pint pos = 2;
    pint auton = autonomia_max_sinistra(curr);
    while (curr ->kms < arr ->kms){
        if(auton <= lastref ->kms)
        {
            m = AggiungiinCodaMark(m, curr, pos, statnum);
            pos++;
            curr = curr->next;
            auton = autonomia_max_sinistra(curr);
        }
        else
        {
            lastref = lastref ->next;
            statnum++;
        }
        if(lastref == curr)
            return NULL;
    }
    while (lastref->kms < arr->kms)
    {
       if(auton <= lastref->kms){
            m = AggiungiinCodaMark(m,arr,pos,statnum);
            break;
       }
       else
       {
            lastref = lastref->next;
            statnum++;
       }
    }
    if(m->km != arr->kms)
        return NULL;
    //puoi sennò provare con i puntatori... Viene pesante, però non devo tenere traccia della posizione.
}






*/