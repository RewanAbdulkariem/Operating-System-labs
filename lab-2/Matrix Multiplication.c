#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include "main.h"
int **matOut = NULL;
int **matA = NULL, **matB = NULL;
char *OutputFile;
int rows[3];
int cols[3];

int main(int argc, char *argv[])
{
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
    /**
    printf("matrix a\n");
    for (int i = 0; i < rows[0]; i++) {
        for (int j = 0; j < cols[0]; j++) {
            printf("%d ", matA[i][j]);
        }
        printf("\n");
    }
    printf("matrix b\n");
    for (int i = 0; i < rows[1]; i++) {
        for (int j = 0; j < cols[1]; j++) {
            printf("%d ", matB[i][j]);
        }
        printf("\n");
    }
    */
    AllocOutMat();
    PerMatrix();
    /**
    printf("matrix out\n");
    for (int i = 0; i < rows[2]; i++) {
        for (int j = 0; j < cols[2]; j++)
            printf("%d ", matOut[i][j]);
        printf("\n");
    }
    */
    per_row();
    free(matA),free(matB),free(matOut);
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
void AllocOutMat()
{
    rows[2] = rows[0];
    cols[2] = cols[1];
    matOut = MatrixSize(2);
}
void PerMatrix()
{
    for(int i = 0; i < rows[2]; i++)
    {
        for(int j = 0; j < cols[2]; j++)
        {
            for(int k = 0; k < cols[0]; k++)
                matOut[i][j] += matA[i][k] * matB[k][j];
        }
    }
    char filename[50];
    strcpy(filename, OutputFile);
    strcat(filename, "_per_matrix.txt");
    PrintArray(filename);
}
void PrintArray(char *filename)
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
void per_row()
{
    int numThreads = rows[2];
    pthread_t RowThread[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        void* arg = &i;
        if (pthread_create(&RowThread[i], NULL, &RowThreadRoutine, arg))
        {
            perror("Error: ");
        }
    }
    for (int i = 0; i < rows[2]; i++) // wait for all threads to terminate
        pthread_join(RowThread[i], NULL);
}
void *RowThreadRoutine(void *row)
{
    int* i = (int*)row;
    int row_number = *i;
    for (int c1 = 0; c1 < rows[2]; c1++)
    {
        for (int c2 = 0; c2 < cols[2]; c2++)
            matOut[row_number][c1] += matA[row_number][c2] * matB[c2][c1];


    }
    char filename[50];
    strcpy(filename, OutputFile);
    strcat(filename, "_per_row.txt");
    PrintArray(filename);
    i++;
}