
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Graph.h"
#include <stdbool.h>


struct dictionary
{
    char data[1000][30];
    int size;
    int length[1000];// the max length of chain ended at this word
};

typedef struct dictionary Dictionary;



//  check whether str1 and str2 share an edge in graph
bool differByOne(char * str1, char *str2)
{
    // exactly the same, return false
    if (strcmp(str1, str2) == 0)
    {
        return false;
    }
    else if (strlen(str1) == strlen(str2))
    {
        // find the first letter not equal
        while (*str1 == *str2)
        {
            str1++;
            str2++;
        }
        return !strcmp(++str1, ++str2);
    }
    else if (strlen(str1) == strlen(str2) + 1)
    {
        // find the first letter not equal
        while (*str1 == *str2)
        {
            str1++;
            str2++;
        }
        return !strcmp(++str1, str2);

    }
    else if (strlen(str1) + 1 == strlen(str2))
    {
        // find the first letter not equal
        while (*str1 == *str2)
        {
            str1++;
            str2++;
        }
        return !strcmp(str1, ++str2);
    }
    else
    {
        return false;
    }
}

// read the word from stdin and create a dicitonary
Dictionary createDictionary()
{
    int ret;
    Dictionary d;
    d.size = 0;
    char buffer [40];
    while (true)
    {
        ret=scanf("%s", buffer);
        if (ret != 1)
        {
            break;
        }

        // check whether have duplicate words
        if (d.size > 0)
        {
            if (strcmp(d.data[d.size - 1], buffer) != 0)
            {
                // copy to the dictionary
                strcpy(d.data[d.size++], buffer);
            }
        }
        else
        {
            // copy to the dictionary
            strcpy(d.data[d.size++], buffer);
        }
    }

    // empty file
    if (d.size == 0)
    {
        exit(0);
    }
    return d;
}

// display the dictionary
void printDictionary(Dictionary * d)
{
    printf("Dictionary\n");
    int i;
    for (i = 0; i < d->size; i++)
    {
        printf("%d: %s\n", i, d->data[i]);
    }
}

// create a graph from dictionary
Graph createGraph(Dictionary * d)
{
    Graph g = newGraph(d->size);
    int s1, s2;

    // check each pair of word, and insert to the graph
    for (s1 = 0; s1 < d->size; s1++)
    {
        for (s2 = s1+1; s2 < d->size; s2++)
        {
            if (differByOne(d->data[s1], d->data[s2]))
            {
                Edge e = { s1,s2 };
                insertEdge(e, g);
            }
        }
    }
    return g;
}

void generateLength(Dictionary * d, Graph g)
{
    int s1, s2;
    int maxLen;

    // dynamic programming
    for (s1 = 0; s1 < d->size; s1++)
    {
        maxLen = 0;
        for (s2 = 0; s2 < s1; s2++)
        {
            Edge e = { s1,s2 };
            if (isEdge(e, g) && maxLen < 1+d->length[s2])
            {
                maxLen = 1 + d->length[s2];
            }
        }
        d->length[s1] = maxLen;
    }
}

// print the word in stack
void printStack(Dictionary * d, int s[], int sCount)
{
    static int index = 1;
    int s1;
    // print the word in stack with certain format
    if (index<100)
    {
        printf("%2d: ", index++);
        for (s1 = 0; s1 < sCount - 1; s1++)
        {
            printf("%s -> ", d->data[s[s1]]);
        }
        printf("%s\n", d->data[s[s1]]);
    }
}

// DFS search, retrive the route
void printChain(int current,int s[1000],int sCount, Dictionary * d, Graph g,int maxLen)
{
    int s1;
    s[sCount] = current;
    sCount++;
    if (d->length[current] == maxLen)
    {
        printStack(d,s, sCount);
    }

    for (s1 = current + 1; s1 < d->size; s1++)
    {
        Edge e = {current,s1};
        if (d->length[current] + 1 == d->length[s1] && isEdge(e, g))
        {
            printChain(s1, s, sCount, d, g,maxLen);
        }
    }
}


 
int main( )
{
    int i;
    Dictionary dict = createDictionary();

    // print the dictionary
    printDictionary(&dict);

    Graph gra = createGraph(&dict);

    // print the graph
    printf("Ordered Word Ladder Graph\n");
    showGraph(gra);

    // calculate the max length of chain ended at each word
    generateLength(&dict, gra);

    // find the max length
    int maxLen = 0;
    for (i = 0; i < dict.size; i++)
    {
        if (dict.length[i] > maxLen)
        {
            maxLen = dict.length[i];
        }
    }
    printf("Longest ladder length: %d\n", maxLen + 1);


    printf("Longest ladders:\n");

    int s[1000];
    for (i = 0; i < dict.size; i++)
    {
        if (dict.length[i] == 0)
        {
            printChain(i, s, 0, &dict, gra, maxLen);
        }
    }

    return 0;
}
