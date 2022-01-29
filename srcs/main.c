/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasselin <gasselin@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 18:18:00 by gasselin          #+#    #+#             */
/*   Updated: 2022/01/29 17:19:10 by gasselin         ###   ########.fr       */
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
void listFilesRecursively(char *basePath, t_pc *pc)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    int fd_check;

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            char *tmp = strnstr(dp->d_name, ".txt", strlen(dp->d_name));
            if ((fd_check = open(path, O_DIRECTORY)) == -1 && strlen(tmp) == 4) {
                pc->maps_lst[pc->maps_counts] = strdup(path);
                pc->maps_counts++;
            }
            else
                close(fd_check);

            listFilesRecursively(path, pc);
        }
    }

    closedir(dir);
}

bool    switch_input(char input[50], t_pc *pc)
{
    if (!strcmp(input, "B") || !strcmp(input, "E") || !strcmp(input, "I")
        || !strcmp(input, "H") || !strcmp(input, "X")) {
        switch (*input) {
            case 'B':
                SIDE = 5;
                pc->diff_folder = "./maps/5x5";
                break;
            case 'E':
                SIDE = 10;
                pc->diff_folder = "./maps/10x10";
                break;
            case 'I':
                SIDE = 20;
                pc->diff_folder = "./maps/20x20";
                break;
            case 'H':
                SIDE = 35;
                pc->diff_folder = "./maps/35x35";
                break;
            case 'X':
                SIDE = 50;
                pc->diff_folder = "./maps/50x50";
                break;
        }
        return true;
    }
    return false;
}

void    set_custom(t_pc *pc)
{
    char input_diff[50];
    char input_level[50];
    char path[1000];

    pc->custom = true;

    printf("Enter a difficulty level (same as above) : ");
    scanf("%s", input_diff);
    printf("Enter a .txt filename : ");
    scanf("%s", input_level);

    if (!switch_input(input_diff, pc)) { 
        printf("Incorrect input!\n");
        exit (1);
    }

    strcpy(path, pc->diff_folder);
    strcat(path, "/");
    strcat(path, input_level);

    int fd_check;
    char *tmp = strnstr(input_level, ".txt", strlen(input_level));
     if ((fd_check = open(path, O_DIRECTORY)) == 0 || strlen(tmp) != 4) {
        close(fd_check);
        printf("%s is not a valid level file format\n", path);
        exit (1);
    }

    strcpy(pc->full_path, path);
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
    printf("'C' to play a specific level\n");
    scanf("%s", input);

    if (switch_input(input, pc))
        ;
    else if (!strcmp(input, "C")) {
        set_custom(pc);
    } else { 
        printf("Incorrect input!\n");
        exit (1);
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
    if (pc->custom == false) {
        listFilesRecursively(pc->diff_folder, pc);

        if (pc->maps_counts == 0) {
            printf("No level available for this difficulty\n");
            exit (1);
        }
        
        int map = rand() % pc->maps_counts;
        strcpy(pc->full_path, pc->maps_lst[map]);
    }
    
    FILE *fd = fopen(pc->full_path, "r");
    for (int i = 0; i < SIDE; i++)
        fscanf(fd, "%s\n", pc->realBoard[i]);
    fclose(fd);

    // Validate the map

    get_hints(pc);

    for (int i = 0; i < pc->maps_counts; i++)
        free (pc->maps_lst[i]);
}

int main(void)
{
    t_pc pc;
    
    srand(time(NULL));

    pc.maps_counts = 0;

    pc.diff_folder = "./maps/5x5";
    SIDE = 5;
    pc.custom = false;

    init_grids(&pc);
    
    initialize_difficulty(&pc);

    // Find a way to randomly select a level and transfer it into realBoard
    choose_level(&pc);
}