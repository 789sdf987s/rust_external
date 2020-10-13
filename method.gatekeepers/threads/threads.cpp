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
    /* Log that the thread was started */
    print(10, "[->]", "Discovery thread spawned successfully\n");

    /* Get the base of the entity list */
    uint64_t list_base = mem->Read<uint64_t>(globals->c_ents + 0x18);

    /* Will contain a map between prefab IDs and entity names */
    std::map<uint64_t, std::string> prefab_map;

    /* Loop infinitely and read the entity list */
    while (true)
    {
        /* Sleep for 1ns every iteration */
        SleepEx(3, false);

        /* Get the size of the entity list */
        int32_t list_size = mem->Read<int32_t>(globals->c_ents + 0x10);

        /* Allocate the buffer for the BaseNetworkable list */
        uint64_t* entities = new uint64_t[list_size * sizeof(uint64_t)];

        /* Read BaseNetworkable into the entity list */
        mem->Read(list_base + 0x20, entities, list_size * sizeof(uint64_t));

        /* Loop through the entity list and process each entity */
        for (int32_t i = 0; i < list_size; i++)
        {
            SleepEx(3, false);

            /* Get the address at the current index */
            uint64_t entity = entities[i];

            /* Check if this entity has already been processed */
            if (std::find(globals->discovered.vec.begin(), globals->discovered.vec.end(), entity) != globals->discovered.vec.end())
            {
                /* We can skip this, it has already been found. */
                continue;
            }

            /* Read the entity's prefab ID */
            uint64_t prefab_id = mem->Read<uint64_t>(entity + 0x48);

            /* Check if the prefab ID has been found yet */
            if (prefab_map.find(prefab_id) == prefab_map.end())
            {
                /* Read the entity's class name */
                std::string ent_cls = mem->ReadASCII(mem->ReadChain<uint64_t>(entity, { 0x0, 0x10 }));

                /* Map the prefab ID to the entity class */
                prefab_map[prefab_id] = ent_cls;
            }

            /* Lock the discovered vector */
            globals->discovered.mutex.lock();

            /* Add address to discovered list */
            globals->discovered.vec.push_back(entity);

            /* Unlock the discovered vector */
            globals->discovered.mutex.unlock();

            /* Read the entity's GameObject address */
            uint64_t game_object = mem->ReadChain<uint64_t>(entity, { 0x10, 0x30 });

            /* Read the entity's tag */
            int16_t tag = mem->Read<int16_t>(game_object + 0x54);

            /* Initialize a new Entity object for this entry */
            Entity ent = {
                game_object,
                tag
            };

            /* Check the tag and see if it is a player */
            if (ent.tag == 6)
            {
                /* Lock the to_process entities list to prevent a race condition */
                globals->to_process.mutex.lock();

                /* Push the player onto the to_process entities list */
                globals->to_process.map[entity] = ent;

                /* Unlock the to_process entities list to allow processing */
                globals->to_process.mutex.unlock();
            }
        }

        /* Delete the entity list for the next iteration */
        delete[] entities;
    }
}

/* Used to dispose of an entity once it is no longer networked */
void DisposeEntity(uint64_t address)
{
    /* Check if the entity is in the to_process array */
    if (globals->to_process.map.find(address) != globals->to_process.map.end())
    {
        /* Lock the to_process map */
        globals->to_process.mutex.lock();

        /* Remove the entity from the to_process map */
        globals->to_process.map.erase(address);

        /* Unlock the to_process map */
        globals->to_process.mutex.unlock();
    }

    /* Check if the entity is in the players map */
    if (globals->players.map.find(address) != globals->players.map.end())
    {
        /* Lock the players map */
        globals->players.mutex.lock();

        /* Remove the entity from the players map */
        globals->players.map.erase(address);

        /* Unlock the players map */
        globals->players.mutex.unlock();
    }

    /* Mutex lock the discovered vector */
    globals->discovered.mutex.lock();

    /* Get the index of the address in the array */
    std::vector<uint64_t>::iterator it = std::find(globals->discovered.vec.begin(), globals->discovered.vec.end(), address);

    /* Remove the element from the vector */
    globals->discovered.vec.erase(it);

    /* Unlock the discovered vector */
    globals->discovered.mutex.unlock();

}

/* Used to process a player entity */
Player ProcessPlayer(Player* in_player)
{
    /* Make a local copy of the player */
    Player player = *in_player;

    /* Update the player */
    player.Update();

    /* Check if the player is LocalPlayer */
    if (player.local_player == true)
    {
        globals->local_player = player;
    }

    /* Return the updated player */
    return player;
}

/* Used to process to_process entities */
void threads::EntityProcessing()
{
    SleepEx(200, false);

    /* Log that the thread was started */
    print(10, "[->]", "Processing thread spawned successfully\n");

    /* Loop until exit */
    while (true)
    {
        SleepEx(1, false);

        /* Will store a list of entities to be removed from the to_process list */
        std::vector<uint64_t> to_dispose;

        /* Lock the to_process list to allow us to process without race conditions */
        globals->to_process.mutex.lock();

        /* Used to track how many iterations we have done */
        int iterations = 0;

        /* A list of player updating tasks */
        std::vector<std::future<Player>> tasks;

        /* Loop through the to_process entities list and update */
        for (auto& entry : globals->to_process.map)
        {
            iterations++;
            if (iterations % 50 == 0)
                SleepEx(1, false);

            /* Read the entity's networked status */
            bool net = static_cast<bool>(mem->Read<uint64_t>(entry.first + 0x50));

            /* Check that the entity is still networked */
            if (net == false)
            {
                SleepEx(1, false);

                /* Add the address of this entity to the disposal array */
                to_dispose.push_back(entry.first);

                /* Skip this entry */
                continue;
            }

            /* Process players */
            if (entry.second.tag == 6)
            {
                /* Check if the player has already been found */
                if (globals->players.map.find(entry.first) == globals->players.map.end())
                {
                    /* Lock the players map */
                    globals->players.mutex.lock();

                    /* Initialize a player object */
                    Player player(entry.first, entry.second.game_object, mem);

                    /* Push the player object onto the list of players */
                    globals->players.map[entry.first] = player;

                    /* Unlock the players map */
                    globals->players.mutex.unlock();
                }

                /* Spawn an update task for this player */
                tasks.push_back(std::async(ProcessPlayer, &globals->players.map[entry.first]));
            }
        }

        iterations = 0;
        /* Wait for the players to finish updating */
        while (true)
        {
            iterations++;
            if (iterations % 10 == 0)
                SleepEx(1, false);

            /* Used to determine if the players are finished updating */
            bool not_finished = false;

            /* Loop through the task list and check if any are finished */
            for (auto& task : tasks)
            {
                if (!task._Is_ready())
                {
                    not_finished = true;
                }
            }

            /* Check if we should exit the loop */
            if (not_finished == false)
            {
                break;
            }
        }

        /* Lock the player list */
        globals->players.mutex.lock();

        iterations = 0;
        /* Loop through the updated player tasks */
        while (tasks.size() > 0)
        {
            iterations++;
            if (iterations % 10 == 0)
                SleepEx(1, false);

            /* Get the updated player object */
            Player updated = tasks[0].get();

            /* Check if the task is complete */
            globals->players.map[updated.bn] = updated;

            /* Remove the task from the list */
            tasks.erase(tasks.begin());
        }

        /* Unlock the player list */
        globals->players.mutex.unlock();

        /* Unlock the to_process list to allow new entities to be added */
        globals->to_process.mutex.unlock();

        iterations = 0;
        /* Loop through the list of entities to be disposed of and remove them */
        while (to_dispose.size() > 0)
        {
            iterations++;
            if (iterations % 10 == 0)
                SleepEx(1, false);

            /* Erase the element at the start of the disposal array */
            DisposeEntity(to_dispose[0]);

            /* Remove the first element from the disposal array, it has been removed. */
            to_dispose.erase(to_dispose.begin());
        }
    }
}