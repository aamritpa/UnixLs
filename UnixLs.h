#ifndef UNIXLS_H
#define UNIXLS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <locale.h>
#include <langinfo.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

#define thisDirectory "."

// Variables 
bool i_check = false; // to check if 'i' is passed in input
bool l_check = false; // to check if 'l' is passed in input
bool directory_passed = false;  // to check if direcory is passed or not.
int total_directory=0; // Total directory passed
int options =0;  // Total options passed

// Functions 
void print(int len, char *argv[], int path);// print information relared to 'i', 'l', 'il'
void checkInformation(int len, char *argv[]);// check what is passed i? l? il?
void remove_slash(char *dir); //remove extra slash from end from directory if passed
void printPermissions(struct stat statbuf);// print the permissions of file.


#endif