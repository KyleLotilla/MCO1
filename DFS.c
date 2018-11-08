#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
	int key;
	struct Node *next;
} Node;

void purge(Node *adjList)
{
	Node *nRun = adjList;
	Node *nTemp;
	
	while (nRun != NULL)
	{
		nTemp = nRun;
		nRun = nRun->next;
		free(nTemp);
	}
}

Node *add(Node *adjList, int key)
{
	/* Adds to the adjency list */
	int i;
	Node *nRun = adjList;
	
	if (adjList == NULL) {
		adjList = malloc(sizeof(Node));
		adjList->key = key;
		adjList->next = NULL;
	}
	else {
		for (i = 0; nRun->next != NULL; i++)
			nRun = nRun->next;
		nRun->next = malloc(sizeof(Node));
		nRun->next->next = NULL;
		nRun->next->key = key;
	}
	
	return adjList;
}

Node *solve (Node **adjList, int *discovered, int vertex, int end)
{
	Node *path = NULL;
	Node *nRun = *(adjList + vertex);
	Node *nTemp = NULL;
	
	*(discovered + vertex) = 1; // Marks the vertex as discovered
	
	if (vertex == end) {
		/* Base Case: The end vertex is reached, the list for the path is now created */
		path = malloc(sizeof(Node));
		path->key = vertex;
		path->next = NULL;
		return path;
	}
	
	while (nRun != NULL) {
		if (!(*(discovered + nRun->key))) {
			/* Only searchs undiscovered vertices */
			nTemp = solve(adjList, discovered, nRun->key, end);
			if (nTemp != NULL) {
				/* The vertex is being added to the path */
				path = malloc(sizeof(Node));
				path->key = vertex;
				path->next = nTemp;
				return path;
			}
		}
		nRun = nRun->next;
	}
		
	return path; //Returns NULL if the end vertex is not reached and the Vertex searched all of its adjacent vertex
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
	int start, end, size, vertex1, vertex2, i, *discovered;
	FILE *input;
	Node **adjList, *path, *nRun;
	
	input = fopen("input.txt", "rt");
	
	fscanf(input, "%d", &start);
	fscanf(input, ",%d\n", &end);
	
	fscanf(input, "1..%d\n", &size);
	adjList = malloc(sizeof(Node) * size);
	discovered = malloc(sizeof(Node) * size);
	
	
	for (i = 0; i < size; i++) {
	/* Marks all vertex as undiscoverd */
		*(adjList + i) = NULL;
		*(discovered + i) = 0;
	}
		
	while ((fscanf(input,"(%d,%d)\n", &vertex1, &vertex2)) == 2) {
	/* Adds to Adjacency List */
		vertex1 -= 1;
		vertex2 -= 1;
		*(adjList + vertex1) = add(*(adjList + vertex1), vertex2);
		*(adjList + vertex2) = add(*(adjList + vertex2), vertex1);
	}
	
	fclose(input);
	
	path = solve(adjList, discovered, start - 1, end - 1);
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
