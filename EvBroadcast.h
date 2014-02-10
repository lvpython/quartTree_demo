#ifndef EVBOARDCAST_H
#define EVBOARDCAST_H
#include ".\4treenode\quarttree.h"
#include "localdef.h"

class EvBoardcast
{
public:
	EvBoardcast();
	~EvBoardcast();

	void PreReadPoint(SMG_Point& pt);
	void GetAroundObjects(SMG_Point& pt,CGeoObjects **gObjects);
	void AddEvPoint(DB_msgpoint& pt);
	void RemovePoint(ULONG objectID);
	
	CQuartTree tree;
private:
	void Release();



};




#endif 