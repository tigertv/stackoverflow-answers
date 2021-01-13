#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ENTRIES 128
void handler_func(int index, char *s , int size); 

char *cur = NULL;
	
typedef struct node {
	enum node_type {TERM, NON_TERM, PARAM, OR, AND, REPEAT} type;
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

struct st_entry st[ENTRIES] = {0};
int st_last_index = 0;

struct st_entry* find_entry(char *name) {
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

struct st_entry* add_entry(char* name, Node* node) {
	st[st_last_index].name = name;
	st[st_last_index].node = node;
	return &st[st_last_index++];
}

int st_num_param(char* name) {
	char * s = "%";
	char *p = s;
	char *n = name;
	while (*p == *n && *p != '\0') {
		++p;
		++n;
	}
	if (*p != '\0') return -1;

	return atoi(n);
}

void skip_spaces() {
	while (*cur == ' ' || *cur == '\n') ++cur;
}

void ex(char c) { // expect char
	skip_spaces();
	
	if (*cur != c) {
		printf("\nError: expected '%c' but not '%c'\n", c, *cur);
		exit(-1);
	}
	++cur;
}

char* id() { // identificator
	skip_spaces();

	char* p = cur;
	while (*cur != ' ' && *cur != '\0') {
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

Node* expr() {
	//enum node_type operation = AND;
	skip_spaces();

	Node* pnode = NULL;

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
//				printf("terminal = %c\n", c);
			ex('\'');

			Node* term = malloc(sizeof(Node));
			term->type = TERM;			
			term->value = c; 

			if (pnode) {
				Node* anode = malloc(sizeof(Node));
				anode->type = AND;
				anode->left = pnode;
				anode->right = term;
				pnode = anode;
			} else {
				pnode = term;
			}

		} else if (*cur == '|') {
			++cur;
			Node* n = expr();
			if (pnode) {
				Node* anode = malloc(sizeof(Node));
				anode->type = OR;
				anode->left = pnode;
				anode->right = n;
				pnode = anode;
			} else {
				pnode = n;
			}
			
		} else if (*cur == '(') {
			 
		} else if (*cur == ')') {

		} else if (*cur == '{') {
			++cur;
			Node* n = expr();

			Node* rep = malloc(sizeof(Node));
			rep->type = REPEAT;
			rep->left = n;
			rep->right = NULL;

			if (pnode) {
				Node* anode = malloc(sizeof(Node));
				anode->type = AND;
				anode->left = pnode;
				anode->right = rep;
			} else {
				pnode = rep;
			}

		} else if (*cur == '}') {
			++cur;
			return pnode;
		} else {
			// non-terminal

			char* name = id();
//				printf("non-terminal = %s\n", name);
			// search in symbol_table
			struct st_entry * entry = find_entry(name);

			if (entry) {
				free(name);
			} else {
				// add in table
//					printf("add in table: %s\n", name);
				Node* anode = malloc(sizeof(Node));
				anode->type = NON_TERM;
				anode->id = name;
				anode->left = NULL;
				anode->right = NULL;

				entry = add_entry(name, anode);

				int num = st_num_param(name);

				if (num != -1) {
					entry->node->type = PARAM;
					entry->node->param_value = num;
				}
			}
			
			if (pnode) {
				Node* anode = malloc(sizeof(Node));
				anode->type = AND;//operation;
				anode->left = pnode;
				anode->right = entry->node;

				pnode = anode;
			} else {
				pnode = entry->node;
			}
		}
		skip_spaces();
	}
	return pnode;
}

Node* parse_grammar(const char * grammar) {
	cur = grammar;

	while(*cur != '\0') {
		char* record = id();
//		printf("%s\n", record);
		
		struct st_entry* root_entry = find_entry(record);

		if (!root_entry) {
			Node* anode = malloc(sizeof(Node));
			anode->type = NON_TERM;
			anode->id = record;
			anode->left = NULL;
			anode->right = NULL;

			root_entry = add_entry(record, anode);
		}

		ex('=');

		Node* pnode = expr();
		root_entry->node->left = pnode;
		if (*cur == ';') ++cur;
		skip_spaces();
	}

	return st[0].node;
}


bool visit(Node* node) {
	if (node == NULL) {
		printf("Node is NULL\n");
		return false;
	}
	switch(node->type) {
		case TERM:
			skip_spaces();

			if (node->value == *cur) {
				//printf("term=%c\n", node->value);
				//printf("equal\n");
				++cur;
				return true;
			} 
			//printf("Expected '%c' but not '%c'\n", node->value, *cur);
			return false;
			break;

		case NON_TERM:
			//printf("NON TERMINAL: %s\n", node->id);
			return visit(node->left);
			break;

		case REPEAT: {
				//printf("REPEAT: %s\n", "mystring");
				char* tmp = cur;	
				while(visit(node->left)) {
					tmp = cur;
				}
				cur = tmp;
				return true;
			}
			break;

		case PARAM:
			//printf("PARAM: %s\n", node->id);
			{
				// remember string
				skip_spaces();
				char* begin_visit = cur;
				bool r = visit(node->left);
				int diff = (int)(cur - begin_visit);
				handler_func(node->param_value, begin_visit, diff);
				return r;
			}
			break;

		case AND: {
			//printf("AND\n");
			bool a = visit(node->left);
			if (a == false) return a;
			//printf("AND: a = %d\n", a);
			bool b = visit(node->right);
			//printf("AND: b = %d\n", b);
			return a && b;
		}
			break;

		case OR: {
			//printf("OR\n");
			char* tmp = cur;	
			bool a = visit(node->left);
			//printf("OR: a = %d\n", a);
			if (a) return true;
			cur = tmp;
			bool b = visit(node->right);
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

/////////////////////////////////////////////////////////////////////////////////////////

struct record {
	char* name;
	int param[4];
};

struct record* precord = NULL;
struct record records[256];
int records_last_index = 0;
int current_param_index = 0;


void param0(char* s, int size) {
	//printf("param0 executed\nstr=");
	char* name = malloc(size + 1);
	char* n = name;
	while(size != 0) {
		*n++ = *s;
		//printf("%c", *s);
		++s;
		--size;
	}
	//printf("\n");
	*n = '\0';
	records[records_last_index].name = name;
}

void param1(char* s, int size) {
	int r = atoi(s);
	records[records_last_index].param[current_param_index++] = r;
}

void dummy_func(char* s, int size) {
	printf("DUMMY FUNCTION executed\nstr=");
	while(size != 0) {
		printf("%c", *s++);
		--size;
	}
	printf("\n");
}

void empty_func(char* s, int size) {
	
}

#define HANDLERS_SIZE 16
void (*handlers[HANDLERS_SIZE])(char *s , int size);

void init_handlers() {
	handlers[0] = param0;
	handlers[1] = param1;

	for(int i = 2; i < HANDLERS_SIZE; ++i) {
		handlers[i] = empty_func;
		//handlers[i] = dummy_func;
	}
}

void parse_data(Node* gram, const char* s) {
	cur = s;
	while(*cur != '\0') {
		bool result = visit(gram);
		//printf("result = %d\n", result);
		if (result) {
			// make record
			current_param_index = 0;	
			++records_last_index;
		} else {
			break;
		}
	}
}

void handler_func(int index, char *s , int size) {
	handlers[index](s, size);
}

int main() {
	const char *grammar = 
		"record = '{' pstring ';' two_param ';' two_param '}' ;"
		"two_param = '[' %1 '&' %1 ']' ;"
		"%1 = { digit } ;"
		"pstring = ''' %0 ''' ;"
		"%0 = { char } ;"
		"char = letter | digit | ' ' ;"
		"letter = small_letter | capital_letter ; "
		"small_letter   = 'a'|'b'|'c'|'d'|'e'|'f'|'g'|'h'|'i'|'j'|'k'|'l'|'m'|'n'|'o'|'p'|'q'|'r'|'s'|'t'|'u'|'v'|'w'|'x'|'y'|'z';"
		"capital_letter = 'A'|'B'|'C'|'D'|'E'|'F'|'G'|'H'|'I'|'J'|'K'|'L'|'M'|'N'|'O'|'P'|'Q'|'R'|'S'|'T'|'U'|'V'|'W'|'X'|'Y'|'Z';"
		"digit = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' ;";

	Node* gram = parse_grammar(grammar);	

	/*
	printf("\nentries:\n");
	for (int i = 0; i < st_last_index; ++i) {
		printf("%s\n", st[i].name);
	}
	printf("\n");
	//*/

	init_handlers();

	const char * s[] = {
		" { 'Parametr1 2000' ; [ 5 & 2 ] ; [ 3 & 2 ] }{'ParametrLoooooooong 2021';[10&5];[ 6 &4]}",
		"{'Parametr 2000' ; [ 5&2 ] ; [ 3 & 4 ] }",
		"{ 'Parametr 2000';[ 5 & 2 ];[ 3 &10 ] }"
	};

	char ** sp = (char**) s;
	char ** end = sp + sizeof(s) / sizeof(s[0]);
	for(; sp != end; ++sp) {
		parse_data(gram, *sp);
	}

	for(int i = 0; i < records_last_index; ++i) {
		printf("str %d = %s %d %d %d %d\n", i, records[i].name, records[i].param[0], records[i].param[1], records[i].param[2], records[i].param[3]);
	}
}

