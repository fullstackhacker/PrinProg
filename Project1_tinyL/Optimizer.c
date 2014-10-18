/*
 *********************************************
 *  314 Principles of Programming Languages  *
 *  Fall 2014                                *
 *********************************************
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "InstrUtils.h"
#include "Utils.h"
//#include "Instr.h"

int main()
{
	Instruction *head;
	Instruction *instr1, *instr2, *instr3;
	int opt_flag, opt_calc;
	
	head = ReadInstructionList(stdin);

    /* ---  FIRST: ALGEBRAIC SIMPLIFICATION PASS --- */

	if (!head) {
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}


    /* --- SECOND: CONSTANT FOLDING PASS --- */

	if (!head) { //no instructions!
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}
	
	if(head->next && head->next->next){  //can only do constant folding pass if there is 3 instructions
		//initialization	
		instr1 = head; 
		instr2 = head->next; 
		instr3 = head->next->next; 
	
		while(instr3){
			printf("instruction!");
			if(instr1->opcode == LOADI && instr2->opcode == LOADI){
				switch(instr3->opcode){ 
					case ADD: 
						printf(" optimize ADD");
						//get the sum
						int sum = instr1->field2 + instr2->field2;
						//create a new node
						Instruction *newI = (Instruction *)calloc(1, sizeof(Instruction)); 
						newI->opcode = LOADI;	
						newI->field1 = instr3->field1;	
						newI->field2 = sum; 
						newI->prev = instr1->prev;  //okay if null
						newI->next = instr3->next; //okay if null 
						if(instr3->next) instr3->next->prev = newI; 
						if(instr1->prev) instr1->prev->next = newI; 
						//check if instr1 is the head
						if(instr1 == head) head = newI;  
						//delete them
						free(instr1); 
						free(instr2); 
						free(instr3); 
						instr1 = newI; 
						if(newI->next) instr2 = newI->next; 
						else instr2 = NULL; 
						if(newI->next->next) instr3 = newI->next->next; 
						else instr3 = NULL; 
						break;
					case SUB: 
						printf(" optimize SUB");
						//get the difference
						int diff = instr1->field2 - instr2->field2;
						//create a new node
						newI = (Instruction *)calloc(1, sizeof(Instruction)); 
						newI->opcode = LOADI;	
						newI->field1 = instr3->field1;	
						newI->field2 = diff; 
						newI->prev = instr1->prev;  //okay if null
						newI->next = instr3->next; //okay if null 
						if(instr3->next) instr3->next->prev = newI; 
						if(instr1->prev) instr1->prev->next = newI; 
						//check if instr1 is the head
						if(instr1 == head) head = newI;  
						//delete them
						free(instr1); 
						free(instr2); 
						free(instr3); 
						instr1 = newI; 
						if(newI->next) instr2 = newI->next; 
						else instr2 = NULL; 
						if(newI->next->next) instr3 = newI->next->next; 
						else instr3 = NULL; 
						break;
					case MUL: 
						printf(" optimize MUL");
						//get the product
						int product = instr1->field2 * instr2->field2;
						//create a new node
						newI = (Instruction *)calloc(1, sizeof(Instruction)); 
						newI->opcode = LOADI;	
						newI->field1 = instr3->field1;	
						newI->field2 = product; 
						newI->prev = instr1->prev;  //okay if null
						newI->next = instr3->next; //okay if null 
						if(instr3->next) instr3->next->prev = newI; 
						if(instr1->prev) instr1->prev->next = newI; 
						//check if instr1 is the head
						if(instr1 == head) head = newI;  
						//delete them
						free(instr1); 
						free(instr2); 
						free(instr3); 
						instr1 = newI; 
						if(newI->next) instr2 = newI->next; 
						else instr2 = NULL; 
						if(newI->next->next) instr3 = newI->next->next; 
						else instr3 = NULL; 
						break; 
					default: 
						printf("Nope");
						break;
				} 
			} 
			else{ 
				printf("continuing \n");
				instr1 = instr1->next; 
				instr2 = instr2->next; 
				instr3 = instr3->next;
			}
			printf("\n");
		}

	} 

	/** //works like a charm
	//initialize
	instr1 = head;	
	instr2 = head->next;
	instr3 = head->next->next;

	while(instr3){ 
		instr1 = instr1->next; 
		instr2 = instr2->next; 
		instr3 = instr3->next; 
		printf("moved forward\n");
	}
	// */

	PrintInstructionList(stdout, head);
	DestroyInstructionList(head);
	return EXIT_SUCCESS;
}
