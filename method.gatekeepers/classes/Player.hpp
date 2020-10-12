/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 */

#ifndef _PLAYER_HPP
#define _PLAYER_HPP

/* Includes for this file */
#include <cstdint>
#include <string>
#include <xmmintrin.h>
#include <emmintrin.h>
#include "../memory/Memory.hpp"
#include "../classes/Math/Math.hpp"
#include "HeldItem.hpp"
#include "../globals/utils.hpp"
#include <future>

/* PlayerFlags enum from DnSpy */
enum class PlayerFlags
{
	Unused1 = 1,
	Unused2 = 2,
	IsAdmin = 4,
	ReceivingSnapshot = 8,
	Sleeping = 16,
	Spectating = 32,
	Wounded = 64,
	IsDeveloper = 128,
	Connected = 256,
	ThirdPersonViewmode = 1024,
	EyesViewmode = 2048,
	ChatMute = 4096,
	NoSprint = 8192,
	Aiming = 16384,
	DisplaySash = 32768,
	Relaxed = 65536,
	SafeZone = 131072,
	ServerFall = 262144,
	Workbench1 = 1048576,
	Workbench2 = 2097152,
	Workbench3 = 4194304
};

/* The bone ID enum */
enum Bones : int
{
	l_hip = 1,
	l_knee = 2,
	l_foot = 3,
	l_toe = 4,
	l_ankle_scale = 5,
	pelvis = 6,
	penis = 7,
	r_hip = 13,
	r_knee = 14,
	r_foot = 15,
	r_toe = 16,
	spine1 = 18,
	spine2 = 20,
	spine3 = 21,
	spine4 = 22,
	l_upperarm = 24,
	l_forearm = 25,
	l_hand = 26,
	neck = 46,
	head = 47,
	jaw = 48,
	r_upperarm = 55,
	r_forearm = 56,
	r_hand = 57,
};

/* A class used to represent a player in Rust */
class Player
{
public:

	/* Default constructor */
	Player() {}

	/* Constructor for this class */
	Player(uint64_t _bn, uint64_t _obj, Memory* _mem)
	{
		/* Store the constructor arguments */
		this->mem = _mem;
		this->obj = _obj;
		this->bn = _bn;

		/* Get the player's BaseEntity address
		 * Reading Entity's public BasePlayer basePlayer;
		 */
		this->ent = this->mem->ReadChain<uint64_t>(this->obj, { 0x30, 0x18, 0x28 });

		/* Get the player's transform address */
		this->transform = this->mem->ReadChain<uint64_t>(this->obj, { 0x30, 0x8, 0x38 });

		// Get the player's name
		this->name = this->GetPlayerName();

		// Get the player's steam3 id and convert to steam64
		this->steam_id = (this->mem->Read<uint64_t>(this->ent + offsets->playerUserID) + 0x0110000100000000);

		// Check if the player is localplayer
		this->local_player = this->mem->ReadChain<bool>(this->ent, { (uint64_t)offsets->playerModel, (uint64_t)offsets->isLocalPlayer });

		/* Get the bone transform addy */
		this->bone_transforms = mem->ReadChain<uint64_t>(this->ent, { (uint64_t)offsets->entityModel, (uint64_t)offsets->boneTransforms });
	}

	/* Used to calculate the distance to another player */
	float Distance(Player* player)
	{
		return this->position.Distance(player->position);
	}

	/* Used to check if a player is behind you */
	bool IsInView(Matrix* matrix, uint64_t camera)
	{
		/* W2S the player's position */
		Vector2 screen_pos = utils->W2S(this->position, matrix, camera);

		/* Check if position is out of screen bounds */
		if (screen_pos.x >= 2080 || screen_pos.x <= 0.0 || screen_pos.y >= 1080 || screen_pos.y <= 0.0)
		{
			return false;
		}
		return true;
	}

	/* Used to get the distance as a string */
	std::string DistanceStr(Player* player)
	{
		/* Will store the distance string */
		char distance[10];

		/* Get the distance */
		float dist = this->Distance(player);

		/* Copy the string into the output array */
		sprintf(distance, "%.2f", dist);

		/* Return the distance string */
		return distance;
	}

	/* used to get the cock size as a string */
	std::string CockStr()
	{
		/* Will store the distance string */
		char size_str[10];

		/* get the size */
		float size = this->bones[penis].Distance(this->bones[l_hip]) * 1000;

		/* Copy the string into the output array */
		sprintf(size_str, "%.2f", size);

		/* Return the distance string */
		return size_str;
	}

	/* team shit nigga */
	bool is_same_team();

	/* Used to update the player object */
	void Update()
	{
		/* Read the player's position */
		this->UpdatePosition();

		/* Read the player's health */
		this->health = this->mem->Read<float>(this->ent + offsets->health);

		/* Read the playerflags */
		this->player_flags = this->mem->Read<int32_t>(this->ent + offsets->playerFlags);

		this->same_team = this->is_same_team();
	}

	Vector3 GetVelocity()
	{
		/* Get to the PlayerModel */
		uint64_t player_model = mem->Read<uint64_t>(this->ent + offsets->playerModel);

		/* Read and return the velocity vector */
		return mem->Read<Vector3>(player_model + offsets->playerVelocity);
	}

	/* Used to update the player's position */
	void UpdatePosition()
	{
		/* Read the player's forward vec */
		this->forward = this->mem->Read<Vector3>(this->transform + 0x9C);

		/* Read the player's position */
		this->position = this->mem->Read<Vector3>(this->transform + 0x90);

		/* Update the bones map */
		this->UpdateBones();
	}

	/* Used to check if the player has a player flag */
	bool HasPlayerFlag(PlayerFlags flag)
	{
		return (this->player_flags & static_cast<int32_t>(flag)) == static_cast<int32_t>(flag);
	}

	/* Used to give the player a player flag */
	void GivePlayerFlag(PlayerFlags flag)
	{
		/* Write to playerflags */
		this->mem->Write(this->ent + offsets->playerFlags, this->player_flags | static_cast<int32_t>(flag));;
	}

	/* Used to determine whether or not the player is connected */
	bool IsConnected()
	{
		return this->HasPlayerFlag(PlayerFlags::Connected);
	}

	struct Matrix3x4
	{
		byte v0[16];
		byte v1[16];
		byte v2[16];
	};

	/* Used to update the player's bone positions */
	void UpdateBones()
	{
		int bone_array[] = { 1, 2, 3, 4, 5, 6, 7, 13, 14, 15, 16, 18, 20, 21, 22, 24, 25, 26, 46, 47, 48, 55, 56, 57 };

		/* Iterate through the bone array */
		for (int i : bone_array)
		{
			/* Get the address of the bone */
			uint64_t bone = mem->Read<uint64_t>(this->bone_transforms + (0x20 + (i * 0x8)));

			/* Get the transform address */
			uint64_t transform = mem->Read<uint64_t>(bone + 0x10);

			/* Check that we got the transform */
			if (!transform)
			{
				/* Add an empty transform to the list */
				this->bones[(Bones)i] = Vector3();

				/* Skip this bone */
				continue;
			}
			/* _m128 -> Vec4 */
			const __m128 mul_vec0 = { -2.000, 2.000, -2.000, 0.000 };
			const __m128 mul_vec1 = { 2.000, -2.000, -2.000, 0.000 };
			const __m128 mul_vec2 = { -2.000, -2.000, 2.000, 0.000 };

			/* Read the transform index for this bone */
			int index = mem->Read<int>(transform + 0x40);

			/* Read the transform data ptr */
			uint64_t p_transform_data = mem->Read<uint64_t>(transform + 0x38);

			/* Designate the transform data array */
			uint64_t transform_data[2];

			/* Read the player transform data into the transform data array */
			mem->Read((p_transform_data + 0x18), &transform_data, 16);

			/* Set the size of the matricies buffer */
			size_t size_matricies_buffer = 48 * index + 48;

			/* Set the indicies buffer size */
			size_t size_indices_buffer = 4 * index + 4;

			/* Allocate memory to the matricies buffer */
			PVOID p_matricies_buffer = malloc(size_matricies_buffer);

			/* Allocate memory to the indices buffer */
			PVOID p_indices_buffer = malloc(size_indices_buffer);

			/* Transform position if matrices buffer and indices buffer are both correct */
			if (p_matricies_buffer && p_indices_buffer)
			{
				/* Read the matricies buffer into an array */
				mem->Read(transform_data[0], p_matricies_buffer, static_cast<uint32_t>(size_matricies_buffer));

				/* Read the indices buffer into an array */
				mem->Read(transform_data[1], p_indices_buffer, static_cast<uint32_t>(size_indices_buffer));

				__m128 result = *(__m128*)((ULONGLONG)p_matricies_buffer + 0x30 * index);
				int transform_index = *(int*)((ULONGLONG)p_indices_buffer + 0x4 * index);

				/* Used to track loop iterations */
				int iterations = 0;

				while (transform_index >= 0)
				{
					if (iterations > 30) continue;

					Matrix3x4 matrix34 = *(Matrix3x4*)((ULONGLONG)p_matricies_buffer + 0x30 * transform_index);
					__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0x00));
					__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0x55));
					__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0x8E));
					__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0xDB));
					__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0xAA));
					__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.v1), 0x71));
					__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.v2), result);

					result = _mm_add_ps(
						_mm_add_ps(
							_mm_add_ps(
								_mm_mul_ps(
									_mm_sub_ps(
										_mm_mul_ps(_mm_mul_ps(xxxx, mul_vec1), zwxy),
										_mm_mul_ps(_mm_mul_ps(yyyy, mul_vec2), wzyw)),
									_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
								_mm_mul_ps(
									_mm_sub_ps(
										_mm_mul_ps(_mm_mul_ps(zzzz, mul_vec2), wzyw),
										_mm_mul_ps(_mm_mul_ps(xxxx, mul_vec0), yxwy)),
									_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
							_mm_add_ps(
								_mm_mul_ps(
									_mm_sub_ps(
										_mm_mul_ps(_mm_mul_ps(yyyy, mul_vec0), yxwy),
										_mm_mul_ps(_mm_mul_ps(zzzz, mul_vec1), zwxy)),
									_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
								tmp7)), *(__m128*)(&matrix34.v0));
					try {
						transform_index = *(int*)((ULONGLONG)p_indices_buffer + 0x4 * transform_index);
					}
					catch (...)
					{
						/* Yeah its fucking stupid kys */
					}

					/* Increment iterations for performance management */
					iterations++;
				}

				/* Get the world pos */
				Vector3 world = Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);

				/* Update bone */
				bones[(Bones)i] = world;
			}

			/* Free the allocated memory */
			free(p_matricies_buffer);
			free(p_indices_buffer);
		}
	}

	/* Used to get the player's ASCII name */
	std::string GetASCIIName()
	{
		return std::string(this->name.begin(), this->name.end());
	}

	/* Used to get the player's recoilangles */
	Vector3 GetRecoilAngles()
	{
		return mem->ReadChain<Vector3>(this->ent, { (uint64_t)offsets->playerInput, (uint64_t)offsets->recoilAngles });
	}

	/* Used to get the player's viewangles */
	Vector3 GetViewAngles()
	{
		return mem->ReadChain<Vector3>(this->ent, { (uint64_t)offsets->playerInput, (uint64_t)offsets->bodyAngles });
	}

	/* Used to set the player's viewangles */
	void SetViewAngles(Vector3 angles)
	{
		mem->Write<Vector3>(mem->Read<uint64_t>(this->ent + offsets->playerInput) + offsets->bodyAngles, angles);
	}

	/* Used to get the address of the mounted entity */
	uint64_t GetMountedEntity()
	{
		/* Read ent_cached from 'mounted' EntityRef */
		return this->mem->Read<uint64_t>(this->ent + offsets->mountedEntRef);
	}

	/* Used to get the player's held item */
	HeldItem GetHeldItem()
	{
		/* Read active item identifier */
		int active_weapon_id;
		mem->Read(this->ent + offsets->clActiveItem, &active_weapon_id, sizeof(active_weapon_id));

		/* Read items array */
		uint64_t items = mem->ReadChain<uint64_t>(this->ent, { (uint64_t)offsets->playerInventory, (uint64_t)offsets->containerBelt, (uint64_t)offsets->beltContentsList, 0x10 });

		/* Iterate the this->ents belt */
		for (int items_on_belt = 0; items_on_belt <= 6; items_on_belt++)
		{
			/* Read current item */
			uint64_t item = mem->Read<uint64_t>(items + 0x20 + (items_on_belt * 0x8));

			/* Read the this->ents active weapon */
			int active_weapon = mem->Read<uint32_t>(item + 0x28);

			/* Check if the active weapon matches this iterations item */
			if (active_weapon_id == active_weapon)
			{
				HeldItem item_obj = HeldItem(item);

				/* Return this iterations item if it is active */
				return item_obj;
			}
		}

		/* Return as failed */
		return 0;
	}

private:

	/* Used to get the player's name */
	std::wstring GetPlayerName()
	{
		std::wstring name = this->mem->ReadUnicode(this->mem->Read<uint64_t>(this->ent + offsets->playerDisplayName) + 0x14);

		if (name.find(L"Scientist") == 0)
			return L"Scientist";

		return name;
	}


public:
	Memory* mem;			// A pointer to the global memory object
	uint64_t	obj;			// The GameObject address
	uint64_t	ent = 0;			// The BaseEntity address
	uint64_t    bn;				// The BaseNetworkable entry address
	uint64_t    transform;		// The player's transform address
	uint64_t	bone_transforms; // The address of the player's bone transforms 

	/* Player attributes */

	bool local_player = false;			// Used to determine whether the player is the LocalPlayer.
	std::wstring name = std::wstring();	// The player's in-game name.
	uint64_t steam_id = 0;				// The player's steam64 id
	int32_t	player_flags = 0;				// The player's playerflags.
	float health = 0.f;				// The player's current health
	Vector3	position = {};				// The player's 3d position#
	Vector3 forward = {};
	std::map<Bones, Vector3> bones = {};
	bool same_team;

	int index;
};

/* Used to get a weapon's name */
std::string GetItemName(uint64_t item);

/* Used to get a weapon's velocity */
float WeaponVelocity(uint64_t item);

#endif // _PLAYER_HPP