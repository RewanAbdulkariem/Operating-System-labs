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
void PerMatrix()
{
    matOutA = AllocOutMat();
    for(int i = 0; i < rows[2]; i++)
    {
        for(int j = 0; j < cols[2]; j++)
        {
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
void per_row()
{
    matOutB = AllocOutMat();
    int numThreads = rows[2];
    pthread_t RowThread[numThreads];
    for (int i = 0; i < numThreads; i++)
    {
        int* a = malloc(sizeof(int));
        *a = i;
        printf("pthread i is %d\n",i);

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
    printf("i is %d\n",i);
    for (int c1 = 0; c1 < cols[1]; c1++)
    {
        matOutB[i][c1] = 0;
        for (int c2 = 0; c2 < cols[0]; c2++)
            matOutB[i][c1] += matA[i][c2] * matB[c2][c1];
    }
    free(row);
}