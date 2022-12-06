#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "colors.h"

#define CHECKUS(condition, return_num)                                                      \
do {                                                                                        \
    if (!(condition))                                                                       \
    {                                                                                       \
        printf("\n" WHITE BRIGHT "%s: " NRM "In function <" WHITE BRIGHT "%s" NRM ">:\n"    \
        WHITE BRIGHT "%s:%i: " NRM "Assertion <" RED BRIGHT "%s" NRM "> failed\n",          \
        __FILE__, __PRETTY_FUNCTION__, __FILE__, __LINE__, #condition);                     \
        return return_num;                                                                  \
    }                                                                                       \
} while (0)


typedef struct tree_node tree_node;

struct tree_node
{
    char* data;
    tree_node* parent;
    tree_node* right;
    tree_node* left;
};

struct data_base
{
    tree_node* tree;
    tree_node** answers_list;
};

int ReadDataFromFile(data_base* data_p, const char* file_name);


void GetAll(char* str, tree_node* node_p, int* counter, tree_node** answers_list);

void GetSomething(char* str, tree_node* node_p, int* counter, tree_node** answers_list);

void GetQuession(char* str, tree_node* node_p, int* counter, tree_node** answers_list);

void GetAnswer(char* str, tree_node* node_p, int* counter, tree_node** answers_list);

void AddNodeToList(tree_node** answers_list, tree_node* node_p);

// void CopyStrFor(char* dest, char* src, char stop_symbol);

char* ReadFile(FILE* text_input);

int CountSimbols(FILE* text_input);

int Akenator(data_base* data_p);

int AddQuestionNode(tree_node* node_p);

int PrintOrigin(data_base* data_p, const char* str);

int PrintParents(tree_node* node_p);

int PrintAnswersAlphabetically(data_base* data_p);

int SortAnswersAlphabetically(tree_node** answers_list);

int AlphabetAnswersComparator(const void* answer1, const void* answer2);

int BiQuestion();

void ClearInput();

char* ScanStringColor(const char* color);

int SaveDataToFile(data_base* data_p, FILE* file);

int SaveTreeToFile(const tree_node* node_p, FILE* file);

int FreeData(data_base* data_p);

int FreeTree(tree_node* node_p);

int AddRightBranch(tree_node* node_p, char* data);

int AddLeftBranch(tree_node* node_p, char* data);



#endif
