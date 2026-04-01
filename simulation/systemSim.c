#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define CHECK_INTERVAL 600

/* 

I will have to mimic 10 sensor readings 
calculate the avarage of these 10 readings 
compare it to a set threshold 
if the comparison is equal to dry each and every time for 5 counts 
comfirm DRY and then send an update 


// it needs to produce 10 random readings 
// get collected into an array [10] readings
// then these readings need to be avaraged   
// then the avaraged value is compared to the set threshold
// if its under the threshold 
// 1 tick of DRY will be incremented... it could be done in many ways other than incrementaion
// if the DRY tick reaches 5 thats when DRY gets flaged and comfirmed as a state and gets updated 

*/

// Presentaion Layer
void readingMsg (int *readings, int count)
{
    for (int i = 0; i < count; ++i)
    {
        printf("Reading %d: %d\n", i+1, readings[i]);
    }
    
}

void sumMsg (int *sum)
{
    printf("Sum of readings: %d \n", *sum); 
}

void averageMsg(int *average)
{
    printf("Avarage of readings: %d\n", *average);
}

void dryStatusMsg(int dryCount)
{
    if (dryCount >= 5)
    {
        printf("SOIL CONDITON: DRY\n");
    }
}

void wetStatusMsg(int wetCount)
{
    if (wetCount >= 5)
    {
        printf("SOIL CONDITON: WET\n");
    }
    
}

// System Logic 
void sampleReadings(int *readings, int count)
{
    for (int i = 0; i < count; ++i)
    {
        readings[i] = rand() % 65536; // to simulate the 16 bit values on micropython
    }
}


void sumOfSamples(int *sum ,int *readings, int count)
{
    for (int i = 0; i < count; ++i)
    {
        *sum += readings[i];
    }
}

void averageOfSamples(int *average, int sum, int count)
{
    *average = sum/count;  
}

void checkDryness(int average, int threshold, int *dryCount, int *wetCount)
{
    if(*dryCount >= 5) return; 
    if (average < threshold)  
        {
            (*dryCount)++;
            if(*dryCount >= 5) 
            {
                dryStatusMsg(*dryCount); 
                *wetCount = 0; 
            }
        } 
}

void checkWetness (int average, int threshold, int *wetCount, int *dryCount)
{
    if(*wetCount >= 5) return;
    if (average > threshold)
    {
        (*wetCount)++;
        if(*wetCount >= 5)
        {
            wetStatusMsg(*wetCount);
            *dryCount = 0;
        }
    }
    else 
    {
        *wetCount = 0; 
    }
}

int main() 
{
    srand(time(NULL));
    // States 
    int readings[10];
    int count = 10;
    int average = 0; 
    int sum = 0; 
    int dryCount = 0; 
    int wetCount = 5; 

    int threshold = 16500; // randomly set threshold value



    // if i wanted user set threshold but its a sim so no need 
    //printf("Enter a soil dryness threshold (0-1023):");
    //scanf("%d", &threshold);

    //control layer 
    while (1)
    {
        // technically i wont have the sample reading being shown... only wet and dry states
        sampleReadings(readings, count);
        // Testing: Worked readingMsg(readings, count);
        sumOfSamples(&sum,readings,count);
        // Testing: Worked sumMsg(&sum);
        averageOfSamples(&average, sum, count);
        averageMsg(&average);
        checkDryness(average, threshold, &dryCount, &wetCount);
        checkWetness(average, threshold, &wetCount, &dryCount); 

        sum = 0; 
        average =0;
        //sleep(CHECK_INTERVAL); for 10 min
        sleep(1); 
    }
    return 0;
}