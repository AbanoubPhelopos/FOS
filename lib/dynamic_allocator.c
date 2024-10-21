/*
 * dynamic_allocator.c
 *
 *  Created on: Sep 21, 2023
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"


//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//=====================================================
// 1) GET BLOCK SIZE (including size of its meta data):
//=====================================================
__inline__ uint32 get_block_size(void* va)
{
	uint32 *curBlkMetaData = ((uint32 *)va - 1) ;
	return (*curBlkMetaData) & ~(0x1);
}

//===========================
// 2) GET BLOCK STATUS:
//===========================
__inline__ int8 is_free_block(void* va)
{
	uint32 *curBlkMetaData = ((uint32 *)va - 1) ;
	return (~(*curBlkMetaData) & 0x1) ;
}

//===========================
// 3) ALLOCATE BLOCK:
//===========================

void *alloc_block(uint32 size, int ALLOC_STRATEGY)
{
	void *va = NULL;
	switch (ALLOC_STRATEGY)
	{
	case DA_FF:
		va = alloc_block_FF(size);
		break;
	case DA_NF:
		va = alloc_block_NF(size);
		break;
	case DA_BF:
		va = alloc_block_BF(size);
		break;
	case DA_WF:
		va = alloc_block_WF(size);
		break;
	default:
		cprintf("Invalid allocation strategy\n");
		break;
	}
	return va;
}

//===========================
// 4) PRINT BLOCKS LIST:
//===========================

void print_blocks_list(struct MemBlock_LIST list)
{
	cprintf("=========================================\n");
	struct BlockElement* blk ;
	cprintf("\nDynAlloc Blocks List:\n");
	LIST_FOREACH(blk, &list)
	{
		cprintf("(size: %d, isFree: %d)\n", get_block_size(blk), is_free_block(blk)) ;
	}
	cprintf("=========================================\n");

}
//
////********************************************************************************//
////********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

bool is_initialized = 0;
//==================================
// [1] INITIALIZE DYNAMIC ALLOCATOR:
//==================================
void initialize_dynamic_allocator(uint32 daStart, uint32 initSizeOfAllocatedSpace)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		if (initSizeOfAllocatedSpace % 2 != 0) initSizeOfAllocatedSpace++; //ensure it's multiple of 2
		if (initSizeOfAllocatedSpace == 0)
			return ;
		is_initialized = 1;
	}
	//==================================================================================
	//==================================================================================

	//TODO: [PROJECT'24.MS1 - #04] [3] DYNAMIC ALLOCATOR - initialize_dynamic_allocator
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("initialize_dynamic_allocator is not implemented yet");
	//Your Code is Here...

}
//==================================
// [2] SET BLOCK HEADER & FOOTER:
//==================================
void set_block_data(void* va, uint32 totalSize, bool isAllocated)
{
	//TODO: [PROJECT'24.MS1 - #05] [3] DYNAMIC ALLOCATOR - set_block_data
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("set_block_data is not implemented yet");
	//Your Code is Here...
}


//=========================================
// [3] ALLOCATE BLOCK BY FIRST FIT:
//=========================================
void *alloc_block_FF(uint32 size)
{
	//==================================================================================
	//DON'T CHANGE THESE LINES==========================================================
	//==================================================================================
	{
		if (size % 2 != 0) size++;	//ensure that the size is even (to use LSB as allocation flag)
		if (size < DYN_ALLOC_MIN_BLOCK_SIZE)
			size = DYN_ALLOC_MIN_BLOCK_SIZE ;
		if (!is_initialized)
		{
			uint32 required_size = size + 2*sizeof(int) /*header & footer*/ + 2*sizeof(int) /*da begin & end*/ ;
			uint32 da_start = (uint32)sbrk(ROUNDUP(required_size, PAGE_SIZE)/PAGE_SIZE);
			uint32 da_break = (uint32)sbrk(0);
			initialize_dynamic_allocator(da_start, da_break - da_start);
		}
	}
	//==================================================================================
	//==================================================================================

	//TODO: [PROJECT'24.MS1 - #06] [3] DYNAMIC ALLOCATOR - alloc_block_FF
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("alloc_block_FF is not implemented yet");
	//Your Code is Here...

}
//=========================================
// [4] ALLOCATE BLOCK BY BEST FIT:
//=========================================
void *alloc_block_BF(uint32 size) {
	//TODO: [PROJECT'24.MS1 - BONUS] [3] DYNAMIC ALLOCATOR - alloc_block_BF
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	//panic("alloc_block_BF is not implemented yet");
	//Your Code is Here...

	{
		if (size % 2 != 0)
			size++;	//ensure that the size is even (to use LSB as allocation flag)
		if (size < DYN_ALLOC_MIN_BLOCK_SIZE)
			size = DYN_ALLOC_MIN_BLOCK_SIZE;
		if (!is_initialized) {
			uint32 required_size = size + 2 * sizeof(int) /*header & footer*/
			+ 2 * sizeof(int) /*da begin & end*/;
			uint32 da_start = (uint32) sbrk(
			ROUNDUP(required_size, PAGE_SIZE) / PAGE_SIZE);
			uint32 da_break = (uint32) sbrk(0);
			initialize_dynamic_allocator(da_start, da_break - da_start);
		}
	}

	struct BlockElement *best_fit = NULL;
		uint32 best_fit_size = 0xffffffff;
		struct BlockElement *current = LIST_FIRST(&freeBlocksList);

		while (current != NULL) {
			uint32 current_size = get_block_size(current);
			if (is_free_block(current) && current_size >= size + 2 * sizeof(int)) {
				if (current_size < best_fit_size) {
					best_fit = current;
					best_fit_size = current_size;
				}
			}
			current = LIST_NEXT(current);
		}

		if (best_fit == NULL) {
			return sbrk(0);
		}

		if (best_fit_size - (size + 2 * sizeof(uint32)) >= 16) {

			struct BlockElement *newelement =
					(struct BlockElement*) ((uint8*) best_fit + size
							+ 2 * sizeof(uint32));
			LIST_REMOVE(&freeBlocksList, best_fit);
			set_block_data(best_fit, size, 1);
			set_block_data(newelement,
					best_fit_size - (size + 2 * sizeof(uint32)), 0);

		} else if (best_fit_size - (size + 2 * sizeof(uint32)) < 16) {

			LIST_REMOVE(&freeBlocksList, best_fit);
			set_block_data(best_fit, best_fit_size, 1);
		}

		return best_fit;
}

//===================================================
// [5] FREE BLOCK WITH COALESCING:
//===================================================
void free_block(void *va)
{
	//TODO: [PROJECT'24.MS1 - #07] [3] DYNAMIC ALLOCATOR - free_block
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("free_block is not implemented yet");
	//Your Code is Here...
}

//=========================================
// [6] REALLOCATE BLOCK BY FIRST FIT:
//=========================================
void *realloc_block_FF(void* va, uint32 new_size)
{
	//TODO: [PROJECT'24.MS1 - #08] [3] DYNAMIC ALLOCATOR - realloc_block_FF
			//COMMENT THE FOLLOWING LINE BEFORE START CODING
			//panic("realloc_block_FF is not implemented yet");
			//Your Code is Here...

			if(new_size==0){
				free_block(va);
				return (void *)NULL;
			}
			else if(va==(void*)NULL && new_size !=0){
				return alloc_block_FF(new_size);
			}
			else if(va==(void*)NULL && new_size ==0){
				return (void *)NULL;
			}
			//check in el size +8 is even
			if (new_size%2!=0) // QQQQQ return null or make it even
				new_size++;

			uint32 *header_next= (uint32 *)va+new_size+1;//pointer to the begining of the header of the next block

			if (new_size+8==get_block_size(va))
			{
				cprintf("first test done");
				return va;
			}
			// in case new size > old size
			else if(new_size+8>get_block_size(va))
			{
				// what about next block
				int8 free_or_not=is_free_block((void *)(header_next+1));
				if(free_or_not==1)
				{// allocated //the next block is not free
					free_block(va);
					cprintf("2 test done");
					return alloc_block_FF(new_size);
				}
				else
				{ // free space after the block
					if(get_block_size((void *)(header_next+1))<(new_size+8)-get_block_size(va)){ // allow space in the next block
						free_block(va);
						cprintf("3 test done");
						return alloc_block_FF(new_size);
					}
					else if(get_block_size((void *)(header_next+1))-((new_size+8)-get_block_size(va))<16)
					{//update el size and take all the block // will take all the block
						set_block_data(va,(get_block_size(va)+get_block_size((void *)(header_next+1))),1);
						cprintf("4 test done");
						return va;
					}
					else
					{ // update el size
						//uint32 *footer=header_next-1;
						uint32 size_of_next_block=get_block_size(header_next+1);
						*header_next=((new_size+8)-get_block_size(va));// not logical
						header_next=header_next-1;
						//footer=((new_size+8)-get_block_size(va));
						set_block_data(va,new_size+8,1); // iam not sure
						set_block_data(header_next+1,(size_of_next_block-((new_size+8)-get_block_size(va))),0);
						cprintf("5 test done");
						return va;
					}
				}

			}
			else
			{// if new size < size
				uint32 size_of_block=get_block_size(va);
				uint32 size_of_new_block=new_size+8;
				if (size_of_block-size_of_new_block>=16)
				{
					set_block_data(va,size_of_new_block,1);
					uint32 *header_next= (uint32 *)va+new_size+1;
					set_block_data(header_next+1,size_of_block-size_of_new_block,0);
					cprintf("6 test done");
					return va;
					//set the next block
				}
				else
				{
					free_block(va);
					cprintf("7 test done");
					return alloc_block_FF(new_size);
				}
				return (void *)NULL;
			}
			return (void *)NULL;
}

/*********************************************************************************************/
/*********************************************************************************************/
/*********************************************************************************************/
//=========================================
// [7] ALLOCATE BLOCK BY WORST FIT:
//=========================================
void *alloc_block_WF(uint32 size)
{
	panic("alloc_block_WF is not implemented yet");
	return NULL;
}

//=========================================
// [8] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
void *alloc_block_NF(uint32 size)
{
	panic("alloc_block_NF is not implemented yet");
	return NULL;
}
