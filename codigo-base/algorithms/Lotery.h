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
    int i, j, inicio=0, fim=0, verifica=1, aleatorio, cont_ordem=0;
    char **bilhetes, **ordem;
    Process aux;

    int current = 0, quantity=0, vet[len], cont = 0;

    srand((unsigned)time(NULL));
    srand(time(NULL));

    ordem = malloc(len * sizeof(char*));
    for(i=0 ; i<len ; i++){
        ordem[i] = malloc (5 * sizeof(char));
    }

    merge_sort_by_arrive_time(p, 0, len);

    while(cont_ordem<len){
        cont = 0;
        quantity = 0;
        if(current == 0){
            for(i=0 ; i<len ; i++){
                if(p[i].arrive_time == p[i+1].arrive_time){
                    cont++;
                }
            }
        }else{
            for(i=0 ; i<len ; i++){
                if(p[i].completed == FALSE && p[i].arrive_time < current){
                    cont++;
                }
            }
        }
        for(i=0 ; i<cont ; i++){
            quantity += p[i].priority;
        }

        //aloca um vetor do tamanho das prioridades
        bilhetes = malloc(quantity * sizeof(char*));
        for(i=0 ; i<quantity ; i++){
            bilhetes[i] = malloc (5 * sizeof(char));
        }

        inicio = 0;
        fim = 0;
        for(i=0 ; i<cont ; i++){
            fim = inicio + p[i].priority;
            for(j=inicio ; j<fim ; j++){
                strcpy(bilhetes[j], p[i].id);
            }
            inicio = fim;
        }
        

        do{
            verifica = 0;
            aleatorio = (rand()%quantity);
            for(i=0 ; i<len ; i++){
                if(strcmp(ordem[i], bilhetes[aleatorio]) == 0){
                    verifica = 1;
                    break;
                }
            }
        }while(verifica);

        strcpy(ordem[cont_ordem], bilhetes[aleatorio]);

        for(i=0 ; i<len ; i++){
            if(strcmp(p[i].id, ordem[cont_ordem]) == 0){
                if(current < p[i].arrive_time){
                    current = p[i].burst + (p[i].arrive_time - current);
                }else{
                    current+= p[i].burst;
                }
                p[i].completed = TRUE;
                aux = p[i];
                for(j=i ; j<len ; j++){
                    p[j] = p[j+1];
                }
                p[len-1] = aux;
            }
        }
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
            p[i].return_time = p[i].burst + p[i].arrive_time;
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

    printf("\tLotery \n\n");

    lotery_print_gantt_chart(p, len);
    printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
    printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);
    printf("\tAverage Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)len);

    print_table(p, len);
}

#endif