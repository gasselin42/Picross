/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   picross.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gasselin <gasselin@student.42quebec.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 18:27:25 by gasselin          #+#    #+#             */
/*   Updated: 2022/01/29 17:05:08 by gasselin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PICROSS_H
#define PICROSS_H

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXSIDE 50

int SIDE;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		len;
	int		endian;
}	t_img;

typedef struct s_pc {
    void	*mlx;
	void	*win;
	int		win_width;
	int		win_height;
	int		crt_x;
	int		crt_y;
    int     maps_counts;
    char    *diff_folder;
	char	full_path[1000];

    // new_images
	t_img	*img[2];

    // grids
	char	realBoard[MAXSIDE][MAXSIDE];
	char	myBoard[MAXSIDE][MAXSIDE];

    // list
    char    *maps_lst[1000];

	// hints
	int		hints_up[MAXSIDE][MAXSIDE];
	int		hints_left[MAXSIDE][MAXSIDE];

	// boolean switches
	bool	custom;
}   t_pc;

#endif