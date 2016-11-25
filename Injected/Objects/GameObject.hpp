#pragma once
#include "Object.hpp"

class GameObject : public Object
{
public:
	GameObject(int pointer)	:Object(pointer) {}

	uint64 CreatedBy(){ return GetDescriptor<uint64>(OBJECT_FIELD_CREATED_BY); }

	bool IsBusy(){ return addr? *(char*)(addr + 0xBC) == 1 : false; }

	int DisplayId(){ return GetDescriptor<int>(GAMEOBJECT_DISPLAYID); }

	int Flags(){ return GetDescriptor<int>(GAMEOBJECT_FLAGS); }

	int Faction(){ return GetDescriptor<int>(GAMEOBJECT_FACTION); }
	
	int Level(){ return GetDescriptor<int>(GAMEOBJECT_LEVEL); }

	bool Locked(){ return (Flags() & GO_FLAG_LOCKED) > 0; }	

	bool InUse(){ return (Flags() & GO_FLAG_IN_USE) > 0; }

	bool IsTransport(){ return (Flags() & GO_FLAG_TRANSPORT) > 0; }

	bool CreatedByMe(){ return CreatedBy() == GetLocalPlayerGuid(); }
};