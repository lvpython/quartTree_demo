#ifndef BASEDEF_H
#define BASEDEF_H
#include "Windows.h"
#include <stdlib.h>
#include <stdio.h>


typedef struct _SMG_Point
{
    ULONG  x;
    ULONG  y;
#ifdef TEST
	_SMG_Point()
	{
		x=rand();
		y=rand();
	}
#endif
	void operator()(ULONG x1,ULONG y1)
	{
		x =x1;
		y =y1; 
	}
} SMG_Point;

typedef struct _SMG_Rect

{

    ULONG    left;
    ULONG    top;
    ULONG    right;
    ULONG    bottom;
	
	_SMG_Rect()
	{	
	}
    _SMG_Rect(ULONG lf,ULONG tp, ULONG rt, ULONG bm)
	{
		left =lf;
		top=tp;
		right=rt;
		bottom=bm;
	}
	void operator()(ULONG lf,ULONG tp, ULONG rt, ULONG bm)
	{
		left =lf;
		top=tp;
		right=rt;
		bottom=bm; 
	}
	struct _SMG_Rect operator=( const struct tagRECT  &rcSrc)
	{
		struct _SMG_Rect rcDst;
		rcDst.top=rcSrc.top;
		rcDst.right=rcSrc.right;
		rcDst.bottom=rcSrc.bottom;
		rcDst.left=rcSrc.left;
		return rcDst;
	}

} SMG_Rect;

struct tagRECT& operator<<(struct tagRECT &rcDst,SMG_Rect &rcSrc);


typedef struct _DB_POINTMSG
{
	SMG_Point    ptGeoPoint;
	wchar_t         wMsg[11];
	
	_DB_POINTMSG()
	{
		ptGeoPoint.x=rand();
		ptGeoPoint.y=rand();
	}

} 
DB_msgpoint, *ptrDB_msgpoint;

typedef struct _DB_INDEX
{
	ULONG IID;
	ULONG nodeID;
	ULONG nOffset;
	short GeoType;
	short dirty; 
	_DB_INDEX()
	{	
	}
    _DB_INDEX(ULONG id,ULONG nID, ULONG nFt,short pGeo)
	{
		IID =id;
		nodeID=nID;
		nOffset=nFt;
		GeoType =pGeo;
		dirty =0;
	}
}
DB_index,*ptrDB_index;

#ifdef WIN32
typedef	unsigned short wchar_t;
#endif

typedef unsigned long ULONG;

#endif 