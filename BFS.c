#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
	int key;
	struct Node *next;
} Node;

int dequeue(Node **queue) {
	
	int key;
	Node *nTemp;
	
	key = (*(queue))->key;
	nTemp = *queue;
	
	*queue = (*(queue))->next;
	free(nTemp);
	
	return key;
}

int *initDiscovered(int size)
{
	int *discovered, i;
	discovered = malloc(sizeof(int) * size);
	
	for (i = 0; i < size; i++)
		*(discovered + i) = 0;
	
	return discovered;
}

int *initParent(int size)
{
	int *parent, i;
	parent = malloc(sizeof(int) * size);
	
	for (i = 0; i < size; i++)
		*(parent + i) = -1;
	
	return parent;
}

void purge(Node *list)
{
	Node *nRun = list;
	Node *nTemp;
	
	while (nRun != NULL) {
		nTemp = nRun;
		nRun = nRun->next;
		free(nTemp);
	}
}

Node *add(Node *list, int key)
{
	/* Adds to the adjency list */
	Node *nRun = list;
	
	if (list == NULL) {
		list = malloc(sizeof(Node));
		list->key = key;
		list->next = NULL;
	}
	else {
		while (nRun->next != NULL)
			nRun = nRun->next;
		nRun->next = malloc(sizeof(Node));
		nRun->next->next = NULL;
		nRun->next->key = key;
	}
	
	return list;
}

Node *buildPath(int *parent, int start, int end)
{
	Node *path, *nTemp;
	int vertex;
	path = malloc(sizeof(Node));
	path->key = end;
	path->next = NULL;
	vertex = end;
	
	while (vertex != start) {
		vertex = *(parent + vertex);
		nTemp = path;
		path = malloc(sizeof(Node));
	    path->key = vertex;
	    path->next = nTemp;
	}
	
	return path;
}

Node *solve (Node **adjList, int *discovered, int *parent, int start, int end)
{
	int vertex, bFound = 0;
	Node *frontier = NULL, *nRun;
	frontier = add(frontier, start);
	
	while (frontier != NULL && !bFound)
	{
		vertex = dequeue(&frontier);
		nRun = *(adjList + vertex);
		while (nRun != NULL && !bFound) {
			if (!(*(discovered + nRun->key))) {
				frontier = add(frontier, nRun->key);
				*(parent + nRun->key) = vertex;
				*(discovered + nRun->key) = 1;
				if (nRun->key == end)
					bFound = 1;
			}
			nRun = nRun->next;
		}
	}
	
	free(discovered);
	return buildPath(parent, start, end);
}

int isThereEdge(Node *adjList, int vertex)
{
	int edgeFlag = 0;
	Node *nRun = adjList;
	
	while (nRun != NULL && !edgeFlag)
		if (nRun->key == vertex)
			edgeFlag = 1;
		else
			nRun = nRun->next;
	
	return edgeFlag;
}

void printTop(Node *adjList, int vertex, int length)
{
	if (isThereEdge(adjList, vertex - length))
		printf("  ");
	else if (isThereEdge(adjList, vertex - 1) || vertex % length == 0)
		printf("__");
	else
		printf("__");
}

void printSides(Node *adjList, Node *path, int vertex, int start, int end)
{
	if (isThereEdge(adjList, vertex - 1))
		printf(" ");
	else
		printf("|");
	
	if (vertex == start)
		printf("S");
	else if (vertex == end)
		printf("E");
	else if (isThereEdge(path, vertex))
		printf("*");
	else
		printf(".");
}

void printGraph(Node **adjList, Node *path, int size, int start, int end)
{
	int i, j, length;
	
	length = (int)(sqrt((double)(size)));
	
	for (i = 0; i < length; i++) {
		for (j = i * length; j <= i * length + (length - 1); j++)
			printTop(*(adjList + j), j, length);
		printf("\n");
		for (j = i * length; j <= i * length + (length - 1); j++)
			printSides(*(adjList + j), path, j, start, end);
		printf("|\n");
	}
	
	for (i = 0; i < length; i++)
		printf("__");
	printf("\n");
}
	
int main()
{
	int start, end, size, vertex1, vertex2, i;
	FILE *input;
	Node **adjList, *path, *nRun;
	
	input = fopen("input.txt", "rt");
	
	fscanf(input, "%d", &start);
	fscanf(input, ",%d\n", &end);
	
	fscanf(input, "1..%d\n", &size);
	adjList = malloc(sizeof(Node) * size);
	
	for (i = 0; i < size; i++)
	/* Marks all vertex as undiscoverd */
		*(adjList + i) = NULL;
		
	while ((fscanf(input,"(%d,%d)\n", &vertex1, &vertex2)) == 2) {
	/* Adds to Adjacency List */
		vertex1 -= 1;
		vertex2 -= 1;
		*(adjList + vertex1) = add(*(adjList + vertex1), vertex2);
		*(adjList + vertex2) = add(*(adjList + vertex2), vertex1);
	}
	
	fclose(input);
	
	path = solve(adjList, initDiscovered(size), initParent(size), start - 1, end - 1);
	nRun = path;
	
	printGraph(adjList, path, size, start - 1, end - 1);
	printf("\n");
	while (nRun != NULL) {
		printf("%d ", nRun->key + 1);
		nRun = nRun->next;
	}
	
	for (i = 0; i < size; i++)
		purge(*(adjList + i));
	
	purge(path);
	free(adjList);
	
	getchar();	
}
