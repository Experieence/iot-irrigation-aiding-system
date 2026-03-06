#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define CHECK_INTERVAL 600

// Presentaion Layer
void dryMsg(int voltage)
{
    printf("ADC: %d ALERT:The soil is dry!\n",voltage);
}

void wetMsg(int voltage)
{
    printf("ADC: %d The soil has moisture!\n",voltage);
}

// System Logic 
void simulation(int *voltage, int *isDry, int threshold)
{
    for (int i = 0; i < 10; ++i)
    {
        *voltage = rand() % 1024;
        if(*voltage < threshold)
        {
            *isDry = 1;
            dryMsg(*voltage); 
        }
        else
        {
            *isDry = 0; 
            wetMsg(*voltage);
        }
    }
}


int main() 
{
    srand(time(NULL));
    // States 
    int voltage = 0;
    int isDry = 0; 
    int threshold = 600; 
    
    
    //control layer 
    simulation(&voltage, &isDry, threshold);
    //sleep(CHECK_INTERVAL); for 10 min
    sleep(1); 
    return 0;
}