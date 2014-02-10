#ifndef QUARTTREE_H
#define QUARTTREE_H

#include "..\basedef.h"
#include "..\fileindex.h"
#include "treenode.h"
#include "..\hash_map\portarray.h"
#include <map>


class CTreeNode;
class FileIndex;
extern CSMGMemAllocator		g_CmnAlloc;
class CQuartTree
{
public:
	CQuartTree();
	~CQuartTree();

	bool LoadIndex();
	bool constructTree(SMG_Point& curPt,long height=255,long width=255);

	
	void FinalRelease();
	CTreeNode* GetBootNode();
	bool GetObjectsInPoint(SMG_Point &point ,CGeoObjects *gObjects);
	void RemoveAll();
	bool GetBoundObjects(SMG_Rect bound,CGeoObjects **gObjects);

	SMG_Rect			m_rcMapBound;			//地图矩形范围
	CTreeNode*	m_TreeBoot;				//四叉树根	
	

	list<ptrDB_index>  m_mapQuartTree;		//四叉树哈希表
	CGeoObjects m_list;                  //节点数据集
	
	ULONG m_nMaxObject;       //ObjectID号
	long m_lMaxID;
	long m_nLayer;    //数据层数
	ULONG m_nCurNodeID;
	FileIndex fAccess;

	void GetObjects(SMG_Rect bound, CGeoObjects *gObjects);
	void CreateTree(CGeoObjects *gObjects);
	void AddObject(GeoBase * gBase);
	void AddDataObject(GeoBase * gBase);
	void RemoveObject(GeoBase * gBase);
	void SetMapBound(SMG_Rect bound);
	bool AddTree(CTreeNode* treeNode , GeoBase *gBase, SMG_Rect bnd, SMG_Point cenpt,bool bAddIndex=false);
};
#endif 