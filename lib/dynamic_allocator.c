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
	//DON'T CHANGE THESE LINES=======================================================
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

	//uint32* adbeg =(uint32*) daStart;
	//uint32* adend =(uint32*)( daStart + initSizeOfAllocatedSpace-sizeof(int));
	uint32 beg = 1;
	uint32 end = 1;
	uint32* ptr_beg = (uint32*) daStart;
	uint32* ptr_end = (uint32*) (daStart+initSizeOfAllocatedSpace-sizeof(uint32));
	*ptr_beg = beg;
	*ptr_end = end;
	LIST_INIT(&freeBlocksList);

	uint32* ptr_header=(uint32*) (daStart+sizeof(uint32));// adress of header and footer
	uint32* ptr_footer=(uint32*) (daStart+initSizeOfAllocatedSpace-sizeof(uint32)-sizeof(uint32));

	uint32 header=initSizeOfAllocatedSpace-2*sizeof(uint32); //size
	uint32 footer=header;

	*ptr_header=header; // asign the values to the address
	*ptr_footer=footer;

	struct BlockElement *element=(struct BlockElement*)(daStart+2*sizeof(uint32));

	 // might comment
	//element->header_block=header;
	//element->footer_block=footer;

    LIST_INSERT_HEAD(&freeBlocksList,element);
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


	uint32* header=(uint32*)(va-sizeof(uint32));
	uint32* footer=(uint32*)(va+totalSize-2*sizeof(uint32));
	//*header=totalSize;
	//*footer=totalSize;
	//cprintf("Setting block: va = %p, header = %p, footer = %p, totalSize = %d\n", va, header, footer, totalSize);

	  if (isAllocated == 1) {
	        *header = totalSize | 0x1;
	        *footer = totalSize | 0x1;
	    } else {
	        *header = totalSize & ~0x1;
	        *footer = totalSize & ~0x1;

       // cprintf("Header set: %d, Footer set: %d\n", *header, *footer);

		struct BlockElement *s;//itr
		struct BlockElement *element=(struct BlockElement*)va;
		struct BlockElement *maxadress=NULL;
		LIST_FOREACH(s,&freeBlocksList)
		{
			if(element>s)
			{
				maxadress=s;
			}
			else
				break;
		}
		if (maxadress != NULL)
		{
		    LIST_INSERT_AFTER(&freeBlocksList, maxadress, element);
		}
		else
		{
		    LIST_INSERT_HEAD(&freeBlocksList, element);
		}
	}

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
	//panic("alloc_block_FF is not implemented yet");
	//Your Code is Here...

	struct BlockElement *element;
	LIST_FOREACH(element,&freeBlocksList)
	{
	uint32 free_block_size = get_block_size(element);
	//cprintf("%d\n",free_block_size);
	if (size + 8 <= free_block_size)
	{
	if (free_block_size - (size + 2 * sizeof(uint32)) >= 16) {

	struct BlockElement *newelement = (struct BlockElement*) ((uint8*) element + size + 2 * sizeof(uint32));
	LIST_REMOVE(&freeBlocksList, element);
	set_block_data(element, size + 8, 1);
	set_block_data(newelement, free_block_size - (size + 2 * sizeof(uint32)), 0);
	return element;
	}
	else if (free_block_size - (size + 2 * sizeof(uint32)) < 16) {

	LIST_REMOVE(&freeBlocksList, element);
	set_block_data(element, free_block_size, 1);
	return element;
	}
	} else if (size == 0) {
	return sbrk(0);

	}
	}


	return NULL;
}




//=========================================
// [4] ALLOCATE BLOCK BY BEST FIT:
//=========================================
void *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT'24.MS1 - BONUS] [3] DYNAMIC ALLOCATOR - alloc_block_BF
	//COMMENT THE FOLLOWING LINE BEFORE START CODING
	panic("alloc_block_BF is not implemented yet");
	//Your Code is Here...

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
	panic("realloc_block_FF is not implemented yet");
	//Your Code is Here...
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
