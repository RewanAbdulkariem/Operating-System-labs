//
// Created by rewan on 3/16/23.
//

#ifndef UNTITLED_MAIN_H
#define UNTITLED_MAIN_H
void ReadMatrix(char *a, int index);
int **MatrixSize(int index);
int ** AllocOutMat();
void PerMatrix();
void per_row();
void *RowThreadRoutine(void *row);
void PrintArray(char *filename,int **matOut);
#endif //UNTITLED_MAIN_H
