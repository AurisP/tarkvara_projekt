#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

void generateEller(short **cells, short **cellsets, int size) {
	srand(time(NULL));
	int wallcounter = 0;
	int RandNr = 2;
	int setcounter = 0;
	int setbuf;
	int setcount;
	for (int i = 1; i < size; i += 2) {
		
		for(int a = 1; a < size; a++) { //Generate unique sets
			if(cellsets[i][a] == 0)
				cellsets[i][a] = RandNr++;
		}
		/*
		for(int e = 0; e < size; e += 2){
			if (cellsets[i][e] == cellsets[i][e+2]){//go through row and add walls between same sets
				cellsets[i][e+1] = 1;
				cells[i][e+1] = 1;
			}
		}
		*/
		
		for(int b = 1; b < size-2; b += 2){ 
			if (i == size - 1){
				if (cellsets[i][b] == cellsets[i][b+2]) 
					cells[i][b+1] = 1;
				else{
					setbuf = cellsets[i][b+2];
					setcount = 1;
					while(setcount != size + 1) { //go thorugh all of the row and assign all sets with right set value to left set value
						if(cellsets[i][setcount] == setbuf){
							cellsets[i][setcount] = cellsets[i][b]; //assign left set value to right and middle (middle not really necessary)
							//cellsets[i][b+setcount - 1] = cellsets[i][b];
							setcount += 2;
						}else{
							setcount += 2;
						}
					}
					
				}
			
			}else if (cellsets[i][b] != cellsets[i][b+2] && cellsets[i][b+1] != 1){ //If rigth cell is not equal then rand to combine or to not combine
				if (rand() % 2) {
					setbuf = cellsets[i][b+2];
					setcount = 1;
					while(setcount != size + 1) { //go thorugh all of the row and assign all sets with right set value to left set value
						if(cellsets[i][setcount] == setbuf){
							cellsets[i][setcount] = cellsets[i][b]; //assign left set value to right and middle (middle not really necessary)
							//cellsets[i][b+setcount - 1] = cellsets[i][b];
							setcount += 2;
						}else{
							setcount += 2;
						}
					}
				}else{
					cells[i][b+1] = 1; //add wall between sets
					cellsets[i][b+1] = 1;
				}
			}else{
				cells[i][b+1] = 1;
				cellsets[i][b+1] = 1;//if they are already equal then wall is needed (already done before)
		
			}
		}
		
		
		
		for (int c = 1; c < size; c += 2) {
			cells[i+1][c+1] = 1; //add walls between row and column sets for filler
			cellsets[i+1][c+1] = 1;
			if (cellsets[i][c] == cellsets[i][c+2] && c != size - 1){//if sets are same. find whole set from row
				setcounter += 2;
			}else{
				for (int d = 0; d <= setcounter; d += 2) {
					//if (cellsets[i][c] == 1) {//skip wall set
						//cells[i+1][c] = 1;
						//break;
					if (d == setcounter && wallcounter == setcounter) { //if at the end of set and all previous floors set
						cellsets[i+2][c - setcounter + d] = cellsets[i][c - setcounter +d]; //atleast 1 cell in set needs a path (no floor)
						//break;
					}else if (rand() % 2) {
						cellsets[i+2][c - setcounter + d] = cellsets[i][c - setcounter +d]; //make path and assing above value to bottom set
					}else{
						cells[i+1][c - setcounter + d] = 1; //if not make path
						cellsets[i+1][c - setcounter + d] = 1;
						wallcounter += 2;
					}	
				}
				setcounter = 0;
				wallcounter = 0;
			}	
		}	
	}
}

void addBorders_Eller( short **cells, int size){
	int flag1, flag2 = 0;
	
	for (int b = 0; b < size; b++)
		cells[b][0] = 1;
	
	for (int a = 0; a < size; a++){
		cells[0][a] = 1;
		if (cells[1][a] == 0 && flag1 == 0) {
			cells[0][a] = 0;
			flag1 = 1;
		}
	}
	
	for (int c = size-2; c >= 0; c--){
		cells[size - 1][c] = 1;
		if (cells[size - 2][c] == 0 && flag2 == 0) {
			cells[size - 1][c] = 0;
			flag2 = 1;
		}
	}
	
	for (int d = 0; d < size; d++)
		cells[d][size - 1] = 1;
}

void Ellermaze(int size, short **maze){
	int i, j;
	short **cellsets = calloc(size + 4, sizeof(short*));
	for(i = 0; i < size + 4; i++) {
		cellsets[i] = calloc(size + 4, sizeof(short));
	}
	generateEller(maze, cellsets, size - 1);
	addBorders_Eller(maze, size);
	for(i = 0; i < size; i++)
		for(j = 0; j < size; j++)
			maze[i][j] = abs(maze[i][j] - 1);
			
	for(i = 0; i < size + 4; i++){
		free(cellsets[i]);
	}
}
