/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 */

#ifndef _DROPPEDITEM_HPP
#define _DROPPEDITEM_HPP


/* Includes */
#include <cstdint>
#include "../Memory/Memory.hpp"
#include "../Math/Math.hpp"

/* Collectible resource class */
class DroppedItem
{
public:
	/* Default constructor */
	DroppedItem() {}

	/* Used to initialize an ore object */
	DroppedItem(uint64_t _bn, uint64_t _obj, Memory* _mem)
	{
		/* Store the constructor arguments */
		this->mem = _mem;
		this->obj = _obj;
		this->bn = _bn;

		/* Get the player's BaseEntity address */
		this->ent = this->mem->ReadChain<uint64_t>(this->obj, { 0x30, 0x18, 0x28 });

		/* Get the player's transform address */
		this->transform = this->mem->ReadChain<uint64_t>(this->obj, { 0x30, 0x8, 0x38 });

		/* Get the position */
		this->position = this->mem->Read<Vector3>(this->transform + 0x90);

		/* Read the name of the object */
		name = mem->ReadNative(this->obj + 0x60);

		/* Make the ore names clean */
		if (name.find("hemp-collectable") != std::string::npos)
			name = "Hemp";

		else if (name.find("stone-collectable") != std::string::npos)
			name = "Stone";

		else if (name.find("sulfur-collectable") != std::string::npos)
			name = "Sulfur";

		else if (name.find("metal-collectable") != std::string::npos)
			name = "Metal";

		else if (name.find("mushroom-cluster") != std::string::npos)
			name = "Mushrooms";

		else if (name.find("wood-collectable") != std::string::npos)
			name = "Wood";

		else if (name.find("potato-collectable") != std::string::npos)
			name = "Potato";

		else
			name = "Collectible";
	}

public:
	Memory* mem;			// A pointer to the global memory object
	uint64_t	obj;			// The GameObject address
	uint64_t	ent;			// The BaseEntity address
	uint64_t    bn;				// The BaseNetworkable entry address
	uint64_t    transform;		// The player's transform address
	Vector3		position;
	std::string name;
};

#endif // _DROPPEDITEM_HPP