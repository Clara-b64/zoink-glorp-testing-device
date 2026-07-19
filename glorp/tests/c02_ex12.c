/*
** c02_ex12 -- ft_print_memory: void *ft_print_memory(void *addr,
** unsigned int size);
** Dumps size bytes of memory in the subject's format, one line per 16 bytes:
** the address as 16 lowercase zero-padded hex digits, ": ", eight groups of
** four hex digits (two bytes) each followed by a space (missing bytes on the
** last line padded with spaces), then the bytes as characters with every
** non-printable shown as '.', then a newline. Prints nothing when size is 0
** and always returns addr. The expected text is rebuilt by the test from the
** real buffer address, so it works wherever the buffer lands in memory.
*/

#include "runner.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

extern void	*ft_print_memory(void *addr, unsigned int size);

/* Runs ft_print_memory with stdout captured; returns byte count, stores the
** function's return value through ret. */
static int	captureMemory(void *addr, unsigned int size, char *out, int cap,
        void **ret)
{
    int	pfd[2];
    int	saved;
    int	n;

    out[0] = '\0';
    *ret = NULL;
    saved = dup(STDOUT_FILENO);
    if (saved == -1)
        return (-1);
    if (pipe(pfd) == -1)
    {
        close(saved);
        return (-1);
    }
    dup2(pfd[1], STDOUT_FILENO);
    close(pfd[1]);
    *ret = ft_print_memory(addr, size);
    dup2(saved, STDOUT_FILENO);
    close(saved);
    n = (int)read(pfd[0], out, cap - 1);
    close(pfd[0]);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    return (n);
}

/* Builds the subject-format dump of buf into expected. */
static void	buildExpected(unsigned char *buf, unsigned int size, char *expected)
{
    char			*p;
    unsigned int	line;
    unsigned int	i;
    unsigned int	idx;

    p = expected;
    line = 0;
    while (line < size)
    {
        p += sprintf(p, "%016lx: ", (unsigned long)(uintptr_t)(buf + line));
        i = 0;
        while (i < 16)
        {
            idx = line + i;
            if (idx < size)
                p += sprintf(p, "%02x", buf[idx]);
            else
                p += sprintf(p, "  ");
            if (i % 2 == 1)
                p += sprintf(p, " ");
            i++;
        }
        i = 0;
        while (i < 16 && line + i < size)
        {
            idx = line + i;
            if (buf[idx] >= 32 && buf[idx] <= 126)
                *p++ = (char)buf[idx];
            else
                *p++ = '.';
            i++;
        }
        *p++ = '\n';
        line += 16;
    }
    *p = '\0';
}

static void	assertMemoryDump(unsigned char *buf, unsigned int size)
{
    char	out[1024];
    char	expected[1024];
    void	*ret;
    int		n;

    n = captureMemory(buf, size, out, sizeof(out), &ret);
    buildExpected(buf, size, expected);
    TEST_ASSERT_EQUAL_PTR(buf, ret);
    TEST_ASSERT_EQUAL_INT((int)strlen(expected), n);
    TEST_ASSERT_EQUAL_STRING(expected, out);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* One exactly full line of printable characters. */
static void	testMemoryFullLine(void)
{
    assertMemoryDump((unsigned char *)"abcdefghijklmnop", 16);
}

/* A partial line: hex area padded with spaces, short character column. */
static void	testMemoryPartialLine(void)
{
    assertMemoryDump((unsigned char *)"Hello", 5);
    assertMemoryDump((unsigned char *)"abc", 3);
}

/* An odd tail inside a multi-line dump. */
static void	testMemoryMultiLine(void)
{
    assertMemoryDump((unsigned char *)"The quick brown fox jumps over the lazy dog", 43);
}

/* Non-printable bytes show as dots in the character column. */
static void	testMemoryNonPrintable(void)
{
    unsigned char	buf[20];
    unsigned int	i;

    i = 0;
    while (i < 20)
    {
        buf[i] = (unsigned char)i;
        i++;
    }
    buf[3] = 'A';
    buf[17] = '~';
    assertMemoryDump(buf, 20);
}

/* High bytes (>= 0x80) keep two lowercase hex digits and print as dots. */
static void	testMemoryHighBytes(void)
{
    unsigned char	buf[4];

    buf[0] = 0x80;
    buf[1] = 0xff;
    buf[2] = 'a';
    buf[3] = 0xcd;
    assertMemoryDump(buf, 4);
}

/* size 0 prints nothing and still returns addr. */
static void	testMemorySizeZero(void)
{
    char	out[64];
    void	*ret;
    char	buf[4];
    int		n;

    n = captureMemory(buf, 0, out, sizeof(out), &ret);
    TEST_ASSERT_EQUAL_INT(0, n);
    TEST_ASSERT_EQUAL_PTR(buf, ret);
}

void	glorpRun(void)
{
    RUN_TEST(testMemoryFullLine);
    RUN_TEST(testMemoryPartialLine);
    RUN_TEST(testMemoryMultiLine);
    RUN_TEST(testMemoryNonPrintable);
    RUN_TEST(testMemoryHighBytes);
    RUN_TEST(testMemorySizeZero);
}
