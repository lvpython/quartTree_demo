// 4node_demo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
//#include ".\4treenode\quarttree.h"
//#include "localdef.h"
#include "EvBroadcast.h"
//typedef void(*memoryunenough)(void *)

void test()
{
/*	CQuartTree tree;
	set_new_handler(0);
//morton¬Î≤‚ ‘
	ULONG ii =LOG4(4);
	ii =LOG4(0);
	ii=GetLayerNum(4);
	ii=GetLayerNum(0);
	ii=GetLayerNum(5);
	ii=GetLayerNum(16);
	ii=GetLayerNum(20);
	ii=GetLayerNum(21);
	ii=GetParentNodeID(5);
	ii=GetParentNodeID(1);
	ii=GetParentNodeID(0);
	ii=GetParentNodeID(20);
	ii=GetParentNodeID(21);
	ii=GetParentNodeID(72);
	ii=GetParentNodeID(69);
	ii=GetParentNodeID(68);
	ii=GetParentNodeID(84);
	ii=GetParentNodeID(85);
//	char* tree1=NULL;
//	makelowmem(tree1);
	SMG_Point pt1;
	pt1(0xfffffe,0xfffffe);

	SMG_Rect rc(0,0,0xffffff,0xffffff);
	int id=0;
	ii = GetBndIndex(rc, pt1,1,id);
	tree.SetMapBound(rc);
	GeoMsgPoint *pt;
	for (int i=0;i<1000;i++)
	{
		pt= new GeoMsgPoint;
		tree.AddObject(pt);
	}
	tree.RemoveObject(pt);
	CGeoObjects* record =new CGeoObjects;
	rc(0,0,0xffff,0xffff);
	tree.GetBoundObjects(rc , &record);
	list<GeoBase*>::iterator listiter;
	int dw =record->size();
	for(listiter=record->begin(); listiter != record->end();listiter++)
	{
			ULONG i = (*listiter)->GetNodeID();
			char ch[32]="";
			sprintf(ch,"the Node ID is %d \r\n", i);
			SMG_Rect rc;
			(*listiter)->get_Bound(&rc);
			OutputDebugString(ch);		
	}
	record->clear();
	delete record;
	*/
}
void makelowmem(char * ch)
{
	ch=new char[1000];
	if(ch ==NULL)
	{
		assert(1);
	}
	makelowmem(ch);
	
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	{
		EvBoardcast event;
		SMG_Point pt;
		//	pt.x=13567;
		//	pt.y=12345;
		event.PreReadPoint(pt);
		
//		msgpt.ptGeoPoint.x=pt.x;
//		msgpt.ptGeoPoint.y=pt.y;
		for (int i=0;i<10000;i++)
		{
			DB_msgpoint msgpt;
			event.AddEvPoint(msgpt);
		}
	event.tree.constructTree(pt,0xffffffff,0xffffffff);
	CGeoObjects* record =new CGeoObjects;
	event.GetAroundObjects(pt , &record);
	list<GeoBase*>::iterator listiter;
	int dw =record->size();
	for(listiter=record->begin(); listiter != record->end();listiter++)
	{
			ULONG i = (*listiter)->GetNodeID();
			char ch[32]="";
			sprintf(ch,"the Node ID is %d \r\n", i);
			SMG_Rect rc;
			(*listiter)->get_Bound(&rc);
			OutputDebugString(ch);		
	}
	record->clear();
	delete record;
	}


/*	SMG_Rect rc(0,0,0xffffffff,0xffffffff);
	SMG_Rect rd=GetNodeBound(rc ,5);
	ULONG ii =LOG4(2);
	ii =LOG4(0);
	ii=GetLayerNum(4);
	ii=GetLayerNum(0);
	ii=GetLayerNum(5);
	ii=GetLayerNum(16);
	ii=GetLayerNum(20);
	ii=GetLayerNum(21);
	ii=GetParentNodeID(5);
	ii=GetParentNodeID(1);
	ii=GetParentNodeID(0);
	ii=GetParentNodeID(20);
	ii=GetParentNodeID(21);
	ii=GetParentNodeID(72);
	ii=GetParentNodeID(69);
	ii=GetParentNodeID(68);
	ii=GetParentNodeID(84);
	ii=GetParentNodeID(85);
	ii=GetCurrentNodeID(0);
	*/
	return 0;
}



