
#include "treeNode.h"
#include "quarttree.h"
extern FILE* index;
extern FILE* data;
/*
4007570400 0��  ����ܳ�
4000854800 0��  �������ܳ�
*/
#define NODE_MAXOBJECTNUM 12


#include "..\localdef.h"
CTreeNode::CTreeNode()
{
	m_lNodeID = 0;
	ZeroMemory(&m_rcNode, sizeof(SMG_Rect));
	ZeroMemory(&m_rcBound, sizeof(SMG_Rect));
	m_pPrevNode = NULL;
	for(int i= 0;i< 4;  i++)
	{
		m_pNextNode[i] = NULL;
	}
	m_pTree = NULL;
}
CTreeNode::~CTreeNode()
{
	FinalRelease();
}
void CTreeNode::FinalRelease()
{
	for(int i= 0;i< 4; i++)
	{
		if(m_pNextNode[i]) delete m_pNextNode[i];
		m_pNextNode[i] = NULL;
	}
	m_pPrevNode = NULL;
	ULONG dw = m_list.size();
	m_listiter = m_list.begin();
	while(dw)
	{
		GeoBase * gBase = *m_listiter;
		delete gBase;
		m_listiter ++;
		dw --;
	}
	m_list.clear();
	
}
//ȡ�ö��������ID
long CTreeNode::GetObjectID(int &nIndex)
{
	int objectID = -1;
	if(nIndex >= m_list.size())return E_FAIL;
	m_listiter = m_list.begin();
	for(int i = 0; i< m_list.size(); i++)
	{
		if ( i == nIndex)
		{
			GeoBase* gBase = *m_listiter;
			objectID = gBase->getType();
			return S_OK;
		}
		m_listiter++;
	}
	return objectID;
}

CTreeNode* CTreeNode::GetNextNode(int &nIndex)
{
	CTreeNode *pNode = NULL;
	if(nIndex >= 4) return NULL;
	pNode = m_pNextNode[nIndex];
	return pNode; 
}
bool CTreeNode::GetObjectsInPoint(SMG_Point &point ,CGeoObjects *gObjects)
{
	if(!PointInBound(point, &m_rcBound))return false;

	ULONG dw = m_list.size();
	m_listiter = m_list.begin();
	while(dw)
	{
		gObjects->end();
		GeoBase * gBase = *m_listiter;
		SMG_Rect bound;
		gBase->get_Bound(&bound);
		if(PointInBound(point, &bound))
			gObjects->push_back(*m_listiter);
		m_listiter ++;
		dw --;
	}
	if(!m_pNextNode[0])return false;
	for(int i= 0;i< 4; i++)
	{
		m_pNextNode[i]->GetObjectsInPoint(point, gObjects);
	}
	return true;
}
bool CTreeNode::GetObjects(SMG_Rect bound ,CGeoObjects **gObjects)
{
//	if(!IntersectRect(m_rcBound , bound)) 
//		return false;

	ULONG dw = m_list.size();
	m_listiter = m_list.begin();
	while(dw)
	{
		(*gObjects)->end();
		(*gObjects)->push_back(*m_listiter);
		ULONG id = (*m_listiter)->GetNodeID();
		m_listiter ++;
		dw --;
	}
	if(!m_pNextNode[0]) return false;
	for(int i= 0;i< 4; i++)
	{
		m_pNextNode[i]->GetObjects(bound, gObjects);
	}
	return true;
}
int CTreeNode::GetBndIndex(SMG_Rect bnd, SMG_Point &objcenpt)
{
	int index = -1;
	float width, height;
	SMG_Point cenpt;
	cenpt.x = ( m_rcNode.right + m_rcNode.left ) / 2;
	cenpt.y = (m_rcNode.top + m_rcNode.bottom ) / 2;
	
	if( bnd.left < cenpt.x && bnd.right > cenpt.x 
		&& bnd.top < cenpt.y && bnd.bottom > cenpt.y )
	{
			index = -1;
	}
	else
	{

		//�������ĸ��ӽ�㶼�ཻ, �Ա����ڸý��
		width = ( m_rcNode.right - m_rcNode.left ) / 2;
		height = ( m_rcNode.bottom -m_rcNode.top ) / 2;
		int row,col;
		col = (int)(( objcenpt.x - m_rcNode.left ) / width);
		if(col >=2)col = 1;
		if(col < 0) col = 0;
		row = (int)( ( objcenpt.y - m_rcNode.top ) / height );
		if(row >=2)row = 1;
		if(row < 0) row = 0;
		index = col + row * 2;
	}	
	return index;
}

void CTreeNode::SetNodeRect(SMG_Rect bnd)
{
	m_rcNode = bnd;
}

SMG_Rect &CTreeNode::GetNodeRect()
{
	return m_rcNode;
	
}
void CTreeNode::SetBound(SMG_Rect &bound)
{
	m_rcBound = bound;
}
SMG_Rect &CTreeNode::GetBound()
{
	return m_rcBound;
}
CTreeNode* CTreeNode::GetParentNode()
{
	return m_pPrevNode;
}
void CTreeNode::SetParentNode(CTreeNode* pTreeNode)
{
	m_pPrevNode=pTreeNode;
}
ULONG CTreeNode::GetNodeID()
{
	return m_lNodeID;
}
void CTreeNode::SetNodeID(ULONG nID)
{
	m_lNodeID = nID;
}

void CTreeNode::ReCalBound()
{
	GeoBase* gbase;
	SMG_Rect			rc;			//������η�Χ

	BOOL bNoBnd = TRUE;	//
	ZeroMemory(&m_rcBound, sizeof(SMG_Rect));
	if(m_pNextNode[0])
	{
		for(int i = 0; i< 4 ; i++)
		{
			BOOL  bHaveSubTree;
			long			  nObjNum;
			SMG_Rect			  bound;
			bHaveSubTree = m_pNextNode[i]->HaveSubNode();	
			nObjNum = m_pNextNode[i]->GetObjectNum();	
			bound = m_pNextNode[i]->GetBound();
			if(bNoBnd )
			{
				if(bHaveSubTree || ((!bHaveSubTree) && nObjNum))
				{
					m_rcBound = bound; 
					bNoBnd = FALSE;
				}
			}
			else
			{
				if(bHaveSubTree || ((!bHaveSubTree) && nObjNum))
				{
					if( bound.left   < m_rcBound.left   ) 
							m_rcBound.left   = bound.left  ;
					if( bound.right  > m_rcBound.right  ) 
							m_rcBound.right  = bound.right ;
					if( bound.top    < m_rcBound.top    ) 
							m_rcBound.top    = bound.top   ;
					if( bound.bottom > m_rcBound.bottom ) 
							m_rcBound.bottom = bound.bottom;
				}
			}
		}
	}

	long			dw = m_list.size();
	if(dw < 1) return  ;
	m_listiter = m_list.begin();
	if(bNoBnd)
	{
		gbase = *m_listiter;
		gbase->get_Bound(&rc);
		m_rcBound = rc;
		m_listiter ++;
		dw --;
	}
	while(dw)
	{
		gbase = *m_listiter;
		gbase->get_Bound(&rc);
		if( rc.left   < m_rcBound.left   ) m_rcBound.left   = rc.left  ;
		if( rc.right  > m_rcBound.right  ) m_rcBound.right  = rc.right ;
		if( rc.top    < m_rcBound.top    ) m_rcBound.top    = rc.top   ;
		if( rc.bottom > m_rcBound.bottom ) m_rcBound.bottom = rc.bottom;
		m_listiter ++;
		dw --;
	}
}
void CTreeNode::SplitSubTree(bool bAddIndex)
{
	float width, height;
	SMG_Point cenpt;
	width = ( m_rcNode.right - m_rcNode.left ) / 2;
	height = (m_rcNode.bottom - m_rcNode.top ) / 2;
	cenpt.x = ( m_rcNode.right + m_rcNode.left ) / 2;
	cenpt.y = (m_rcNode.top + m_rcNode.bottom ) / 2;

	for(int i= 0;i< 2; i++)
	{
		for(int j= 0;j< 2; j++)
		{
			int index = i * 2 + j;
			SMG_Rect		bound;
			CTreeNode * pNode = new CTreeNode;
			if (pNode==NULL)
			{
				return;
			}
			((CTreeNode*)pNode)->m_pTree = m_pTree;
			/*
			����Ե�ַ��������
			*/
			unsigned long nId =GetNodeID();
			nId=(nId<<2)+index+1;
			pNode->SetNodeID(nId);
			pNode->SetParentNode(this);
			//����ÿ�����ľ��η�Χ
			bound.left  = m_rcNode.left + width * j;
			bound.right  = m_rcNode.left + width * ( j + 1);
			bound.top  = m_rcNode.top +  height * i;
			bound.bottom  = m_rcNode.top  +  height * ( i + 1);
			
			pNode->SetNodeRect(bound);
			m_pNextNode[index] = pNode;

		}
	}
	GeoBase* gbase;
	SMG_Rect			rc;			//������η�Χ
	SMG_Point		objCenpt;		//�������ĵ�����
	ULONG			dw = m_list.size();
	m_listiter = m_list.begin();
	while(dw)
	{
		gbase = *m_listiter;
		gbase->get_Bound(&rc);
		objCenpt.x = ( rc.right + rc.left ) / 2;
		objCenpt.y = (rc.top + rc.bottom ) / 2;
		if( rc.left < cenpt.x && rc.right > cenpt.y 
			&& rc.top > cenpt.y && rc.bottom < cenpt.y )
		{
			//�������ĸ��ӽ�㶼�ཻ, �Ա����ڸý��
			m_listiter ++;
		}
		else
		{
			//�ж϶������������ĸ��ӽ����
			int index;
			bool bChange=false;
			int row,col;
			col = (int)(( objCenpt.x - m_rcNode.left ) / width);
			if(col >=2)col = 1;
			if(col < 0) col = 0;
			row = (int)( ( m_rcNode.top -  objCenpt.y ) / height );
			if(row >=2)row = 1;
			if(row < 0) row = 0;

			index = col + row * 2;
			bChange = m_pNextNode[index]->AddObject(gbase,false);//���ﲻ��Ҫ����
			//�޸�node��,�޸�����
			m_pTree->fAccess.UpdateIndex(gbase->GetObjectID(),m_pNextNode[index]->GetNodeID());
			//��������ɾ���ý���д�Ķ���ָ��
			list<GeoBase*>::iterator listiter;
			listiter = m_listiter;
			m_listiter ++;
			m_list.erase(listiter);
		}
		dw --;
	}
	for(i= 0;i< 4; i++)
	{
		m_pNextNode[i]->ReCalBound();
	}
	dw = m_list.size();

}
//�����ݵļ��뷽����������Ҫ�����ݵĴ��ڽ����ж�,bAddIndexΪtrue����ô�������ݼ���
bool CTreeNode::AddObject(GeoBase* gBase,bool bAddIndex)
{
	SMG_Rect  rc;
	bool bChangeBnd = false;
	gBase->get_Bound(&rc);
	gBase->SetNodeID(m_lNodeID); //�������ýڵ�ĵ�ַ��
	if(m_list.size() <1 && !m_pNextNode[0])
	{
		//������Ϊ0��û���ӽ��
		m_rcBound = rc;
		bChangeBnd = true;
	}
	else
	{
		if( rc.left   < m_rcBound.left   ) {m_rcBound.left   = rc.left  ; bChangeBnd = true;}
		if( rc.right  > m_rcBound.right  ) {m_rcBound.right  = rc.right ; bChangeBnd = true;}
		if( rc.top    < m_rcBound.top    ) {m_rcBound.top    = rc.top   ; bChangeBnd = true;}
		if( rc.bottom > m_rcBound.bottom ) {m_rcBound.bottom = rc.bottom; bChangeBnd = true;}
	}
	m_listiter = m_list.end();
	m_list.insert(m_listiter, gBase);
//������ӶԵ��ظ��Ե��ж� 
	if (bAddIndex)
	{
		gBase->SetObjectID(m_pTree->m_nMaxObject);   //����ΨһID
		DB_index *dbIndex = new DB_index( m_pTree->m_nMaxObject,m_lNodeID, 0,gBase->getType());
		m_pTree->m_nMaxObject++;
		//�����ļ�,���ݰ�dataд�룬ƫ����д��index���ݣ�indexд�������ļ���
		//��������ڵ�ID,����ΨһID�������ļ���λ��
//		if (GetLayerNum(dbIndex->nodeID)>m_pTree->m_nLayer)
//			m_pTree->m_nLayer=GetLayerNum(dbIndex->nodeID);
		if (dbIndex)
			m_pTree->m_mapQuartTree.push_back(dbIndex);
		{
			m_pTree->fAccess.WriteData(dbIndex,gBase);
			m_pTree->fAccess.WriteIndex(dbIndex);
		}
	}
	return bChangeBnd;
}

bool CTreeNode::HaveSubNode()
{
	if(m_pNextNode[0]) 
		return true;
	else
		return false;
}

void CTreeNode::UnitSubNode()
{
	if(!m_pNextNode[0]) return ;

	bool	bHaveSubTree = false;
	int count = m_list.size();
	for(int i = 0;i < 4 ;i ++)
	{
		long objNum;
		bool b;
		objNum = m_pNextNode[i]->GetObjectNum();
		count += objNum;
		b = m_pNextNode[i]->HaveSubNode();
		if(b)
			bHaveSubTree = true;
	}
	//���ӽ���»��������������ϲ�
	if(bHaveSubTree)return ;
	//�������ܺʹ���������ֵ�������ϲ�
	if(count > NODE_MAXOBJECTNUM) 
	{
		return ;
	}
	//
	count = m_list.size();
	for(i = 0;i < 4 ;i ++)
	{
		CTreeNode* pNode = (CTreeNode*)m_pNextNode[i];
		int dw = pNode->m_list.size();
		if(dw )
		{
			pNode->m_listiter = pNode->m_list.begin();
			while(dw)
			{	
				bool bChange;
				GeoBase * gBase = * pNode->m_listiter;
				bChange = AddObject(gBase,false);
				pNode->m_listiter ++;
				dw --;
				count ++;
			}
		}
		pNode->m_list.clear();
		delete m_pNextNode[i];
		m_pNextNode[i] = NULL;
	}
	if(count < NODE_MAXOBJECTNUM  && m_pPrevNode)
	{
		m_pPrevNode->UnitSubNode();
	}
}

void CTreeNode::RemoveObject(GeoBase* gBase)
{
	list<GeoBase*>::iterator listiter;
	listiter = find(m_list.begin(), m_list.end(), gBase);
	if (listiter == m_list.end()) return ;
	GeoBase* geoBase = (*listiter); 
	delete geoBase;
	m_list.erase(listiter);

	SMG_Rect oldBound = m_rcBound;
	ReCalBound();
	if(! InGfxRect( oldBound, m_rcBound) )  
	{
		//�����η����仯
		CTreeNode* pPrevNode = m_pPrevNode;
		while(pPrevNode)
		{
			SMG_Rect   newBound;
			oldBound = pPrevNode->GetBound();
			pPrevNode->ReCalBound();
			newBound = pPrevNode->GetBound();
			if(! InGfxRect( oldBound,  newBound))
				pPrevNode = pPrevNode->GetParentNode();
			else
				break;
		}
	}
	if(m_pPrevNode)
	{
		m_pPrevNode->UnitSubNode();
	}
}

long CTreeNode::GetObjectNum()
{
	return m_list.size();
}

void CTreeNode::Clear()
{
	m_lNodeID = 0;
	ZeroMemory(&m_rcNode, sizeof(SMG_Rect));
	ZeroMemory(&m_rcBound, sizeof(SMG_Rect));
	m_pPrevNode = NULL;

	for(int i= 0;i< 4; i++)
	{
		if(m_pNextNode[i]) delete m_pNextNode[i]; m_pNextNode[i] = NULL;

	}
	m_list.clear();
}