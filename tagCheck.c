/*
 * tagCheck.c
 * 
 * Copyright 2016 Matthew Mashewske <matthew@matthew-W65-67SJ>
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
#include <string.h>

typedef struct tag{
	bool isClose;
	bool selfClose;
	int lineNum;
	char* type;
}Tag;

//Stack structs
typedef struct node
{
    Tag* datum;
    struct node* next;
}Node;
typedef struct stack
{
    Node* top;
}Stack;

//Stack functions initialize
bool isEmpty(Stack* s);
Tag* pop(Stack* s);
void push(Stack* s, Tag* newItem);
Tag* peek(Stack* s);
void printStack(Node* s);
size_t size(Stack* s);
Node* makeNode(Tag* x);

//
const size_t LINE_MAX = 4096;
int main(int argc, char** argv)
{
    char* html = argv[1];
	
	Stack * tags = malloc(sizeof(Stack));
    tags -> top = NULL;
    
    Stack * closed = malloc(sizeof(Stack));
    closed -> top = NULL;
    
    Stack * orderedClosed = malloc(sizeof(Stack));
    orderedClosed -> top = NULL;
    FILE* fp = fopen(html, "r");
    if(fp==NULL){
        fprintf(stderr, "Oh boy something went wrong with %s\n", html);
        exit(1);
    }
    char buf[LINE_MAX+1];
    int lineNum = 1;
    while(fgets(buf, LINE_MAX, fp) != NULL)
    {
		char* reading = buf;
		for(size_t place = 0; place < strlen(buf);)
		{
			printf("Reading: %zu:%zu:%d\n", place,LINE_MAX,lineNum);
			
			printf(reading); 
			size_t openPlace = strchr(reading+place,'<')-reading;
			size_t closePlace = strchr(reading+place,'>')-reading;
			
			printf("Open place: %zu, Close place:%zu\n", openPlace,closePlace);
			if(closePlace != openPlace && closePlace > openPlace && closePlace < LINE_MAX && openPlace < LINE_MAX && reading[openPlace + 1] != '!')
			{
				Tag * newTag = malloc(sizeof(Tag));
				switch(reading[openPlace+1]=='/')
				{
					case true:
						openPlace+=1;
						newTag->isClose=true;
						break;
					case false:
						newTag->isClose=false;
						break;
				}
				newTag -> lineNum = lineNum;
				char* newTypeName = malloc(sizeof(char)*closePlace-openPlace+1);
				for(int i = 0; i < closePlace-openPlace-1;i++)
				{
					if(reading[openPlace+i+1] == ' ')
					{
						break;
					}
					newTypeName[i] = reading[openPlace+i+1];
				}
				newTypeName[closePlace-openPlace+1] = '\0';
				printf("%s\n", newTypeName);
				newTag -> type = newTypeName;
				if(newTag -> isClose)
				{
					push(closed,newTag);
				}
				else
				{
					push(tags,newTag);
				}
				place += closePlace+1;
				continue;
			}
			else
			{
				break;
			}
		}
	lineNum++;
	}
	fclose(fp);
	
	printf("Going through\n");
	while( !(isEmpty(tags)) && !(isEmpty(closed)))
	{
		Tag* openTag = pop(tags);
		Tag* closeTag = pop(closed);

		printf("Opentype: '%s', Closetype: '%s'\n\n", openTag->type, closeTag->type);
		if( strcmp(openTag->type,closeTag->type) != 0 )
		{
			printf("ERROR! Tag of Type '%s' on line %d does not have a proper closing tag! Closing tag should be of type '%s', but was found to be of type '%s'!\n", openTag->type, openTag->lineNum,openTag->type, closeTag->type);
		}
		free(openTag->type);
		free(closeTag->type);
		free(openTag);
		free(closeTag);
	}
	
	if( !(isEmpty(tags)) )
	{
		printf("You do not have enough closing tags!\n");
		while( !(isEmpty(tags)) )
		{
			Tag* erase = pop(tags);
			free(erase->type);
			free(erase);
		}
	}
	else if( !(isEmpty(closed)) )
	{
		printf("You have too many closing tags!\n");
		while( !(isEmpty(closed)) )
		{
			Tag* erase = pop(closed);
			free(erase->type);
			free(erase);
		}
	}
	else
	{
		printf("Your tags are okay!\n");
	}

	free(tags);
	free(closed);
	free(orderedClosed);
    return 0;
}

//Stack functions

bool isEmpty(Stack* s)
{
    //what does an empty stack look like?
    //the top node is null, says Mashewske.
    return s -> top == NULL;
}
Node* makeNode(Tag* x)
{
    //Let's make a heap o' objects
    Node* out = malloc(sizeof(Node));
    out-> datum = x;
    return out;
}
void push(Stack* s, Tag* newItem)
{
    Node* n = makeNode(newItem);//put on clothing; this is not a nudist camp
    n -> next = s -> top;
    s-> top = n;
}


Tag* peek(Stack* s)
{
    return s -> top -> datum;
}

Tag* pop(Stack* s)
{
    #ifdef DEBUG
    assert(s != NULL);
    #endif 
    //NO LEAKS!
    Node* choppingBlock = s -> top;
    Tag* out = s -> top -> datum;//get datum before chopping the top
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
