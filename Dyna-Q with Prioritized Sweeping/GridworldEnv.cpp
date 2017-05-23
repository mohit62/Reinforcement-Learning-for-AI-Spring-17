//
//  GridworldEnv.cpp
//  
//
//  Created by Mohit Saraf on 4/5/17.
//
//


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rlglue/Environment_common.h" /* Required for RL-Glue interface*/
#include "utils.h"
#include <algorithm>    // std::min
#include <cmath>
#include "iostream"

/*** IMPORTANT:
 * Any private variables to the environment must be declared static
 * Otherwise they are default public global variables that can be
 * accessed in other files
 ***/

static gsl_vector* local_observation;
static reward_observation_terminal_t this_reward_observation;



void env_init()
{
    //std::cout<<"Envinit\n";
    local_observation = gsl_vector_calloc(2);
    
    this_reward_observation.observation=local_observation;
    this_reward_observation.reward=0;
    this_reward_observation.terminal=0;

    return;
}

const observation_t* env_start()
{
    //std::cout<<"EnvStart\n";

    //start with  state (2,0)
    int state_x1 = 2;
    int state_y1 = 0;

    gsl_vector_set(local_observation,0,state_x1);
    gsl_vector_set(local_observation,1,state_y1);

    return this_reward_observation.observation;
}

const reward_observation_terminal_t *env_step(const action_t *this_action)
{
    //get previous state from local observation
    int state_x1 = (int)gsl_vector_get(local_observation,0);
    int state_y1 = (int)gsl_vector_get(local_observation,1);
    int save_state_x1=state_x1;
    int save_state_y1=state_y1;
    
    int episode_over = 0;
    //get action from agent
    int atp1 = gsl_vector_get(this_action,0);
    
    double the_reward = 0;
    if(atp1==0)//Up
    {
    state_x1=std::fmax(state_x1-1,0);
    }
    else if(atp1==1)//down
    {
    state_x1=std::fmin(state_x1+1,5);
    }
    else if(atp1==2)//left
    {
        state_y1=std::fmax(state_y1-1,0);
    }
    else//right
    {
        state_y1=std::fmin(state_y1+1,8);
    }
    
    if ((state_x1==1 && state_y1==2)||(state_x1==2 && state_y1==2)||(state_x1==3 && state_y1==2)||(state_x1==4 && state_y1==5)||(state_x1==0 && state_y1==7)||(state_x1==1 && state_y1==7)||(state_x1==2 && state_y1==7))//If obstacle detected no change in state
        
    {
        state_x1=save_state_x1;
        state_y1=save_state_y1;
    }
    if (state_x1==0 && state_y1==8) {
        the_reward = 1;
        episode_over = 1;   //GoalState reached, terminate episode
    }
    //store new state to local_oberservation
    gsl_vector_set(local_observation, 0,state_x1);
    gsl_vector_set(local_observation, 1,state_y1);

    
    //return new state and reward
    this_reward_observation.reward = the_reward;
    this_reward_observation.terminal = episode_over;
    //std::cout<<"EnvStep\n";
    return &this_reward_observation;
}

void env_cleanup()
{
    gsl_vector_free(local_observation);
}

const char* env_message(const char* inMessage) {
    if(strcmp(inMessage,"what is your name?")==0)
        return "my name is skeleton_environment!";
    
    /* else */
    return "I don't know how to respond to your message";
}

