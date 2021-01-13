#ifndef DATA_PARSER_H
#define DATA_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ENTRIES 128
void handler_func(int index, char *s , int size); 
char *cur;
	
typedef struct node {
	enum node_type {TERM, NON_TERM, PARAM, OR, AND, REPEAT, DIGIT, LETTER} type;
	char value; // for TERM 
	char* id;   // for NON_TERM
	int param_value; // for PARAM
	struct node* left;
	struct node* right;
} Node;

struct st_entry {
	char* name;
	Node* node;
};

struct st_entry st[ENTRIES];
int st_last_index;

struct st_entry* find_entry(char *name);
struct st_entry* add_entry(char* name, Node* node);

int st_num_param(char* name);
void skip_spaces();
void ex(char c);
char* id();

Node* expr();
Node* parse_grammar(const char * grammar);
bool visit(Node* node);

#endif
