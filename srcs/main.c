/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasselin <gasselin@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 18:18:00 by gasselin          #+#    #+#             */
/*   Updated: 2022/01/28 23:25:00 by gasselin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/picross.h"

void	init_grids(t_pc *pc)
{
	for (int i = 0; i < MAXSIDE; i++) {
		memset(pc->realBoard[i], '\0', MAXSIDE);
		memset(pc->myBoard[i], '\0', MAXSIDE);
        memset(pc->hints_left[i], 0, MAXSIDE);
        memset(pc->hints_up[i], 0, MAXSIDE);
	}
}

// Recursive function to list all files in current and sub-directories
void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            printf("%s\n", dp->d_name);

            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
        }
    }

    closedir(dir);
}

int    initialize_difficulty(t_pc *pc)
{
    char input[50];
    
    printf("Enter a difficulty level :\n");
    printf("'B' for Beginner     -> 5x5 grid\n");
    printf("'E' for Easy         -> 10x10 grid\n");
    printf("'I' for Intermediate -> 20x20 grid\n");
    printf("'H' for Hard         -> 35x35 grid\n");
    printf("'X' for Extreme      -> 50x50 grid\n");
    scanf("%s", input);

    if (!strcmp(input, "B") || !strcmp(input, "E") || !strcmp(input, "I")
        || !strcmp(input, "H") || !strcmp(input, "X")) {
        switch (*input) {
            case 'B':
                SIDE = 5;
                pc->diff_folder = "./maps/5x5/";
                break;
            case 'E':
                SIDE = 10;
                pc->diff_folder = "./maps/10x10/";
                break;
            case 'I':
                SIDE = 20;
                pc->diff_folder = "./maps/20x20/";
                break;
            case 'H':
                SIDE = 35;
                pc->diff_folder = "./maps/35x35/";
                break;
            case 'X':
                SIDE = 50;
                pc->diff_folder = "./maps/50x50/";
                break;
        }
    }
    else { 
        printf("Incorrect input!\n");
        return (1);
    }
    
    return (0);
}

void get_hints(t_pc *pc)
{
    int index;
    int count;

    // hints_up
    for (int i = 0; i < SIDE; i++) {
        index = 0;
        count = 0;
        for (int j = 0; j < SIDE; j++) {
            if (count > 0 && pc->realBoard[j][i] == 'X') {
                pc->hints_up[i][index++] = count;
                count = 0;
            }
            if (pc->realBoard[j][i] == 'O')
                count++;
        }
        if (count > 0)
            pc->hints_up[i][index] = count;
    }

    // hints_left
    for (int i = 0; i < SIDE; i++) {
        index = 0;
        count = 0;
        for (int j = 0; j < SIDE; j++) {
            if (count > 0 && pc->realBoard[i][j] == 'X') {
                pc->hints_left[i][index++] = count;
                count = 0;
            }
            if (pc->realBoard[i][j] == 'O')
                count++;
        }
        if (count > 0)
            pc->hints_left[i][index] = count;
    }
}

void choose_level(t_pc *pc)
{
    struct dirent *entry;
    DIR *dir;
    char tmp_path[1000];
    int fd_check;

    dir = opendir(pc->diff_folder);
    while ((entry = readdir(dir)) != NULL) {
        if (strlen(entry->d_name) >= 5) {
            char *tmp = strnstr(entry->d_name, ".txt", strlen(entry->d_name));
            if (strlen(tmp) == 4) {
                memset(tmp_path, '\0', 1000);
                strcpy(tmp_path, pc->diff_folder);
                strcat(tmp_path, entry->d_name);
                
                if ((fd_check = open(tmp_path, O_DIRECTORY)) == -1) {
                    pc->maps_lst[pc->maps_counts] = strdup(entry->d_name);
                    pc->maps_counts++;
                }
                else
                    close(fd_check);
            }
        }
    }
    closedir(dir);

    for (int i = 0; i < pc->maps_counts; i++)
        printf("%s\n", pc->maps_lst[i]);

    if (pc->maps_counts == 0) {
        printf("No level available for this difficulty\n");
        exit (1);
    }
    
    int map = rand() % pc->maps_counts;

    strcpy(pc->full_path, pc->diff_folder);
    strcat(pc->full_path, pc->maps_lst[map]);

    FILE *fd = fopen(pc->full_path, "r");
    for (int i = 0; i < SIDE; i++)
        fscanf(fd, "%s\n", pc->realBoard[i]);
    fclose(fd);

    get_hints(pc);

}

int main(void)
{
    t_pc pc;
    
    srand(time(NULL));

    pc.maps_counts = 0;

    pc.diff_folder = "./maps/5x5/";
    SIDE = 5;

    init_grids(&pc);
    
    // // initialize_difficulty(&pc);

    // // Find a way to randomly select a level and transfer it into realBoard
    choose_level(&pc);


}