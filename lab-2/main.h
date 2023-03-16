//
// Created by rewan on 3/16/23.
//

#ifndef UNTITLED_MAIN_H
#define UNTITLED_MAIN_H
void ReadMatrix(char *a, int index);
int **MatrixSize(int index);
void AllocOutMat();
void PerMatrix();
void per_row();
void *RowThreadRoutine(void *row);
void PrintArray(char *filename);
#endif //UNTITLED_MAIN_H
