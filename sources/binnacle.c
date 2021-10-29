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
    
    
    return 0;
}


void createBinnacle(char*binnacleRoute){
    time_t rawtime;
    struct tm *timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    strftime(binnacleRoute, strlen("binnacle/DD-MMM-YYYY HH:MM:SS.txt")+1,"binnacle/%d-%b-%Y_%H-%M-\%S.txt",timeinfo);
    
    writeLine(binnacleRoute,"Binnacle created.");
}