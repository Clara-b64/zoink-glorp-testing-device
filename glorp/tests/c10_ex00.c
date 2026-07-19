/*
** c10_ex00 -- ft_display_file (program): ./ft_display_file <file> writes the
** file's content to stdout. With no argument it writes "File name missing."
** and a newline to stderr; with more than one, "Too many arguments." and a
** newline; when the file cannot be read, "Cannot read file." and a newline.
** Compiled with -Dmain=studentMain; the test fabricates argv and temp files.
*/

#include "runner.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern int	studentMain(int argc, char **argv);

/* Runs studentMain with stdout and stderr captured separately. */
static void	captureProgram(int argc, char **argv, char *out, int outCap,
        char *err, int errCap)
{
    int	outPipe[2];
    int	errPipe[2];
    int	savedOut;
    int	savedErr;
    int	n;

    savedOut = dup(STDOUT_FILENO);
    savedErr = dup(STDERR_FILENO);
    pipe(outPipe);
    pipe(errPipe);
    dup2(outPipe[1], STDOUT_FILENO);
    dup2(errPipe[1], STDERR_FILENO);
    close(outPipe[1]);
    close(errPipe[1]);
    studentMain(argc, argv);
    dup2(savedOut, STDOUT_FILENO);
    dup2(savedErr, STDERR_FILENO);
    close(savedOut);
    close(savedErr);
    n = (int)read(outPipe[0], out, outCap - 1);
    if (n < 0)
        n = 0;
    out[n] = '\0';
    close(outPipe[0]);
    n = (int)read(errPipe[0], err, errCap - 1);
    if (n < 0)
        n = 0;
    err[n] = '\0';
    close(errPipe[0]);
}

/* Creates a temp file holding content; path is copied into path (>= 64). */
static void	makeTempFile(char *content, char *path)
{
    int	fd;

    strcpy(path, "/tmp/glorp_c10_ex00_XXXXXX");
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

/* One argument: the file content lands on stdout, stderr stays silent. */
static void	testDisplayFileContent(void)
{
    char	path[64];
    char	out[256];
    char	err[256];
    char	*argv[3];

    makeTempFile("Hello from a file!\nsecond line\n", path);
    argv[0] = "ft_display_file";
    argv[1] = path;
    argv[2] = NULL;
    captureProgram(2, argv, out, sizeof(out), err, sizeof(err));
    unlink(path);
    TEST_ASSERT_EQUAL_STRING("Hello from a file!\nsecond line\n", out);
    TEST_ASSERT_EQUAL_STRING("", err);
}

/* An empty file prints nothing at all. */
static void	testDisplayFileEmpty(void)
{
    char	path[64];
    char	out[64];
    char	err[64];
    char	*argv[3];

    makeTempFile("", path);
    argv[0] = "ft_display_file";
    argv[1] = path;
    argv[2] = NULL;
    captureProgram(2, argv, out, sizeof(out), err, sizeof(err));
    unlink(path);
    TEST_ASSERT_EQUAL_STRING("", out);
    TEST_ASSERT_EQUAL_STRING("", err);
}

/* No argument: the exact error message goes to stderr, stdout stays empty. */
static void	testDisplayFileNoArgument(void)
{
    char	out[64];
    char	err[64];
    char	*argv[2];

    argv[0] = "ft_display_file";
    argv[1] = NULL;
    captureProgram(1, argv, out, sizeof(out), err, sizeof(err));
    TEST_ASSERT_EQUAL_STRING("", out);
    TEST_ASSERT_EQUAL_STRING("File name missing.\n", err);
}

/* Two file arguments: too many. */
static void	testDisplayFileTooManyArguments(void)
{
    char	out[64];
    char	err[64];
    char	*argv[4];

    argv[0] = "ft_display_file";
    argv[1] = "a";
    argv[2] = "b";
    argv[3] = NULL;
    captureProgram(3, argv, out, sizeof(out), err, sizeof(err));
    TEST_ASSERT_EQUAL_STRING("", out);
    TEST_ASSERT_EQUAL_STRING("Too many arguments.\n", err);
}

/* An unreadable file reports "Cannot read file." on stderr. */
static void	testDisplayFileUnreadable(void)
{
    char	out[64];
    char	err[64];
    char	*argv[3];

    argv[0] = "ft_display_file";
    argv[1] = "/tmp/glorp_c10_ex00_does_not_exist";
    argv[2] = NULL;
    captureProgram(2, argv, out, sizeof(out), err, sizeof(err));
    TEST_ASSERT_EQUAL_STRING("", out);
    TEST_ASSERT_EQUAL_STRING("Cannot read file.\n", err);
}

void	glorpRun(void)
{
    RUN_TEST(testDisplayFileContent);
    RUN_TEST(testDisplayFileEmpty);
    RUN_TEST(testDisplayFileNoArgument);
    RUN_TEST(testDisplayFileTooManyArguments);
    RUN_TEST(testDisplayFileUnreadable);
}
