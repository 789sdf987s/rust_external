/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 */

#ifndef _GOM_HPP
#define _GOM_HPP

/* Includes for this file */
#include "../Memory/Memory.hpp"

/* A class for interacting with the GameObjectManager */
class GameObjectManager
{
public:

	/* Constructor for this class */
	GameObjectManager()
	{
		this->gom_addr = mem->Read<uint64_t>(mem->GetModuleBase("UnityPlayer.dll") + offsets->gameObjectManager);
	}

	enum CursorLockMode
	{
		None = 0,
		Locked,
		Confined
	};

	CursorLockMode getLockState()
	{
		auto event = mem->Read<uintptr_t>(mem->GetModuleBase("UnityPlayer.dll") + 0x17E4938);
		auto cursorStatus = mem->Read<int>(event + 0x18);
		return (CursorLockMode)cursorStatus;
	}

	/* Used to get the 'Camera' address */
	uint64_t GetCamera()
	{
		/* Read into the first entry in the list */
		uint64_t address = mem->Read<uint64_t>(this->gom_addr + 0x8);

		/* Loop until we hit tag 5, which is camera */
		while (true)
		{
			/* Read into the GameObject */
			uint64_t game_object = mem->Read<uint64_t>(address + 0x10);

			/* Read this object's tag */
			int16_t tag = mem->Read<int16_t>(game_object + 0x54);

			if (tag == 5)
			{
				return mem->ReadChain<uint64_t>(game_object, { 0x30, 0x18 });;
			}

			/* Read into the next entry */
			address = mem->Read<uint64_t>(address + 0x8);
		}

		return address;
	}

	/* Used to get the 'Sky' address */
	uint64_t GetSky()
	{
		/* Read into the first entry in the list */
		uint64_t address = mem->Read<uint64_t>(this->gom_addr + 0x8);

		/* Loop until we hit tag 20011, which is sky dome */
		while (true)
		{
			/* Read into the GameObject */
			uint64_t game_object = mem->Read<uint64_t>(address + 0x10);

			/* Read this object's tag */
			int16_t tag = mem->Read<int16_t>(game_object + 0x54);

			/* Check for sky dome tag */
			if (tag == 20011)
			{
				/* Camera was found, return the address */
				return game_object;
			}

			/* Read into the next entry */
			address = mem->Read<uint64_t>(address + 0x8);
		}

		return address;
	}

public:

	// The GameObjectManager address
	uint64_t gom_addr = 0;
};

/* Allow GOM to be accessible by any file importing this */
extern GameObjectManager* gom;

#endif // _GOM_HPP