#pragma once

int max(int a, int b);

int min(int a, int b);

#define getProb() ((float) (rand() % 100) / 100)

#define DP printf("%s : %d\n", __FILE__, __LINE__);