#ifndef	_PORTMEM_H_
#define	_PORTMEM_H_

#define NULL 0
#include "windows.h"

class CSMGMemAllocator 

{

public:

	void Free(void * mem) const;

	void * Alloc(unsigned long ulSize) const;

	unsigned long SizeOf(void * mem) const;

	void Init(unsigned long ulInitialSize) ;		//初始化内存池大小

	CSMGMemAllocator(unsigned long ulInitialSize = 0):m_hHeap(NULL)

	{

		Init(ulInitialSize);

#ifdef TEST_MEM

		m_max = 0;

		m_cur = 0;

#endif

	}

	~CSMGMemAllocator();

#ifdef TEST_MEM

	LONG m_max, m_cur;

#endif

private:

	void*	m_hHeap;

};



//移植区:Begin

inline CSMGMemAllocator::~CSMGMemAllocator()

{

	if(m_hHeap){

		HeapDestroy(m_hHeap);

		m_hHeap = NULL;

	}

	

}



inline void CSMGMemAllocator::Init(unsigned long ulInitialSize)

{
	if(m_hHeap){

		HeapDestroy(m_hHeap);

		m_hHeap = NULL;

	}

	if(ulInitialSize<0xFFFFFFFF){

		m_hHeap = HeapCreate(0,ulInitialSize,0);

		if(!m_hHeap)

			m_hHeap = HeapCreate(0,0,0);

	}

}



inline void * CSMGMemAllocator::Alloc(unsigned long ulSize) const

{

#ifdef TEST_MEM

	*(long*)(LONG)(const long *)&m_cur += ulSize;

	if(m_cur > m_max)

	{

		*(long*)(LONG)(const long *)&m_max = m_cur;

	}

#endif

	return HeapAlloc(m_hHeap,0,ulSize);

}



inline void CSMGMemAllocator::Free(void *mem) const

{

#ifdef TEST_MEM

	*(long*)(LONG)(const long *)&m_cur -= SizeOf(mem);

	if(m_cur<0)

		*(long*)(LONG)(const long *)&m_cur = 0;

#endif

	



		HeapFree(m_hHeap,0,mem);



}



inline unsigned long CSMGMemAllocator::SizeOf(void *mem) const

{

	if(m_hHeap)

		return HeapSize(m_hHeap,0,mem);

	else return 0;

}

//移植区:End



#endif

