#include <stdio.h>
#include <stdlib.h>

char *cur = NULL;
char text[256];

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

void str() { // string
	ex('\'');
	char* tp = text;
	while(*cur != '\'' && *cur != '\0') {
		*tp++ = *cur++;
	}
	*tp = '\0';
	ex('\'');

	printf("%s ", text);
}

void num() { // number
	skip_spaces();

	char* tp = text;
	while(*cur >= '0' && *cur <= '9') {
		*tp++ = *cur++;
	}
	*tp = '\0';

	if (tp == text) {
		printf("\nError: expected number but not '%c'\n", *cur);
		exit(-1);
	}

	printf("%s ", text);
}

void two_p() { // two parameters
	ex('['); num(); ex('&'); num(); ex(']');
}

int record = 0;
void parse_data(const char* s) {
	cur = (char*) s;
	while(*cur != '\0') {
		printf("str %d - ", ++record);
		ex('{'); str(); ex(';'); two_p(); ex(';'); two_p(); ex('}');
		printf("\n");
	}
}

int main() {
	const char * s[] = {
		" { 'Parametr1 2000' ; [ 5 & 2 ] ; [ 3 & 2 ] }{'ParametrLoooooooong 2021';[10&5];[ 6 &4]}",
		"{'Parametr 2000' ; [ 5&2 ] ; [ 3 & 4 ] }",
		"{ 'Parametr 2000';[ 5 & 2 ];[ 3 &10 ] }"
	};

	char ** sp = (char**) s;
	char ** end = sp + sizeof(s) / sizeof(s[0]);
	for(; sp != end; ++sp) {
		parse_data(*sp);
	}
	
}
