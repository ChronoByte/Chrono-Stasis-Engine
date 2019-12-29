#pragma once
#include "csGlobals.h"
#include "pcg/pcg C/include/pcg_variants.h"
#include "pcg/pcg C/extras/entropy.h"
#include <time.h>
#include <math.h>



void InitSeedRNG();
void InitSeedEntropy();
UID GenerateUUID();
int GetRandomBetween(int min, int max);