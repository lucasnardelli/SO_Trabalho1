#ifndef __Lotery__
#define __Lotery__


#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"
#include <string.h>
#include <time.h>

void lotery_print_gantt_chart(Process *p, int len) {
    int i, j;
    printf("\t ");
    for (i = 0; i < len; i++)
    {
        for (j = 0; j < p[i].burst; j++)
            printf("--");

        printf(" ");
    }

    printf("\n\t|");


    for (i = 0; i < len; i++)
    {
        for (j = 0; j < p[i].burst - 1; j++)
            printf(" ");

        printf("%s", p[i].id);

        for (j = 0; j < p[i].burst - 1; j++)
            printf(" ");

        printf("|");
    }

    printf("\n\t ");

    for (i = 0; i < len; i++)
    {
        for (j = 0; j < p[i].burst; j++)
            printf("--");

        printf(" ");
    }

    printf("\n\t");


    printf("0");

    for (i = 0; i < len; i++)
    {
        for (j = 0; j < p[i].burst; j++)
            printf("  ");

        if (p[i].return_time > 9)
            printf("\b");

        printf("%d", p[i].return_time);

    }

    printf("\n");
}

void Lotery(Process *p, int len) {

    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int total_response_time = 0;
    int i, j, cont = 0, inicio=0, fim=0, verifica=1, aleatorio, cont_ordem=0;
    char **bilhetes, **ordem;
    Process aux;

    srand((unsigned)time(NULL));
    srand(time(NULL));

    ordem = malloc(len * sizeof(char*));
    for(i=0 ; i<len ; i++){
        ordem[i] = malloc (5 * sizeof(char));
    }

    while(cont_ordem<len){
        //Soma as prioridades
        for(i=0 ; i<len ; i++){
            cont+= p[i].priority;
        }
        
        //aloca um vetor do tamanho das prioridades
        bilhetes = malloc(cont * sizeof(char*));
        for(i=0 ; i<cont ; i++){
            bilhetes[i] = malloc (5 * sizeof(char));
        } 
        
        for(i=0 ; i<len ; i++){
            fim = inicio + p[i].priority;
            for(j=inicio ; j<fim ; j++){
                strcpy(bilhetes[j], p[i].id);
            }
            inicio = fim;
        }

        do{
            verifica = 0;
            aleatorio = (rand()%cont);
            for(i=0 ; i<len ; i++){
                if(strcmp(ordem[i], bilhetes[aleatorio]) == 0){
                    verifica = 1;
                    break;
                }
            }
        }while(verifica);

        strcpy(ordem[cont_ordem], bilhetes[aleatorio]);
        cont_ordem++;

    }
    for(i=0 ; i<len ; i++){
        for(j=0 ; j<len ; j++){
            if(strcmp(ordem[i], p[j].id) == 0){
                aux = p[i];
                p[i] = p[j];
                p[j] = aux;
            }
        }
    }

    for (i = 0; i < len; i++)
    {
        if(i==0){
            p[i].response_time = 0;
            p[i].waiting_time = 0;
            p[i].return_time = p[i].burst;
        } else {
            p[i].return_time = p[i-1].return_time + p[i].burst;
            p[i].waiting_time = p[i-1].return_time - p[i].arrive_time;
            p[i].response_time = p[i-1].return_time - p[i].arrive_time;
        }
    }

    for (i = 0; i < len; i++)
    {
        p[i].turnaround_time = p[i].burst + p[i].waiting_time;
        total_waiting_time += p[i].waiting_time;
        total_turnaround_time += p[i].turnaround_time;
        total_response_time += p[i].response_time;
    }

    printf("\tExemplo dado pelo professor: Lotery \n\n");

    lotery_print_gantt_chart(p, len);
    printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
    printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);
    printf("\tAverage Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)len);

    print_table(p, len);
}

#endif