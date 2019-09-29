#include <lazarus/Random.h>

#include <algorithm>
#include <chrono>
#include <random>

using namespace lz;

std::mt19937 Random::generator;  // Initialize with default seed (5489u)

void Random::seed()
{
    try
    {
        // Use a random device if available
        std::random_device randomDevice;
        generator.seed(randomDevice());
    }
    catch (const std::exception &e)
    {
        // Random device not available, use a time seed
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        generator.seed(seed);
    }
}

void Random::seed(unsigned seed)
{
    generator.seed(seed);
}

unsigned long Random::roll(unsigned sides, unsigned times)
{
    if (sides == 1)
        return times;
    if (sides == 0 || times == 0)
        return 0;
    std::uniform_int_distribution<unsigned> dist(1, sides);
    unsigned long total = 0;
    for (unsigned t = 0; t < times; ++t)
        total += dist(generator);
    return total;
}

bool Random::one_in(unsigned n)
{
    if (n < 2)
        return true;
    return Random::range(static_cast<unsigned>(1), n) == 1;
}

double Random::normal(double mean, double stdev)
{
    std::normal_distribution<double> dist(mean, stdev);
    return dist(generator);
}
