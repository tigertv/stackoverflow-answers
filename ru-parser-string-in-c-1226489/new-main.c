#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

char *cur = NULL;

#define HANDLERS_SIZE 256
void (*handlers[HANDLERS_SIZE])(char *s , int size);
	
typedef struct node {
	enum node_type {TERM, NON_TERM, PARAM, OR, AND} type;
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

struct st_entry st[256] = {0};
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
	while (*cur == ' ') ++cur;
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

			root_entry = add_entry(record, anode);
		}

		ex('=');
		skip_spaces();

		enum node_type operation = AND;
		Node* pnode = NULL;

		while (*cur != '\0' && *cur != ';') { 
			//printf("operation = %d\n", operation);
			if (*cur == '\'') {
				// terminal
				++cur;
				char c = *cur++;
//				printf("terminal = %c\n", c);
				ex('\'');

				Node* term = malloc(sizeof(Node));
				term->type = TERM;			
				term->value = c; 

				if (pnode) {
					Node* anode = malloc(sizeof(Node));
					anode->type = operation;
					anode->left = pnode;
					anode->right = term;
					pnode = anode;
				} else {
					pnode = term;
				}

			} else if (*cur == '|') {
				operation = OR;
				++cur;
				skip_spaces();
				continue;
			} else if (*cur == '{') {
				// push a node on stack
			} else if (*cur == '}') {
				// pop a node on stack
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

					entry = add_entry(name, anode);

					int num = st_num_param(name);

					if (num != -1) {
						entry->node->type = PARAM;
						entry->node->param_value = num;
					}
				}

				
				if (pnode) {
					Node* anode = malloc(sizeof(Node));
					anode->type = operation;
					anode->left = pnode;
					anode->right = entry->node;
					pnode = anode;
				} else {
					pnode = entry->node;
				}
			}
			skip_spaces();
			operation = AND;
		}


		root_entry->node->left = pnode;

		if (*cur == ';') ++cur;

		skip_spaces();
	}

	return st[0].node;
	//return st[st_last_index - 1].node;
}


char * visit_str = NULL;

void visit_skip_spaces() {
	while (*visit_str == ' ') ++visit_str;
}

bool visit(Node* node) {
	if (node == NULL) {
		printf("Node is NULL\n");
		return false;
	}
	switch(node->type) {
		case TERM:
			visit_skip_spaces();

			if (node->value == *visit_str) {
				printf("term=%c\n", node->value);
				//printf("equal\n");
				++visit_str;
				return true;
			} 
			//printf("Expected '%c' but not '%c'\n", node->value, *visit_str);
			return false;
			break;

		case NON_TERM:
			printf("NON TERMINAL: %s\n", node->id);
			return visit(node->left);
			break;

		case PARAM:
			printf("PARAM: %s\n", node->id);
			{
				// remember string
				visit_skip_spaces();
				char* begin_visit = visit_str;
				bool r = visit(node->left);
				int diff = (int)(visit_str - begin_visit);
				handlers[node->param_value](begin_visit, diff);
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
			bool a = visit(node->left);
			//printf("OR: a = %d\n", a);
			if (a) return true;
			bool b = visit(node->right);
			//printf("OR: b = %d\n", b);
			return a || b;
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

void dummy_func(char* s, int size) {
	printf("DUMMY FUNCTION executed\nstr=");
	while(size != 0) {
		printf("%c", *s++);
		--size;
	}
	printf("\n");
}

void param0(char* s, int size) {
	printf("param0 executed\nstr=");
	char* name = malloc(size + 1);
	char* n = name;
	while(size != 0) {
		*n++ = *s;
		printf("%c", *s++);
		--size;
	}
	printf("\n");
	*n = '\0';
	records[records_last_index].name = name;
}

void param1(char* s, int size) {
	int r = atoi(s);
	records[records_last_index].param[current_param_index++] = r;
}

void empty_func(char* s, int size) {
	
}

void init_handlers() {
	handlers[0] = param0;
	handlers[1] = param1;

	for(int i = 2; i < HANDLERS_SIZE; ++i) {
		handlers[i] = empty_func;
		//handlers[i] = dummy_func;
	}
}

int main() {
	const char *grammar = 
		"record = '{' pstring ';' two_param ';' two_param '}' ;"
		"two_param = '[' %1 '&' %1 ']' ;"
		"%1 = digit ;"
		"pstring = ''' %0 ''' ;"
		"string = ''' char char ''' ;"
		"%0 = char char ;"
		"char = letter | digit ;"
		"letter = small_letter | capital_letter ; "
		"small_letter = 'a' | 'b' | 'c' | 'd' ;"
		"capital_letter = 'A'|'B'|'C'|'D';"
		"digit = '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9' ;";

	Node* gram = parse_grammar(grammar);	

	printf("\nentries:\n");
	for (int i = 0; i < st_last_index; ++i) {
		printf("%s\n", st[i].name);
	}
	printf("\n");

	init_handlers();

	char * in =  
		"{ 'b1' ; [ 2 & 7 ] ; [1&3] }"
		"{ 'Bc' ; [ 3 & 9 ] ; [6&4] }"
		"{ '27' ; [ 3 & 8 ] ; [7&4] }"
		"{ 'ad' ; [ 1 & 0 ] ; [ 3 & 1 ] }";

	visit_str = in;
	while(*visit_str != '\0') {
		bool result = visit(gram);
		printf("result = %d\n", result);
		if (result) {
			// make record
			current_param_index = 0;	
			++records_last_index;
		} else {
			break;
		}
	}

	for(int i = 0; i < records_last_index; ++i) {
		printf("str %d = %s %d %d %d %d\n", i, records[i].name, records[i].param[0], records[i].param[1], records[i].param[2], records[i].param[3]);
	}
/*
	const char * s[] = {
		" { 'Parametr1 2000' ; [ 5 & 2 ] ; [ 3 & 2 ] }{'ParametrLoooooooong 2021';[10&5];[ 6 &4]}",
		"{'Parametr 2000' ; [ 5&2 ] ; [ 3 & 4 ] }",
		"{ 'Parametr 2000';[ 5 & 2 ];[ 3 &10 ] }"
	};
	//*/
}

