/*
 * tags.c
 * 
 * Ashvin Warrier
 * Partner: Matthew Mashewske
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct tag{	
	bool isClose;
	bool selfClose;
	int lineNum;
	char* type;
}Tag;

//Stack structs
typedef struct node
{
    Tag datum;
    struct node* next;
}Node;
typedef struct stack
{
    Node* top;
}Stack;

//Stack functions initialize
bool isEmpty(Stack* s);
Tag pop(Stack* s);
void push(Stack* s, Tag newItem);
Tag peek(Stack* s);
void printStack(Node* s);
size_t size(Stack* s);
Node* makeNode(Tag x);


void check(Stack tags[], len)
{
	
	
}


//
const size_t LINE_MAX = 4096;
int main(int argc, char** argv)
{
    char* html = argv[1];
	
    Stack * tags = malloc(sizeof(Stack));
    tags -> top = NULL;
    
    Stack * broken = malloc(sizeof(Stack));
    broken -> top = NULL;
    
    FILE* fp = fopen(html, "r");
    if(fp==NULL){
        fprintf(stderr, "Oh boy something went wrong with %s\n", html);
        exit(1);
    }
    char buf[LINE_MAX+1];
    int lineNum;
    while(fgets(buf, LINE_MAX, fp) != NULL)
    {
		Tag * newTag = malloc(sizeof(Tag));
		newTag -> lineNum = lineNum;
	}
    return 0;
}

//Stack functions

bool isEmpty(Stack* s)
{
    //what does an empty stack look like?
    //the top node is null, says Mashewske.
    return s -> top == NULL;
}
Node* makeNode(Tag x)
{
    //Let's make a heap o' objects
    Node* out = malloc(sizeof(Node));
    out-> datum = x;
    return out;
}
void push(Stack* s, Tag newItem)
{
    Node* n = makeNode(newItem);//put on clothing; this is not a nudist camp
    n -> next = s -> top;
    s-> top = n;
}


Tag peek(Stack* s)
{
    return s -> top -> datum;
}

Tag pop(Stack* s)
{
    #ifdef DEBUG
    assert(s != NULL);
    #endif 
    //NO LEAKS!
    Node* choppingBlock = s -> top;
    Tag out = s -> top -> datum;//get datum before chopping the top
    s -> top = s -> top -> next;
    free(choppingBlock);
    return out;
}
size_t size(Stack* s)
{
    int population = 0;
    Node* nav = s -> top;
    for(; nav != NULL; nav = nav -> next )
    {
        population++;
    }
    return population;
}
