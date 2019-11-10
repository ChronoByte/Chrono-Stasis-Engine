#include "csGlobals.h"
#include "pcg/pcg C/include/pcg_variants.h"
#include "pcg/pcg C/extras/entropy.h"
#include <time.h>
#include <math.h>

pcg32_random_t rng;

void InitSeedRNG()
{
	pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);
}

uint GenerateUUID()
{
	return pcg32_random_r(&rng);
}