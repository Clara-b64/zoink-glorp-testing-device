/*
** c05_ex07 -- ft_find_next_prime: int ft_find_next_prime(int nb);
** Returns the first prime >= nb. Since the smallest prime is 2, any nb <= 2
** yields 2 (negatives, 0, 1 included). A prime nb returns itself ("or equal").
** INT_MAX (2147483647) is prime, so a valid answer always exists within int and
** an upward search never needs to pass INT_MAX -- no overflow. The oracle reuses
** trial division, then scans upward; near-INT_MAX cases are checked against it
** rather than hand-picked primes.
*/

#include "runner.h"
#include <limits.h>

extern int	ft_find_next_prime(int nb);

/* Trial division up to sqrt(nb); nb < 2 -> 0. */
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

/* First prime >= nb; nb < 2 collapses to 2. Always terminates by INT_MAX. */
static int	refNextPrime(int nb)
{
    if (nb < 2)
        return (2);
    while (!refIsPrime(nb))
        nb++;
    return (nb);
}

static void	assertNextPrime(int nb)
{
    TEST_ASSERT_EQUAL_INT(refNextPrime(nb), ft_find_next_prime(nb));
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Anything <= 2 -- negatives, 0, 1, 2 -- maps to the smallest prime, 2. */
static void	testNextPrimeAtOrBelowTwo(void)
{
    TEST_ASSERT_EQUAL_INT(2, ft_find_next_prime(2));
    TEST_ASSERT_EQUAL_INT(2, ft_find_next_prime(1));
    TEST_ASSERT_EQUAL_INT(2, ft_find_next_prime(0));
    TEST_ASSERT_EQUAL_INT(2, ft_find_next_prime(-1));
    TEST_ASSERT_EQUAL_INT(2, ft_find_next_prime(-100));
    TEST_ASSERT_EQUAL_INT(2, ft_find_next_prime(INT_MIN));
}

/* A prime input returns itself (the bound is inclusive). */
static void	testNextPrimeOnPrimeReturnsItself(void)
{
    TEST_ASSERT_EQUAL_INT(3, ft_find_next_prime(3));
    TEST_ASSERT_EQUAL_INT(5, ft_find_next_prime(5));
    TEST_ASSERT_EQUAL_INT(7, ft_find_next_prime(7));
    TEST_ASSERT_EQUAL_INT(13, ft_find_next_prime(13));
    TEST_ASSERT_EQUAL_INT(97, ft_find_next_prime(97));
    TEST_ASSERT_EQUAL_INT(7919, ft_find_next_prime(7919));
}

/* Composite inputs jump up to the next prime. */
static void	testNextPrimeKnownValues(void)
{
    TEST_ASSERT_EQUAL_INT(5, ft_find_next_prime(4));
    TEST_ASSERT_EQUAL_INT(7, ft_find_next_prime(6));
    TEST_ASSERT_EQUAL_INT(11, ft_find_next_prime(8));
    TEST_ASSERT_EQUAL_INT(11, ft_find_next_prime(9));
    TEST_ASSERT_EQUAL_INT(17, ft_find_next_prime(14));
    TEST_ASSERT_EQUAL_INT(23, ft_find_next_prime(20));
    TEST_ASSERT_EQUAL_INT(29, ft_find_next_prime(24));
    TEST_ASSERT_EQUAL_INT(97, ft_find_next_prime(90));
    TEST_ASSERT_EQUAL_INT(101, ft_find_next_prime(100));
    TEST_ASSERT_EQUAL_INT(211, ft_find_next_prime(200));
}

/* Large inputs, including the INT_MAX ceiling and the gap just below it. */
static void	testNextPrimeLargeValues(void)
{
    TEST_ASSERT_EQUAL_INT(INT_MAX, ft_find_next_prime(INT_MAX));
    TEST_ASSERT_EQUAL_INT(15485863, ft_find_next_prime(15485863));
    assertNextPrime(15485864);
    assertNextPrime(1000000);
    assertNextPrime(2147483629);
    assertNextPrime(2147483630);
    assertNextPrime(2147483646);
}

/* Every value -10..10000 matches the reference. */
static void	testNextPrimeSweep(void)
{
    int		nb;

    nb = -10;
    while (nb <= 10000)
    {
        assertNextPrime(nb);
        nb++;
    }
}

void	glorpRun(void)
{
    RUN_TEST(testNextPrimeAtOrBelowTwo);
    RUN_TEST(testNextPrimeOnPrimeReturnsItself);
    RUN_TEST(testNextPrimeKnownValues);
    RUN_TEST(testNextPrimeLargeValues);
    RUN_TEST(testNextPrimeSweep);
}
