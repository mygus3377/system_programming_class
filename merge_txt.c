#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_directory> <output_file>\n", argv[0]);
        return 1;
    }
    DIR *dp = opendir(argv[1]);
    if (dp == NULL) {
        return 1;
    }
    int fd_out = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_out < 0) {
        closedir(dp);
        return 1;
    }
    struct dirent *de;
    while ((de = readdir(dp)) != NULL) {        
        int len = strlen(de->d_name);
        if (len < 4) continue;
        if (strcmp(de->d_name + len - 4, ".txt") != 0) continue;
        char path[512];
        snprintf(path, sizeof(path), "%s/%s", argv[1], de->d_name);
        int fd_in = open(path, O_RDONLY);
        if (fd_in < 0) {
            perror("open input");
            continue;
        }
        char ch;
        while (read(fd_in, &ch, 1) > 0) {
            if (ch == ' ') continue;
            write(fd_out, &ch, 1);
        }
        close(fd_in);
    }
    close(fd_out);
    closedir(dp);
    return 0;
}