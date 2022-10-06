#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <ftw.h>

static int num_dirs, num_regular;

#define MAX_FTW_DEPTH 16

static int callback(const char* fpath, const struct stat *sb, int typeflag) {
    if (S_ISDIR(sb->st_mode)) {
        num_dirs++;
    }
    else {
        num_regular++;
    }

    return 0;
}

int main (int argc, char *argv[]) {
    // Ensure an argument was provided.
    if (argc != 2) {
        printf ("Usage: %s <path>\n", argv[0]);
        printf ("       where <path> is the file or root of the tree you want to summarize.\n");
        return 1;
    }

    num_dirs = 0;
    num_regular = 0;

    ftw(argv[1], callback, MAX_FTW_DEPTH);

    printf("There were %d directories.\n", num_dirs);
    printf("There were %d regular files.\n", num_regular);

    return 0;
}