/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Jeu.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiallo <mdiallo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 23:04:01 by mdiallo           #+#    #+#             */
/*   Updated: 2021/12/17 19:22:39 by mdiallo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __JEU_H__
#define __JEU_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


typedef enum e_bool
{
	false,
	true
}t_bool;

typedef struct s_elm
{
	int				nb_point;
	int				value;
	struct s_elm	*next;
}				t_elm;

typedef struct s_stack
{
	int		set;
	int		len;
	int		total;
	char	*name_player;
	t_elm	*first;
	struct s_stack	*nxt;
}				t_stack;

typedef struct liste
{
    int   	size;
	int	  	nbplayer;
    t_elm 	*premier;
	t_stack	*stack;
}Liste;

void    game();
char    *prompt(char *mess, int size, int i);
t_elm	*ft_new_elm(int value, int nb_point);
Liste   *init_lst();
t_bool  is_emptylst(Liste *lst);
t_bool  is_emptyPile(t_stack *stack);
t_elm   *pull_lst(Liste *lst, int nb);
t_stack *init_stack(int i);
void    start_game(Liste *liste);
int     jets_d(Liste *liste, t_stack *stack);
Liste	*init_game();
void    spillElm(t_elm *elm);
t_bool  checkAdversary(Liste *liste, t_stack *stack, int value);
int    	getValue(Liste *liste, int nb_d, int *stock, int *d, t_stack *stack);
void    add_lst_stack(Liste *lst, t_stack *stack);
void    push_back(t_elm *elm, Liste *lst);
void    push_stack(t_elm *elm, t_stack *stack);
t_elm   *pull_stack(t_stack *stack);
void    free_stack(t_stack *stack);
void    free_lst(Liste *lst);
void    print_stack(t_stack *stack);
void 	game_winner(Liste *liste);
void    display_pickominos(Liste *lst);

// ******************************** BONUS ************************************

void	create_file(Liste *liste, t_stack *stack);
Liste   *restart_game();
void    re_init_stack(Liste *liste, char *s);
Liste   *re_init_lst(char *str);
void    parse_line(char *s, Liste *liste, t_stack *stack, int v);
void    parse_value(Liste *liste, t_stack *stack, char *str, int v);
void    push_backStack(t_elm *elm, t_stack *stack);
void    save_value(t_elm *elm, FILE *fichier);
char	*ft_itoa(int n);
t_bool	check_directory();

#endif