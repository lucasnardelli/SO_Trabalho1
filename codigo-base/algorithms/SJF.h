#ifndef __SHORTEST__JOB__FIRST__
#define __SHORTEST__JOB__FIRST__


#include "./Process.h"
#include "./SortingFunction.h"
#include "./PrintTable.h"


void sjf_calculate_time(Process *p, int len)
{
	int i;

	p[0].completed = TRUE;
	p[0].return_time = p[0].burst + p[0].arrive_time;
	p[0].waiting_time = 0;
	p[0].response_time = 0;

	for(i = 1; i < len; i++)
	{ 
		if(p[i-1].return_time > p[i].arrive_time){
			p[i].return_time = p[i-1].return_time + p[i].burst;
			p[i].waiting_time = p[i-1].return_time - p[i].arrive_time;
		} else {
			p[i].return_time = p[i].arrive_time + p[i].burst;
			p[i].waiting_time = 0;
		}
		p[i].response_time = p[i].waiting_time;
	}
}


void sjf_print_gantt_chart(Process *p, int len)
{
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

		if (p[i].turnaround_time > 9)
			printf("\b");

		printf("%d", p[i].return_time);
	}

	printf("\n");
}


void SJF(Process *p, int len){

    int i, j, aux, cont=1;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int total_response_time = 0;
	Process auxi;

    process_init(p, len);
	merge_sort_by_arrive_time(p, 0, len);

	cont=0;
	while(p[cont].arrive_time < (p[0].burst + p[0].arrive_time)){
		cont++;
	}

    for(i=1 ; i<cont ; i++){
		aux = i;
		for(j=i ; j<cont ; j++){
			if(p[aux].burst > p[j].burst){
				aux = j;
			}
		}
		auxi = p[aux];
		p[aux] = p[i];
		p[i] = auxi;
	}

	for(i=cont ; i<len ; i++){
		aux = i;
		for(j=i ; j<len ; j++){
			if(p[aux].burst > p[j].burst){
				aux = j;
			}
		}
		auxi = p[aux];
		p[aux] = p[i];
		p[i] = auxi;
	}
	sjf_calculate_time(p, len);

    for (i = 0; i < len; i++)
    {
		p[i].response_time = p[i].waiting_time;
        p[i].turnaround_time = p[i].return_time - p[i].arrive_time;
        total_waiting_time += p[i].waiting_time;
        total_turnaround_time += p[i].turnaround_time;
        total_response_time += p[i].response_time;
    }

    printf("\tSJF\n\n");

    sjf_print_gantt_chart(p, len);
    printf("\n\tAverage Waiting Time     : %-2.2lf\n", (double)total_waiting_time / (double)len);
    printf("\tAverage Turnaround Time  : %-2.2lf\n", (double)total_turnaround_time / (double)len);
    printf("\tAverage Response Time    : %-2.2lf\n\n", (double)total_response_time / (double)len);

    print_table(p, len);

}

#endif
