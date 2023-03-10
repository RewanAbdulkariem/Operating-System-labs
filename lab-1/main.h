//
// Created by rewan on 3/7/23.
//

#ifndef MAIN_H
#define MAIN_H

void on_child_exit(int signal);
void setup_environment();
void shell();
void executeCommand();
void  parse_input(char *cmd,char *delim);
void shellBultin();


#endif //MAIN_H
