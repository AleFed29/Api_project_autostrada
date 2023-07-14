#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct head
{
    int * first;
    int lastindex;
    int len;
} head_vetture;

head_vetture * Initialize(){
    head_vetture * h = (head_vetture *) malloc(sizeof(head_vetture));
    int * ptr = (int *) malloc(sizeof(int)*128);
    h->len = 128;
    h->lastindex = 0;
    h->first = ptr[0];
    return h;
}
int search_index(head_vetture*h, int key,int start, int stop, int forinsertion){
    int mid = 0;
    while (start < stop)
    {
        mid = ceil((start+stop)/2);
        if(h->first[mid] == key)
            return mid;
        if(h->first[mid] < key)
            start = mid;
        if(h->first[mid]>key)
            stop = mid; 
    }
    if(forinsertion == 1)
        return mid; //indice più vicino.
    return -1;
}
int cerca(head_vetture*h, int key){
    return search_index(h,key,0,h->lastindex,0);
}
int max(head_vetture*h){
    return h->first[h->lastindex-1];
}
int min(head_vetture*h){
    return h->first[0];
}
void Check_vetture(head_vetture*h){
    if(h ->len - h->lastindex < 2)
    {
        h->first = (int*)realloc(h->first, sizeof(int)*h->len*2);
        h->len = h->len*2;
    }
}
void insert(head_vetture*h, int key){
    if(key > max(h))
        h->fist[h->lastindex+1] = key;
    int zone = search_index(h,key,0,h->lastindex,1);
    int i = zone;
    while (h->first[i]<= key)
        i++;
    while (h->first[i]<key)
        i--;
    i++;
    int temp = h->first[i];
    zone = i;
    h->first[i] = key;
    while (i < h->lastindex)
    {
        int temp2 = h->first[i+1];
        h->first[i+1] = temp;
        temp = temp2;
        i++;
    }
    h->lastindex++;
    Check_vetture(h);
}
void cancella(head_vetture*h, int key){
    int i = search_index(h,key,0,h->lastindex,0);
    if(i == -1)
        return;//andrà scritto messaggio.
    while (i < h->lastindex)
        h->first[i++] = h->first[i+1];
    h->lastindex--;
}