/*
Brenden Blake
CS 450
11/05/2021
Lab 7

Implementing and analyzing FCFS, SSTF, and SCAN disk scheduling algorithms
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILEPATH 64
#define NUMBER_OF_REQUESTS 10


// function to print the menu
void menu()
{
    printf("Please choose one of the following options:\n");
    printf("1. Number of Cylinders\n");
    printf("2. Generate Random Requests File\n");
    printf("3. Initial Position of Disk Head\n");
    printf("4. Apply FCFS\n");
    printf("5. Apply SSTF\n");
    printf("6. Apply SCAN\n");
    printf("7. Report the best algorithm\n");
    printf("8. Exit\n");

}
// function to get the total number of cylinders
int getCylinders()
{
    int cylinders;
    do
    {
        printf("Please enter the number of cylinders: ");
        scanf(" %d", &cylinders);

        if(cylinders < 0)
        {
            printf("Please enter a positive number.\n");
        }
    }while(cylinders < 0);

    return cylinders;
}
// function to generate a file that contains the requests
void generateRequestFile(int max)
{
    srand(time(NULL));
    char filePath[MAX_FILEPATH];
    printf("Please enter the name of the file you wish to write to:\n");
    scanf(" %s", filePath);

    FILE* file = fopen(filePath, "w+");

    int request;
    for(int i = 0; i < NUMBER_OF_REQUESTS; i++)
    {
        request = rand() % (max-1);
        fprintf(file, "%d\n", request);
    }

    fclose(file);
}
// function to get the initial position of the disk head
int getInitialPosition(int total) // total is the maximum number of cylinders
{
    int position;
    // check to see if the total number of cylinders has been set
    if(total == 0)
    {
        printf("\nThe number of cylinders is not set.\n");
        printf("Please enter the number of cylinders from the menu first.\n\n");
        return -1;
    }
    else
    {
        // get the intial head position, ensure that it is between 0 and the specified total number of cylinders
         do
        {
            printf("Please enter the initial position of the disk head.\n");
            printf("Entered value should be between 0 and %d: ", total-1);
            scanf(" %d", &position);

        } while (position < 0 || position > total-1);

        return position;
    }
    
}
// selection sort function, used in SCAN algorithm
void selectionSort(int queue[], int n)
{
  int min_idx;
  int temp;

  for(int i = 0; i < n-1; i++)
  {
    min_idx = i;
    for(int j = i+1; j < n; j++)
    {
      if(queue[j] < queue[min_idx])
      {
        min_idx = j;
      }

    }

    temp = queue[min_idx];
    queue[min_idx] = queue[i];
    queue[i] = temp;

  }
}
// function to perform the first come first server disk scheduling algorithm
int FCFS(int headPosition)
{
    int queue[NUMBER_OF_REQUESTS]; // the queue that holds the request numbers from the file
    int nextPosition; // the current position of the disk head, starts at the initial position provided
    int distanceTraveled = 0;
    // ask for the file path to the requests file, and verify that it exists
    char filePath[MAX_FILEPATH];
    FILE* file;
    do
    {
        printf("\nPlease enter the location of the file containing the requests.\n");
        scanf(" %s", filePath);

        file = fopen(filePath, "r");

        if(file == NULL)
        {
            printf("\nThis file does not exist.\n\n");
        }
    } while (file == NULL);
    
    // copy the requests from the file, into the queue
    for(int i = 0; i < NUMBER_OF_REQUESTS; i++)
    {
        fscanf(file, "%d", &queue[i]);
    }
    
    printf("\nORDER OF SERVICE:\n\n");
    printf("%d ->", headPosition);
    // apply the FCFS algorithm
    for(int j = 0; j < NUMBER_OF_REQUESTS; j++)
    {
        nextPosition = queue[j];
        if(j == NUMBER_OF_REQUESTS-1)
        {
            printf(" %d\n\n", nextPosition);
        }
        else
        {
            printf(" %d ->", nextPosition);
        }
        // calculate the distance traveled
        distanceTraveled += abs(nextPosition - headPosition);
        // the new head position is the next position in the queue
        headPosition = nextPosition;
    }

    return distanceTraveled;
}
// function to perform the shortest scan time first disk scheduling algorithm
int SSTF(int headPosition)
{
    int queue[NUMBER_OF_REQUESTS];
    int sequence[NUMBER_OF_REQUESTS]; // array to keep track of the sequence of completed requests
    char filePath[MAX_FILEPATH];
    FILE* file;
    int currentHeadPosition = headPosition;
    int currentDistance = 0;
    int distanceTraveled = 0;
    int completed = 0;
    // get the file containing the requests
    do
    {
        printf("\nPlease enter the location of the file containing the requests.\n");
        scanf(" %s", filePath);

        file = fopen(filePath, "r");

        if(file == NULL)
        {
            printf("\nThis file does not exist.\n\n");
        }
    } while (file == NULL);
    
    // copy the requests from the file, into the queue
    for(int i = 0; i < NUMBER_OF_REQUESTS; i++)
    {
        fscanf(file, "%d", &queue[i]);
    }

    // apply the SSTF algorithm
    // while there are still requests to be serviced
    while(completed != NUMBER_OF_REQUESTS)
    {
        int minimum = 99999;
        int idx = -1;
        // find the requests the closest to the current head position
        for(int i = 0; i < NUMBER_OF_REQUESTS; i++)
        {
            currentDistance = abs(queue[i] - currentHeadPosition);
            if(minimum > currentDistance)
            {
                minimum = currentDistance;
                idx = i;
            }
        }
        // service the closest request
        distanceTraveled += minimum;
        sequence[completed] = queue[idx];
        currentHeadPosition = queue[idx];
        queue[idx] = 99999;
        completed++;
    }

    // print the completed sequence
    printf("\nORDER OF SERVICE:\n\n");
    printf("%d -> ", headPosition);
    for(int i = 0; i < NUMBER_OF_REQUESTS; i++)
    {
        if(i == NUMBER_OF_REQUESTS-1)
        {
            printf(" %d\n\n", sequence[i]);
        }
        else
        {
            printf(" %d ->", sequence[i]);
        }
    }

    return distanceTraveled;
}
// function to perform the SCAN disk scheduling algorithm
int SCAN(int headPosition)
{
    int queue[NUMBER_OF_REQUESTS];
    char filePath[MAX_FILEPATH];
    int sequence[NUMBER_OF_REQUESTS];
    int currentHeadPosition = headPosition;
    FILE* file;
    int completed = 0;
    int distanceTraveled = 0;
    // will be used to find the start position
    int minimum = 99999;
    int distance = 0;
    int startPosition = 0;

    // get the file that contains the requests
    do
    {
        printf("\nPlease enter the location of the file containing the requests.\n");
        scanf(" %s", filePath);

        file = fopen(filePath, "r");

        if(file == NULL)
        {
            printf("\nThis file does not exist.\n\n");
        }
    } while (file == NULL);
    
    // copy the requests from the file, into the queue
    for(int i = 0; i < NUMBER_OF_REQUESTS; i++)
    {
        fscanf(file, "%d", &queue[i]);
    }
    // the queue must be sorted for SCAN
    selectionSort(queue, NUMBER_OF_REQUESTS);
    // find the start position
    for(int i = 0; i < NUMBER_OF_REQUESTS; i++)
    {
        distance = abs(queue[i] - currentHeadPosition);
        if(minimum > distance && queue[i] > currentHeadPosition)
        {
            minimum = distance;
            startPosition = i;
        }
    }
    // apply the SCAN algorithm
    while(completed != NUMBER_OF_REQUESTS)
    {
        int i;
        // begin servicing requests in ascending order
        // start at the closest value that is greater than the current head position
        for(i = startPosition; i < NUMBER_OF_REQUESTS; i++)
        {
            distanceTraveled += abs(queue[i] - currentHeadPosition);
            currentHeadPosition = queue[i];
            sequence[completed] = queue[i];
            completed++;

            if(completed == NUMBER_OF_REQUESTS)
            {
                break;
            }
        }
        // if i is equal to the total number of requests, then restart at the beginning
        // this only matters if all the requests have not been serviced
        if(i == NUMBER_OF_REQUESTS)
        {
            startPosition = 0;
        }
    }

    // print the completed sequence
    printf("\nORDER OF SERVICE:\n\n");
    printf("%d -> ", headPosition);
    for(int i = 0; i < NUMBER_OF_REQUESTS; i++)
    {
        if(i == NUMBER_OF_REQUESTS-1)
        {
            printf(" %d\n\n", sequence[i]);
        }
        else
        {
            printf(" %d ->", sequence[i]);
        }
    }

    return distanceTraveled;

}
// function to choose the best performing algorithm
void chooseBest(int traveledFCFS, int traveledSSTF, int traveledSCAN)
{
    printf("\nFCFS: %d    SSTF: %d    SCAN: %d\n", traveledFCFS, traveledSSTF, traveledSCAN);
    if(traveledFCFS == 0 || traveledSSTF == 0 || traveledSCAN == 0)
    {
        printf("\nError. One of the disk scheduling algorithms has not been run.\n");
        printf("Please run this algorithm before running this analysis.\n");
        printf("Refer to the above output of the travel distances to see which one has not been run.\n");
        printf("The values should be postive integers.\n\n");
        return;
    }

    if(traveledFCFS < traveledSSTF && traveledFCFS < traveledSCAN)
    {
        printf("\nFCFS performed the best given these requests.\n\n");
    }
    else if(traveledSSTF < traveledFCFS && traveledSSTF < traveledSCAN)
    {
        printf("\nSSTF performed the best given these requests.\n\n");
    }
    else if(traveledSCAN < traveledFCFS && traveledSCAN < traveledSSTF)
    {
        printf("\nSCAN performed the best given these requests.\n\n");
    }
    else if(traveledFCFS == traveledSSTF && traveledFCFS < traveledSCAN)
    {
        printf("\nFCFS and SSTF performed the best given these requests.\n\n");
    }
    else if(traveledFCFS == traveledSCAN && traveledFCFS < traveledSSTF)
    {
        printf("\nFCFS and SCAN performed the best given these requests.\n\n");
    }
    else if(traveledSSTF == traveledSCAN && traveledSSTF < traveledFCFS)
    {
        printf("\nSSTF and SCAN performed the best given these requests.\n\n");
    }
    else
    {
        printf("\nError. Missed case.\n\n");
    }
}

int main()
{
    int choice = 0;
    int cylinders = 0;
    int initialPosition = 0;
    int totalTraveledFCFS = 0;
    int totalTraveledSSTF = 0;
    int totalTraveledSCAN = 0;
    while(1)
    {
        do
        {
            menu();
            scanf(" %d", &choice);
        } while (choice < 1 || choice > 8);

        if(choice == 1) // get number of cylinders
        {
            cylinders = getCylinders();
            printf("\nNumber of cylinders succesfully entered.\n\n");
        }
        else if(choice == 2)
        {
            // generate request file
            generateRequestFile(cylinders);
        }
        else if(choice == 3)
        {
            // get initial disk head position
            initialPosition = getInitialPosition(cylinders);
            if(initialPosition > -1)
            {
                printf("\nInitial disk head position succesfully set.\n\n");
            }
        }
        else if(choice == 4)
        {
            // FCFS
            totalTraveledFCFS =  FCFS(initialPosition);
            printf("TOTAL NUMBER OF CYLINDERS TRAVERSED: %d\n\n", totalTraveledFCFS);
        }
        else if(choice == 5)
        {
            // SSTF
            totalTraveledSSTF = SSTF(initialPosition);
            printf("TOTAL NUMBER OF CYLINDERS TRAVERSED: %d\n\n", totalTraveledSSTF);
        }
        else if(choice == 6)
        {
            // SCAN
            totalTraveledSCAN = SCAN(initialPosition);
            printf("TOTAL NUMBER OF CYLINDERS TRAVERSE: %d\n\n", totalTraveledSCAN);
        }
        else if(choice == 7)
        {
            chooseBest(totalTraveledFCFS, totalTraveledSSTF, totalTraveledSCAN);
        }
        else if(choice == 8)
        {
            printf("Thank you for using this program.\n");
            break;
        }
        
    }
}