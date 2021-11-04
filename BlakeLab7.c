#include <stdio.h>

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

FILE* generateRequestFile();

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
int main()
{
    int choice = 0;
    int cylinders = 0;
    int initialPosition = 0;
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