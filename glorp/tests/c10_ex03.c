/*
** c10_ex03 -- ft_hexdump (program): ./ft_hexdump -C file... behaves like
** hexdump -C: per 16 bytes one line "%08x  xx xx xx xx xx xx xx xx  xx xx xx
** xx xx xx xx xx  |chars|" (lowercase hex, missing bytes space-padded,
** non-printables as '.'), then a final line holding the total offset. The
** expected text is rebuilt by the test with the same algorithm (verified
** against the real hexdump -C). Repeating lines are avoided in the fixtures,
** so the real tool's '*'-collapsing never kicks in and is not required.
** Compiled with -Dmain=studentMain.
*/

#include "runner.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>

extern int	studentMain(int argc, char **argv);

/* Runs studentMain with stdout captured. stderr is left alone: error text is
** not asserted, and a sanitizer report must stay visible on a crash. */
static void	captureProgram(int argc, char **argv, char *out, int outCap)
{
    int	outPipe[2];
    int	savedOut;
    int	n;

    savedOut = dup(STDOUT_FILENO);
    pipe(outPipe);
    dup2(outPipe[1], STDOUT_FILENO);
    close(outPipe[1]);
    studentMain(argc, argv);
    dup2(savedOut, STDOUT_FILENO);
    close(savedOut);
    n = (int)read(outPipe[0], out, outCap - 1);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    close(outPipe[0]);
}

/* Creates a temp file holding len raw bytes; path buffer must be >= 64. */
static void	makeTempFile(unsigned char *content, int len, char *path)
{
    int	fd;

    strcpy(path, "/tmp/glorp_c10_ex03_XXXXXX");
    fd = mkstemp(path);
    TEST_ASSERT_TRUE(fd >= 0);
    write(fd, content, (size_t)len);
    close(fd);
}

/* Builds the hexdump -C rendering of buf into expected. */
static void	buildExpected(unsigned char *buf, int size, char *expected)
{
    char	*p;
    int		line;
    int		i;

    p = expected;
    line = 0;
    while (line < size)
    {
        p += sprintf(p, "%08x  ", (unsigned int)line);
        i = 0;
        while (i < 16)
        {
            if (line + i < size)
                p += sprintf(p, "%02x ", buf[line + i]);
            else
                p += sprintf(p, "   ");
            if (i == 7)
                *p++ = ' ';
            i++;
        }
        *p++ = ' ';
        *p++ = '|';
        i = 0;
        while (i < 16 && line + i < size)
        {
            if (buf[line + i] >= 32 && buf[line + i] <= 126)
                *p++ = (char)buf[line + i];
            else
                *p++ = '.';
            i++;
        }
        *p++ = '|';
        *p++ = '\n';
        line += 16;
    }
    p += sprintf(p, "%08x\n", (unsigned int)size);
    *p = '\0';
}

/* Dumps the file through the program and compares with the reference. */
static void	assertHexdump(unsigned char *content, int len)
{
    char	path[64];
    char	out[2048];
    char	expected[2048];
    char	*argv[4];

    makeTempFile(content, len, path);
    argv[0] = "ft_hexdump";
    argv[1] = "-C";
    argv[2] = path;
    argv[3] = NULL;
    captureProgram(3, argv, out, sizeof(out));
    unlink(path);
    buildExpected(content, len, expected);
    TEST_ASSERT_EQUAL_STRING(expected, out);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* A short printable file: one padded line plus the final offset. */
static void	testHexdumpShortLine(void)
{
    assertHexdump((unsigned char *)"hello world\n", 12);
}

/* An exactly-16-byte file: one full line, offset 00000010. */
static void	testHexdumpExactLine(void)
{
    assertHexdump((unsigned char *)"abcdefghijklmnop", 16);
}

/* Several lines with distinct content and a partial tail. */
static void	testHexdumpMultiLine(void)
{
    assertHexdump((unsigned char *)"The quick brown fox jumps over the lazy dog", 43);
}

/* Control bytes render as dots and two lowercase hex digits. */
static void	testHexdumpControlBytes(void)
{
    unsigned char	buf[20];
    int				i;

    i = 0;
    while (i < 20)
    {
        buf[i] = (unsigned char)(i + 1);
        i++;
    }
    buf[4] = 'A';
    buf[18] = '~';
    assertHexdump(buf, 20);
}

/* High bytes (>= 0x80) and NUL bytes must dump like any other byte. */
static void	testHexdumpBinaryBytes(void)
{
    unsigned char	buf[10];

    buf[0] = 0x00;
    buf[1] = 0x80;
    buf[2] = 0xff;
    buf[3] = 'a';
    buf[4] = 0x00;
    buf[5] = 0x7f;
    buf[6] = 0xc3;
    buf[7] = 'z';
    buf[8] = 0x01;
    buf[9] = 0xfe;
    assertHexdump(buf, 10);
}

void	glorpRun(void)
{
    RUN_TEST(testHexdumpShortLine);
    RUN_TEST(testHexdumpExactLine);
    RUN_TEST(testHexdumpMultiLine);
    RUN_TEST(testHexdumpControlBytes);
    RUN_TEST(testHexdumpBinaryBytes);
}
