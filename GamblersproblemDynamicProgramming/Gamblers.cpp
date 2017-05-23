#include <iostream>
#include <limits>
#include <cmath>

void saveResults(double* data, int dataSize, const char* filename);


using namespace std;

int main()
{
    const int noOfStates=101;//States from 0 to 100 inclusive
    int gamma=1;
    double theta=1e-8;
    double prh;
    int bet;
    cout<<"Enter probability of head"<<endl;
    cin>>prh;//probability of head
    cout<<"Enter starting bet"<<endl;
    cin>>bet;//enter starting bet
    double V[noOfStates]={0};// initializing all state values to zero
    int R[noOfStates]={0};//initializing rewards at all states to zero
    R[noOfStates-1]=1;//initializing reward at 100 state to 1
    double Pi[noOfStates]={0};//initializing policy at all states  to zero
    double delta;
    do
    {
        delta=0;
        for(int state=1;state<noOfStates-1;state++)//traverse through all non terminal states
        {
           double v=V[state];
            double max=-1e-8;
            for (int action=bet;action<std::min(state,noOfStates-state-1)+1;action++)// select actions based on current state starting initial bet from 0 or 1 depending on imput
            {
                int winhead=state+action;//capital increases on win incase of head
                int losetail=state-action;//capital decrease on loss incase of tails
                double sigma=prh * (R[winhead] + gamma * V[winhead])+(1 - prh) * (R[losetail] + gamma * V[losetail]);//calculate state value
                if (sigma>max)
                {
                    V[state]=sigma;//optimal state value
                    Pi[state]=action;//optimal policy state
                    max=sigma;
                }
                
            }
            delta=std::max(delta,abs(v-V[state]));
            
        }
       
        
    }
     while (delta>=theta);
    V[noOfStates-1]=1;//dummy value
    saveResults(V, noOfStates, "StateValues.dat");
    saveResults(Pi, noOfStates, "PolicyValue.dat");


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



