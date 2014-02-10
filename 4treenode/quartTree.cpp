#include "quarttree.h"
#include "..\localdef.h"
//#include "..\basedef.h"
#define NODE_MAXOBJECTNUM 12
FILE* index;
FILE* data;
typedef GeoBase * ptrGeoBase ;
CSMGMemAllocator			g_CmnAlloc;
CQuartTree::CQuartTree()
{
	OutputDebugString("  Construct QuartTree ...\n");

	m_lMaxID = 0;                                       //ID 
	m_nMaxObject = 0;

	m_TreeBoot = NULL;						//�����ڵ�
	m_TreeBoot = new CTreeNode;
	m_nLayer = 0;
	if(m_TreeBoot)
	{
		((CTreeNode*)m_TreeBoot)->m_pTree = this;
		m_TreeBoot->SetNodeID(m_lMaxID);
	}
	LoadIndex();
}
CQuartTree::~CQuartTree()
{
	OutputDebugString("  Release QuartTree ...\n");
	FinalRelease();

}

void CQuartTree::FinalRelease()
{
	ULONG dw = m_mapQuartTree.size();
	list<ptrDB_index>::iterator m_listiter = m_mapQuartTree.begin();
	while(dw)
	{
		DB_index * gBase = *m_listiter;
		delete gBase;
		m_listiter ++;
		dw --;
	}
	m_mapQuartTree.clear();
	delete m_TreeBoot;
}
CTreeNode* CQuartTree::GetBootNode()
{
	CTreeNode *pNode = NULL;
	if(!m_TreeBoot)return NULL;
	pNode = m_TreeBoot;
	return pNode;
}
// CGeoObjects �������ⲿ����
bool CQuartTree::GetObjectsInPoint(SMG_Point &point ,CGeoObjects *gObjects)
{
	bool ret=m_TreeBoot->GetObjectsInPoint(point, gObjects);
	return ret;
}
void CQuartTree::RemoveAll()
{
	m_mapQuartTree.clear();
	m_TreeBoot->Clear();
}
bool CQuartTree::GetBoundObjects(SMG_Rect bound,CGeoObjects **gObjects)
{
	bool ret = m_TreeBoot->GetObjects(bound, gObjects);
	return ret;
}

void CQuartTree::GetObjects(SMG_Rect bound, CGeoObjects *gObjects)
{
	
}

void CQuartTree::AddObject(GeoBase * gBase)
{
	SMG_Rect		bnd;		//����������
	SMG_Point		cenpt;		//�������ĵ�
	gBase->get_Bound(&bnd);
	cenpt.x = ( bnd.left + bnd.right ) / 2;
	cenpt.y = ( bnd.top  + bnd.bottom ) / 2;

	AddTree(m_TreeBoot, gBase, bnd, cenpt);
}

void CQuartTree::AddDataObject(GeoBase * gBase)
{
	SMG_Rect			bnd;		//����������
	SMG_Point		cenpt;		//�������ĵ�
	gBase->get_Bound(&bnd);
	cenpt.x =  bnd.left/2 + bnd.right  / 2;
	cenpt.y =  bnd.top /2 + bnd.bottom  / 2;

	AddTree(m_TreeBoot, gBase, bnd, cenpt,true);
}
void CQuartTree::RemoveObject(GeoBase * gBase)
{//ɾ���������ݼ���ɾ��Tree�е�gbase��ɾ������
	ULONG nObjectID;
	DB_index * pNode;
	nObjectID = gBase->GetObjectID();
	//�ҵ���Ӧ������
	if(m_mapQuartTree.Lookup(nObjectID, pNode))
	{
		((CTreeNode *)pNode->ptrGeo)->RemoveObject(gBase);
	}

}

void CQuartTree::SetMapBound(SMG_Rect bound)
{
	m_rcMapBound = bound;
	m_TreeBoot->SetNodeRect(bound);
}

bool CQuartTree::AddTree(CTreeNode* treeNode , GeoBase *gBase, SMG_Rect bnd, SMG_Point cenpt,bool bAddIndex)
{
	//��û���ཻ, �򷵻�
	SMG_Rect rcNode;
	bool	b;
	rcNode = treeNode->GetNodeRect();
	SMG_Rect rc;
	rc=treeNode->GetBound();
//	if(!IntersectRect(rcNode , bnd)) 
	if(!InGfxRect(rcNode , bnd))
	{//���������Ҫ��������GetnodeRnd�룬point,width,height���岻ͬ
		delete gBase;
		return false;    
	}
	b = treeNode->HaveSubNode();
	if(!b)
	{
		//��û���ӽ�㣬 �жϽ��Ķ�������Ƿ���� ��������-1
		ULONG   nObjectNum;
		nObjectNum = treeNode->GetObjectNum();
		((CTreeNode*)m_TreeBoot)->m_pTree = this;
		if( nObjectNum >= NODE_MAXOBJECTNUM - 1)
		{
			//�з�����
			treeNode->SplitSubTree(bAddIndex);
			for(int i= 0; i< 4 ;i++)
			{
				ULONG lNodeID;
				CTreeNode * pNode = ((CTreeNode*)treeNode)->m_pNextNode[i];
				lNodeID = pNode->GetNodeID();
			}
		}
		else
		{
			bool bChange;
//�������gBase��List��,����m_rcBound���е���
//����gBase �ӵ�����β
			bChange = treeNode->AddObject(gBase,bAddIndex);
			
			return bChange;
		}
	}
	int index;
	index = treeNode->GetBndIndex(bnd, cenpt);
	if(index < 0)
	{
		bool bChange;
		bChange = treeNode->AddObject(gBase,bAddIndex);
		return bChange;
	}
	CTreeNode* pNode = ((CTreeNode*)treeNode)->m_pNextNode[index];
	if(AddTree( pNode, gBase,  bnd, cenpt,bAddIndex))
	{
		//���η�Χ��		
		bool bChangeBnd = false;
		SMG_Rect rc;
		SMG_Rect bound;
		bound = treeNode->GetBound();
		rc = pNode->GetBound();
		//�жϸ����ľ���
		if( rc.left   < bound.left   ) 
			{bound.left   = rc.left  ; bChangeBnd = true;}
		if( rc.right  > bound.right  ) 
			{bound.right  = rc.right ; bChangeBnd = true;}
		if( rc.top    < bound.top    ) 
			{bound.top    = rc.top   ; bChangeBnd = true;}
		if( rc.bottom > bound.bottom ) 
			{bound.bottom = rc.bottom; bChangeBnd = true;}
		treeNode->SetBound(bound);
		return bChangeBnd;
	}
	return FALSE;

}
//��ȡ����
bool CQuartTree::LoadIndex()
{
	bool b=fAccess.ReadIndex(this);
	return b;
}

//TreeԤ��,��,��
bool CQuartTree::constructTree(SMG_Point& curPt,long height,long width)
{
	static ULONG lastNodeID=-1;  //�ϴε������ 
	int id = 0;
	if (m_nLayer>16)
	{
		assert(0);
	}
	ULONG left;
	ULONG top;
	ULONG right;
	ULONG bottom;
	((ULONG)(width>curPt.x))?(left=0):(left=curPt.x-width);
	((ULONG)(height>curPt.y))?(top=0):(top=curPt.y-height);
	((ULONG)(0xffffffff-width)<(ULONG)curPt.x)?(right=-1):(right=curPt.x+width);
	((ULONG)(0xffffffff-height)<(ULONG)curPt.y)?(bottom=-1):(bottom=curPt.y+height);
	SMG_Rect rc(left,top,right,bottom);
	curPt.x=left/2+right/2;
	curPt.y=top/2+bottom/2;
	SMG_Rect rcTemp;
	m_nCurNodeID = GetBndIndex(m_rcMapBound, curPt,15,id);
	rcTemp= GetNodeBound(m_rcMapBound,m_nCurNodeID);
	while ( !InGfxRect( rcTemp, rc) )  //��rcTemp����rc,jump off
	{
		m_nCurNodeID= GetParentNodeID(m_nCurNodeID);
		rcTemp= GetNodeBound(m_rcMapBound,m_nCurNodeID);
	}
	if (m_nCurNodeID==lastNodeID)
		return true;
	else
		lastNodeID = m_nCurNodeID;
	//��ʱ��m_nCurNodeIDΪ��Ӧ�ĵ�ַ��
	//��������Ѱ�����ݿ�
	/*���������1�޸��ڵ㣬���ӽڵ� //�Ըõ���ΪTreeNode
				2�޸��ڵ㣬���ӽڵ� //�޸ÿ����� ȡ��Ϊ���ڵ�
				3�и��ڵ㣬���ӽڵ� //ȡID���ĸ��ڵ���ΪTreeNode
				4�и��ڵ㣬���ӽڵ� //�Ըõ���ΪTreeNode�����ڵ㲻���
	*/
	list<ptrDB_index>::iterator  iterater;
	ULONG mask=0;   //01��Ϊ�и��ڵ㣬10�����ӽڵ�
	ULONG maxIndex=0;
	ptrGeoBase* pMsg =new ptrGeoBase;
	for (iterater=m_mapQuartTree.begin();iterater!=m_mapQuartTree.end();iterater++)
	{
		if ((*iterater)->nodeID>=m_nCurNodeID)
		{
			//����iterater�Ƿ�Ϊm_nCurNodeID���ӽڵ�.�����Ϊ�ӽڵ�
			if (IsParent((*iterater)->nodeID,m_nCurNodeID))
			{
				mask = (mask|0x10);
				//װ������
				if (fAccess.ReadData(pMsg,(*iterater)->GeoType,(*iterater)->nOffset))
				{
					(*pMsg)->SetObjectID((*iterater)->IID);
					(*pMsg)->SetNodeID((*iterater)->nodeID);
					m_list.push_back((*pMsg));
				}
/*				if (data)
				{
					m_nCurParentID = (*iterater)->nodeID;
					pMsg =new GeoMsgPoint;
					if (!pMsg)
					{
						return false;
					}
				pMsg->SetNodeID((*iterater)->nodeID);
					pMsg->SetObjectID((*iterater)->IID);
					DB_msgpoint point;
					fseek(data,(*iterater)->nOffset,SEEK_SET);
					fread(&point,sizeof(char),sizeof(DB_msgpoint),data);
					pMsg->putmsgPoint(point);
					(*iterater)->ptrGeo=(ULONG)pMsg;
					m_list.push_back(pMsg);
				}
*/			}
		}
		else
		{
			if ((*iterater)->nodeID<m_nCurNodeID)
			{
				//����iterater�Ƿ�Ϊm_nCurNodeID�ĸ��ڵ�
				if (IsParent(m_nCurNodeID,(*iterater)->nodeID))
				{//����ֻ������ӽ�RCNode�Ľڵ�
					mask = (mask|0x01);
					if ((*iterater)->nodeID>=maxIndex)
						{
							maxIndex=(*iterater)->nodeID;
							if (fAccess.ReadData(pMsg,(*iterater)->GeoType,(*iterater)->nOffset))
							{
								(*pMsg)->SetObjectID((*iterater)->IID);
								(*pMsg)->SetNodeID((*iterater)->nodeID);
								m_list.push_back((*pMsg));
							}
							//װ������
						}
				}
			}
		}
	}
	delete pMsg;
	m_TreeBoot->FinalRelease();
	if (mask&0xf0)   //���ӽڵ�
		m_lMaxID = m_nCurNodeID;
	else
	{
		if(mask&0xf)
		{
			m_lMaxID = maxIndex;
		}
		else
			m_lMaxID = 0;
	}
	m_TreeBoot->SetNodeRect(GetNodeBound(m_rcMapBound,m_lMaxID));
	m_TreeBoot->SetNodeID(m_lMaxID);
	//����Tree�ṹ
	CreateTree(&m_list);
	m_list.clear();
	return true;
}


void CQuartTree::CreateTree(CGeoObjects *gObjects)
{
	CGeoObjects::iterator  iterater;
	for (iterater=gObjects->begin();iterater!=gObjects->end();iterater++)
	{
		AddObject(*iterater);
	}
}
