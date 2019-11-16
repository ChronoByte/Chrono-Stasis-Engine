#include "PseudoRandom.h"


pcg32_random_t random;

void InitSeedRNG()
{
	pcg32_srandom_r(&random, time(NULL), (intptr_t)&random);
}

void InitSeedEntropy()
{
	uint64_t seeds[2];
	entropy_getbytes((void*)seeds, sizeof(seeds));
	pcg32_srandom_r(&random, seeds[0], seeds[1]);
}

UID GenerateUUID()
{
	return pcg32_random_r(&random);
}