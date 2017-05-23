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
#include "ParameterSweep.h"

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
static int Model[6][9][4][3];
static double Q[6][9][4];
static double P;
static int planningsteps;
static double discount;
static std::map<double,int> State_X;
static std::map<double,int> State_Y;
static std::map<double,int> Action;
static int Noofaction=4;
static double theta;
static double alpha=0;
static int currentstateY;
static int currentstateX;
static double epsilon;

void agent_init()
{
    //std::cout<<"initialize\n"<<alpha;
    discount = 0.95;
    planningsteps=5;
    P=0;
    epsilon=0.1;
    theta=0.07;
    
    //alpha=0.8;//α ∈ {0.025,0.05,0.1,0.2,0.4, 0.5, 0.8}
    for(int i=0;i<6;i++)
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
    
    }

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
    
    Model[State_X_Prev][State_Y_Prev][prev_Action][0]=State_X_Current;
    Model[State_X_Prev][State_Y_Prev][prev_Action][1]=State_Y_Current;
    Model[State_X_Prev][State_Y_Prev][prev_Action][2]=reward;
    P=fabs(reward+discount*max_Q-Q[State_X_Prev][State_Y_Prev][prev_Action]);
    if (P>theta)
    {
    State_X.insert ( std::pair<double,int>(P,State_X_Prev) );
    State_Y.insert ( std::pair<double,int>(P,State_Y_Prev) );
    Action.insert ( std::pair<double,int>(P,prev_Action) );
    }
    int n_planningsteps=0;
    while(n_planningsteps<planningsteps && !State_X.empty() && !State_Y.empty() && !Action.empty())
    {
        double largest_key_1 = State_X.rbegin()->first;
        State_X_Prev=State_X.at(largest_key_1);
        State_X.erase (largest_key_1);
        double largest_key_2 = State_Y.rbegin()->first;
        State_Y_Prev=State_Y.at(largest_key_2);
        State_Y.erase (largest_key_2);
        double largest_key_3 = Action.rbegin()->first;
        prev_Action=Action.at(largest_key_3);
        Action.erase (largest_key_3);
    State_X_Current=Model[State_X_Prev][State_Y_Prev][prev_Action][0];
    State_Y_Current=Model[State_X_Prev][State_Y_Prev][prev_Action][1];
    reward=Model[State_X_Prev][State_Y_Prev][prev_Action][2];
    max_Q=Q[State_X_Current][State_Y_Current][0];
    for(int i=1;i<Noofaction;i++)
    {
        if (max_Q<Q[State_X_Current][State_Y_Current][i])
        {
                max_Q=Q[State_X_Current][State_Y_Current][i];
        }
            
    }
    
        Q[State_X_Prev][State_Y_Prev][prev_Action]+=alpha*(reward+discount*max_Q-Q[State_X_Prev][State_Y_Prev][prev_Action]);
    
        
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<9;j++)
        {
            
            for(int k=0;k<4;k++)
            {
                
                
                
                    
                   if (Model[i][j][k][0]==State_X_Prev && Model[i][j][k][1]==State_Y_Prev)
                   {
                       int State_X_bar=i;
                       int State_Y_bar=j;
                       int Action_bar=k;
                       double Reward_bar=Model[State_X_bar][State_Y_bar][Action_bar][2];
                       max_Q=Q[State_X_Prev][State_Y_Prev][0];
                       for(int i=1;i<Noofaction;i++)
                       {
                           if (max_Q<Q[State_X_Prev][State_Y_Prev][i])
                           {
                               max_Q=Q[State_X_Prev][State_Y_Prev][i];
                           }
                           
                       }
                       P=fabs(Reward_bar+discount*max_Q-Q[State_X_bar][State_Y_bar][Action_bar]);
                       if (P>theta)
                       {
                           State_X.insert ( std::pair<double,int>(P,State_X_bar) );
                           State_Y.insert ( std::pair<double,int>(P,State_Y_bar) );
                           Action.insert ( std::pair<double,int>(P,Action_bar) );
                       }
                   }
            }
            
        }
        
    }
        n_planningsteps+=1;
    }
    currentstateX=State_X_Current;
    currentstateY=State_Y_Current;
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
    
    Model[StateX_Prev][StateY_Prev][prev_Action][0]=0;
    Model[StateX_Prev][StateY_Prev][prev_Action][1]=8;
    Model[StateX_Prev][StateY_Prev][prev_Action][2]=reward;
    P=fabs(reward-Q[StateX_Prev][StateY_Prev][prev_Action]);
    if (P>theta)
    {
        State_X.insert ( std::pair<double,int>(P,StateX_Prev) );
        State_Y.insert ( std::pair<double,int>(P,StateY_Prev) );
        Action.insert ( std::pair<double,int>(P,prev_Action) );
    }
    int n_planningsteps=0;
    while(n_planningsteps<planningsteps && !State_X.empty() && !State_Y.empty() && !Action.empty())
    {
        double largest_key_1 = State_X.rbegin()->first;
        StateX_Prev=State_X.at(largest_key_1);
        State_X.erase (largest_key_1);
        double largest_key_2 = State_Y.rbegin()->first;
        StateY_Prev=State_Y.at(largest_key_2);
        State_Y.erase (largest_key_2);
        double largest_key_3 = Action.rbegin()->first;
        prev_Action=Action.at(largest_key_3);
        Action.erase (largest_key_3);
        //State_X_Current=Model[StateX_Prev][StateY_Prev][prev_Action][0];
        //State_Y_Current=Model[StateX_Prev][StateY_Prev][prev_Action][1];
        reward=Model[StateX_Prev][StateY_Prev][prev_Action][2];
        
        Q[StateX_Prev][StateY_Prev][prev_Action]+=alpha*(reward-Q[StateX_Prev][StateY_Prev][prev_Action]);
        
        
        for(int i=0;i<6;i++)
        {
            for(int j=0;j<9;j++)
            {
                
                for(int k=0;k<4;k++)
                {
                    
                    
                    if (Model[i][j][k][0]==StateX_Prev && Model[i][j][k][1]==StateY_Prev)
                    {
                        int State_X_bar=i;
                        int State_Y_bar=j;
                        int Action_bar=k;
                        double Reward_bar=Model[State_X_bar][State_Y_bar][Action_bar][2];
                       double max_Q=Q[StateX_Prev][StateY_Prev][0];
                        for(int i=1;i<Noofaction;i++)
                        {
                            if (max_Q<Q[StateX_Prev][StateY_Prev][i])
                            {
                                max_Q=Q[StateX_Prev][StateY_Prev][i];
                            }
                            
                        }
                        P=fabs(Reward_bar+discount*max_Q-Q[State_X_bar][State_Y_bar][Action_bar]);
                        if (P>theta)
                        {
                            State_X.insert ( std::pair<double,int>(P,State_X_bar) );
                            State_Y.insert ( std::pair<double,int>(P,State_Y_bar) );
                            Action.insert ( std::pair<double,int>(P,Action_bar) );
                        }
                    }
                }
                
            }
            
        }
        n_planningsteps+=1;
    }
    //std::cout<<"agentend";
}


void agent_cleanup() {
    /* clean up mememory */
    gsl_vector_free(local_action);
    gsl_vector_free(last_observation);
    
}

const char* agent_message(const char* inMessage) {
    /* might be useful to get information from the agent */
    
    //Read message and parse from Experiment.
    int recievedint = atoi(inMessage);
    
    //The recieved integer coresponds to the alpha vector
    printf("Agent Recieved Alpha Index: %d, With alpha Value %f\n", recievedint, alphaValues[recievedint]);
    //Set values.
    alpha = alphaValues[recievedint];
    
    return "I responded to your message by setting my alpha value.";
}
