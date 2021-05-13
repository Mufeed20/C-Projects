#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#define MAXOP 100   // max size of operator or operand 
#define NUMBER '0'  // signal that a number was found 
#define NAME 'n'	// signal that a name was found

int getop(char []);
void push(double);
double pop(void);
void mathfun(char []);

// reverse polish calculator
 main()
{
	int type;
	double op2;
	char s[MAXOP];
	
	while((type = getop(s)) != EOF)
	{
		switch(type)
		{
			case NUMBER:
				push(atof(s));
				break;
			case NAME:
				mathfun(s);
				break;
			case '+':
				push(pop() + pop());
				break;
			case '*':
				push(pop() * pop());
				break;
			case '-':
				op2 = pop();
				push(pop() - op2);
				break;
			case '/':
				op2 = pop();
				if(op2 != 0.0)
					push(pop() / op2);
				else 
					printf("error: zero divisor\n");
				break;
			case '%':
				op2 = pop();
				if(op2 != 0.0)
					push((int)pop() % (int)op2);
				else 
					printf("error: zero divisor\n");
				break;
			case '\n':
				printf("ANSWER :%.4f\n", pop());
				break;
			default:
				printf("error: unknown command %s\n", s);
				break;
		}
	}
	return 0;
}

// mathfun: check string s for supported math functions
void mathfun(char s[])
{
	double op2;
	
	if(strcmp(s, "sin") == 0)
		push(sin(pop()));
	else if(strcmp(s, "cos") == 0)
		push(cos(pop()));
	else if(strcmp(s, "tan") == 0)
		push(tan(pop()));
	else if(strcmp(s, "cosec") == 0)
		op2 = pop();
		if(sin(op2) != 0)
			push(1 / sin(op2));
	else if(strcmp(s, "sec") == 0)
		op2 = pop();
		if(cos(op2) != 0)
			push(1 / cos(op2));
	else if(strcmp(s, "cot") == 0)
		op2 = pop();
		if(tan(op2) != 0)
			push(1 / tan(op2));
	else if(strcmp(s, "log") == 0)
		push(log(pop()));
	else if(strcmp(s, "sqrt") == 0)
		if((op2 = pop()) >= 0)
			push(sqrt(op2));
	else if(strcmp(s, "exp") == 0)
		push(exp(pop()));
	else if(strcmp(s, "pow") == 0)
	{
		op2 = pop();
		push(pow(pop(), op2));
	}
	else
		printf("error: %s is not supported\n", s);
}

#define MAXVAL 100   // maximum depth of val stack 

int sp = 0;			 // next free stack position
double val[MAXVAL];  // value stack

// push : push f onto value stack 
void push(double f)
{
	if(sp < MAXVAL)
		val[sp++] = f;
	else 
		printf("error: stack full, can't push %g\n", f);
}

// pop : pop and return top value from stack
double pop(void)
{
	if(sp > 0)
		return val[--sp];
	else
	{
		printf("error: stack empty\n");
		return 0.0;
	}
}

int getch(void);
void ungetch(int);

// getop : get next operator or numeric operand  or math function
int getop(char s[])
{
	int i, c;
	
	while((s[0] = c = getch()) == ' ' || c == '\t')
		;
		
	s[1] = '\0';
	i = 0;
	if(islower(c))				 // command or name 
	{
		while(islower(s[++i] = c = getch()))
			;
		s[i] = '\0';
		if(c != EOF)
			ungetch(c);			 // wemt one char too far
		if( strlen(s) > 1)
			return NAME;		 // >1 char; It is name 
		else
			return c;
	}
	if(!isdigit(c) && c != '.' && c != '-')
		return c;                // not a number
	if(c == '-')
	{
		if(isdigit(c = getch()) || c == '.')
			s[++i] = c;
		else 
		{
			if(c != EOF)
				ungetch(c);
			return '-';
		}
	}
	if(isdigit(c))				 // collect integer part 
	{
		while(isdigit(s[++i] = c = getch()))
			;
	}
	if(c == '.')				// collect fraction part
	{
		while(isdigit(s[++i] = c = getch()))
			;
	}
	s[i] = '\0';
	if(c != EOF)
		ungetch(c);
	return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE];				// buffer for ungetch
int bufp = 0;					// next free position in buf 

int getch(void)					// get a (possibly pushed back) character 
{
	return (bufp>0) ? buf[--bufp] : getchar();	
}

void ungetch(int c)				// push character back on input
{
	if(bufp >= BUFSIZE)
		printf("ungetch: too many characters\n");
	else 
		buf[bufp++] = c;	
}	
