//
// Created by rewan on 3/7/23.
//

#ifndef MAIN_H
#define MAIN_H

void on_child_exit(int signal);
void setup_environment();
void shell();
void execute_command(char **command, int arg_length);

#endif //MAIN_H
