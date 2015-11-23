#include "event.h"
#include "def.h"
db_user_mission_event::MISSION_EVENT_TYPE GetEventType(EEventType eEventType)
{
	ASSERT(eEventType > EEventType_Min && eEventType < EEventType_Max);
	db_user_mission_event::MISSION_EVENT_TYPE event_type = (db_user_mission_event::MISSION_EVENT_TYPE)eEventType;
	ASSERT(db_user_mission_event::MISSION_EVENT_TYPE_IsValid(event_type));
	return event_type;
}

EEventType GetEventType(db_user_mission_event::MISSION_EVENT_TYPE event_type)
{
	ASSERT(db_user_mission_event::MISSION_EVENT_TYPE_IsValid(event_type));
	EEventType eEventType = EEventType(event_type);
	ASSERT(eEventType > EEventType_Min && eEventType < EEventType_Max);
	return eEventType;
}

db_user_mission_event::MISSION_EVENT_SUB_TYPE GetEventSubType(EEventSubType eEventSubType)
{
	return db_user_mission_event::MISSION_EVENT_SUB_TYPE(eEventSubType);
}

db_user_mission_event::MISSION_EVENT_TYPE TransformType( user_mission::MISSION_EVENT_TYPE  event_type)
{
	ASSERT(event_type > user_mission::min && event_type < user_mission::max);
	ASSERT(db_user_mission_event::MISSION_EVENT_TYPE_IsValid(event_type));
	if (!db_user_mission_event::MISSION_EVENT_TYPE_IsValid(event_type))
	{
		return db_user_mission_event::min;
	}
	return db_user_mission_event::MISSION_EVENT_TYPE(event_type);
}

user_mission::MISSION_EVENT_TYPE TransformType( db_user_mission_event::MISSION_EVENT_TYPE event_type)
{
	ASSERT(event_type > db_user_mission_event::min && event_type < db_user_mission_event::max);
	ASSERT(user_mission::MISSION_EVENT_TYPE_IsValid(event_type));
	if (!user_mission::MISSION_EVENT_TYPE_IsValid(event_type))
	{
		return user_mission::min;
	}
	return user_mission::MISSION_EVENT_TYPE(event_type);
}