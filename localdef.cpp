#include "localdef.h"

void __cdecl memoryunenough()
{
#ifdef DEBUG
	assert(1);
#endif
}

bool __cdecl PointInBound(SMG_Point &point, SMG_Rect *rect)
{
	if ((rect->left<=point.x)&&(point.x<=rect->right)
		&&(rect->top<=point.y)&&(point.y<=rect->bottom))
		return true;
	else
		return false;
}

bool IntersectRect(SMG_Rect &rcSelf,SMG_Rect &rc)
{
	RECT rc1 ;
    rc1 << rcSelf ;
	RECT rc2;
	rc2<<rc;
	RECT rc3;
	return IntersectRect(&rc3,&rc1,&rc2);
}
bool __cdecl InGfxRect(SMG_Rect &rcSelf,SMG_Rect &rc)
{
	if  ((rcSelf.left<=rc.left)&&(rcSelf.right>=rc.right)
		&&(rcSelf.top<=rc.top)&&(rcSelf.bottom>=rc.bottom))
		return true;
	else 
		return false;
}


struct tagRECT& operator<<(struct tagRECT &rcDst,SMG_Rect &rcSrc)
	{

		rcDst.top=rcSrc.top;
		rcDst.right=rcSrc.right;
		rcDst.bottom=rcSrc.bottom;
		rcDst.left=rcSrc.left;
		return rcDst;
	}

ULONG LOG4(ULONG nID)
{
	ULONG nRe=1;
	while (nID=nID>>2)
	 nRe++;
	return nRe;
}

ULONG GetLayerNum(ULONG nID)
{
	int nCount=nID;
	int nLg4 = LOG4(nCount);
	while (nLg4)
	{
		nCount -=(1<<((nLg4-1)<<1));
		nLg4--;
	}
	if (nCount>=0)
		return LOG4(nID);
	else
		return (LOG4(nID)-1);
}

ULONG GetParentNodeID(ULONG nID)
{
	ULONG i =(nID)>>2;
	//借位
	if ((GetYu4(i)==1)&&(GetYu4(nID)==0))
		i--;
	return i;
}

ULONG GetYu4(ULONG nID)
{
    return 	nID-=(((nID)>>2)<<2);
}

ULONG GetCurrentNodeID(ULONG nID)
{
	ULONG i = GetYu4(nID);
	if (i==0)
	{
		if (GetYu4(nID>>2)==1)
			return 4;
	}
	return i;
}

SMG_Rect const &GetNodeBound(const SMG_Rect &bnd,ULONG nID)
{
	SMG_Point ltPt;
	SMG_Rect bound = bnd;
	double width,height;
	std::list<ULONG> nlist;
	std::list<ULONG>::iterator nlistiter;
	int i;
	int j;
	int nTempID = nID;
	while(nTempID)
	{
		nlist.push_front(GetCurrentNodeID(nTempID));
		nTempID = GetParentNodeID(nTempID);	
	}
	for (nlistiter = nlist.begin(); nlistiter != nlist.end() ; nlistiter++)
	{
		ltPt.x = bound.left ;
		ltPt.y = bound.top ;
		width = ( bound.right - bound.left ) / 2;
		height = ( bound.bottom - bound.top ) / 2;
		i = ((*nlistiter)-1)>>1;
		j = ((*nlistiter)-1) - ( ( ((*nlistiter)-1)>>1 )<<1 );
		bound.left  = ltPt.x + width * j;
		bound.right  = ltPt.x + width * ( j + 1);
		bound.top  = ltPt.y +  height * i;
		bound.bottom  = ltPt.y  +  height * ( i + 1);
	}
	nlist.clear();
	return bound;
}


//
bool IsParent(ULONG nID,ULONG nParentID)
{
	if (nID==nParentID)
		return true;
	if (nID<nParentID)
		return false;
	else
	{
		while(nID)
		{
			nID=GetParentNodeID(nID);
			if (nID==nParentID)
				return true;
			if (nID<nParentID)
				return false;
		}
	}
	return false;

}
int GetBndIndex(SMG_Rect rcNode, SMG_Point &objcenpt,int nLayer, int& nId)
{

	int index = -1;
	double width, height;
	int row,col;
	SMG_Point cenpt;
	cenpt.x = ( rcNode.right + rcNode.left ) / 2;
	cenpt.y = (rcNode.top + rcNode.bottom ) / 2;
	
	if( rcNode.left < cenpt.x && rcNode.right > cenpt.x 
		&& rcNode.top < cenpt.y && rcNode.bottom > cenpt.y )
	{	
		width = ( rcNode.right - rcNode.left ) / 2.0;
		height = ( rcNode.bottom -rcNode.top ) / 2.0;
		col = (int)(( objcenpt.x - rcNode.left ) / width);
		if(col >=2)col = 1;
		if(col < 0) col = 0;
		row = (int)( ( objcenpt.y - rcNode.top ) / height );
		if(row >=2)row = 1;
		if(row < 0) row = 0;
		index = col + row * 2;
	}
	else
	{
		//对象与四个子结点都相交, 仍保留在该结点	
		index = -1;
	}

	if ((index!=-1)&&(nLayer))
	{
		nLayer--;
		nId = (nId<<2)+index+1;
		rcNode(rcNode.left+col*width,rcNode.top+row*height,\
				rcNode.left+(col+1)*width,rcNode.top+(row+1)*height);
		GetBndIndex(rcNode,objcenpt,nLayer,nId);
	}
	return nId;
}


void BackupElement(long pID)
{
	ptrDB_index pIndex = (ptrDB_index) pID;
}


