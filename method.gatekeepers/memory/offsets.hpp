/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 */

#ifndef _OFFSET_HPP
#define _OFFSET_HPP

/* A class for interacting with the Offsets */
class Offsets
{
public:

	/* Constructor for this class */
	Offsets()
	{
		/**
		 * some shit in here is wrong :P
		 * update offsets in here every time rust updates!
		 */
	}

public:

	/*
	 * Game Object Manager ( GOM )
	 * UnityPlayer.dll + Offset
	 */
	uint64_t gameObjectManager = 0x17C1F18;

	/**
	 * protected string _displayName
	 * BasePlayer + Offset
	 */
	uint64_t playerDisplayName = 0x650;

	/**
	 * public ulong userID
	 * BasePlayer + Offset
	 */
	uint64_t playerUserID = 0x640;

	/**
	 * public PlayerModel playerModel
	 * BasePlayer + Offset
	 */
	uint64_t playerModel = 0x4A8;

	/**
	 * private ModelState modelState
	 * PlayerModel + Offset
	 */
	uint64_t modelState = 0x1D0;

	/**
	 * public float waterLevel;
	 * ModelState + Offset
	 */
	uint64_t waterLevel = 0x14;

	/**
	 * public int flags
	 * ModelState + Offset
	 */
	uint64_t modelFlags = 0x24;

	/**
	 * public Model model
	 * BaseEntity + Offset
	 */
	uint64_t entityModel = 0x118;

	/**
	 * internal bool isLocalPlayer
	 * PlayerModel + Offset
	 */
	uint64_t isLocalPlayer = 0x259;

	/**
	 * public Transform[] boneTransforms
	 * Model + Offset
	 */
	uint64_t boneTransforms = 0x48;

	/**
	 * private Vector3 newVelocity
	 * PlayerModel + Offset
	 */
	uint64_t playerVelocity = 0x1FC;

	/**
	 * public bool adminCheat
	 * BaseMovement + Offset
	 */
	uint64_t adminCheat = 0x18;
	
	/**
	 * public BaseMovement movement
	 * BasePlayer + Offset
	 */
	uint64_t playerBaseMovement = 0x4D0;

	/**
	 * public float clothingMoveSpeedReduction
	 * BasePlayer + Offset
	 */
	uint64_t clothingMoveSpeedReduction = 0x670;

	/**
	 * public float timeScale;
	 * LTSeq + Offset
	 */
	uint64_t timeScale = 0x2C;

	/**
	 * private float groundAngle
	 * PlayerWalkMovement + Offset
	 */
	uint64_t groundAngle = 0xB4;

	/**
	 * private float groundAngleNew
	 * PlayerWalkMovement + Offset
	 */
	uint64_t groundAngleNew = 0xB8;

	/**
	 * public float maxAngleClimbing;
	 * PlayerWalkMovement + Offset
	 */
	uint64_t maxAngleClimbing = 0x80;

	/**
	 * public float maxAngleWalking;
	 * PlayerWalkMovement + Offset
	 */
	uint64_t maxAngleWalking = 0x7C;

	/**
	 * public float groundTime;
	 * PlayerWalkMovement + Offset
	 */
	uint64_t groundTime = 0xBC;

	/**
	 * public PlayerEyes eyes;
	 * BasePlayer + offset
	 */
	uint64_t playerEyes = 0x600;

	/**
	 * private Vector3 viewOffset
	 * PlayerEyes + offset
	 */
	uint64_t ViewOffset = 0x38;

	/**
	 * public static readonly Vector3 EyeOffset;
	 * PlayerEyes + offset
	 */
	uint64_t EyeOffset = 0x0;

	/**
	 * public PlayerInput input
	 * BasePlayer + Offset
	 */
	uint64_t playerInput = 0x4C8;

	/**
	 * public float clientTickInterval
	 * BasePlayer + Offset
	 */
	uint64_t clientTickInterval = 0x5C8;

	/**
	 * public InputState state;
	 * Input + Offset
	 */
	uint64_t state = 0x20;

	/**
	 * public InputMessage current;
	 * state + Offset
	 */
	uint64_t current = 0x10;

	/**
	 * public Vector3 aimAngles;
	 * current + Offset
	 */
	uint64_t aimAngles = 0x18;

	/**
	 * public Vector3 recoilAngles
	 * PlayerInput + Offset
	 */
	uint64_t recoilAngles = 0x64;

	/**
	 * private Vector3 bodyAngles
	 * PlayerInput + Offset
	 */
	uint64_t bodyAngles = 0x3C;

	/**
	 * private float _health
	 * BaseCombatEntity + Offset
	 */
	uint64_t health = 0x20C;

	/**
	 * private EntityRef mounted
	 * BasePlayer + Offset
	 */
	uint64_t mountedEntRef = 0x590;

	/**
	 * private uint clActiveItem
	 * BasePlayer + Offset
	 */
	uint64_t clActiveItem = 0x570;

	/**
	 * public PlayerInventory inventory
	 * BasePlayer + Offset
	 */
	uint64_t playerInventory = 0x608;

	/**
	 * public ItemContainer containerBelt
	 * PlayerInventory + Offset
	 */
	uint64_t containerBelt = 0x28;

	/**
	 * public List<Item> contents
	 * ItemContainer + Offset
	 */
	uint64_t beltContentsList = 0x38;

	/**
	 * public BasePlayer.PlayerFlags playerFlags
	 * BasePlayer + Offset
	 */
	uint64_t playerFlags = 0x5F8;

	/**
	 * public float recoilYawMin
	 * RecoilProperties + Offset
	 */
	uint64_t recoilYawMin = 0x18;

	/**
	 * public float recoilYawMax
	 * RecoilProperties + Offset
	 */
	uint64_t recoilYawMax = 0x1C;

	/**
	 * public float recoilPitchMin
	 * RecoilProperties + Offset
	 */
	uint64_t recoilPitchMin = 0x20;

	/**
	 * public float recoilPitchMax
	 * RecoilProperties + Offset
	 */
	uint64_t recoilPitchMax = 0x24;

	/**
	 * public float timeToTakeMin
	 * RecoilProperties + Offset
	 */
	uint64_t timeToTakeMin = 0x28;

	/**
	 * public float timeToTakeMax
	 * RecoilProperties + Offset
	 */
	uint64_t timeToTakeMax = 0x2C;

	/**
	 * public float movementPenalty
	 * BasePlayer + Offset
	 */
	uint64_t movementPenalty = 0x34;

	/**
	 * public float aimCone
	 * BasePlayer + Offset
	 */
	uint64_t aimCone = 0x2D0;

	/**
	 * public float hipAimCone
	 * BasePlayer + Offset
	 */
	uint64_t hipAimCone = 0x2D4;

	/**
	 * public float aimConePenaltyMax
	 * BasePlayer + Offset
	 */
	uint64_t aimConePenaltyMax = 0x2DC;

	/**
	 * public float aimconePenaltyPerShot
	 * BasePlayer + Offset
	 */
	uint64_t aimconePenaltyPerShot = 0x2D8;

	/**
	 * public float aimconePenaltyRecoverTime
	 * BasePlayer + Offset
	 */
	uint64_t aimconePenaltyRecoverTime = 0x2E0;

	/**
	 * public float stancePenaltyScale
	 * BasePlayer + Offset
	 */
	uint64_t stancePenaltyScale = 0x2E8;
};

extern Offsets* offsets;

#endif // _OFFSET_HPP