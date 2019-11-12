#include "PseudoRandom.h"

pcg32_random_t rng;

void InitSeedRNG()
{
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
}

uint GenerateUUID()
{
	return pcg32_random_r(&rng);
}