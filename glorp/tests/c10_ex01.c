/*
** c10_ex01 -- ft_cat (program): behaves like cat with no options. Each file
** argument is written to stdout in order; with no arguments stdin is echoed.
** Error-message wording for unreadable files is not pinned by the subject, so
** on the missing-file test only stdout is asserted (the remaining files must
** still be shown). Compiled with -Dmain=studentMain.
*/

#include "runner.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

extern int	studentMain(int argc, char **argv);

/* Runs studentMain with stdout captured; when stdinData is not NULL, stdin is
** fed from a pipe holding that data (closed so the program sees EOF). stderr
** is left alone: error text is not asserted, and a sanitizer report must stay
** visible if the exercise crashes. */
static void	captureProgram(int argc, char **argv, char *stdinData, char *out,
        int outCap)
{
    int	outPipe[2];
    int	inPipe[2];
    int	savedOut;
    int	savedIn;
    int	n;

    savedOut = dup(STDOUT_FILENO);
    savedIn = dup(STDIN_FILENO);
    pipe(outPipe);
    dup2(outPipe[1], STDOUT_FILENO);
    close(outPipe[1]);
    if (stdinData != NULL)
    {
        pipe(inPipe);
        write(inPipe[1], stdinData, strlen(stdinData));
        close(inPipe[1]);
        dup2(inPipe[0], STDIN_FILENO);
        close(inPipe[0]);
    }
    studentMain(argc, argv);
    dup2(savedOut, STDOUT_FILENO);
    dup2(savedIn, STDIN_FILENO);
    close(savedOut);
    close(savedIn);
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

    strcpy(path, "/tmp/glorp_c10_ex01_XXXXXX");
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

/* A single file is written to stdout as-is. */
static void	testCatSingleFile(void)
{
    char	path[64];
    char	out[256];
    char	*argv[3];

    makeTempFile("line one\nline two\n", path);
    argv[0] = "ft_cat";
    argv[1] = path;
    argv[2] = NULL;
    captureProgram(2, argv, NULL, out, sizeof(out));
    unlink(path);
    TEST_ASSERT_EQUAL_STRING("line one\nline two\n", out);
}

/* Several files are concatenated in argument order. */
static void	testCatTwoFiles(void)
{
    char	path1[64];
    char	path2[64];
    char	out[256];
    char	*argv[4];

    makeTempFile("first\n", path1);
    makeTempFile("second\n", path2);
    argv[0] = "ft_cat";
    argv[1] = path1;
    argv[2] = path2;
    argv[3] = NULL;
    captureProgram(3, argv, NULL, out, sizeof(out));
    unlink(path1);
    unlink(path2);
    TEST_ASSERT_EQUAL_STRING("first\nsecond\n", out);
}

/* The same file twice prints twice. */
static void	testCatSameFileTwice(void)
{
    char	path[64];
    char	out[256];
    char	*argv[4];

    makeTempFile("dup\n", path);
    argv[0] = "ft_cat";
    argv[1] = path;
    argv[2] = path;
    argv[3] = NULL;
    captureProgram(3, argv, NULL, out, sizeof(out));
    unlink(path);
    TEST_ASSERT_EQUAL_STRING("dup\ndup\n", out);
}

/* An empty file adds nothing. */
static void	testCatEmptyFile(void)
{
    char	path[64];
    char	out[64];
    char	*argv[3];

    makeTempFile("", path);
    argv[0] = "ft_cat";
    argv[1] = path;
    argv[2] = NULL;
    captureProgram(2, argv, NULL, out, sizeof(out));
    unlink(path);
    TEST_ASSERT_EQUAL_STRING("", out);
}

/* No arguments: stdin is echoed until EOF. */
static void	testCatStdin(void)
{
    char	out[256];
    char	*argv[2];

    argv[0] = "ft_cat";
    argv[1] = NULL;
    captureProgram(1, argv, "typed input\nmore\n", out, sizeof(out));
    TEST_ASSERT_EQUAL_STRING("typed input\nmore\n", out);
}

/* A missing file must not stop the following files from printing (the error
** text itself goes to stderr and is not asserted). */
static void	testCatMissingFileContinues(void)
{
    char	path1[64];
    char	path2[64];
    char	out[256];
    char	*argv[5];

    makeTempFile("before\n", path1);
    makeTempFile("after\n", path2);
    argv[0] = "ft_cat";
    argv[1] = path1;
    argv[2] = "/tmp/glorp_c10_ex01_does_not_exist";
    argv[3] = path2;
    argv[4] = NULL;
    captureProgram(4, argv, NULL, out, sizeof(out));
    unlink(path1);
    unlink(path2);
    TEST_ASSERT_EQUAL_STRING("before\nafter\n", out);
}

void	glorpRun(void)
{
    RUN_TEST(testCatSingleFile);
    RUN_TEST(testCatTwoFiles);
    RUN_TEST(testCatSameFileTwice);
    RUN_TEST(testCatEmptyFile);
    RUN_TEST(testCatStdin);
    RUN_TEST(testCatMissingFileContinues);
}
