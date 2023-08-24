#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int split(char * command, char delimiter, char ** dest){
    int len = strlen(command);
    int i = 0,k = 0;
    int j;
    for(j = 0; j < len; j++){
        if(command[j] == delimiter){
            i++;
            k = 0;
        }
        else
        {
            dest[i][k] = command[j];
            k++;
        }
    }  
    return i;
}
int stringcast(char * s){
    int i = 0;
    int sum = 1;
    while (s[i] != '\0')
    {
        if(i == 0)
            sum = (int)(s[i]);
        else
            sum = sum * 10 + (int)(s[i]);
        i++;
    }
    return sum;
}

int main(){
    char * str = (char *)malloc(sizeof(char)*30);
    char ** strcom = (char **)malloc(sizeof(char*)*30);
    while(!feof(stdin)){
    scanf("%s \n", str);
    printf("%s \n", str);
    /*int len = split(str,' ', strcom);
    int i = 0;
    while(i < len)
    {
        printf("%s \n", strcom[i]);
        i++;
    }*/
    }
}
