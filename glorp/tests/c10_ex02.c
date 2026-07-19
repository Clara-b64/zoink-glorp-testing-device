/*
** c10_ex02 -- ft_tail (program): behaves like the system tail restricted to
** the -c option: ./ft_tail -c N file... writes the last N bytes of each file.
** With several files each block is preceded by a header, "==> name <==" plus
** newline, and headers after the first get a leading newline (exactly like
** the real tail). Only well-formed invocations are tested; error-message
** wording is not pinned by the subject. Compiled with -Dmain=studentMain.
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

/* Creates a temp file holding content; the path is copied into path (>= 64). */
static void	makeTempFile(char *content, char *path)
{
    int	fd;

    strcpy(path, "/tmp/glorp_c10_ex02_XXXXXX");
    fd = mkstemp(path);
    TEST_ASSERT_TRUE(fd >= 0);
    write(fd, content, strlen(content));
    close(fd);
}

void	glorpUp(void)
{
}

void	glorpDown(void)
{
}

/* Last N bytes of a single file, no header. */
static void	testTailLastBytes(void)
{
    char	path[64];
    char	out[128];
    char	*argv[5];

    makeTempFile("hello world!", path);
    argv[0] = "ft_tail";
    argv[1] = "-c";
    argv[2] = "5";
    argv[3] = path;
    argv[4] = NULL;
    captureProgram(4, argv, out, sizeof(out));
    unlink(path);
    TEST_ASSERT_EQUAL_STRING("orld!", out);
}

/* N bigger than the file: the whole file. */
static void	testTailCountBiggerThanFile(void)
{
    char	path[64];
    char	out[128];
    char	*argv[5];

    makeTempFile("short\n", path);
    argv[0] = "ft_tail";
    argv[1] = "-c";
    argv[2] = "100";
    argv[3] = path;
    argv[4] = NULL;
    captureProgram(4, argv, out, sizeof(out));
    unlink(path);
    TEST_ASSERT_EQUAL_STRING("short\n", out);
}

/* -c 0 prints nothing. */
static void	testTailZeroBytes(void)
{
    char	path[64];
    char	out[64];
    char	*argv[5];

    makeTempFile("content", path);
    argv[0] = "ft_tail";
    argv[1] = "-c";
    argv[2] = "0";
    argv[3] = path;
    argv[4] = NULL;
    captureProgram(4, argv, out, sizeof(out));
    unlink(path);
    TEST_ASSERT_EQUAL_STRING("", out);
}

/* Two files: tail-style headers, the second one preceded by a blank line. */
static void	testTailTwoFilesHeaders(void)
{
    char	path1[64];
    char	path2[64];
    char	out[512];
    char	expected[512];
    char	*argv[6];

    makeTempFile("abcdef", path1);
    makeTempFile("uvwxyz", path2);
    argv[0] = "ft_tail";
    argv[1] = "-c";
    argv[2] = "3";
    argv[3] = path1;
    argv[4] = path2;
    argv[5] = NULL;
    captureProgram(5, argv, out, sizeof(out));
    snprintf(expected, sizeof(expected),
        "==> %s <==\ndef\n==> %s <==\nxyz", path1, path2);
    unlink(path1);
    unlink(path2);
    TEST_ASSERT_EQUAL_STRING(expected, out);
}

void	glorpRun(void)
{
    RUN_TEST(testTailLastBytes);
    RUN_TEST(testTailCountBiggerThanFile);
    RUN_TEST(testTailZeroBytes);
    RUN_TEST(testTailTwoFilesHeaders);
}
