#ifndef TREENODE_H
#define TREENODE_H

#include "..\basedef.h"
#include "..\geobase.h"

#include <list>
#include <algorithm>
#include <map>
using namespace std ;


typedef list<GeoBase*> CGeoObjects;
class CQuartTree;
class CTreeNode
{
public :
	CTreeNode();
	virtual ~CTreeNode();
	void	FinalRelease();

	long GetObjectID(int &nIndex);
	CTreeNode* GetNextNode(int &nIndex);
    bool GetObjectsInPoint(SMG_Point &point ,CGeoObjects *gObjects);
	bool GetObjects(SMG_Rect bound ,CGeoObjects **gObjects);
	int GetBndIndex(SMG_Rect bnd, SMG_Point &objcenpt);

	void SetNodeRect(SMG_Rect bnd);
	SMG_Rect &GetNodeRect();
	void SetBound(SMG_Rect &bound);
	SMG_Rect &GetBound();
	CTreeNode* GetParentNode();
	void SetParentNode(CTreeNode* pTreeNode);
	ULONG GetNodeID();
	void SetNodeID(ULONG nID);

	void ReCalBound();
	void SplitSubTree(bool bAddIndex);
	bool AddObject(GeoBase* gBase,bool bAddIndex=false);//缺省不记录数据
	bool HaveSubNode();
	void UnitSubNode();
	void RemoveObject(GeoBase* gBase);
	long GetObjectNum();
	void Clear();

	CQuartTree*		m_pTree;
	ULONG			m_lNodeID;				//结点ID
	CTreeNode*  m_pNextNode[4];			//子结点数组
	list<GeoBase*> m_list;
	list<GeoBase*>::iterator m_listiter;
private:
	SMG_Rect			m_rcNode;					//结点矩形
	SMG_Rect			m_rcBound;				//结点中所有对象的外包络矩形
	CTreeNode*			m_pPrevNode;				//前驱结点指针
};
#endif
