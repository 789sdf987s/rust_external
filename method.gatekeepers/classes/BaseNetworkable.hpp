/**
 * <3 skit#2567
 *
 * don't make this into a shitty p2c
 */

#ifndef _BN_HPP
#define _BN_HPP

/* Includes for this file */
#include "../Memory/Memory.hpp"

/* A class for interacting with the BaseNetworkable */
class BaseNetworkable
{
public:

	/* Constructor for this class */
	BaseNetworkable()
	{
		this->bn_addr = mem->IL2CPPScan("BaseNetworkable");
		this->cents_addr = mem->ReadChain<uint64_t>(this->bn_addr, { 0xb8, 0x0, 0x10, 0x28 });

		/* Check that finding BN succeeded */
		if (!this->bn_addr)
		{
			/* Log failure */
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
			printf("[:(] ");
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7); // gray
			printf("Failed to get the BaseNetworkable class definition. \n");

			/* Let the user read */
			SleepEx(3000, false);

			/* Exit because FUCK sector */
			exit(EXIT_FAILURE);
		}
	}

public:

	// The BaseNetworkable address
	uint64_t bn_addr = 0;

	// The ClientEntities address
	uint64_t cents_addr = 0;
};

/* Allow BN to be accessible by any file importing this */
extern BaseNetworkable* bn;

#endif // _BN_HPP