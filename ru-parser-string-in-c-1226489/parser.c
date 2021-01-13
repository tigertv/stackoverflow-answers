#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parser.h"

struct myparser_st_entry st[ENTRIES] = {0};
int st_last_index = 0;
char *cur = NULL;

struct myparser_st_entry* myparser_find_entry(char *name) {
	for (int j = 0; j < st_last_index; ++j) {
		char* p = st[j].name;
		char* ii = name;
		// find entry by name
		while (*p == *ii && *p != '\0')  {
			++p;
			++ii;
		}
		
		if (*p == '\0' && *ii == *p) {
			return &st[j];
		}
	}
	return NULL;
}

struct myparser_st_entry* myparser_add_entry(char* name, myparser_node_t* node) {
	st[st_last_index].name = name;
	st[st_last_index].node = node;
	return &st[st_last_index++];
}

int myparser_st_num_param(char* name) {
	char *p = "%";
	char *n = name;
	while (*p == *n && *p != '\0') {
		++p;
		++n;
	}
	if (*p != '\0') return -1;

	return atoi(n);
}

void myparser_skip_spaces() {
	while (*cur == ' ' || *cur == '\n') ++cur;
}

void myparser_ex(char c) { // expect char
	myparser_skip_spaces();
	
	if (*cur != c) {
		printf("\nError: expected '%c' but not '%c'\n", c, *cur);
		exit(-1);
	}
	++cur;
}

char* myparser_id() { // identificator
	myparser_skip_spaces();

	char* p = cur;
	while (*cur != ' ' && *cur != '\0' && *cur != '}' && *cur != '|' && *cur != ';') {
		++cur;
	}
	unsigned int size = cur - p;
	char* ret = malloc(size + 1);
	char *pret = ret;
	for(unsigned int i = size; i != 0; --i) {
		 *pret++ = *p++;
	}

	pret = '\0';
	return ret;	
}


myparser_node_t* myparser_expr() {
	//enum node_type operation = AND;
	myparser_skip_spaces();

	myparser_node_t* pnode = NULL;

	while (*cur != '\0' && *cur != ';') { 
		//printf("operation = %d\n", operation);
		if (*cur == '\'') {
			// terminal
			++cur;
			char c = *cur;
			if (c == '\0') {
				printf("Error: expected a char\n");
				exit(-1);
			}
			++cur;

			//printf("terminal = '%c'\n", c);

			myparser_node_t* term = malloc(sizeof(myparser_node_t));
			term->value = c; 
			//if (c >= '0' && c <= '9') {
			//	term->type = DIGIT;			
			//} else {
				term->type = TERM;			
			//}
			myparser_ex('\'');

			if (pnode) {
				myparser_node_t* anode = malloc(sizeof(myparser_node_t));
				anode->type = AND;
				anode->left = pnode;
				anode->right = term;
				pnode = anode;
			} else {
				pnode = term;
			}
			myparser_skip_spaces();

		} else if (*cur == '|') {
			++cur;
			myparser_node_t* n = myparser_expr();
			if (pnode) {
				myparser_node_t* anode = malloc(sizeof(myparser_node_t));
				anode->type = OR;
				anode->left = pnode;
				anode->right = n;
				pnode = anode;
			} else {
				pnode = n;
			}
			myparser_skip_spaces();
			
		} else if (*cur == '(') {
			 
		} else if (*cur == ')') {

		} else if (*cur == '{') {
			++cur;
			myparser_node_t* n = myparser_expr();

			myparser_node_t* rep = malloc(sizeof(myparser_node_t));
			rep->type = REPEAT;
			rep->left = n;
			rep->right = NULL;

			if (pnode) {
				myparser_node_t* anode = malloc(sizeof(myparser_node_t));
				anode->type = AND;
				anode->left = pnode;
				anode->right = rep;
			} else {
				pnode = rep;
			}

		} else if (*cur == '}') {
			++cur;
			myparser_skip_spaces();
			return pnode;
		} else if (*cur == ' ') {
			printf("SPACE HERE");
			myparser_skip_spaces();
		} else {
			// non-terminal

			char* name = myparser_id();
//				printf("non-terminal = %s\n", name);
			// search in symbol_table
			struct myparser_st_entry * entry = myparser_find_entry(name);

			if (entry) {
				free(name);
			} else {
				// add in table
//					printf("add in table: %s\n", name);
				myparser_node_t* anode = malloc(sizeof(myparser_node_t));
				anode->type = NON_TERM;
				anode->id = name;
				anode->left = NULL;
				anode->right = NULL;

				entry = myparser_add_entry(name, anode);

				int num = myparser_st_num_param(name);

				if (num != -1) {
					entry->node->type = PARAM;
					entry->node->param_value = num;
				}
			}
			
			if (pnode) {
				myparser_node_t* anode = malloc(sizeof(myparser_node_t));
				anode->type = AND;//operation;
				anode->left = pnode;
				anode->right = entry->node;

				pnode = anode;
			} else {
				pnode = entry->node;
			}
			myparser_skip_spaces();
		}

	}
	return pnode;
}

myparser_node_t* parse_grammar(const char * grammar) {
	cur = grammar;

	while(*cur != '\0') {
		char* record = myparser_id();
//		printf("%s\n", record);
		
		struct myparser_st_entry* root_entry = myparser_find_entry(record);

		if (!root_entry) {
			myparser_node_t* anode = malloc(sizeof(myparser_node_t));
			anode->type = NON_TERM;
			anode->id = record;
			anode->left = NULL;
			anode->right = NULL;

			root_entry = myparser_add_entry(record, anode);
		}

		myparser_ex('=');

		myparser_node_t* pnode = myparser_expr();
		if(root_entry->node->type == NON_TERM) {
			myparser_node_t* p = root_entry->node;
			p->type = pnode->type;
			p->left = pnode->left;
			p->right = pnode->right;
			p->id = pnode->id;
			p->param_value = pnode->param_value;
			p->value = pnode->value;
			free(pnode);
		} else {
			root_entry->node->left = pnode;
		}
		if (*cur == ';') ++cur;
		myparser_skip_spaces();
	}

	struct myparser_st_entry* entry = myparser_find_entry("digit");
	if (entry && entry->node->type == NON_TERM) {
		// set "digit" to default if there is no NON_TERMINAL "digit"
		// NON_TERM node shouldn't be presented in the end of creating of grammar AST
		myparser_node_t* p = entry->node;
		p->type = DIGIT;
	}

	entry = myparser_find_entry("letter");
	if (entry && entry->node->type == NON_TERM) {
		// set "letter" to default if there is no NON_TERMINAL "letter"
		// NON_TERM node shouldn't be presented in the end of creating of grammar AST
		myparser_node_t* p = entry->node;
		p->type = LETTER;
	}

	return st[0].node;
}


bool myparser_visit(myparser_node_t* node) {
	if (node == NULL) {
		printf("myparser_node_t is NULL\n");
		return false;
	}

	switch(node->type) {
		case TERM:
			myparser_skip_spaces();

			if (node->value == *cur) {
				printf("term=%c\n", *cur);
				//printf("equal\n");
				++cur;
				return true;
			} 
			//printf("Expected '%c' but not '%c'\n", node->value, *cur);
			return false;
			break;

		/*
		case NON_TERM:
			printf("NON TERMINAL: %s\n", node->id);
			return myparser_visit(node->left);
			break;
		//*/
		case LETTER : {
			//printf("visit LETTER: %c\n", *cur);
			if ( (*cur >= 'a' && *cur <= 'z') || (*cur >= 'A' && *cur <= 'Z') ) {
				printf("LETTER: %c\n", *cur);
				++cur;
				return true;
			}
			return false;
		}	break;

		case DIGIT : {
			//myparser_skip_spaces();
			//printf("visit DIGIT: %c\n", *cur);
			if (*cur >= '0' && *cur <= '9') {
				printf("DIGIT: %c\n", *cur);
				++cur;
				return true;
			}
			return false;
		}
			break;

		case REPEAT: {
			//printf("REPEAT: %s\n", "g");
			char* tmp = cur;	
			while(myparser_visit(node->left)) {
				tmp = cur;
			}
			cur = tmp;
			//printf("REPEAT end: %c\n", *cur);
			return true;
		}
			break;

		case PARAM:
			myparser_skip_spaces();
			//printf("PARAM: %s\n", node->id);
			{
				// remember string
				char* begin_visit = cur;
				bool r = myparser_visit(node->left);
				int diff = (int)(cur - begin_visit);
				handler_func(node->param_value, begin_visit, diff);
				return r;
			}
			break;

		case AND: {
			myparser_skip_spaces();
			//printf("AND\n");
			bool a = myparser_visit(node->left);
			if (a == false) return a;
			//printf("AND: a = %d\n", a);
			bool b = myparser_visit(node->right);
			//printf("AND: b = %d\n", b);
			return a && b;
		}
			break;

		case OR: {
			myparser_skip_spaces();
			//printf("OR\n");
			//printf("OR cur = %c\n", *cur);
			char* tmp = cur;	
			bool a = myparser_visit(node->left);
			//printf("OR a cur = %c\n", *cur);
			//printf("OR: a = %d\n", a);
			if (a) return true;
			cur = tmp;
			bool b = myparser_visit(node->right);
			//printf("OR b cur = %c\n", *cur);
			//printf("OR: b = %d\n", b);
			return b;
		}
			break;

		default:
			// error
			printf("Error: wrong node type\n");
			exit(-1);
			break;
	}

}

