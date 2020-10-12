/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 */

#ifndef _THREADS_HPP
#define _THREADS_HPP

/* thread namespace */
namespace threads
{
	/* Implementation for entity discovery */
	void EntityDiscovery(uint64_t client_ents);
	void EntityUpdate();
};

#endif // _THREADS_HPP