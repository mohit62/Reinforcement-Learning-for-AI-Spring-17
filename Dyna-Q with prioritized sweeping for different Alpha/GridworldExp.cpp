
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "rlglue/RL_glue.h"
#include "iostream"
#include "ParameterSweep.h"
#define	N_EPISODES_PER_RUN	50
#define	N_RUNS			30
#define	N_STEPS_PER_EPISODE	2000

void saveResults(double* data, int dataSize, const char* filename);


int main(int argc, char *argv[]) {
    
    srand(time(NULL));
    int alpha_ind;
    double alphaedge[alphaValuesSize];
    double averagesteps[alphaValuesSize];
    
    for(int i = 0; i < alphaValuesSize; i++)
    {
        averagesteps[i]=0.0;
        alphaedge[i]=0.0;
    }
    for(alpha_ind = 0; alpha_ind < alphaValuesSize; alpha_ind++)
    {
        //Get the alpha value.
        printf("Experiment Alpha Value: %f\n", alphaValues[alpha_ind]);
        alphaedge[alpha_ind]=alphaValues[alpha_ind];
        //Make a string
        char agent_message[10];
        //Print to string what you want to send. In this case the index coresponding to the alpha value desired.
        sprintf(agent_message, "%d", alpha_ind);
        //Send message.
        RL_agent_message(agent_message);
    
    for(int i = 0; i < N_RUNS; i++){
        RL_init();
        
        for(int j = 0; j < N_EPISODES_PER_RUN; j++){
            RL_episode(N_STEPS_PER_EPISODE);
            //printf("Episode Over\n");
            //std::cout<<RL_num_steps()<<"\n";
            averagesteps[alpha_ind] += RL_num_steps();
        }
        
        printf(".");
    }
        
    printf("\nDone\n");
    }
    for(int i = 0; i < alphaValuesSize; i++){
        averagesteps[i] = averagesteps[i]/(double)N_RUNS;
    }
    /*for(int i = 0; i < N_EPISODES_PER_RUN; i++){
        std::cout<<"Episode\t"<<i<<n_steps_to_goal[i]<<"\n";
    }*/

    /* Save data to a file */

    saveResults(averagesteps, alphaValuesSize, "DYNAQ.dat");
    saveResults(alphaedge, alphaValuesSize, "alpha.dat");
    return 0;
}


void saveResults(double* data, int dataSize, const char* filename) {
    FILE *dataFile;
    int i;
    dataFile = fopen(filename, "w");
    for(i = 0; i < dataSize; i++){
        fprintf(dataFile, "%lf\n", data[i]);
    }
    fclose(dataFile);
}
