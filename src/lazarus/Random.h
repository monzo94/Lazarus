#pragma once

#include <stdexcept>
#include <random>
#include <type_traits>

#include <lazarus/common.h>

namespace lz
{
/**
 * The Random class provides a simple interface for commonly used RNG functionality.
 * 
 * It is meant to be available from everywhere by providing static methods, but
 * encapsulating the generation logic inside of it.
 * 
 * It is important that the user calls `lz::Random::seed()` at the beginning of
 * their program to correctly seed the generator used with a pseudo-random number.
 * After that, the user can call the methods from anywhere in their program, without
 * having to ever instantiate a Random object, by just calling the methods within the
 * namespace provided.
 * 
 * For example, `lz::Random::range(1, 5)` will produce a random integer between 1 and 5.
 */
class Random
{
public:
    /**
     * Sets a new seed for the random generator using a hardware random device if
     * available, or a seed using the current time.
     */
    static void seed();

    /**
     * Sets the random generator to use the given seed.
     * 
     * Note that using the same seed will produce the same sequences of random
     * numbers in different executions.
     * This can be useful, for example, for testing.
     */
    static void seed(unsigned seed);

    /**
    * Return a random integral between the two given numbers with equal probability.
    * 
    * If a < b, then it will return an integral in [a, b].
    * Otherwise, it will be in [b, a].
    * 
    * Type is automatically detected, and the return type will be the most
    * appropriate type depending on the argument types.
    * For example, range(int, unsigned) will return an unsigned, and
    * range(short, long) will return a long.
    */
    template <typename T, typename U,
              typename std::enable_if_t<
                (std::is_integral<T>::value || std::is_unsigned<T>::value)
                && (std::is_integral<U>::value || std::is_unsigned<U>::value)
                >* = nullptr>
    static typename std::common_type<T, U>::type range(T a, U b)
    {
        typedef typename std::common_type<T, U>::type common_type;
        auto _a = static_cast<common_type>(a);
        auto _b = static_cast<common_type>(b);
        if (_a > _b)
            std::swap(_a, _b);
        return std::uniform_int_distribution<common_type>(_a, _b)(generator);
    }

    /**
    * Return a random floating point number between the two given numbers
    * with equal probability.
    * 
    * If a < b, then it will return an integral in [a, b).
    * Otherwise, it will be in [b, a).
    * 
    * Type is automatically detected, and the return type will be the most
    * appropriate type depending on the argument types.
    * For example, range(float, double) will return a double, and
    * range(int, float) will return a float.
    */
    template <typename T, typename U,
              typename std::enable_if_t<
                std::is_floating_point<T>::value || std::is_floating_point<U>::value
                >* = nullptr>
    static typename std::common_type<T, U>::type range(T a, U b)
    {
        typedef typename std::common_type<T, U>::type common_type;
        auto _a = static_cast<common_type>(a);
        auto _b = static_cast<common_type>(b);
        if (_a > _b)
            std::swap(_a, _b);
        return std::uniform_real_distribution<common_type>(_a, _b)(generator);
    }

    /**
     * Rolls a dice with the specified number of sides a certain number of times
     * and returns the total result.
     */
    static unsigned long roll(unsigned sides=6, unsigned times=1);

    /**
     * Return true with a 1 in n probability.
     */
    static bool one_in(unsigned n);

    /**
     * Return a random number generated from a normal distribution.
     * 
     * The normal (or Gaussian) distribution will use the given mean and
     * standard deviation.
     * 
     * @param mean The mean of the distribution.
     * @param stdev The standard deviation of the distribution.
     */
    static double normal(double mean, double stdev);

    /**
     * Return a reference to a random item from a container.
     * 
     * The container must support the size() method, and the
     * operator [] to get an element from the container.
     * If the container is empty, an exception will be thrown.
     */
    template <typename C, typename T = typename C::value_type>
    static T& choice(C& container);

private:
    static std::mt19937 generator;
};

template <typename C, typename T>
T& Random::choice(C& container)
{
    // Use size() instead of empty() to make conditions less restrictive
    size_t size = container.size();
    if (size == 0)
        throw __lz::LazarusException("Container is empty");
    
    size_t idx = Random::range(0, size - 1);
    return container[idx];
}

}  // namespace lz
