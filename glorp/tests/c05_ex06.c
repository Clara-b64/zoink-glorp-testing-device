/*
** c05_ex06 -- ft_is_prime: int ft_is_prime(int nb);
** Returns 1 if nb is prime, 0 otherwise. Primes are integers > 1 divisible only
** by 1 and themselves, so nb < 2 (negatives, 0, 1) -> 0; 2 is the only even
** prime. The contract is strict: the return must be exactly 0 or 1, so every
** assertion pins the precise value with EQUAL_INT -- any other return fails.
** INT_MAX (2147483647 = 2^31-1) is itself prime. The oracle trial-divides up to
** sqrt(nb) in long to stay overflow-safe.
*/

#include "runner.h"
#include <limits.h>

extern int	ft_is_prime(int nb);

/* Trial division up to sqrt(nb); nb < 2 -> 0. Returns strictly 0 or 1. */
static int	refIsPrime(int nb)
{
    long	i;

    if (nb < 2)
        return (0);
    i = 2;
    while (i * i <= (long)nb)
    {
        if (nb % i == 0)
            return (0);
        i++;
    }
    return (1);
}

static void	assertPrime(int nb)
{
    TEST_ASSERT_EQUAL_INT(refIsPrime(nb), ft_is_prime(nb));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Small primes, including the only even prime (2). */
static void	testIsPrimeSmallPrimes(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(2));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(3));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(5));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(7));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(11));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(13));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(17));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(19));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(23));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(97));
}

/* Small composites, including perfect squares and odd composites. */
static void	testIsPrimeSmallComposites(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(4));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(6));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(8));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(9));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(10));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(15));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(21));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(25));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(49));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(100));
}

/* Everything below 2 -- negatives, 0 and 1 -- is not prime. */
static void	testIsPrimeBelowTwo(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(1));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(0));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(-1));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(-2));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(-7));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(-100));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(INT_MIN));
}

/* Known large primes (indexed primes) up to INT_MAX, which is itself prime. */
static void	testIsPrimeLargePrimes(void)
{
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(7919));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(104729));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(1299709));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(15485863));
    TEST_ASSERT_EQUAL_INT(1, ft_is_prime(INT_MAX));
}

/* Large composites, including 7919^2 (smallest factor 7919 forces a deep scan)
** and near-INT_MAX values caught by 2 and 5. */
static void	testIsPrimeLargeComposites(void)
{
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(100000));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(1000000000));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(62710561));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(2147395600));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(2147483645));
    TEST_ASSERT_EQUAL_INT(0, ft_is_prime(2147483646));
}

/* Every value -10..10000 matches the reference (each pins an exact 0 or 1). */
static void	testIsPrimeSweep(void)
{
    int		nb;

    nb = -10;
    while (nb <= 10000)
    {
        assertPrime(nb);
        nb++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testIsPrimeSmallPrimes);
    RUN_TEST(testIsPrimeSmallComposites);
    RUN_TEST(testIsPrimeBelowTwo);
    RUN_TEST(testIsPrimeLargePrimes);
    RUN_TEST(testIsPrimeLargeComposites);
    RUN_TEST(testIsPrimeSweep);
}
