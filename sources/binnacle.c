#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


void writeLine(char*binnacleRoute, char*strToWrite){
    FILE *fptr;
    fptr = fopen(binnacleRoute,"w");
    
    if(fptr == NULL)
    {
        printf("Error creating binnacle \n");   
        exit(1);             
    }

    //Get time
    time_t rawtime;
    struct tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    
    char strTime [100]; 
    strftime(strTime, strlen("HH:MM:SS")+1,"%H:%M:\%S", timeinfo);

    fprintf(fptr, "%s %s \n", strToWrite, strTime);

    fclose(fptr);
}


void createBinnacle(char*binnacleRoute){
    time_t rawtime;
    struct tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(binnacleRoute, strlen("binnacle/DD-MMM-YYYY HH:MM:SS.txt")+1,"binnacle/%d-%b-%Y_%H-%M-\%S.txt",timeinfo);
    
    writeLine(binnacleRoute,"Binnacle created at:");
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