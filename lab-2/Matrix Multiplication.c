#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include "main.h"
/**
 * This is a C code that performs matrix multiplication using three different methods.
 * The program reads two matrices from files, multiplies them using the three methods,
 * and writes the results to files.
 */
int **matOutA = NULL, **matOutB = NULL, **matOutC = NULL;
int **matA = NULL, **matB = NULL;
char *OutputFile;
int rows[3];
int cols[3];

int main(int argc, char *argv[])
{
    struct timeval stop, start;

	if (argc == 4)          // Checking if enough arguments were passed
    {
        // Reading matrices from files
        ReadMatrix(argv[1],0);
		ReadMatrix(argv[2],1);
        OutputFile = argv[3];
	}
    else
    {
        // Reading default matrices
        ReadMatrix("a",0);
        ReadMatrix("b",1);
        OutputFile = "c";
    }

    // Performing matrix multiplication using first method
    gettimeofday(&start, NULL); //start checking time
    PerMatrix();
    gettimeofday(&stop, NULL); //end checking time
    printf("Seconds taken in first method %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken in the first method: %lu\n\n", stop.tv_usec - start.tv_usec);

    // Performing matrix multiplication using second method
    gettimeofday(&start, NULL); //start checking time
    per_row();
    gettimeofday(&stop, NULL); //end checking time
    printf("Seconds taken in second method %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken in second method: %lu\n\n", stop.tv_usec - start.tv_usec);

    // Performing matrix multiplication using third method
    gettimeofday(&start, NULL); //start checking time
    per_element();
    gettimeofday(&stop, NULL); //end checking time
    printf( "Seconds taken in third method %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken in third method: %lu\n\n", stop.tv_usec - start.tv_usec);

    // Freeing memory allocated to matrices
    free(matA),free(matB);
}
void ReadMatrix(char *a, int index)
{
    // Declare variables
    int **Matrix = NULL;    // 2D dynamic array to hold the matrix values
	int i;
    char filename[50];      // String to hold the filename
    strcpy(filename, a);    // Copy the input argument a (the file name) to the filename string
    strcat(filename, ".txt");    // Add ".txt" to the end of the filename string
	FILE *fp = fopen(filename, "r");    // Open the file in read mode
    if (fp == NULL)      // Check if the file was opened successfully
        perror("Failed to open input file\n");  // Print an error message to the console if it failed
	// read the number of rows and columns from the first line
	fscanf(fp, "row=%d col=%d\n", &rows[index], &cols[index]);
    // Allocate memory for the 2D array based on the number of rows and columns read from the file
    Matrix = MatrixSize(index);
    // Read the matrix values from the file and save them to the 2D array
    for (i = 0; i < rows[index]; i++)
    {
        for (int j = 0; j < cols[index]; j++)
        {
            fscanf(fp, "%d", &Matrix[i][j]);    // Read a single value from the file and save it to the array
        }
        fscanf(fp, "\n");   // Move to the next line in the file
    }
    // Check the value of index to determine whether to save the matrix to matA or matB
    if (index == 0)
        matA = Matrix;
    else
        matB = Matrix;
}

int **MatrixSize(int index)
{
    // Allocate memory for an array of integer pointers with size equal to the number of rows
    int **Matrix = malloc(rows[index] * sizeof(int *));
    if (Matrix == NULL)         // Check if memory allocation was successful
        perror("out of memory");
    // Allocate memory for each row of an array of integers with size equal to the number of columns
    for(int i = 0; i < rows[index]; i++)
    {
        Matrix[i] = malloc(cols[index] * sizeof(int));
        if(Matrix[i] == NULL)
            perror( "out of memory\n");
    }
    return Matrix;
}
int ** AllocOutMat()
{
    rows[2] = rows[0];
    cols[2] = cols[1];
    int **Matrix = MatrixSize(2);
    for(int i = 0; i < rows[2]; i++)
    {
        for(int j = 0; j < cols[2]; j++)
        {
            Matrix[i][j] = 0;
        }
    }
    return Matrix;
}
void PrintArray(char *filename,int **matOut)
{
    // This function writes a 2D integer array to a file with the specified filename.
    // Open the file for writing
    FILE *fp;
    fp = fopen(filename, "w");

    // Write the matrix to the file
    fprintf(fp,   "Method: A thread %s\n",filename);

    fprintf(fp, "row=%d col=%d\n", rows[2], cols[2]); // First line contains the dimensions
    for (int i = 0; i < rows[2]; i++)
    {
        for (int j = 0; j < cols[2]; j++)
            fprintf(fp, "%d ", matOut[i][j]);
        fprintf(fp, "\n"); // Newline after each row
    }
    // Close the file
    fclose(fp);
}
void PerMatrix()
{
    // This function multiplies two matrices matA and matB
    // and stores the result in matOutA
    // and writes the resulting matrix to a file with the name OutputFile + "_per_matrix.txt".

    matOutA = AllocOutMat();    // Allocate memory for the result matrix

    for(int i = 0; i < rows[2]; i++)
    {
        for(int j = 0; j < cols[2]; j++)
        {
            // Multiply the i-th row of matA with the j-th column of matB to get the (i,j)-th element of matOutA
            for(int k = 0; k < cols[0]; k++)
                matOutA[i][j] += matA[i][k] * matB[k][j];
        }
    }
    char filename[50];

    strcpy(filename, OutputFile);
    strcat(filename, "_per_matrix.txt"); // Append "_per_matrix.txt" to OutputFile to get the name of the output file
    PrintArray(filename,matOutA); // Write the resulting matrix to the output file

    free(matOutA); // Free the memory allocated for matOutA
}
void per_row()
{
    matOutB = AllocOutMat();        // Allocate memory for the output matrix
    int numThreads = rows[2];           // Get the number of rows for which threads need to be created
    pthread_t RowThread[numThreads];        // Create an array to hold thread IDs

    for (int i = 0; i < numThreads; i++)        // Loop through each row and create a thread for it
    {
        // Allocate memory for the row number to pass to the thread
        int* a = malloc(sizeof(int));
        *a = i;

        if (pthread_create(&RowThread[i], NULL, &RowThreadRoutine, a))
        {
            perror("Error: ");
        }
    }
	free(a);
    for (int i = 0; i < numThreads; i++) // wait for all threads to terminate
        pthread_join(RowThread[i], NULL);
    printf("number of threads created in part2: %d\n",numThreads);

    // Create the output file name by appending "_per_row.txt" to OutputFile
    char filename[50];
    strcpy(filename, OutputFile);
    strcat(filename, "_per_row.txt");

    // Write the resulting matrix to the output file
    PrintArray(filename,matOutB);

    // Free the memory allocated for matOutB
    free(matOutB);
}
void *RowThreadRoutine(void *row)
{
    int i = *(int*)row;                // Get the row number from the input parameter

    // Loop over each column in the output matrix
    for (int c1 = 0; c1 < cols[1]; c1++)
    {
        for (int c2 = 0; c2 < cols[0]; c2++)
            matOutB[i][c1] += matA[i][c2] * matB[c2][c1];
    }
    free(row);              // Free the memory allocated for the input parameter
}
void per_element()
{
    matOutC = AllocOutMat();    // Allocate memory for the output matrix
    int numThreads = rows[2]*cols[2];   // Calculate the total number of threads required (one for each element of the output matrix)
    pthread_t ElementThread[numThreads];    // Create an array to store thread IDs
    // Loop through all elements of the output matrix and create a thread for each
    for (int i = 0; i < numThreads; i++)
    {
        // Allocate memory for the element number to pass to the thread
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&ElementThread[i], NULL, &ElementThreadRoutine, a))
        {
            perror("Error: ");
        }
    }
	free(a);
    for (int i = 0; i < numThreads; i++) // wait for all threads to terminate
        pthread_join(ElementThread[i], NULL);
    printf("number of threads created in part3: %d\n",numThreads);

    // Create the output file name by appending "_per_element.txt" to OutputFile
    char filename[50];
    strcpy(filename, OutputFile);
    strcat(filename, "_per_element.txt");
    PrintArray(filename,matOutC);
    /*
    printf("matrix out\n");
    for (int i = 0; i < rows[2]; i++) {
        for (int j = 0; j < cols[2]; j++)
            printf("%d ", matOutC[i][j]);
        printf("\n");}
        */
    // Free the memory allocated for matOutC
    free(matOutC);
}
void *ElementThreadRoutine(void *element)
{
    int elem = *(int*)element;      // Get the element index from the argument passed to the thread
    int row = elem / cols[2];       // Calculate the row index for the element
    int col = elem % cols[2];       // Calculate the column index for the element
    // Loop through the corresponding row in matB and the corresponding column in matA
    for (int i = 0; i < cols[0]; i++ )
    {
        /**printf("row is %d col is %d mata is %d matb is %d",row,col ,matA[row][i],matB[i][col]);*/
        matOutC[row][col] += matA[row][i] * matB[i][col];
    }
    free(element); // Free the memory allocated for the element index
}
