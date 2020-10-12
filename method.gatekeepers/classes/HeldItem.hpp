/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 */

#ifndef _HELD_HPP
#define _HELD_HPP

/* Includes */
#include <cstdint>
#include "../Memory/Memory.hpp"
#include "../classes/Math/Math.hpp"

/* HeldItem class */
class HeldItem
{
public:
	/* Default constructor */
	HeldItem() {}

	/* Used to initialize an HeldItem object */
	HeldItem(uint64_t _ent)
	{
		/* Store the constructor arguments */
		this->ent = _ent;

		/* Read THe Name Of The Item */
		this->name = GetItemName();

		this->bp = mem->Read<uintptr_t>(this->ent + 0x98);

		this->recoil_properties = mem->Read<uintptr_t>(this->bp + 0x2C0);

		this->primary_mag = mem->Read<uint64_t>(this->bp + 0x2A0);
	}

	std::string GetItemName()
	{
		/* Read the items information */
		uint64_t info = mem->Read<uint64_t>(this->ent + 0x20);

		/* Read the display name */
		uint64_t display_name = mem->Read<uint64_t>(info + 0x20);

		/* Read mono string for the name */
		std::wstring wide_name = mem->ReadUnicode(display_name + 0x14);

		/* If Mono-String isn't empty then return the found characters as a string */
		if (!wide_name.empty())
			return std::string(wide_name.begin(), wide_name.end()).c_str();

		/* Return nothing if item is nothing */
		return "No Item";
	}

	bool IsWeapon()
	{
		if (this->name.find("rifle") != std::string::npos)
		{
			return true;
		}
		if (this->name.find("pistol") != std::string::npos)
		{
			return true;
		}
		if (this->name.find("bow") != std::string::npos)
		{
			return true;
		}
		if (this->name.find("lmg") != std::string::npos)
		{
			return true;
		}
		if (this->name.find("shotgun") != std::string::npos)
		{
			return true;
		}
		if (this->name.find("smg") != std::string::npos)
		{
			return true;
		}
		else
			return false;
	}

	std::string GetAmmoType()
	{
		uint64_t item_def = mem->Read<uint64_t>(this->primary_mag + 0x20);
		uint64_t short_name = mem->Read<uint64_t>(item_def + 0x20);

		std::wstring wide_name = mem->ReadUnicode(short_name + 0x14);

		/* If Mono-String isn't empty then return the found characters as a string */
		if (!wide_name.empty())
			return std::string(wide_name.begin(), wide_name.end()).c_str();

		/* Return nothing if item is nothing */
		return "No Ammo";
	}

public:
	uint64_t	ent;			// The BaseItedm address
	uintptr_t	bp;				// The BaseProjectile address
	uintptr_t recoil_properties;// The RecoilProperties address
	uint64_t primary_mag;		// The PrimaryMagazines address
	std::string name;
};

#endif // _HELD_HPP