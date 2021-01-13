#ifndef DATA_PARSER_H
#define DATA_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void handler_func(int index, char *s , int size); 
char *cur;
	
typedef struct node {
	enum node_type {TERM, NON_TERM, PARAM, OR, AND, REPEAT, DIGIT, LETTER} type;
	char value; // for TERM 
	char* id;   // for NON_TERM
	int param_value; // for PARAM
	struct node* left;
	struct node* right;
} myparser_node_t;

struct myparser_st_entry {
	char* name;
	myparser_node_t* node;
};

#define ENTRIES 128
struct myparser_st_entry st[ENTRIES];
int st_last_index;

struct myparser_st_entry* myparser_find_entry(char *name);
struct myparser_st_entry* myparser_add_entry(char* name, myparser_node_t* node);

int myparser_st_num_param(char* name);
void myparser_skip_spaces();
void myparser_ex(char c);
char* myparser_id();

myparser_node_t* myparser_expr();
myparser_node_t* parse_grammar(const char * grammar);
bool myparser_visit(myparser_node_t* node);

#endif
