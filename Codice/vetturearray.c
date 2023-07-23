#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
        if(mid == stop && h->first[mid] !=key)
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
int cerca(head_vetture*h, int key){
    if(key == 0)
        return (int)(h->countzero > 0) - 1;
    if(h->lastindex < 0)
        return -1;
    return search_index(h,key,0,h->lastindex,0);
}
int max(head_vetture*h){
    if(h->lastindex < 0)
        if(h->countzero == 0)
            return -1;
        else
            return 0;
    return h->first[h->lastindex];
}
int min(head_vetture*h){
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
void insert(head_vetture*h, int key){
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
    if(key > max(h))
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
void cancella(head_vetture*h, int key){
    int i = search_index(h,key,0,h->lastindex,0);
    if(i == -1)
        return;//andrà scritto messaggio.
    while (i < h->lastindex){
        h->first[i] = h->first[i+1];
        i++;
    }
    h->lastindex--;
}
#pragma endregion vetture
int main()
{
    head_vetture * h = Initialize();
    insert(h,4);
    insert(h,25);
    //capisci come mai, con questi, esce 4,25,0,3,6,...e non 0, 3,4,6,...
    int i;
    for(i=0; i < 60; i+=3)
        insert(h,i);
    insert(h, 17);
    insert(h,26);
    printf("\n zero:%d", h->countzero);
    for(i=0; i<=h->lastindex; i++)
        printf("\n %d:%d",i, h->first[i]);
    int index21 = cerca(h,21);
    printf("\n");
    printf("\n %d", h->first[index21]);
    cancella(h,21);
    int notindex21 = cerca(h,21);
    printf("\n %d", notindex21);
    printf("\n %d", h->first[index21]);
    
    return 0;
}
