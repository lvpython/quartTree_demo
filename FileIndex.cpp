#include "FileIndex.h"
#include "winbase.h"
#include ".\4treenode\quarttree.h"

#define INVALID_SET_FILE_POINTER ((DWORD)-1)

FileIndex::FileIndex()
{
	hData = NULL;
	hIndex = NULL;
	hData = CreateFile("c:\\data.txt",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,\
				NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	hIndex = CreateFile("c:\\index.txt",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,\
				NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

}
FileIndex::~FileIndex()
{
	if (hData)
		CloseHandle(hData);
	if (hIndex)
		CloseHandle(hIndex);

}
bool FileIndex::WriteData(DB_index *dbIndex,GeoBase* gBase)
{
	DWORD nBytesWrite;
	if (!hData)
		return false;
	dbIndex->nOffset = GetFileSize ( hData, NULL);
	if (dbIndex->nOffset==INVALID_FILE_SIZE)
		return false;
//	if (!SetEndOfFile(hData))
	if (INVALID_SET_FILE_POINTER== SetFilePointer(hData,0,NULL,FILE_END))
		return false;
	if (gBase->getType()==0x101)
		WriteFile(hData,(&((GeoMsgPoint *)gBase)->m_msgPoint),32,&nBytesWrite,NULL);
	return true;
}
bool FileIndex::WriteIndex(DB_index *dbIndex)
{
	DWORD nBytesWrite;
	if (!hIndex)
		return false;
//	if (!SetEndOfFile(hIndex))
	if (INVALID_SET_FILE_POINTER== SetFilePointer(hIndex,0,NULL,FILE_END))
		return false;
	if (WriteFile(hIndex,dbIndex,16,&nBytesWrite,NULL))
		return true;
	else
	{
		int i =GetLastError();
		return false;
	}
}
bool FileIndex::ReadData(GeoBase **gBase,short nType,long nOffset)
{
	if(!hData)
		return false;
	if (INVALID_SET_FILE_POINTER== SetFilePointer(hData,nOffset,NULL,FILE_BEGIN))
		return false;
	if (nType ==0x101)
	{
		(*gBase) =new GeoMsgPoint;
		if ((*gBase)==NULL)
			return false;
		DB_msgpoint point;
		DWORD nBytesRead;
		ReadFile(hData,&point,sizeof(DB_msgpoint),&nBytesRead,NULL);
		if (nBytesRead==0)
		{
			delete (*gBase);
			return false;
		}
		((GeoMsgPoint *)(*gBase))->putmsgPoint(point);
	}
	return true;

}
bool FileIndex::ReadIndex(CQuartTree *tree)
{
	if (!hIndex)
		return false;
	if (INVALID_SET_FILE_POINTER== SetFilePointer(hIndex,0,NULL,FILE_BEGIN))
		return false;
	ptrDB_index dbIndex;
	dbIndex= new DB_index;
	DWORD nBytesRead;
	while(ReadFile(hIndex,dbIndex,sizeof(DB_index),&nBytesRead,NULL))
	{
		if (nBytesRead==0)
			break;
		if (dbIndex->dirty)  //非有效数据
			continue;
		tree->m_mapQuartTree.push_back( dbIndex );
		if (dbIndex->IID >= tree->m_nMaxObject)
			tree->m_nMaxObject=dbIndex->IID+1;
		dbIndex= new DB_index;
		if (dbIndex==NULL)
			break;
	}
	if (dbIndex)
			delete dbIndex;
	return true;
}
bool FileIndex::UpdateIndex(ULONG nBytes,ULONG idNode)
{
	DWORD nBytesWrite;
	if (!hIndex)
		return false;
	nBytes=(nBytes<<4)+4;
	if (INVALID_SET_FILE_POINTER== SetFilePointer(hIndex,nBytes,NULL,FILE_BEGIN))
		return false;
	if (WriteFile(hIndex,&idNode,sizeof(ULONG),&nBytesWrite,NULL))
		return true;
	else
	{
		int i =GetLastError();
		return false;
	}
}
