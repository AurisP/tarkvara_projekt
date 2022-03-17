#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    int P1;
    int P2;
}cellsets_s;

void findneighbours(int P1, int P2, int *neighbours, int *neighbourcount, int **cells, int length) {
	*neighbourcount = 0;
	if(P2 + 2 <= length && cells[P1][P2+2] == 0){//Checks all neigbours and puts 1 in array if there ia a free neighbour in that position
		neighbours[1] = 1;
		(*neighbourcount)++;
	}
	if(P1 + 2 <= length && cells[P1+2][P2] == 0){
		neighbours[2] = 1;
		(*neighbourcount)++;
	}
	if(P1 - 2 >= 1 && cells[P1-2][P2] == 0) {
		neighbours[0] = 1;
		(*neighbourcount)++;
	}
	if(P2 - 2 >= 1 && cells[P1][P2-2] == 0) {
		neighbours[3] = 1;
		(*neighbourcount)++;
	}
}

int searchforcell (int** cells, int length, int* P1, int* P2, int *neighbours, int *neighbourcount){
	int start1 = (rand() % (length-1))+1;
	int start2 = (rand() % (length-1))+1;
	if(start1 % 2 == 0)
		start1++;
	if(start2 % 2 == 0)
        start2++;
	int start1mem = start1;
	int start2mem = start2;
	findneighbours(start1, start2, neighbours, neighbourcount, cells, length);
	while(!(cells[start1][start2] == 1 && *neighbourcount != 0)) {
		if (start1 == length - 1 && start2 == length - 1){
			start1 = 1;
			start2 = 1;
		}else if(start2 == length-1){
			start1 += 2;
			start2 = 1;
		}
		start2 += 2;
		findneighbours(start1, start2, neighbours, neighbourcount, cells, length);
		if (start1mem == start1 && start2mem == start2)
			return 0;
	}
	*P1 = start1;
	*P2 = start2;

	return 1;
}

int searchforcell_backtracer (int** cells, int length, int* P1, int* P2, int *neighbours, int *neighbourcount, cellsets_s *cellsets, int counter){
    while(counter != 0){
        findneighbours(cellsets[counter].P1, cellsets[counter].P2, neighbours, neighbourcount, cells, length);
        if (*neighbourcount != 0){
            *P1 = cellsets[counter].P1;
            *P2 = cellsets[counter].P2;
            return 1;
        }
    counter--;
    }
    return 0;
}

void generateTree(int **cells, cellsets_s *cellsets, int length) {
	
	int* neighbours = calloc(4, sizeof(int*));
	int neighbourcount = 0;
	srand(time(NULL));
	int randneighbour;
	int counter = 1;
	int P1 = (rand() % (length-2))+1;
	int P2 = (rand() % (length-2))+1;
    if (P1 % 2 == 0) {
        P1++;
    }
    if (P2 % 2 == 0) {
        P2++;
    }
	int cellcounter = 0;

	cellsets[counter].P1 = P1;
    cellsets[counter].P2 = P2;
    counter++;
    cells[P1][P2] = 1;
	while(cellcounter <= length*length/2) {
        for (int s = 0; s < 4; s++)
            neighbours[s] = 0;
        findneighbours(P1, P2, neighbours, &neighbourcount, cells, length);

        if (neighbourcount != 0) { //if there are no neighbour cells then skip code in brackets

            randneighbour = 1;
            if (neighbourcount > 1)
                randneighbour = (rand() % (neighbourcount) + 1); //randomly picks a free neighbour
            int i = 0;
            while (randneighbour > 0) {//Finds the correct chosen neighbour, stores position to poscounter
                if (neighbours[i] == 1) {
                    i++;
                    randneighbour--;
                } else {
                    i++;
                }
            }

            switch (i - 1) {//goes to neighbour and carves path. Updates P1 and P2 to neighbours position
                case 0:
                    cellsets[counter].P1 = P1-2;
                    cellsets[counter].P2 = P2;
                    counter++;
                    cells[P1 - 1][P2] = 1;
                    P1 -= 2;
                    break;
                case 1:
                    cellsets[counter].P1 = P1;
                    cellsets[counter].P2 = P2+2;
                    counter++;
                    cells[P1][P2 + 1] = 1;
                    P2 += 2;
                    break;
                case 2:
                    cellsets[counter].P1 = P1+2;
                    cellsets[counter].P2 = P2;
                    counter++;
                    cells[P1 + 1][P2] = 1;
                    P1 += 2;
                    break;
                case 3:
                    cellsets[counter].P1 = P1;
                    cellsets[counter].P2 = P2-2;
                    counter++;
                    cells[P1][P2 - 1] = 1;
                    P2 -= 2;
                    break;
            }

            cellcounter++;
            cells[P1][P2] = 1;
        }else if (!searchforcell_backtracer(cells, length, &P1, &P2, neighbours, &neighbourcount, cellsets, counter-1)) //if there is no free neighbour cell then search for new starting point, if all cells taken, then exit function
            return;
	}
}

void printTree(int **cells, int length, cellsets_s *cellsets, FILE *fpsvg){
	fprintf(fpsvg, "<?xml version=\"1.0\"?>\n");
	fprintf(fpsvg, "<svg width=\"%dcm\" height=\"%dcm\" viewBox=\"0 0 %d %d\"\n", length, length, length*5, length *5);
	fprintf(fpsvg, "\txmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");
	fprintf(fpsvg, "<g>");
	for (int a = 0; a < length; a++) {
		for (int b = 0; b < length; b++) {
			if(cells[a][b] == 1) {
				fprintf(fpsvg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:white\" />\n", b, a);		
			}else{
				fprintf(fpsvg, "<rect x = \"%d\" y = \"%d\" width=\"1\" height=\"1\" style=\"fill:black\" />\n", b, a);
			}
		}
	}
	fprintf(fpsvg, "</g>");
	fprintf(fpsvg, "</svg>\n");
}

void AddBorders_Tree( int **cells, int length){
	int flag1, flag2 = 0;
	
	for (int b = 0; b < length; b++)
		cells[b][0] = 0;
	
	for (int a = 0; a < length; a++){
		cells[0][a] = 0;
		if (cells[1][a] == 1 && flag1 == 0) {
			cells[0][a] = 1;
			flag1 = 1;
		}
	}
	
	for (int c = length-2; c >= 0; c--){
		cells[length - 1][c] = 0;
		if (cells[length - 2][c] == 1 && flag2 == 0) {
			cells[length - 1][c] = 1;
			flag2 = 1;
		}
	}
	
	for (int d = 0; d < length; d++)
		cells[d][length-1] = 0;
}

double treemaze(char *filename, int length, int **maze){
	FILE *fpsvg = fopen(filename, "w");
	if(fpsvg == NULL){
		printf("failed to open file\n");
		exit(0);
	}

    cellsets_s *cellsets = calloc((length + 4)*(length + 4), sizeof(cellsets_s));

	 
	generateTree(maze, cellsets, length - 1);
	AddBorders_Tree(maze, length);
	printTree(maze, length, cellsets, fpsvg);
    free(cellsets);
    double time_spent = 0;//(end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / BILLION;
    return time_spent;
}