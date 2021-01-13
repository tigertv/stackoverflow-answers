#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ENTRIES 128
void handler_func(int index, char *s , int size); 

char *cur = NULL;
	
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
	char *p = "%";
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

			//printf("terminal = '%c'\n", c);

			Node* term = malloc(sizeof(Node));
			term->value = c; 
			//if (c >= '0' && c <= '9') {
			//	term->type = DIGIT;			
			//} else {
				term->type = TERM;			
			//}
			ex('\'');

			if (pnode) {
				Node* anode = malloc(sizeof(Node));
				anode->type = AND;
				anode->left = pnode;
				anode->right = term;
				pnode = anode;
			} else {
				pnode = term;
			}
			skip_spaces();

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
			skip_spaces();
			
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
			skip_spaces();
			return pnode;
		} else if (*cur == ' ') {
			printf("SPACE HERE");
			skip_spaces();
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
			skip_spaces();
		}

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
		if(root_entry->node->type == NON_TERM) {
			Node* p = root_entry->node;
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
		skip_spaces();
	}

	struct st_entry* entry = find_entry("digit");
	if (entry && entry->node->type == NON_TERM) {
		// set "digit" to default if there is no NON_TERMINAL "digit"
		// NON_TERM node shouldn't be presented in the end of creating of grammar AST
		Node* p = entry->node;
		p->type = DIGIT;
	}

	entry = find_entry("letter");
	if (entry && entry->node->type == NON_TERM) {
		// set "letter" to default if there is no NON_TERMINAL "letter"
		// NON_TERM node shouldn't be presented in the end of creating of grammar AST
		Node* p = entry->node;
		p->type = LETTER;
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
			return visit(node->left);
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
			//skip_spaces();
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
			while(visit(node->left)) {
				tmp = cur;
			}
			cur = tmp;
			//printf("REPEAT end: %c\n", *cur);
			return true;
		}
			break;

		case PARAM:
			skip_spaces();
			//printf("PARAM: %s\n", node->id);
			{
				// remember string
				char* begin_visit = cur;
				bool r = visit(node->left);
				int diff = (int)(cur - begin_visit);
				handler_func(node->param_value, begin_visit, diff);
				return r;
			}
			break;

		case AND: {
			skip_spaces();
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
			skip_spaces();
			//printf("OR\n");
			//printf("OR cur = %c\n", *cur);
			char* tmp = cur;	
			bool a = visit(node->left);
			//printf("OR a cur = %c\n", *cur);
			//printf("OR: a = %d\n", a);
			if (a) return true;
			cur = tmp;
			bool b = visit(node->right);
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

void handler_func(int index, char *s , int size) {
	handlers[index](s, size);
}

void parse_data(Node* gram, const char* s) {
	cur = s;
	while(*cur != '\0') {
		bool result = visit(gram);
		//printf("PARSE_DATA result = %d\n", result);
		current_param_index = 0;	
		if (result) {
			// make record
			++records_last_index;
		} else {
			break;
		}
	}
}

int main() {
	const char *grammar = 
		"record = '{' pstring ';' two_param ';' two_param '}' ;"
		"pstring = ''' %0 ''' ;"
		"%0 = {char} ;"
		"char = letter | ' ' | digit ;"
		"two_param = '[' %1 '&' %1 ']' ;"
		"%1 = {digit} ;"
	;
	// "digit" is by default
	// "letter" is by default

	Node* gram = parse_grammar(grammar);	

	//*
	printf("\nentries:\n");
	for (int i = 0; i < st_last_index; ++i) {
		printf("entry: %s\n", st[i].name);
	}
	printf("\n");
	//*/

	init_handlers();

	const char * s[] = {
		" { 'Parametr1 2001' ; [ 5 & 2 ] ; [ 3 & 2 ] }{'ParametrLoooooooong 2021';[10&5];[ 6 &4]}",
		"{'Parametr 2002' ; [ 5&2 ] ; [ 3 & 4 ] }",
		"{ 'Parametr 2003';[ 5 & 2 ];[ 3 &10 ] }"
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

