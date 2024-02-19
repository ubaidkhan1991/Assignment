#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/**
 * @brief Macro for the duration to get input from user
 *
 */
#define SCAN_DURATION_MINUTES (1)

/**
 * @brief Macro to define the user input interval
 *
 */
#define SCAN_INTERVAL_SECONDS (5)

#define SCAN_DURATION_SECONDS (SCAN_DURATION_MINUTES * 60)
#define MAX_INPUTS            (SCAN_DURATION_SECONDS / SCAN_INTERVAL_SECONDS)
#define IS_NUMBER_EVEN(n)     (n % 2 == 0)

/**
 * @brief Pointer to hold User input array
 *
 */
int *pIntArray = NULL;

/**
 * @brief Print data in form of bits
 *
 * @param size Size of data type
 * @param ptr Pointer to data
 */
void printBits(size_t const size, void const *const ptr)
{
    unsigned char *b = (unsigned char *)ptr;
    unsigned char byte;
    int i, j;

    for (i = size - 1; i >= 0; i--)
    {
        for (j = 7; j >= 0; j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
}

/**
 * @brief Prints char array
 *
 * @param pArray Pointer to char array
 * @param size size of char array
 */
void printArray(char *pArray, int size)
{
    while (size--)
    {
        printBits(sizeof(pArray[size]), &pArray[size]);
        printf(" ");
    }
    printf("\n");
    return;
}

/**
 * @brief Set all bit positions in char array
 *
 * @param pIntArray Pointer to integer array
 * @param intArraylen Size of integer array
 * @param pCharArray Pointer to char array
 */
void setBits(int *pIntArray, int intArraylen, char *pCharArray)
{
    for (int i = 0; i < intArraylen; i++)
    {
        int num       = pIntArray[i];
        int byteIndex = num / 8;
        int bitIndex  = num % 8;

        pCharArray[byteIndex] |= (1 << bitIndex);
    }
}

/**
 * @brief Exit function handler
 *
 * @param sig Signal input
 */
void exitfunc(int sig)
{
    int maxInt = 0;
    printf("Input Array:");
    for (int i = 0; i < MAX_INPUTS; i++)
    {
        if (pIntArray[i] == -1)
        {
            break;
        }
        printf("%u ", pIntArray[i]);
        maxInt = (pIntArray[i] > maxInt) ? pIntArray[i] : maxInt;
    }
    printf("\nmaxInt is %u\n", maxInt);

    if (maxInt != 0)
    {
        int charArraySize = ((maxInt + 8) / 8);
        printf("charArraySize %u\n", charArraySize);
        char *pCharArray = (char *)calloc(charArraySize, sizeof(char));
        if (pCharArray == NULL)
        {
            printf("Could not allocate memory!\n");
        }
        else
        {
            setBits(pIntArray, MAX_INPUTS, pCharArray);

            printf("pCharArray:\n");
            printArray(pCharArray, charArraySize);
        }
        free(pCharArray);
    }
    else
    {
        printf("Char array not created\n");
    }

    _exit(0);
}

/**
 * @brief Function to check duplicate entry
 *
 * @param pIntArray Pointer to integer array
 * @param size Size of integer array
 * @param inputValue Entry to check
 * @return bool true if duplicate otherwise false
 */
bool isDuplicate(int *pIntArray, int size, int inputValue)
{
    for (int i = 0; i < size; i++)
    {
        if (pIntArray[i] == inputValue)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Start scanning of user inputs
 * This will reject any duplicates or Even numbers
 *
 * @param pArray Pointer to integer array
 */
void startScanning(int *pArray)
{
    int numCount = 0;

    for (int i = 0; i < MAX_INPUTS; i++)
    {
        int currentNum;
        printf("Please enter odd number\n");
        scanf("%d", &currentNum);

        // Check for valid inputs
        if (!IS_NUMBER_EVEN(currentNum) && !isDuplicate(pArray, numCount, currentNum))
        {
            pArray[numCount++] = currentNum;
        }
        else
        {
            printf("Invalid number\n");
        }
        printf("Please wait for %u Sec(s)\n", SCAN_INTERVAL_SECONDS);
        sleep(SCAN_INTERVAL_SECONDS);
    }
}

/**
 * @brief Main function
 *
 * @return int return value, 0 if success
 */
int main()
{
    signal(SIGALRM, exitfunc);
    alarm(SCAN_DURATION_SECONDS);

    int intArray[MAX_INPUTS];
    for (int i = 0; i < MAX_INPUTS; i++)
    {
        intArray[i] = -1;
    }

    pIntArray = intArray;
    startScanning(pIntArray);

    return 0;
}