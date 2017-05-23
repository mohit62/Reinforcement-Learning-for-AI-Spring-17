//
//  DynaQWithPrioritySweepingAgent.cpp
//  
//
//  Created by Mohit Saraf on 4/5/17.
//
//

#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>     /* abs */
#include <map>
#include "rlglue/Agent_common.h" /* Required for RL-Glue */
//#include "rlglue/TaskSpec_Parser.h" /* helpful functions parsing task spec string */
#include <gsl/gsl_matrix.h>
#include <math.h>
#include "iostream"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

/*** IMPORTANT:
 * Any private variables to the environment must be declared static
 * Otherwise they are default public global variables that can be
 * accessed in other files
 ***/

static gsl_vector* local_action;
static action_t* this_action;
static gsl_vector* last_observation;
//static int Model[6][9][4][3];
static double Q[6][9][4];
//static double P;
//static int planningsteps;
static double discount;
static std::map<double,int> State_X;
static std::map<double,int> State_Y;
static std::map<double,int> Action;
static int Noofaction=4;
//static double theta;
static double alpha;
static int currentstateY;
static int currentstateX;
static double epsilon;

void agent_init()
{
    //std::cout<<"AgentInt\n";
    discount = 0.95;
   // planningsteps=5;
   // P=0;
    epsilon=0.1;
    //theta=0.07;
    alpha=0.5;//α ∈ {0.025,0.05,0.1,0.2,0.4, 0.5, 0.8}
   /* for(int i=0;i<6;i++)
    {
        for(int j=0;j<9;j++)
        {
            
            for(int k=0;k<4;k++)
            {
                
                for(int l=0;l<3;l++)
                {
                    
                    Model[i][j][k][l]=0;
                    
                }
                
                
            }
            
            
        }
    
    }*/

    for(int i=0;i<6;i++)
    {
        
        for(int j=0;j<9;j++)
        {
            
            for(int k=4;k<3;k++)
            {
                
                Q[i][j][k]=0;
                
            }
            
        }

        
    }
    local_action = gsl_vector_calloc(1);
    this_action = local_action;
    last_observation = gsl_vector_calloc(2);
}

const action_t *agent_start(const observation_t *this_observation) {
    //get s0 from environment, select a0 according to initial policy
    int State_X_current= (int)gsl_vector_get(this_observation,0);
    int State_Y_current= (int)gsl_vector_get(this_observation,1);
    int optimalAct[4]={0,0,0,0};
    int suboptimalAct[4]={0,0,0,0};
    int optcount=0;
    int subcount=0;
    double max=Q[State_X_current][State_Y_current][0];
    for(int i=1;i<Noofaction;i++)
    {
    if (max<Q[State_X_current][State_Y_current][i])
    {
        max=Q[State_X_current][State_Y_current][i];
    }
    
    }
    for(int i=0;i<Noofaction;i++)
    {
    if(Q[State_X_current][State_Y_current][i]>=max)
    {
        optimalAct[optcount++]=i;
    }
    else
    {
        suboptimalAct[subcount++]=i;
    }
    }
    int optindex=randInRange(optcount);
    int optAct=optimalAct[(optindex<Noofaction)?optindex:Noofaction-1];
    int suboptindex=randInRange(subcount);
    int suboptAct=suboptimalAct[(suboptindex<Noofaction)?suboptindex:Noofaction-1];
    double l=rand_un();
    int atp1=randInRange(Noofaction);
    atp1=(atp1<Noofaction)?atp1:Noofaction-1;
    if(l>=epsilon || subcount==0)
    {
        atp1=optAct;
    }
    else{
        atp1=suboptAct;
    }
    
    gsl_vector_set(local_action,0,atp1);
    gsl_vector_memcpy(last_observation,this_observation);
    //std::cout<<"agentstart";

    return this_action;
}

const action_t *agent_step(double reward, const observation_t *this_observation) {
    int atp1;
    
    //get state from environment
    int State_X_Current= (int)gsl_vector_get(this_observation,0);
    int State_Y_Current= (int)gsl_vector_get(this_observation,1);
    
    //choose action according to current policy
    int optimalAct[4]={0,0,0,0};
    int suboptimalAct[4]={0,0,0,0};
    int optcount=0;
    int subcount=0;
    double max_Q=Q[State_X_Current][State_Y_Current][0];
    for(int i=1;i<Noofaction;i++)
    {
        if (max_Q<Q[State_X_Current][State_Y_Current][i])
        {
            max_Q=Q[State_X_Current][State_Y_Current][i];
        }
        
    }
    for(int i=0;i<Noofaction;i++)
    {
        if(Q[State_X_Current][State_Y_Current][i]>=max_Q)
        {
            optimalAct[optcount++]=i;
        }
        else
        {
            suboptimalAct[subcount++]=i;
        }
    }
    int optindex=randInRange(optcount);
    int optAct=optimalAct[(optindex<Noofaction)?optindex:Noofaction-1];
    int suboptindex=randInRange(subcount);
    int suboptAct=suboptimalAct[(suboptindex<Noofaction)?suboptindex:Noofaction-1];
    double l=rand_un();
    atp1=randInRange(Noofaction);
    atp1=(atp1<Noofaction)?atp1:Noofaction-1;
    if(l>=epsilon || subcount==0)
    {
        atp1=optAct;
    }
    else{
        atp1=suboptAct;
    }
    int prev_Action=(int)gsl_vector_get(local_action,0);
    int State_X_Prev= (int)gsl_vector_get(last_observation,0);
    int State_Y_Prev= (int)gsl_vector_get(last_observation,1);
    
        max_Q=Q[State_X_Current][State_Y_Current][0];
    for(int i=1;i<Noofaction;i++)
    {
        if (max_Q<Q[State_X_Current][State_Y_Current][i])
        {
                max_Q=Q[State_X_Current][State_Y_Current][i];
        }
            
    }
    
        Q[State_X_Prev][State_Y_Prev][prev_Action]+=alpha*(reward+discount*max_Q-Q[State_X_Prev][State_Y_Prev][prev_Action]);
    
        
   
    gsl_vector_set(local_action,0,atp1);
    gsl_vector_memcpy(last_observation,this_observation);
    //std::cout<<"agentstep";
    //return action choice
    return this_action;
}

void agent_end(double reward) {
    //Final update at end of episode with State action value of terminal state 0
    
    int prev_Action=(int)gsl_vector_get(local_action,0);
    int StateX_Prev= (int)gsl_vector_get(last_observation,0);
    int StateY_Prev= (int)gsl_vector_get(last_observation,1);
    
    
        Q[StateX_Prev][StateY_Prev][prev_Action]+=alpha*(reward-Q[StateX_Prev][StateY_Prev][prev_Action]);
        
           //std::cout<<"agentend";
}


void agent_cleanup() {
    /* clean up mememory */
    gsl_vector_free(local_action);
    gsl_vector_free(last_observation);
    
}

const char* agent_message(const char* inMessage) {
    /* might be useful to get information from the agent */
    return "path setted...";
}
