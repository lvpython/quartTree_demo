#ifndef FILEINDEX_H
#define FILEINDEX_H
#include "basedef.h"
#include "geobase.h"
//#include ".\4treenode\quarttree.h"
class CQuartTree;
class FileIndex
{
public:
	FileIndex() throw();
	~FileIndex();
	bool WriteData(DB_index *dbIndex,GeoBase* gBase);
	bool WriteIndex(DB_index *dbIndex);
	bool ReadData(GeoBase **gBase,short nType,long nOffset);
	bool ReadIndex(CQuartTree *tree);
	bool UpdateIndex(ULONG nBytes,ULONG idNode);

	void* hData;
	void* hIndex;
};




#endif