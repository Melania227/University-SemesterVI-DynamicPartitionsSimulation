#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(){

    char routeDateTime[100];
    time_t rawtime;
    struct tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(routeDateTime, strlen("binnacle/DD-MMM-YYYY HH:MM:SS.txt")+1,"binnacle/%d-%b-%Y_%H-%M-\%S.txt",timeinfo);
    printf("%s \n", routeDateTime );
   
    writeFile(routeDateTime);
    
    
    return 0;
}


void writeFile(char* fileName){

    FILE *fptr;
    fptr = fopen(fileName,"w");
    
    if(fptr == NULL)
    {
        printf("Error!");   
        exit(1);             
    }

    for (int i = 0; i < 10; i++)
        fprintf(fptr, "Aqui deberian ir varas del proceso #%d\n", i + 1);

    fclose(fptr);
}