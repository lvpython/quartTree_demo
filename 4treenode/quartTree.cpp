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

	m_TreeBoot = NULL;						//树根节点
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
// CGeoObjects 生存期外部管理
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
	SMG_Rect		bnd;		//对象包络矩形
	SMG_Point		cenpt;		//对象中心点
	gBase->get_Bound(&bnd);
	cenpt.x = ( bnd.left + bnd.right ) / 2;
	cenpt.y = ( bnd.top  + bnd.bottom ) / 2;

	AddTree(m_TreeBoot, gBase, bnd, cenpt);
}

void CQuartTree::AddDataObject(GeoBase * gBase)
{
	SMG_Rect			bnd;		//对象包络矩形
	SMG_Point		cenpt;		//对象中心点
	gBase->get_Bound(&bnd);
	cenpt.x =  bnd.left/2 + bnd.right  / 2;
	cenpt.y =  bnd.top /2 + bnd.bottom  / 2;

	AddTree(m_TreeBoot, gBase, bnd, cenpt,true);
}
void CQuartTree::RemoveObject(GeoBase * gBase)
{//删除索引数据集，删除Tree中的gbase，删除索引
	ULONG nObjectID;
	DB_index * pNode;
	nObjectID = gBase->GetObjectID();
	//找到对应的子树
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
	//若没有相交, 则返回
	SMG_Rect rcNode;
	bool	b;
	rcNode = treeNode->GetNodeRect();
	SMG_Rect rc;
	rc=treeNode->GetBound();
//	if(!IntersectRect(rcNode , bnd)) 
	if(!InGfxRect(rcNode , bnd))
	{//这里的意外要，来自于GetnodeRnd与，point,width,height定义不同
		delete gBase;
		return false;    
	}
	b = treeNode->HaveSubNode();
	if(!b)
	{
		//若没有子结点， 判断结点的对象个数是否大于 最大对象数-1
		ULONG   nObjectNum;
		nObjectNum = treeNode->GetObjectNum();
		((CTreeNode*)m_TreeBoot)->m_pTree = this;
		if( nObjectNum >= NODE_MAXOBJECTNUM - 1)
		{
			//切分子树
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
//这里加入gBase到List里,并对m_rcBound进行调整
//并把gBase 加到队列尾
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
		//矩形范围变		
		bool bChangeBnd = false;
		SMG_Rect rc;
		SMG_Rect bound;
		bound = treeNode->GetBound();
		rc = pNode->GetBound();
		//判断父结点的矩形
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
//读取索引
bool CQuartTree::LoadIndex()
{
	bool b=fAccess.ReadIndex(this);
	return b;
}

//Tree预读,高,宽
bool CQuartTree::constructTree(SMG_Point& curPt,long height,long width)
{
	static ULONG lastNodeID=-1;  //上次的载入点 
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
	while ( !InGfxRect( rcTemp, rc) )  //当rcTemp包含rc,jump off
	{
		m_nCurNodeID= GetParentNodeID(m_nCurNodeID);
		rcTemp= GetNodeBound(m_rcMapBound,m_nCurNodeID);
	}
	if (m_nCurNodeID==lastNodeID)
		return true;
	else
		lastNodeID = m_nCurNodeID;
	//这时的m_nCurNodeID为适应的地址码
	//遍历索引寻找数据块
	/*四种情况：1无父节点，有子节点 //以该点作为TreeNode
				2无父节点，无子节点 //无该块数据 取根为父节点
				3有父节点，无子节点 //取ID最大的父节点做为TreeNode
				4有父节点，有子节点 //以该点作为TreeNode，父节点不理睬
	*/
	list<ptrDB_index>::iterator  iterater;
	ULONG mask=0;   //01则为有父节点，10则有子节点
	ULONG maxIndex=0;
	ptrGeoBase* pMsg =new ptrGeoBase;
	for (iterater=m_mapQuartTree.begin();iterater!=m_mapQuartTree.end();iterater++)
	{
		if ((*iterater)->nodeID>=m_nCurNodeID)
		{
			//看看iterater是否为m_nCurNodeID的子节点.相等视为子节点
			if (IsParent((*iterater)->nodeID,m_nCurNodeID))
			{
				mask = (mask|0x10);
				//装载数据
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
				//看看iterater是否为m_nCurNodeID的父节点
				if (IsParent(m_nCurNodeID,(*iterater)->nodeID))
				{//这里只保存最接近RCNode的节点
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
							//装载数据
						}
				}
			}
		}
	}
	delete pMsg;
	m_TreeBoot->FinalRelease();
	if (mask&0xf0)   //有子节点
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
	//加入Tree结构
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
