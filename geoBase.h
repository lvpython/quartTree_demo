#ifndef GEOBASE_H
#define GEOBASE_H
#include "basedef.h"


class GeoBase
{
public:
	virtual void get_Bound(SMG_Rect *rect)=0;
	virtual ULONG getType()=0;
	ULONG GetNodeID()
	{  
		return m_nNodeID; 
	};
	ULONG GetObjectID()
	{  
		return m_nObjectID;
	};
	void SetObjectID(ULONG id)
	{  
		m_nObjectID = id;
	};
	void SetNodeID(unsigned long nID) { m_nNodeID = nID ;};
protected:
	ULONG m_nTypeID:32;
	ULONG m_nNodeID:32;
	ULONG m_nObjectID:32;
};


class GeoMsgPoint : public GeoBase
{
public:
	GeoMsgPoint();
	virtual ~GeoMsgPoint();
	
	void putmsgPoint( DB_msgpoint &msgpoint);
	ptrDB_msgpoint getmsgPoint();
	void get_Bound(SMG_Rect *rect);
	ULONG getType();

	DB_msgpoint m_msgPoint;

};
#endif 