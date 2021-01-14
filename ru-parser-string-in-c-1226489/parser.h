#ifndef DATA_PARSER_H
#define DATA_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

typedef struct myparser {
	char* cur;
	struct myparser_st_entry st[ENTRIES];
	int st_size;
	int st_index;
	void (*handler)(int index, char *s , int size); 
} myparser_t;

myparser_t* myparser_new();
void myparser_delete(myparser_t* self);

myparser_node_t* myparser_parse_grammar(myparser_t* self, const char * grammar);
bool myparser_visit(myparser_t* self, myparser_node_t* node);

#endif
