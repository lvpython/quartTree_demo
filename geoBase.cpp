
#include "geoBase.h"

#define DEF_POINTRANGE 10
GeoMsgPoint::GeoMsgPoint()
{
	m_nTypeID=0x101;
}

GeoMsgPoint::~GeoMsgPoint()
{

}

ULONG GeoMsgPoint::getType()
{
  return m_nTypeID;
}

ptrDB_msgpoint GeoMsgPoint::getmsgPoint()
{
	return &m_msgPoint;
}

void GeoMsgPoint::putmsgPoint( DB_msgpoint &msgpoint )
{
	memcpy( &m_msgPoint, &msgpoint ,sizeof(DB_msgpoint));
}

void GeoMsgPoint::get_Bound(SMG_Rect *rect)
{
	static SMG_Rect rcPoint;
	rcPoint.left = m_msgPoint.ptGeoPoint.x - DEF_POINTRANGE;
	rcPoint.right = m_msgPoint.ptGeoPoint.x + DEF_POINTRANGE;
	rcPoint.top = m_msgPoint.ptGeoPoint.y - DEF_POINTRANGE;
	rcPoint.bottom = m_msgPoint.ptGeoPoint.y + DEF_POINTRANGE;
	
	*rect = rcPoint;
}