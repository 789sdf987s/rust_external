/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 *
 * all of these threads need updating to perform decently at a low CPU
 */

#include "../include.hpp"
#include "threads.hpp"
#include "../globals/globals.hpp"
#include <future>
#include "../classes/Player.hpp"

using namespace threads;

void threads::EntityDiscovery(uint64_t client_ents)
{
    /* get the base of the entity list */
    uint64_t list_base = mem->Read<uint64_t>(client_ents + 0x18);

    while (true)
    {
        SleepEx(1000, false);

        /* stores players */
        std::vector<Player> local_players;

        /* read entities list into vector */
        int32_t list_size = mem->Read<int32_t>(client_ents + 0x10);
        uint64_t* entities = new uint64_t[list_size * sizeof(uint64_t)];
        mem->Read(list_base + 0x20, entities, list_size * sizeof(uint64_t));
        std::vector<uint64_t> discovered_entities = std::vector<uint64_t>(entities, entities + list_size);

        /* prevent mem leak */
        delete[] entities;

        /* iterate over entities vector */
        for (auto ent : discovered_entities)
        {
            bool NetworkStatus = static_cast<bool>(mem->Read<uint64_t>(ent + 0x50));

            /* ensure player is networked */
            if (!NetworkStatus)
                continue;

            /* get gameobject & tag */
            uint64_t GameObject = mem->ReadChain<uint64_t>(ent, { 0x10, 0x30 });
            int16_t Tag = mem->Read<int16_t>(GameObject + 0x54);

            /* ensure it's a player */
            if (Tag != 6)
                continue;

            local_players.push_back({ ent, GameObject, mem });
        }

        /* save the Players vector into the mutex */
        globals->discovered.mutex.lock();
        globals->discovered.vec = local_players;
        globals->discovered.mutex.unlock();
    }
}

/* used to process a player entity */
Player ProcessPlayer(Player in_player)
{
    in_player.Update();

    if (in_player.local_player)
        globals->local_player = in_player;

    return in_player;
}

void threads::EntityUpdate()
{
    while (true)
    {
        SleepEx(1, false);

        std::vector<Player> updated_players;

        /* copy discovered_players into local vec */
        globals->discovered.mutex.lock();
        std::vector<Player> players = globals->discovered.vec;
        globals->discovered.mutex.unlock();

        /* iterate over players */
        for (auto entry : players)
        {
            /* catch any errors */
            try
            {
                /* construct new player */
                Player updated_player = ProcessPlayer(entry);
                if (updated_player.health < 0.1) continue;
                if (updated_player.position.x == 0.f || updated_player.position.y == 0.f || updated_player.position.z == 0.f) continue;

                updated_players.push_back(updated_player);
            }
            catch (...)
            {
                SleepEx(1, false);
                print(4, "[!]", "error updating entry 0x%llx\n", entry.ent);
            }
        }

        globals->players.mutex.lock();
        globals->players.vec = updated_players;
        globals->players.mutex.unlock();
    }
}