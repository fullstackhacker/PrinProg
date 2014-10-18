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
	
	instr1 = head; //point it at the head
	if(head->next && head->next->next){ 
		instr2 = head->next; 
		instr3 = head->next->next; 
		
		while(){
		
		}
	} 

	PrintInstructionList(stdout, head);
	DestroyInstructionList(head);
	return EXIT_SUCCESS;
}
