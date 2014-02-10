#ifndef	_PORT_ARRAY_H_
#define	_PORT_ARRAY_H_

#include "PortMem.h"
//class CQuartTree;
typedef void(*CallBackFunc)(long);
typedef void (*CallBackFunction) (long,long);

#define	HashKey	SMG_HashKey
#define TYPENAME	typename

#include "assert.h"
#define ASSERT(x)  assert(x)

#define SMG_INLINE inline
#define SMG_DWORD DWORD
#if	defined(WIN32) && !defined(SMG_NO_VC)

#	ifdef	_WIN32_WCE

#		ifndef	_WIN32_WCE_EMULATION

#			define	SMG_APICALLTYPE		__cdecl

#		else

#			define	SMG_APICALLTYPE		__stdcall

#		endif

#	else

#		define	SMG_APICALLTYPE		__stdcall

#	endif
#endif 

typedef union _SMG_ULARGE_INTEGER {

    struct {

        SMG_DWORD LowPart;

        SMG_DWORD HighPart;

    };

    struct {

        SMG_DWORD LowPart;

        SMG_DWORD HighPart;

    } u;

} SMG_ULARGE_INTEGER;



template<class TYPE>
SMG_INLINE void SMG_APICALLTYPE ConstructElements(TYPE* pElements, int nCount)
{
//	ASSERT(nCount == 0 ||
//		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));

	// first do bit-wise zero initialization
//SQW for optimize	
#ifdef	SMG_DEBUG
	memset((void*)pElements, 0xCD, nCount * sizeof(TYPE));
#endif

	// then call the constructor(s)
		for (; nCount--; pElements++)
		new ((void*)pElements)TYPE;
	
}

template<class TYPE>
SMG_INLINE void SMG_APICALLTYPE DestructElements(TYPE* pElements, int nCount)
{
//	ASSERT(nCount == 0 ||
//		AfxIsValidAddress(pElements, nCount * sizeof(TYPE)));

	// call the destructor(s)
	for (; nCount--; pElements++)
		pElements->~TYPE();
	
}

template<>
SMG_INLINE void SMG_APICALLTYPE ConstructElements(LONG* pElements, int nCount){}
template<>
SMG_INLINE void SMG_APICALLTYPE DestructElements(LONG* pElements, int nCount){}

template<>
SMG_INLINE void SMG_APICALLTYPE ConstructElements(INT* pElements, int nCount){}
template<>
SMG_INLINE void SMG_APICALLTYPE DestructElements(INT* pElements, int nCount){}

template<>
SMG_INLINE void SMG_APICALLTYPE ConstructElements(DWORD* pElements, int nCount){}
template<>
SMG_INLINE void SMG_APICALLTYPE DestructElements(DWORD* pElements, int nCount){}

template<>
SMG_INLINE void SMG_APICALLTYPE ConstructElements(SMG_ULARGE_INTEGER* pElements, int nCount){}
template<>
SMG_INLINE void SMG_APICALLTYPE DestructElements(SMG_ULARGE_INTEGER* pElements, int nCount){}

template<class TYPE>
SMG_INLINE void SMG_APICALLTYPE CopyElements(TYPE* pDest, const TYPE* pSrc, int nCount)
{
//	ASSERT(nCount == 0 ||
//		AfxIsValidAddress(pDest, nCount * sizeof(TYPE)));
//	ASSERT(nCount == 0 ||
//		AfxIsValidAddress(pSrc, nCount * sizeof(TYPE)));

	// default is element-copy using assignment
	while (nCount--)
		*pDest++ = *pSrc++;
}

template<class TYPE, class ARG_TYPE>
BOOL SMG_APICALLTYPE CompareElements(const TYPE* pElement1, const ARG_TYPE* pElement2)
{
	return *pElement1 == *pElement2;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc = &g_CmnAlloc>
class CSMGArray
{
public:
// Construction
	CSMGArray();

// Attributes
	int GetSize() const;
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	TYPE GetAt(int nIndex) const;
	void SetAt(int nIndex, ARG_TYPE newElement);
	TYPE& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
	const TYPE* GetData() const;
	TYPE* GetData();

	// Potentially growing the array
	void SetAtGrow(int nIndex, ARG_TYPE newElement);
	int Add(ARG_TYPE newElement);
	int Append(const CSMGArray& src);
	void Copy(const CSMGArray& src);

	// overloaded operator helpers
	TYPE operator[](int nIndex) const;
	TYPE& operator[](int nIndex);

	// Operations that move elements around
	void InsertAt(int nIndex, ARG_TYPE newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CSMGArray* pNewArray);

// Implementation
protected:
	TYPE* m_pData;   // the actual array of data
	int m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated

public:
	~CSMGArray();
};

/////////////////////////////////////////////////////////////////////////////
// CSMGArray<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE int CSMGArray<TYPE, ARG_TYPE, pAlloc>::GetSize() const
	{ return m_nSize; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE int CSMGArray<TYPE, ARG_TYPE, pAlloc>::GetUpperBound() const
	{ return m_nSize-1; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE void CSMGArray<TYPE, ARG_TYPE,pAlloc>::RemoveAll()
	{ SetSize(0, -1); }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE CSMGArray<TYPE, ARG_TYPE,pAlloc>::GetAt(int nIndex) const
	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE void CSMGArray<TYPE, ARG_TYPE,pAlloc>::SetAt(int nIndex, ARG_TYPE newElement)
	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
		m_pData[nIndex] = newElement; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE& CSMGArray<TYPE, ARG_TYPE,pAlloc>::ElementAt(int nIndex)
	{ ASSERT(nIndex >= 0 && nIndex < m_nSize);
		return m_pData[nIndex]; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE const TYPE* CSMGArray<TYPE, ARG_TYPE,pAlloc>::GetData() const
	{ return (const TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE* CSMGArray<TYPE, ARG_TYPE,pAlloc>::GetData()
	{ return (TYPE*)m_pData; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE int CSMGArray<TYPE, ARG_TYPE,pAlloc>::Add(ARG_TYPE newElement)
	{ int nIndex = m_nSize;
		SetAtGrow(nIndex, newElement);
		return nIndex; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE CSMGArray<TYPE, ARG_TYPE,pAlloc>::operator[](int nIndex) const
	{ return GetAt(nIndex); }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE& CSMGArray<TYPE, ARG_TYPE,pAlloc>::operator[](int nIndex)
	{ return ElementAt(nIndex); }

/////////////////////////////////////////////////////////////////////////////
// CSMGArray<TYPE, ARG_TYPE> out-of-line functions

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
CSMGArray<TYPE, ARG_TYPE,pAlloc>::CSMGArray()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = 0;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
CSMGArray<TYPE, ARG_TYPE,pAlloc>::~CSMGArray()
{
	SMG_ASSERT_VALID(this);

	if (m_pData != NULL)
	{
		DestructElements<TYPE>(m_pData, m_nSize);
		pAlloc->Free(m_pData);
	}
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGArray<TYPE, ARG_TYPE,pAlloc>::SetSize(int nNewSize, int nGrowBy)
{
	SMG_ASSERT_VALID(this);
	ASSERT(nNewSize >= 0);


	if (nNewSize == 0)
	{
		// shrink to nothing
		if (m_pData != NULL)
		{
			DestructElements<TYPE>(m_pData, m_nSize);
			pAlloc->Free(m_pData);
			m_pData = NULL;
		}
		m_nSize = m_nMaxSize = 0;
	}
	else if (m_pData == NULL)
	{
		// create one with exact size
#ifdef SIZE_T_MAX
		ASSERT(nNewSize <= SIZE_T_MAX/sizeof(TYPE));    // no overflow
#endif
		m_pData = (TYPE*)pAlloc->Alloc(nNewSize * sizeof(TYPE));
		ConstructElements<TYPE>(m_pData, nNewSize);
		m_nSize = m_nMaxSize = nNewSize;
	}
	else if (nNewSize <= m_nMaxSize)
	{
		// it fits
		if (nNewSize > m_nSize)
		{
			// initialize the new elements
			ConstructElements<TYPE>(m_pData+m_nSize, nNewSize-m_nSize);
		}
		else if (m_nSize > nNewSize)
		{
			// destroy the old elements
			DestructElements<TYPE>(m_pData+nNewSize, m_nSize-nNewSize);
		}
		m_nSize = nNewSize;
	}
	else
	{
		// otherwise, grow array
		int nGrowBy = 0;
		if (nGrowBy == 0)
		{
			// heuristically determine growth when nGrowBy == 0
			//  (this avoids heap fragmentation in many situations)
			nGrowBy = m_nSize;
			nGrowBy = (nGrowBy < 4) ? 4 : ((nGrowBy > 1024) ? 1024 : nGrowBy);
		}
		int nNewMax;
		if (nNewSize < m_nMaxSize + nGrowBy)
			nNewMax = m_nMaxSize + nGrowBy;  // granularity
		else
			nNewMax = nNewSize;  // no slush

		ASSERT(nNewMax >= m_nMaxSize);  // no wrap around
#ifdef SIZE_T_MAX
		ASSERT(nNewMax <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE * pNewData;
		pNewData = (TYPE*) pAlloc->Alloc(nNewMax * sizeof(TYPE));
#ifdef	SMG_DEBUG
		memset(pNewData,0,nNewMax * sizeof(TYPE));
#endif

		// copy new data from old
		memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));

		// construct remaining elements
		ASSERT(nNewSize > m_nSize);
		ConstructElements<TYPE>(pNewData+m_nSize, nNewSize-m_nSize);

		// get rid of old stuff (note: no destructors called)
		pAlloc->Free(m_pData);
		m_pData = pNewData;
		m_nSize = nNewSize;
		m_nMaxSize = nNewMax;
	}
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
int CSMGArray<TYPE, ARG_TYPE,pAlloc>::Append(const CSMGArray& src)
{
	SMG_ASSERT_VALID(this);
	ASSERT(this != &src);   // cannot append to itself

	int nOldSize = m_nSize;
	SetSize(m_nSize + src.m_nSize);
	CopyElements<TYPE>(m_pData + nOldSize, src.m_pData, src.m_nSize);
	return nOldSize;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGArray<TYPE, ARG_TYPE,pAlloc>::Copy(const CSMGArray& src)
{
	SMG_ASSERT_VALID(this);
	ASSERT(this != &src);   // cannot append to itself

	SetSize(src.m_nSize);
	CopyElements<TYPE>(m_pData, src.m_pData, src.m_nSize);
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGArray<TYPE, ARG_TYPE,pAlloc>::FreeExtra()
{
	SMG_ASSERT_VALID(this);

	if (m_nSize != m_nMaxSize)
	{
		// shrink to desired size
#ifdef SIZE_T_MAX
		ASSERT(m_nSize <= SIZE_T_MAX/sizeof(TYPE)); // no overflow
#endif
		TYPE* pNewData = NULL;
		if (m_nSize != 0)
		{
			pNewData = (TYPE*) pAlloc->Alloc(m_nSize * sizeof(TYPE));
			// copy new data from old
			memcpy(pNewData, m_pData, m_nSize * sizeof(TYPE));
		}

		// get rid of old stuff (note: no destructors called)
		pAlloc->Free(m_pData);
		m_pData = pNewData;
		m_nMaxSize = m_nSize;
	}
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGArray<TYPE, ARG_TYPE,pAlloc>::SetAtGrow(int nIndex, ARG_TYPE newElement)
{
	SMG_ASSERT_VALID(this);
	ASSERT(nIndex >= 0);

	if (nIndex >= m_nSize)
		SetSize(nIndex+1, -1);
	m_pData[nIndex] = newElement;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGArray<TYPE, ARG_TYPE,pAlloc>::InsertAt(int nIndex, ARG_TYPE newElement, int nCount )
{
	SMG_ASSERT_VALID(this);
	ASSERT(nIndex >= 0);    // will expand to meet need
	ASSERT(nCount > 0);     // zero or negative size not allowed

	if (nIndex >= m_nSize)
	{
		// adding after the end of the array
		SetSize(nIndex + nCount, -1);   // grow so nIndex is valid
	}
	else
	{
		// inserting in the middle of the array
		int nOldSize = m_nSize;
		SetSize(m_nSize + nCount, -1);  // grow it to new size
		// destroy intial data before copying over it
		DestructElements<TYPE>(m_pData+nOldSize, nCount);
		// shift old data up to fill gap
		memmove(m_pData+nIndex+nCount, m_pData+nIndex,
			(nOldSize-nIndex) * sizeof(TYPE));

		// re-init slots we copied from
		ConstructElements<TYPE>(m_pData+nIndex, nCount);
	}

	// insert new value in the gap
	ASSERT(nIndex + nCount <= m_nSize);
	while (nCount--)
		m_pData[nIndex++] = newElement;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGArray<TYPE, ARG_TYPE,pAlloc>::RemoveAt(int nIndex, int nCount)
{
	SMG_ASSERT_VALID(this);
	ASSERT(nIndex >= 0);
	ASSERT(nCount >= 0);
	ASSERT(nIndex + nCount <= m_nSize);

	// just remove a range
	int nMoveCount = m_nSize - (nIndex + nCount);
	DestructElements<TYPE>(m_pData+nIndex, nCount);
	if (nMoveCount)
		memmove(m_pData+nIndex, m_pData+nIndex + nCount,
			nMoveCount * sizeof(TYPE));
	m_nSize -= nCount;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGArray<TYPE, ARG_TYPE,pAlloc>::InsertAt(int nStartIndex, CSMGArray* pNewArray)
{
	SMG_ASSERT_VALID(this);
	ASSERT(pNewArray != NULL);
	SMG_ASSERT_VALID(pNewArray);
	ASSERT(nStartIndex >= 0);

	if (pNewArray->GetSize() > 0)
	{
		InsertAt(nStartIndex, pNewArray->GetAt(0), pNewArray->GetSize());
		for (int i = 0; i < pNewArray->GetSize(); i++)
			SetAt(nStartIndex + i, pNewArray->GetAt(i));
	}
}
#define CArray	CSMGArray


/////////////////////////////////////////////////////////////////////////////
// CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE>
struct __SMG_POSITION { };
typedef __SMG_POSITION* SMG_POSITION;
#define	POSITION	SMG_POSITION
#define BEFORE_START_POSITION ((POSITION)-1L)

template<const CSMGMemAllocator *pAlloc>
struct CSMGPlex     // warning variable length structure
{
	CSMGPlex* pNext;
	// BYTE data[maxNum*elementSize];

	void* data() { return this+1; }

	static CSMGPlex* SMG_APICALLTYPE Create(CSMGPlex*& pHead, UINT nMax, UINT cbElement)
	{
		ASSERT(nMax > 0 && cbElement > 0);
		CSMGPlex* p;
		p = (CSMGPlex*)pAlloc->Alloc(sizeof(CSMGPlex) + nMax * cbElement);
				// may throw exception
		p->pNext = pHead;
		pHead = p;  // change head (adds in reverse order for simplicity)
		return p;
	}
	void FreeDataChain()       // free this one and links
	{
		CSMGPlex* p = this;
		while (p != NULL)
		{
			BYTE* bytes = (BYTE*) p;
			CSMGPlex* pNext = p->pNext;
			pAlloc->Free(bytes);
			p = pNext;
		}
	}
};


template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc = &g_CmnAlloc>
class CSMGMap
{
protected:
	// Association
	struct CAssoc
	{
		CAssoc* pNext;
//		UINT nHashValue;  // needed for efficient iteration
		KEY key;
		VALUE value;
	};
public:
// Construction
	CSMGMap(int nBlockSize = 10);

// Attributes
	// number of elements
	int GetCount() const;
	BOOL IsEmpty() const;
	CallBackFunc callback;	
	void SetCallBack( CallBackFunc cb)
	{
		callback=cb;
	};
	void EnumValue(CQuartTree* tree,CallBackFunction cb);	
	// Lookup
	BOOL Lookup(ARG_KEY key, VALUE& rValue) const;
	BOOL Lookup(ARG_KEY key, VALUE**rValue);

// Operations
	// Lookup and add if not there
	VALUE& operator[](ARG_KEY key);
	// add a new (key, value) pair
	void SetAt(ARG_KEY key, ARG_VALUE newValue);

	// removing existing (key, ?) pair
	BOOL RemoveKey(ARG_KEY key);
	void RemoveAll();

	// iterating all (key, value) pairs
	POSITION GetStartPosition() const;
	void GetNextAssoc(POSITION& rNextPosition, KEY& rKey, VALUE& rValue) const;
	void GetNextAssoc(POSITION& rNextPosition, KEY**rKey, VALUE**rValue) ;

	// advanced features for derived classes
	UINT GetHashTableSize() const;
	void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Implementation
protected:
	CAssoc** m_pHashTable;
	UINT m_nHashTableSize;
	int m_nCount;
	CAssoc* m_pFreeList;
	CSMGPlex<pAlloc>* m_pBlocks;
	int m_nBlockSize;
	int m_nHashMask;

	CAssoc* NewAssoc();
	void FreeAssoc(CAssoc*);
	CAssoc* GetAssocAt(ARG_KEY, UINT&) const;

public:
	~CSMGMap();
};

/////////////////////////////////////////////////////////////////////////////
// CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE> inline functions

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
SMG_INLINE int CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::GetCount() const
	{ return m_nHashTableSize; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
SMG_INLINE BOOL CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::IsEmpty() const
	{ return m_nHashTableSize == 0; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
SMG_INLINE void CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::SetAt(ARG_KEY key, ARG_VALUE newValue)
	{ (*this)[key] = newValue; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
SMG_INLINE POSITION CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
SMG_INLINE UINT CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::GetHashTableSize() const
	{ return m_nHashTableSize; }

template<class ARG_KEY>
SMG_INLINE UINT HashKey(ARG_KEY key)
{
	// default identity hash - works for most primitive values
	return ((UINT)(void*)(DWORD)key);
}

template<> 
SMG_INLINE UINT HashKey<LPCWSTR> (LPCWSTR key)
{
	if(!key)return 0;
	UINT nHash = 0;
	while (*key)
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
}

/////////////////////////////////////////////////////////////////////////////
// CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE> out-of-line functions

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::CSMGMap(int nBlockSize)
{
	m_pHashTable = NULL;
	callback=NULL;
	m_nHashTableSize = 17;  // default size
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
	m_nHashMask = 0;
	if(nBlockSize == 0 ) m_nBlockSize = 10;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
void CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::InitHashTable(
	UINT nHashSize, BOOL bAllocNow)
//
// Used to force allocation of a hash table or to override the default
//   hash table size of (which is fairly small)
{

	ASSERT(m_nCount == 0);
	ASSERT(nHashSize > 0);

	if (m_pHashTable != NULL)
	{
		// free hash table
		pAlloc->Free(m_pHashTable);
		m_pHashTable = NULL;
	}

	if (bAllocNow)
	{
		m_pHashTable = (CAssoc**)pAlloc->Alloc(sizeof(CAssoc*) * nHashSize);
		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
	}
	m_nHashTableSize = nHashSize;
	m_nHashMask = 0;
	UINT ni;
	for(ni = 1;ni < m_nHashTableSize;ni<<=1);
	if(ni == m_nHashTableSize)
		m_nHashMask = m_nHashTableSize-1;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
void CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::RemoveAll()
{

	if (m_pHashTable != NULL)
	{
		// destroy elements (values and keys)
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				DestructElements<VALUE>((VALUE*)&pAssoc->value, 1);				
				if (callback)
					callback((long)&pAssoc->value);
				ULONG ii= pAssoc->value->IID;
				delete pAssoc->value;
				DestructElements<KEY>((KEY*)&pAssoc->key, 1);
			}
		}
	}

	// free hash table
	pAlloc->Free(m_pHashTable);
	m_pHashTable = NULL;

	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::~CSMGMap()
{
	RemoveAll();
	ASSERT(m_nCount == 0);
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
TYPENAME CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::CAssoc*
CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::NewAssoc()
{
	if (m_pFreeList == NULL)
	{
		// add another block
#ifdef	SMG_SERVICE
		if(m_nBlockSize == 0 ) m_nBlockSize = 10;
#endif
		CSMGPlex<pAlloc>* newBlock = CSMGPlex<pAlloc>::Create(m_pBlocks, m_nBlockSize, sizeof(CSMGMap::CAssoc));
		// chain them into free list
		CSMGMap::CAssoc* pAssoc = (CSMGMap::CAssoc*) newBlock->data();
		// free in reverse order to make it easier to debug
		pAssoc += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
		{
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
	ASSERT(m_pFreeList != NULL);  // we must have something

	CSMGMap::CAssoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
	ASSERT(m_nCount > 0);  // make sure we don't overflow
#ifdef	SMG_DEBUG
	memset(pAssoc,0,sizeof(CSMGMap::CAssoc));
#endif
	ConstructElements<KEY>((KEY*)&pAssoc->key, 1);
	ConstructElements<VALUE>((VALUE*)&pAssoc->value, 1);   // special construct values
	return pAssoc;
}
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
void CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::EnumValue(CQuartTree* tree,CallBackFunction cb)
{
	if (m_pHashTable != NULL)
	{
		// enum elements (values and keys)
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{			
				 cb((long)tree,(long)pAssoc->value);
			}
		}
	}


}
template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
void CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::FreeAssoc(TYPENAME CSMGMap::CAssoc* pAssoc)
{
	DestructElements<VALUE>((VALUE*)&pAssoc->value, 1);
	DestructElements<KEY>((KEY*)&pAssoc->key, 1);
	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;
	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
TYPENAME CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::CAssoc*
CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::GetAssocAt(ARG_KEY key, UINT& nHash) const
// find association (or return NULL)
{
	if(m_nHashMask)
		nHash = HashKey<ARG_KEY>(key) & m_nHashMask;
	else
		nHash = HashKey<ARG_KEY>(key) % m_nHashTableSize;

	if (m_pHashTable == NULL)
		return NULL;

	// see if it exists
	CAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (CompareElements((KEY*)&pAssoc->key, (KEY*)&key))
			return pAssoc;
	}
	return NULL;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
BOOL CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::Lookup(ARG_KEY key, VALUE& rValue) const
{

	if(!m_nCount)return FALSE;
	UINT nHash;
	CAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rValue = pAssoc->value;
	return TRUE;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
BOOL CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::Lookup(ARG_KEY key, VALUE** rValue)
{
	SMG_ASSERT_VALID(this);

	if(!m_nCount)return FALSE;
	UINT nHash;
	CAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	*rValue = &pAssoc->value;
	return TRUE;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
VALUE& CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::operator[](ARG_KEY key)
{

	UINT nHash;
	CAssoc* pAssoc=NULL;
#ifdef	SMG_SERVICE
	if(m_nHashTableSize == 0)m_nHashTableSize = 17;
#endif
	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
	{
		if (m_pHashTable == NULL){
			InitHashTable(m_nHashTableSize);
		}

		// it doesn't exist, add a new Association
		pAssoc = NewAssoc();
//		pAssoc->nHashValue = nHash;
		pAssoc->key = key;
		// 'pAssoc->value' is a constructed object, nothing more

		// put into hash table
		pAssoc->pNext = m_pHashTable[nHash];
		m_pHashTable[nHash] = pAssoc;
		return pAssoc->value;
	}else
	{
		ARG_VALUE value=NULL;
		return value;  // return new reference
	}
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
BOOL CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::RemoveKey(ARG_KEY key)
// remove key - return TRUE if removed
{
	SMG_ASSERT_VALID(this);

	if (m_pHashTable == NULL)
		return FALSE;  // nothing in the table

	CAssoc** ppAssocPrev;
	if(m_nHashMask)
		ppAssocPrev = &m_pHashTable[HashKey<ARG_KEY>(key) & m_nHashMask];
	else
		ppAssocPrev = &m_pHashTable[HashKey<ARG_KEY>(key) % m_nHashTableSize];

	CAssoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (CompareElements((KEY*)&pAssoc->key, (KEY*)&key))
		{
			// remove it
			*ppAssocPrev = pAssoc->pNext;  // remove from list
			FreeAssoc(pAssoc);
			return TRUE;
		}
		ppAssocPrev = &pAssoc->pNext;
	}
	return FALSE;  // not found
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
void CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::GetNextAssoc(POSITION& rNextPosition,
	KEY** rKey, VALUE** rValue) 
{
	SMG_ASSERT_VALID(this);
	ASSERT(m_pHashTable != NULL);  // never call on empty map

	CAssoc* pAssocRet = (CAssoc*)rNextPosition;
	ASSERT(pAssocRet != NULL);

	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)
	{
		// find the first association
		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
		ASSERT(pAssocRet != NULL);  // must find something
	}

	// find next association
	CAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		UINT nHashValue;
		if(m_nHashMask)
			nHashValue = HashKey<ARG_KEY>(pAssocRet->key) & m_nHashMask;
		else
			nHashValue = HashKey<ARG_KEY>(pAssocRet->key) % m_nHashTableSize;
		for (UINT nBucket = nHashValue + 1;
		  nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	rNextPosition = (POSITION) pAssocNext;

	// fill in return data
	*rKey	= (KEY*)&pAssocRet->key;
	*rValue = (VALUE*)&pAssocRet->value;
}

template<class KEY, class ARG_KEY, class VALUE, class ARG_VALUE,const CSMGMemAllocator * pAlloc>
void CSMGMap<KEY, ARG_KEY, VALUE, ARG_VALUE, pAlloc>::GetNextAssoc(POSITION& rNextPosition,
	KEY& rKey, VALUE& rValue) const
{
	SMG_ASSERT_VALID(this);
	ASSERT(m_pHashTable != NULL);  // never call on empty map

	CAssoc* pAssocRet = (CAssoc*)rNextPosition;
	ASSERT(pAssocRet != NULL);

	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)
	{
		// find the first association
		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
		ASSERT(pAssocRet != NULL);  // must find something
	}

	// find next association
	CAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		UINT nHashValue = HashKey<ARG_KEY>(pAssocRet->key) % m_nHashTableSize;
		for (UINT nBucket = nHashValue + 1;
		  nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	rNextPosition = (POSITION) pAssocNext;

	// fill in return data
	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}

#define	CMap	CSMGMap

/////////////////////////////////////////////////////////////////////////////
// CSMGList<TYPE, ARG_TYPE>

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc = &g_CmnAlloc>
class CSMGList 
{
protected:
	struct CNode
	{
		CNode* pNext;
		CNode* pPrev;
		TYPE data;
	};
public:
// Construction
	CSMGList(int nBlockSize = 10);

// Attributes (head and tail)
	// count of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// peek at head or tail
	TYPE& GetHead();
	TYPE GetHead() const;
	TYPE& GetTail();
	TYPE GetTail() const;

// Operations
	// get head or tail (and remove it) - don't call on empty list !
	TYPE RemoveHead();
	TYPE RemoveTail();

	// add before head or after tail
	POSITION AddHead(ARG_TYPE newElement);
	POSITION AddTail(ARG_TYPE newElement);

	// add another list of elements before head or after tail
	void AddHead(CSMGList* pNewList);
	void AddTail(CSMGList* pNewList);

	// remove all elements
	void RemoveAll();

	// iteration
	POSITION GetHeadPosition() const;
	POSITION GetTailPosition() const;
	TYPE& GetNext(POSITION& rPosition); // return *Position++
	TYPE GetNext(POSITION& rPosition) const; // return *Position++
	TYPE& GetPrev(POSITION& rPosition); // return *Position--
	TYPE GetPrev(POSITION& rPosition) const; // return *Position--

	// getting/modifying an element at a given position
	TYPE& GetAt(POSITION position);
	TYPE GetAt(POSITION position) const;
	void SetAt(POSITION pos, ARG_TYPE newElement);
	void RemoveAt(POSITION position);

	// inserting before or after a given position
	POSITION InsertBefore(POSITION position, ARG_TYPE newElement);
	POSITION InsertAfter(POSITION position, ARG_TYPE newElement);

	// helper functions (note: O(n) speed)
	POSITION Find(ARG_TYPE searchValue, POSITION startAfter = NULL) const;
		// defaults to starting at the HEAD, return NULL if not found
	POSITION FindIndex(int nIndex) const;
		// get the 'nIndex'th element (may return NULL)

// Implementation
protected:
	CNode* m_pNodeHead;
	CNode* m_pNodeTail;
	int m_nCount;
	CNode* m_pNodeFree;
	CSMGPlex<pAlloc>* m_pBlocks;
	int m_nBlockSize;

	CNode* NewNode(CNode*, CNode*);
	void FreeNode(CNode*);

public:
	~CSMGList();
};

/////////////////////////////////////////////////////////////////////////////
// CSMGList<TYPE, ARG_TYPE> inline functions

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE int CSMGList<TYPE, ARG_TYPE, pAlloc>::GetCount() const
	{ return m_nCount; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE BOOL CSMGList<TYPE, ARG_TYPE, pAlloc>::IsEmpty() const
	{ return m_nCount == 0; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE& CSMGList<TYPE, ARG_TYPE, pAlloc>::GetHead()
	{ ASSERT(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE CSMGList<TYPE, ARG_TYPE, pAlloc>::GetHead() const
	{ ASSERT(m_pNodeHead != NULL);
		return m_pNodeHead->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE& CSMGList<TYPE, ARG_TYPE, pAlloc>::GetTail()
	{ ASSERT(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE CSMGList<TYPE, ARG_TYPE, pAlloc>::GetTail() const
	{ ASSERT(m_pNodeTail != NULL);
		return m_pNodeTail->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE POSITION CSMGList<TYPE, ARG_TYPE, pAlloc>::GetHeadPosition() const
	{ return (POSITION) m_pNodeHead; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE POSITION CSMGList<TYPE, ARG_TYPE, pAlloc>::GetTailPosition() const
	{ return (POSITION) m_pNodeTail; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE& CSMGList<TYPE, ARG_TYPE, pAlloc>::GetNext(POSITION& rPosition) // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE CSMGList<TYPE, ARG_TYPE, pAlloc>::GetNext(POSITION& rPosition) const // return *Position++
	{ CNode* pNode = (CNode*) rPosition;
		
		rPosition = (POSITION) pNode->pNext;
		return pNode->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE& CSMGList<TYPE, ARG_TYPE, pAlloc>::GetPrev(POSITION& rPosition) // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE CSMGList<TYPE, ARG_TYPE, pAlloc>::GetPrev(POSITION& rPosition) const // return *Position--
	{ CNode* pNode = (CNode*) rPosition;
		
		rPosition = (POSITION) pNode->pPrev;
		return pNode->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE& CSMGList<TYPE, ARG_TYPE, pAlloc>::GetAt(POSITION position)
	{ CNode* pNode = (CNode*) position;
		
		return pNode->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE TYPE CSMGList<TYPE, ARG_TYPE, pAlloc>::GetAt(POSITION position) const
	{ CNode* pNode = (CNode*) position;
		
		return pNode->data; }
template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
SMG_INLINE void CSMGList<TYPE, ARG_TYPE, pAlloc>::SetAt(POSITION pos, ARG_TYPE newElement)
	{ CNode* pNode = (CNode*) pos;
		
		pNode->data = newElement; }

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
CSMGList<TYPE, ARG_TYPE, pAlloc>::CSMGList(int nBlockSize)
{
	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
	if(nBlockSize == 0 ) m_nBlockSize = 10;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGList<TYPE, ARG_TYPE, pAlloc>::RemoveAll()
{
	SMG_ASSERT_VALID(this);

	// destroy elements
	CNode* pNode;
	for (pNode = m_pNodeHead; pNode != NULL; pNode = pNode->pNext)
		DestructElements<TYPE>((TYPE*)&pNode->data, 1);

	m_nCount = 0;
	m_pNodeHead = m_pNodeTail = m_pNodeFree = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
CSMGList<TYPE, ARG_TYPE, pAlloc>::~CSMGList()
{
	RemoveAll();
	ASSERT(m_nCount == 0);
}

/////////////////////////////////////////////////////////////////////////////
// Node helpers
//
// Implementation note: CNode's are stored in CSMGPlex blocks and
//  chained together. Free blocks are maintained in a singly linked list
//  using the 'pNext' member of CNode with 'm_pNodeFree' as the head.
//  Used blocks are maintained in a doubly linked list using both 'pNext'
//  and 'pPrev' as links and 'm_pNodeHead' and 'm_pNodeTail'
//   as the head/tail.
//
// We never free a CSMGPlex block unless the List is destroyed or RemoveAll()
//  is used - so the total number of CSMGPlex blocks may grow large depending
//  on the maximum past size of the list.
//

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
TYPENAME CSMGList<TYPE, ARG_TYPE, pAlloc>::CNode*
CSMGList<TYPE, ARG_TYPE, pAlloc>::NewNode(TYPENAME CSMGList::CNode* pPrev, TYPENAME CSMGList::CNode* pNext)
{
	if (m_pNodeFree == NULL)
	{
		// add another block
#ifdef	SMG_SERVICE
		if(m_nBlockSize == 0 ) m_nBlockSize = 10;
#endif
		CSMGPlex<pAlloc>* pNewBlock = CSMGPlex<pAlloc>::Create(m_pBlocks, m_nBlockSize,
				 sizeof(CNode));

		// chain them into free list
		CNode* pNode = (CNode*) pNewBlock->data();
		// free in reverse order to make it easier to debug
		pNode += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
		{
			pNode->pNext = m_pNodeFree;
			m_pNodeFree = pNode;
		}
	}
	ASSERT(m_pNodeFree != NULL);  // we must have something

	CSMGList::CNode* pNode = m_pNodeFree;
	m_pNodeFree = m_pNodeFree->pNext;
	pNode->pPrev = pPrev;
	pNode->pNext = pNext;
	m_nCount++;
	ASSERT(m_nCount > 0);  // make sure we don't overflow

	ConstructElements<TYPE>((TYPE*)&pNode->data, 1);
	return pNode;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGList<TYPE, ARG_TYPE, pAlloc>::FreeNode(TYPENAME CSMGList::CNode* pNode)
{
	DestructElements<TYPE>((TYPE*)&pNode->data, 1);
	pNode->pNext = m_pNodeFree;
	m_pNodeFree = pNode;
	m_nCount--;
	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
POSITION CSMGList<TYPE, ARG_TYPE, pAlloc>::AddHead(ARG_TYPE newElement)
{
	SMG_ASSERT_VALID(this);

	CNode* pNewNode = NewNode(NULL, m_pNodeHead);
	pNewNode->data = newElement;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = pNewNode;
	else
		m_pNodeTail = pNewNode;
	m_pNodeHead = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
POSITION CSMGList<TYPE, ARG_TYPE, pAlloc>::AddTail(ARG_TYPE newElement)
{
	SMG_ASSERT_VALID(this);

	CNode* pNewNode = NewNode(m_pNodeTail, NULL);
	pNewNode->data = newElement;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = pNewNode;
	else
		m_pNodeHead = pNewNode;
	m_pNodeTail = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGList<TYPE, ARG_TYPE, pAlloc>::AddHead(CSMGList* pNewList)
{
	SMG_ASSERT_VALID(this);

	ASSERT(pNewList != NULL);
	SMG_ASSERT_VALID(pNewList);

	// add a list of same elements to head (maintain order)
	POSITION pos = pNewList->GetTailPosition();
	while (pos != NULL)
		AddHead(pNewList->GetPrev(pos));
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGList<TYPE, ARG_TYPE, pAlloc>::AddTail(CSMGList* pNewList)
{
	SMG_ASSERT_VALID(this);
	ASSERT(pNewList != NULL);
	SMG_ASSERT_VALID(pNewList);

	// add a list of same elements
	POSITION pos = pNewList->GetHeadPosition();
	while (pos != NULL)
		AddTail(pNewList->GetNext(pos));
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
TYPE CSMGList<TYPE, ARG_TYPE, pAlloc>::RemoveHead()
{
	SMG_ASSERT_VALID(this);
	ASSERT(m_pNodeHead != NULL);  // don't call on empty list !!!

	CNode* pOldNode = m_pNodeHead;
	TYPE returnValue = pOldNode->data;

	m_pNodeHead = pOldNode->pNext;
	if (m_pNodeHead != NULL)
		m_pNodeHead->pPrev = NULL;
	else
		m_pNodeTail = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
TYPE CSMGList<TYPE, ARG_TYPE, pAlloc>::RemoveTail()
{
	SMG_ASSERT_VALID(this);
	ASSERT(m_pNodeTail != NULL);  // don't call on empty list !!!

	CNode* pOldNode = m_pNodeTail;
	TYPE returnValue = pOldNode->data;

	m_pNodeTail = pOldNode->pPrev;
	if (m_pNodeTail != NULL)
		m_pNodeTail->pNext = NULL;
	else
		m_pNodeHead = NULL;
	FreeNode(pOldNode);
	return returnValue;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
POSITION CSMGList<TYPE, ARG_TYPE, pAlloc>::InsertBefore(POSITION position, ARG_TYPE newElement)
{
	SMG_ASSERT_VALID(this);

	if (position == NULL)
		return AddHead(newElement); // insert before nothing -> head of the list

	// Insert it before position
	CNode* pOldNode = (CNode*) position;
	CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);
	pNewNode->data = newElement;

	if (pOldNode->pPrev != NULL)
	{
		pOldNode->pPrev->pNext = pNewNode;
	}
	else
	{
		ASSERT(pOldNode == m_pNodeHead);
		m_pNodeHead = pNewNode;
	}
	pOldNode->pPrev = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
POSITION CSMGList<TYPE, ARG_TYPE, pAlloc>::InsertAfter(POSITION position, ARG_TYPE newElement)
{
	SMG_ASSERT_VALID(this);

	if (position == NULL)
		return AddTail(newElement); // insert after nothing -> tail of the list

	// Insert it before position
	CNode* pOldNode = (CNode*) position;
	
	CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);
	pNewNode->data = newElement;

	if (pOldNode->pNext != NULL)
	{
		pOldNode->pNext->pPrev = pNewNode;
	}
	else
	{
		ASSERT(pOldNode == m_pNodeTail);
		m_pNodeTail = pNewNode;
	}
	pOldNode->pNext = pNewNode;
	return (POSITION) pNewNode;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
void CSMGList<TYPE, ARG_TYPE, pAlloc>::RemoveAt(POSITION position)
{
	SMG_ASSERT_VALID(this);

	CNode* pOldNode = (CNode*) position;
	

	// remove pOldNode from list
	if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;
	}
	else
	{
		pOldNode->pPrev->pNext = pOldNode->pNext;
	}
	if (pOldNode == m_pNodeTail)
	{
		m_pNodeTail = pOldNode->pPrev;
	}
	else
	{
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}
	FreeNode(pOldNode);
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
POSITION CSMGList<TYPE, ARG_TYPE, pAlloc>::FindIndex(int nIndex) const
{
	SMG_ASSERT_VALID(this);

	if (nIndex >= m_nCount || nIndex < 0)
		return NULL;  // went too far

	CNode* pNode = m_pNodeHead;
	while (nIndex--)
	{
		
		pNode = pNode->pNext;
	}
	return (POSITION) pNode;
}

template<class TYPE, class ARG_TYPE,const CSMGMemAllocator * pAlloc>
POSITION CSMGList<TYPE, ARG_TYPE, pAlloc>::Find(ARG_TYPE searchValue, POSITION startAfter) const
{
	SMG_ASSERT_VALID(this);

	CNode* pNode = (CNode*) startAfter;
	if (pNode == NULL)
	{
		pNode = m_pNodeHead;  // start at head
	}
	else
	{
		
		pNode = pNode->pNext;  // start after the one specified
	}

	for (; pNode != NULL; pNode = pNode->pNext)
		if (CompareElements<TYPE>((TYPE*)&pNode->data, (TYPE*)&searchValue))
			return (POSITION)pNode;
	return NULL;
}

#define	CList	CSMGList


template<class T,const CSMGMemAllocator * pAlloc = &g_CmnAlloc>
class CBlockAllocator{
#ifndef	SMG_SERVICE
public:
	struct CNode{
		T		data;
		CNode*	pNext;
	};
private:
	CNode* m_pNodeFree;
	CSMGPlex<pAlloc>* m_pBlocks;
	int m_nBlockSize;
	BOOL m_bSerialize;
#endif
public:
	CBlockAllocator(int nBlockSize = 32,BOOL bSerialize = FALSE)
	{
#ifndef	SMG_SERVICE
		m_pNodeFree	= NULL;
		m_pBlocks = NULL;
		m_nBlockSize = nBlockSize;
		m_bSerialize = bSerialize;
#endif
	}
	~CBlockAllocator()
	{
		RemoveAll();
	}
	void RemoveAll()
	{
#ifndef	SMG_SERVICE
		if (!m_bSerialize){
			m_pNodeFree = NULL;
			if(m_pBlocks)m_pBlocks->FreeDataChain();
			m_pBlocks = NULL;
		}
#endif
	}
	T * AllocData()
	{
#ifndef	SMG_SERVICE
		if (m_bSerialize){
#endif
			T * data = (T*)pAlloc->Alloc(sizeof(T));
			new (data) T;
			return data;
#ifndef	SMG_SERVICE
		}
		if (m_pNodeFree == NULL)
		{
			// add another block
			CSMGPlex<pAlloc>* pNewBlock = CSMGPlex<pAlloc>::Create(m_pBlocks, m_nBlockSize,
					 sizeof(CNode));

			// chain them into free list
			CNode* pNode = (CNode*) pNewBlock->data();
			// free in reverse order to make it easier to debug
			pNode += m_nBlockSize - 1;
			for (int i = m_nBlockSize-1; i >= 0; i--, pNode--)
			{
				pNode->pNext = m_pNodeFree;
				m_pNodeFree = pNode;
			}
		}
		ASSERT(m_pNodeFree != NULL);  // we must have something

		CNode* pNode = m_pNodeFree;
		m_pNodeFree = m_pNodeFree->pNext;
		pNode->pNext = NULL;
		new (&pNode->data) T;
		return &pNode->data;
#endif
	}
	VOID FreeData(T* data)
	{
#ifndef	SMG_SERVICE
		if (m_bSerialize){
#endif
			data->~T();
			pAlloc->Free(data);
			return;
#ifndef	SMG_SERVICE
		}
		data->~T();
		CNode *pNode = (CNode*)(VOID*)data;
		pNode->pNext = m_pNodeFree;
		m_pNodeFree = pNode;
#endif
	}
};
	

#endif
