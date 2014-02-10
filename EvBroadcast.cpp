#include "Evbroadcast.h"

EvBoardcast::EvBoardcast()
{
	SMG_Rect rc(0,0,0xffffffff,0xffffffff);
	tree.SetMapBound(rc);
//	tree.LoadIndex();
}

EvBoardcast::~EvBoardcast()
{

}

void EvBoardcast::PreReadPoint(SMG_Point& pt)
{
	tree.constructTree(pt);
}

void EvBoardcast::GetAroundObjects(SMG_Point& pt,CGeoObjects **gObjects)
{
	tree.GetBoundObjects(tree.m_rcMapBound,gObjects);
}
void EvBoardcast::AddEvPoint(DB_msgpoint& pt)
{
	PreReadPoint(pt.ptGeoPoint);
	GeoMsgPoint *pMsg =new GeoMsgPoint;
	pMsg->putmsgPoint(pt);
	tree.AddDataObject(pMsg);
}

void EvBoardcast::RemovePoint(ULONG objectID)
{
	
}