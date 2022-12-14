#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

static int num_dirs, num_regular;

bool is_dir(const char* path) {
    /*
     * Use the stat() function (try "man 2 stat") to determine if the file
     * referenced by path is a directory or not. Call stat, and then use
     * S_ISDIR to see if the file is a directory. Make sure you check the
     * return value from stat() in case there is a problem, e.g., maybe the
     * the file doesn't actually exist.
     *
     * You'll need to allocate memory for a buffer you pass to stat(); make
     * sure you free up that space before you return from this function or
     * you'll have a substantial memory leak (think of how many times this
     * will be called!).
     */
    struct stat buffer;

    if (stat(path, &buffer) < 0) {
      perror("");
      fprintf(stderr, "unable to stat path '%s', aborting...\n", path);
      exit(1);
    }

    return S_ISDIR(buffer.st_mode);
}

/*
 * This is necessary this because the mutual recursion means there's no way to
 * order them so that all the functions are defined before they're called.
 */
void process_path(const char*);

void process_directory(const char* path) {
    /*
     * Update the number of directories seen, use opendir() to open the
     * directory, and then use readdir() to loop through the entries
     * and process them. You have to be careful not to process the
     * "." and ".." directory entries, or you'll end up spinning in
     * (infinite) loops. Also make sure you closedir() when you're done.
     *
     * You'll also want to use chdir() to move into this new directory,
     * with a matching call to chdir() to move back out of it when you're
     * done.
     */

    num_dirs++;

    // Open the directory
    DIR* directory = opendir(path);
    if (directory == NULL) {
      perror("");
      fprintf(stderr, "unable to open directory '%s', aborting...\n", path);
      exit(1);
    }

    // Get the current working directory
    char cwd[PATH_MAX];
    if (getcwd(cwd, PATH_MAX) == NULL) {
      perror("");
      fprintf(stderr, "unable to get current working directory, aborting...\n");
      exit(1);
    }

    // Switch into the directory
    if (chdir(path) < 0) {
      perror("");
      fprintf(stderr, "unable to get change into directory, aborting...\n");
      exit(1);
    }

    // Get all of the children of this directory
    while (true) {
        struct dirent* entry = readdir(directory);

        if (entry == NULL) {
            break;
        }
       

        // printf("'%s'\n", entry->d_name);

        if (entry->d_name[0] == '.') {
            continue;
        }

        process_path(entry->d_name);
    }

    if (errno != 0) {
        perror("");
        fprintf(stderr, "unable to read from directory '%s', aborting...\n", path);
        exit(1);
    }

    if (closedir(directory) < 0) {
        perror("");
        fprintf(stderr, "unable to close directory '%s', aborting...\n", path);
        exit(1);
    }

    // Change the working directory back
    chdir(cwd);
}

void process_file(const char* path) {
    /*
     * Update the number of regular files.
     * This is as simple as it seems. :-)
     */
    num_regular++;
}

void process_path(const char* path) {
    if (is_dir(path)) {
        process_directory(path);
    } else {
        process_file(path);
    }
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

    process_path(argv[1]);

    printf("There were %d directories.\n", num_dirs);
    printf("There were %d regular files.\n", num_regular);

    return 0;
}