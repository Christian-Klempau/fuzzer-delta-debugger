#include <stdlib.h>

#include "random.h"

/*
 * seed: seed for the random number generator
 *
 * returns: nothing
 */
void set_seed(int seed)
{
    srand(seed);
    return;
};
/*
 * min: minimum value
 * max: maximum value
 *
 * returns: a random int in [min, max], inclusive on both ends
 */
int random_range(int min, int max)
{
    return rand() % (max - min + 1) + min; // 0 to max, inclusive
}

/*
 * DEPRECATED
 */
int random3(int max)
{
    // return a random int between 0 and max
    // Original source: https://gist.github.com/lucis/96dc883af28c037104ef42ecedb8c3bf

    if (max <= 0)
    {
        return 1;
    }

    max++;

    static int z1 = 12345; // 12345 for rest of zx
    static int z2 = 12345; // 12345 for rest of zx
    static int z3 = 12345; // 12345 for rest of zx
    static int z4 = 12345; // 12345 for rest of zx

    int b;

    b = (((z1 << 6) ^ z1) >> 13);
    z1 = (((z1 & 4294967294) << 18) ^ b);
    b = (((z2 << 2) ^ z2) >> 27);
    z2 = (((z2 & 4294967288) << 2) ^ b);
    b = (((z3 << 13) ^ z3) >> 21);
    z3 = (((z3 & 4294967280) << 7) ^ b);
    b = (((z4 << 3) ^ z4) >> 12);
    z4 = (((z4 & 4294967168) << 13) ^ b);

    // if we have an argument, then we can use it
    int rand = ((z1 ^ z2 ^ z3 ^ z4)) % max;

    if (rand < 0)
    {
        rand = rand * -1;
    }

    return rand;
}

/*
 * returns: a random float in [0, 1], sampled from a uniform distribution
 */
float get_random_uniform()
{
    return random() / (double)((1L << 31) - 1);
}
/*
 * probability: a float between 0 and 1
 *
 * returns: 1 with probability p, 0 with probability 1-p, using a uniform distribution
 */
int random_binary_uniform(float probability)
{
    float uniform = get_random_uniform();
    if (uniform < probability)
    {
        return 1;
    }
    else
    {
        return 0;
    }
};
