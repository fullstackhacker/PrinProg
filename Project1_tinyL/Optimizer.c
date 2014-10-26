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

int main()
{
	Instruction *head;
	Instruction *instr1, *instr2, *instr3;
	int opt_flag, opt_calc;
	
	head = ReadInstructionList(stdin);

  /* ---  FIRST: ALGEBRAIC SIMPLIFICATION PASS --- */

	if (!head) { //no instructions
		ERROR("No instructions\n");
		exit(EXIT_FAILURE);
	}

	if(head->next && head->next->next){  //can't optimize if there aren't enough
		//initialization
		instr1 = head; 
		instr2 = head->next; 
		instr3 = head->next->next; 
		
		while(instr3){
			//handle the cases of (a + 0)(0 + a)(a - 0)(0 - a)(0 * a)(a * 0)
			switch(instr3->opcode){ 
			case ADD: 
				;// blank statement
				if(instr1->opcode == LOADI && instr2 == LOAD){ // (0 + a) 
					if(instr1->field2 == 0){ 
						instr2->field1 = instr3->field1; //set the register in the load to the new one	
						instr2->next = instr3->next; //move past it
						if(instr3->next) instr3->next->prev = instr2; //if the next one exists, point it to instr2
						instr2->prev = instr1->prev; 
						if(instr1->prev) instr1->prev->next = instr2;  //if the prev one exists, point it to intr2
						if(instr1 == head) head =  instr2; 	
						//free
						free(instr1); 
						free(instr3);
						//new vars
						instr1 = instr2; 
						instr2 = instr1->next; 
						instr3 = instr2->next;
					}
					else{ //move forward
						instr1 = instr2; 
						instr2 = instr3; 
						instr3 = instr3->next;
					}
				} 
				else if(instr1->opcode == LOAD && instr2->opcode == LOADI){ // (a + 0)   
					if(instr2->field2 == 0) {
						instr1->field1 = instr3->field1;  //set it to the register
						instr1->next = instr3->next; 
						if(instr3->next) instr3->next->prev = instr1; 
						//free
						free(instr2); 
						free(instr3);
						//new vars
						instr2 = instr1->next; 
						instr3 = instr2->next;
					} 
					else{ //move forward
						instr1 = instr2; 
						instr2 = instr3; 
						instr3 = instr3->next;
					}
				}
				else{ 
					instr1 = instr2; 
					instr2 = instr3; 
					instr3 = instr3->next;
				}
				break;
			case SUB: 
				; //blank statment
				if(instr1->opcode == LOAD && instr2->opcode == LOADI){  //(a - 0)
					if(instr2->field2 == 0){ 
						instr1->field1 = instr3->field1; //change the registers
						instr1->next = instr3->next; 
						if(instr3->next) instr3->next->prev = instr1; 
						//free
						free(instr2); 
						free(instr3); 
						//new vars
						instr2 = instr1->next; 
						instr3 = instr2->next; 
					} 
					else{ //move forward
						instr1 = instr2; 
						instr2 = instr3; 
						instr3 = instr3->next;
					}
				} 
				else if(instr1->opcode == LOAD && instr2->opcode == LOAD){  //(a - a)
					if(instr1->field2 == instr2->field2){ 
						instr3->opcode = LOADI;  //SUB -> LOADI
						instr3->field2 = 0;  //a-a = 0
						instr3->prev = instr1->prev;  //change prev
						if(instr1->prev) instr1->prev->next = instr3; //change prev->next if have to 
						if(instr1 == head) head = instr3;  //change head if have to 
						//free
						free(instr1); 
						free(instr2); 
						//new vars
						instr1 = instr3; 
						instr2 = instr1->next; 
						instr3 = instr2->next;
					} 
					else{ //move forward
						instr1 = instr2; 
						instr2 = instr3; 
						instr3 = instr3->next;
					}
				}
				else{ 
					instr1 = instr2; 
					instr2 = instr3; 
					instr3 = instr3->next;
				}
				break;
			case MUL:
				;//blank statement
				if(instr1->opcode == LOADI && instr2->opcode == LOAD){ //(0 * a) || (1 * a)
					if(instr1->field2 == 0){ //(0 * a)
						instr1->field1 = instr3->field1; //change the load regsiter
						instr1->next = instr3->next; //skip 2 and 3
						if(instr3->next) instr3->next->prev = instr1; //change the prev pointer 
						//free	
						free(instr2); 
						free(instr3); 
						//repoint
						instr2 = instr1->next; 
						instr3 = instr2->next; 
					}
					else if(instr1->field2 == 1){ //(1 * a)
						instr2->field1 = instr3->field1;  //change register
						instr2->next = instr3->next;  //change the nex pointer
						if(instr3->next) instr3->next->prev = instr2; 
						instr2->prev = instr1->prev; //change prev 
						if(instr1->prev) instr1->prev->next = instr2; 
						if(instr1 == head) head = instr2;  //change head
						//free
						free(instr1); 
						free(instr3); 
						//repoint
						instr1 = instr2; 
						instr2 = instr1->next; 
						instr3 = instr2->next;
					} 
					else{ //move forward
						instr1 = instr2; 
						instr2 = instr3; 
						instr3 = instr3->next;
					}
				}
				else if(instr1->opcode == LOAD && instr2->opcode == LOADI){ //(a * 0) || (a * 1)
					if(instr2->field2 == 0){ 
						instr2->field1 = instr3->field1;  //change register
						instr2->next = instr3->next;  //change the nex pointer
						if(instr3->next) instr3->next->prev = instr2; 
						instr2->prev = instr1->prev; //change prev 
						if(instr1->prev) instr1->prev->next = instr2; 
						if(instr1 == head) head = instr2;  //change head
						//free
						free(instr1); 
						free(instr3); 
						//repoint
						instr1 = instr2; 
						instr2 = instr1->next; 
						instr3 = instr2->next;
					} 
					else if(instr2->field2 == 1){ 
						instr1->field1 = instr3->field1; //change the load regsiter
						instr1->next = instr3->next; //skip 2 and 3
						if(instr3->next) instr3->next->prev = instr1; //change the prev pointer 
						//free	
						free(instr2); 
						free(instr3); 
						//repoint
						instr2 = instr1->next; 
						instr3 = instr2->next; 
					} 
					else{
						instr1 = instr2; 
						instr2 = instr3; 
						instr3 = instr3->next;
					}
				}
				else{ //move forward
					instr1 = instr2; 
					instr2 = instr3; 
					instr3 = instr3->next;
				}
				break;
			default: 
				instr1 = instr1->next; 
				instr2 = instr2->next; 
				instr3 = instr3->next; 
				break;
			}
		}
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
			if(instr1->opcode == LOADI && instr2->opcode == LOADI){
				switch(instr3->opcode){ 
					case ADD: 
						; //labels can only be followed by statements and declarations aren't statements
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
						; //labels can only be followed by statements and declarations aren't statements
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
						; //labels can only be followed by statements and declarations aren't statements
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
						break;
				} 
			} 
			else{ 
				instr1 = instr1->next; 
				instr2 = instr2->next; 
				instr3 = instr3->next;
			}
		}
	} 

	PrintInstructionList(stdout, head);
	DestroyInstructionList(head);
	return EXIT_SUCCESS;
}
