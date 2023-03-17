#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include "main.h"

int **matOutA = NULL, **matOutB = NULL, **matOutC = NULL;
int **matA = NULL, **matB = NULL;
char *OutputFile;
int rows[3];
int cols[3];

int main(int argc, char *argv[])
{
    struct timeval stop, start;

	if (argc == 4)
	{
		ReadMatrix(argv[1],0);
		ReadMatrix(argv[2],1);
        OutputFile = argv[3];
	}
    else
    {
        ReadMatrix("a",0);
        ReadMatrix("b",1);
        OutputFile = "c";
    }

    gettimeofday(&start, NULL); //start checking time
    PerMatrix();
    gettimeofday(&stop, NULL); //end checking time
    printf("Seconds taken in first method %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken in the first method: %lu\n\n", stop.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL); //start checking time
    per_row();
    gettimeofday(&stop, NULL); //end checking time
    printf("Seconds taken in second method %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken in second method: %lu\n\n", stop.tv_usec - start.tv_usec);

    gettimeofday(&start, NULL); //start checking time
    per_element();
    gettimeofday(&stop, NULL); //end checking time
    printf( "Seconds taken in third method %lu\n", stop.tv_sec - start.tv_sec);
    printf("Microseconds taken in third method: %lu\n\n", stop.tv_usec - start.tv_usec);

    free(matA),free(matB);
}
void ReadMatrix(char *a, int index)
{
    int **Matrix = NULL;
	int i;
    char filename[50];
    strcpy(filename, a);
    strcat(filename, ".txt");
	FILE *fp = fopen(filename, "r");
    if (fp == NULL)
        perror("Failed to open input file\n");
	// read the number of rows and columns from the first line
	fscanf(fp, "row=%d col=%d\n", &rows[index], &cols[index]);
    Matrix = MatrixSize(index);
    // read the matrix values from the file
    for (i = 0; i < rows[index]; i++)
    {
        for (int j = 0; j < cols[index]; j++)
        {
            fscanf(fp, "%d", &Matrix[i][j]);
        }
        fscanf(fp, "\n");
    }
    // read the matrix values from the file
    for (i = 0; i < rows[index]; i++)
    {
        for (int j = 0; j < cols[index]; j++)
        {
            fscanf(fp, "%d", &Matrix[i][j]);
        }
        fscanf(fp, "\n");
    }
    if (index == 0)
        matA = Matrix;
    else
        matB = Matrix;
}

int **MatrixSize(int index)
{
    int **Matrix = malloc(rows[index] * sizeof(int *));
    if (Matrix == NULL)
        perror("out of memory");
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
    return MatrixSize(2);
}
void PrintArray(char *filename,int **matOut)
{
    // Open the file for writing
    FILE *fp;
    fp = fopen(filename, "w");

    // Write the matrix to the file
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
    matOutA = AllocOutMat();
    for(int i = 0; i < rows[2]; i++)
    {
        for(int j = 0; j < cols[2]; j++)
        {
            matOutA[i][j] = 0;
            for(int k = 0; k < cols[0]; k++)
                matOutA[i][j] += matA[i][k] * matB[k][j];
        }
    }
    char filename[50];
    strcpy(filename, OutputFile);
    strcat(filename, "_per_matrix.txt");
    PrintArray(filename,matOutA);
    free(matOutA);
}
void per_row()
{
    matOutB = AllocOutMat();
    int numThreads = rows[2];
    pthread_t RowThread[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&RowThread[i], NULL, &RowThreadRoutine, a))
        {
            perror("Error: ");
        }
    }
    for (int i = 0; i < numThreads; i++) // wait for all threads to terminate
        pthread_join(RowThread[i], NULL);
    char filename[50];
    strcpy(filename, OutputFile);
    strcat(filename, "_per_row.txt");
    PrintArray(filename,matOutB);
    free(matOutB);
}
void *RowThreadRoutine(void *row)
{
    int i = *(int*)row;
    for (int c1 = 0; c1 < cols[1]; c1++)
    {
        matOutB[i][c1] = 0;
        for (int c2 = 0; c2 < cols[0]; c2++)
            matOutB[i][c1] += matA[i][c2] * matB[c2][c1];
    }
    free(row);
}
void per_element()
{
    matOutC = AllocOutMat();
    int numThreads = rows[2]*cols[2];
    pthread_t ElementThread[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        int* a = malloc(sizeof(int));
        *a = i;
        if (pthread_create(&ElementThread[i], NULL, &ElementThreadRoutine, a))
        {
            perror("Error: ");
        }
    }
    for (int i = 0; i < numThreads; i++) // wait for all threads to terminate
        pthread_join(ElementThread[i], NULL);
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
    free(matOutC);
}
void *ElementThreadRoutine(void *element)
{
    int elem = *(int*)element;
    int row = elem / rows[2];
    int col = elem % rows[2];
    matOutC[row][col] = 0;
    for (int i = 0; i < cols[0]; i++ )
    {
        /**printf("row is %d col is %d mata is %d matb is %d",row,col ,matA[row][i],matB[i][col]);*/
        matOutC[row][col] += matA[row][i] * matB[i][col];
    }
}