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

void dryStatusMsg(int isDry)
{
    if (isDry >= 5)
    {
        printf("SOIL CONDITON: DRY\n");
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

void averageOfSamples(int *average, int sum)
{
    *average = sum/10;  
}

void checkDryness(int average, int threshold, int *isDry)
{
    if (average < threshold)
        {
            (*isDry)++;
            if (*isDry >= 5)
            {
                dryStatusMsg(*isDry);
                *isDry = 0; 
            }
        }
        //else
        //{
            //*isDry = 0; //calibaration since using random values right now     
        //}
}




int main() 
{
    srand(time(NULL));
    // States 
    int readings[10];
    int count = 10;
    int average = 0; 
    int sum = 0; 
    int isDry = 0; 

    int threshold = 16500; 



    //printf("Enter a soil dryness threshold (0-1023):");
    //scanf("%d", &threshold);

    //control layer 
    while (1)
    {
        sampleReadings(readings, count);
        // Testing: Worked readingMsg(readings, count);
        sumOfSamples(&sum,readings,count);
        // Testing: Worked sumMsg(&sum);
        averageOfSamples(&average, sum);
        averageMsg(&average);
        checkDryness(average, threshold, &isDry); 

        sum = 0; 
        average =0;
        //sleep(CHECK_INTERVAL); for 10 min
        sleep(1); 
    }
    return 0;
}