/*
** EPITECH PROJECT, 2020
** myhunter
** File description:
** Implements top_score.h
*/

#include "top_score.h"
#include "my/stdio.h"
#include "my/stdlib.h"
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void set_top_score(int32_t top_score)
{
    static const int all_rw = 0666;
    int fd = open("top_score.txt", O_WRONLY | O_CREAT | O_TRUNC, all_rw);

    if (fd < 0) {
        my_dputs("Failed to open top_score.txt\n", STDERR_FILENO);
        return;
    }
    my_dprintf(fd, "%" PRId32, top_score);
    close(fd);
}

int32_t get_top_score(void)
{
    FILE *score_file = fopen("top_score.txt", "r");
    char buffer[7];
    int bytes_read;
    int result;

    if (!score_file)
        return 12000;
    bytes_read = fread(buffer, 1, 6, score_file);
    buffer[bytes_read] = '\0';
    result = my_strtol_base_str(buffer, NULL, "0123456789");
    fclose(score_file);
    return result;
}
