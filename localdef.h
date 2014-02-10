#ifndef LOCALDEF_H
#define LOCALDEF_H

#include "basedef.h"
#include <memory.h>
#include <list>
void __cdecl memoryunenough();
bool __cdecl PointInBound(SMG_Point &point, SMG_Rect *rect);
bool IntersectRect(SMG_Rect &rcSelf,SMG_Rect &rc);
bool __cdecl InGfxRect(SMG_Rect &rcSelf,SMG_Rect &rc);
ULONG LOG4(ULONG nID);
ULONG GetLayerNum(ULONG nID);
ULONG GetParentNodeID(ULONG nID);
ULONG GetCurrentNodeID(ULONG nID);
ULONG GetYu4(ULONG nID);
SMG_Rect const &GetNodeBound(const SMG_Rect &bnd,ULONG nID) ;
bool IsParent(ULONG nID,ULONG nParentID);
int GetBndIndex(SMG_Rect bnd, SMG_Point &objcenpt,int nLayer, int& nId);
void BackupElement(long pID);
void updateIndex(long tr,long id);
#endif 