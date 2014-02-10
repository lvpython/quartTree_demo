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
	bool AddObject(GeoBase* gBase,bool bAddIndex=false);//ȱʡ����¼����
	bool HaveSubNode();
	void UnitSubNode();
	void RemoveObject(GeoBase* gBase);
	long GetObjectNum();
	void Clear();

	CQuartTree*		m_pTree;
	ULONG			m_lNodeID;				//���ID
	CTreeNode*  m_pNextNode[4];			//�ӽ������
	list<GeoBase*> m_list;
	list<GeoBase*>::iterator m_listiter;
private:
	SMG_Rect			m_rcNode;					//������
	SMG_Rect			m_rcBound;				//��������ж������������
	CTreeNode*			m_pPrevNode;				//ǰ�����ָ��
};
#endif
