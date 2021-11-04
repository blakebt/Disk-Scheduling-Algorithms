#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILEPATH 64
#define NUMBER_OF_REQUESTS 10

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

int getInitialPosition(int total) // total is the maximum number of cylinders
{
    int position;
    if(total == 0)
    {
        printf("\nThe number of cylinders is not set.\n");
        printf("Please enter the number of cylinders from the menu first.\n\n");
        return -1;
    }
    else
    {
         do
        {
            printf("Please enter the initial position of the disk head.\n");
            printf("Entered value should be between 0 and %d: ", total-1);
            scanf(" %d", &position);

        } while (position < 0 || position > total-1);

        return position;
    }
    
}

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
        printf("Please enter the location of the file containing the requests.\n");
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
        distanceTraveled += abs(nextPosition - headPosition);
        headPosition = nextPosition;
    }

    return distanceTraveled;
}
int main()
{
    int choice = 0;
    int cylinders = 0;
    int initialPosition = 0;
    int totalTraveledFCFS = 0;
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
        }
        else if(choice == 6)
        {
            // SCAN
        }
        else if(choice == 7)
        {
            // report best
        }
        else if(choice == 8)
        {
            printf("Thank you for using this program.\n");
            break;
        }
        
    }
}