/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 */

#include "Player.hpp"
#include "../globals/globals.hpp"

std::string GetItemName(uint64_t item)
{
    /* Read the items information */
    uintptr_t info = mem->Read<uintptr_t>((uintptr_t)item + 0x20);

    /* Read the display name */
    uintptr_t display_name = mem->Read<uintptr_t>(info + 0x20);

    /* Read mono string for the name */
    std::wstring wide_name = mem->ReadUnicode(display_name + 0x14);

    /* If Mono-String isn't empty then return the found characters as a string */
    if (!wide_name.empty())
        return std::string(wide_name.begin(), wide_name.end()).c_str();

    /* Return nothing if item is nothing */
    return "No Item";
}

bool Player::is_same_team()
{
    const auto enemy_team = mem->Read<std::uint32_t>(this->ent + 0x80, true);
    const auto local_team = mem->Read<std::uint32_t>(globals->local_player.ent + 0x80, true);

    if (local_team == 0 || enemy_team == 0)
        return false;

    return (local_team == enemy_team );
}

float WeaponVelocity(uint64_t item)
{
    /* Read held item to a string value */
    std::string held_item = GetItemName(item);

    /* Return some bogus value if it isn't held just a safety check if other functions don't work */
    if (held_item == "No Item")                 return 300.0f;

    /*
    * Decide weapons velocity based off name (( DEFAULT BULLET TYPE ))
    */

    /* Normal 556 rifle ammunition */
    if (held_item == "rifle.ak")                return 375.0f;
    if (held_item == "rifle.lr300")             return 375.0f;
    if (held_item == "rifle.bolt")              return 656.0f;
    if (held_item == "rifle.l96")               return 1125.0f;
    if (held_item == "rifle.m39")               return 469.0f;
    if (held_item == "rifle.semiauto")          return 375.0f;

    /* Default 556 ammunition */
    if (held_item == "lmg.m249")                return 488.0f;

    /* Normal pistol bullet ammunition */
    if (held_item == "smg.thompson")            return 300.0f;
    if (held_item == "smg.custom")              return 240.0f;
    if (held_item == "smg.mp5")                 return 240.0f;

    /* Normal pistol bullet ammunition, hand made shell for Eoka  */
    if (held_item == "pistol.python")           return 300.0f;
    if (held_item == "pistol.semi")             return 300.0f;
    if (held_item == "pistol.revolver")         return 300.0f;
    if (held_item == "pistol.m92")              return 300.0f;
    if (held_item == "pistol.eoka")             return 100.0f;
    if (held_item == "pistol.nailgun")          return 50.0f;

    /* Plain arrow */
    if (held_item == "crossbow")               return 75.0f;
    if (held_item == "bow.compound")            return 100.0f;
    if (held_item == "bow.hunting")             return 50.0f;

    /* Hand made shell */
    if (held_item == "shotgun.pump")            return 100.0f;
    if (held_item == "shotgun.spas12")          return 100.0f;
    if (held_item == "shotgun.waterpipe")       return 100.0f;
    if (held_item == "shotgun.doublebarrel")    return 100.0f;

    /* If all fails then return a zero value to void all post calculations */
    return 300.0f;
}