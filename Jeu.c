/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Jeu.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiallo <mdiallo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/03 23:03:55 by mdiallo           #+#    #+#             */
/*   Updated: 2021/12/09 22:56:59 by mdiallo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Jeu.h"

char   *prompt(char *mess, int size, int i)
{
    char    *str;

    str = malloc(sizeof(char) * size);
    if (i) {
        if (i == 2) {
            if (mess[strlen(mess) - 1] == '\n')
                mess[strlen(mess) - 1] = '\0';
            printf("\t @%s > ", mess);
        }
        else
            printf("\n\t %s > ", mess);
    }
    else
        printf("\n%s", mess);
    fflush(stdin);
    fgets(str, size, stdin);
    if (ferror(stdin)) {
        fprintf(stderr, "Reading error with code %d\n", errno );
        return (char *)NULL;
    }
    return (str);
}

t_elm	*ft_new_elm(int value, int nb_point) {
    
	t_elm	*new_maillon;

	if (!(new_maillon = malloc(sizeof(t_elm))))
		return (NULL);
    new_maillon->nb_point = nb_point;
	new_maillon->value = value;
	new_maillon->next = NULL;
	return (new_maillon);
}

t_stack    *init_stack(int i) {

    t_stack *stack;

    if (!(stack = malloc(sizeof(t_stack))))
        return ((t_stack *)NULL);
    stack->first = NULL;
    stack->len = 0;
    stack->total = 0;
    stack->name_player = NULL;
    printf("\n\t\t ==== Veuillez donner un pseudo au joueur%d ====\n", i);
    while (stack->name_player == NULL) {
        stack->name_player = prompt("\t Pseudo ", 10, 1);
        if (stack->name_player == NULL)
            printf("\t\t ==== Pseudonyme entre 1 et 9 caractères ====\n");
    }
    stack->nxt = NULL;
    return (stack);
}

Liste    *init_lst() {

    int   nb_point, value;
    Liste *lst;

    if (!(lst = (Liste *)malloc(sizeof(Liste))))
        return (0);
    lst->size = 0;
    lst->premier = NULL;
    lst->stack = NULL;
    nb_point = 0;
    value = 20;
    while (++value <= 36) {
        if (value % 4 == 1)
            ++nb_point;
        push_back(ft_new_elm(value, nb_point), lst);
    }
    return (lst);
}

void    add_lst_stack(Liste *lst, t_stack *stack) {

    t_stack *tmp;

    if (lst->stack == NULL) {
        lst->stack = stack;
        return ;
    }
    tmp = lst->stack;
    while (tmp->nxt)
        tmp = tmp->nxt;
    tmp->nxt = stack;
}

t_bool    is_emptylst(Liste *lst) {

    if (lst->premier)
        return (false);
    return (true);
}

t_bool  is_emptyPile(t_stack *stack) {

    if (stack->first)
        return (false);
    return (true);   
}

void    push_back(t_elm *elm, Liste *lst) {

    t_elm *tmp;

    lst->size++;
    if (is_emptylst(lst) == true) {
        lst->premier = elm;
        return ;
    }
    tmp = lst->premier;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = elm;
}

void    push_stack(t_elm *elm, t_stack *stack) {
    if (elm) {
        if (stack->first)
            elm->next = stack->first;
        stack->first = elm;
        stack->len++;
        stack->total += elm->nb_point;
    }
}

t_elm     *pull_bis(Liste *lst, int nb) {

    t_elm   *curr;
    t_elm   *prev;

    prev = lst->premier;
    curr = prev->next;
    while (curr && curr->next) {
        if (curr->next->value >= nb) {
            if (curr->next->value > nb)
                prev->next = curr->next;
            else {
                prev = prev->next;
                curr = curr->next;
                prev->next = curr->next;
            }
            curr->next = NULL;
            lst->size--;
            return (curr);
        }
        curr = curr->next;
        prev = prev->next;
    }
    lst->size--;
    prev->next = NULL;
    return (curr);
}

t_elm    *pull_lst(Liste *lst, int nb) {

    t_elm   *curr;

    if (is_emptylst(lst) == false) {
        if (lst->premier->next == NULL && lst->premier->value <= nb) {
            curr = lst->premier;
            lst->premier = NULL;
            lst->size--;
            return (curr);
        }
        if (lst->premier->value == nb || \
            (lst->premier->next && lst->premier->next->value > nb)) {
            curr = lst->premier;
            lst->premier = lst->premier->next;
            lst->size--;
            curr->next = NULL;
            return (curr);
        }
        return (pull_bis(lst, nb));
    }
    return ((t_elm *)NULL);
}

void    insert_elm(Liste *lst, t_elm *elm) {

    t_elm   *tmp;

    lst->size++;
    if (is_emptylst(lst) == true) {
        lst->premier = elm;
        return;
    }
    if (lst->premier && lst->premier->value > elm->value) {
        elm->next = lst->premier;
        lst->premier = elm;
        return;       
    }
    tmp = lst->premier;
    while (tmp->next) {
        if (tmp->next->value > elm->value) {
            elm->next = tmp->next;
            tmp->next = elm;
            return;
        }
        tmp = tmp->next;
    }
    tmp->next = elm;
}

t_elm    *pull_stack(t_stack *stack) {

    t_elm   *elm;

    elm = (t_elm *)NULL;
    if (stack->first) {
        elm = stack->first;
        stack->first = elm->next;
        elm->next = NULL;
        stack->len--;
        stack->total -= elm->nb_point;
    }
    return (elm);
}

void    free_stack(t_stack *stack) {

    t_elm *tmp;

    while (stack->first) {
        tmp = stack->first;
        stack->first = stack->first->next;
        tmp->next = NULL;
        free(tmp);
        stack->len--;
    }
    free(stack->name_player);
    free(stack);
}

void    print_stack(t_stack *stack) {

    t_elm *elm;

    if (stack->first == NULL) {
        printf("\n\t ==== Pile vide ====\n\n");
        return;
    }
    for (int i = 0; i < stack->len; i++)
        printf("╔══╗ ");
    printf("\n");
    elm = stack->first;
    while (elm) {
        printf("║%d║ ", elm->value);
        elm = elm->next;
    }
    printf("\n");
    for (int i = 0; i < stack->len; i++)
        printf("╟──╢ ");
    printf("\n");
    elm = stack->first;
    while (elm) {
        printf("║ %d║ ", elm->nb_point);
        elm = elm->next;
    }
    printf("\n");
    for (int i = 0; i < stack->len; i++)
        printf("╚══╝ ");
    printf("\n\n\t Nombre de points ==> %d\n\n", stack->total);
}

void    print_elm(Liste *liste) {

    t_stack *stack;

    stack = liste->stack;
    while (stack) {
        printf("\n\t@%s\n", stack->name_player);
        printf("\t╔══╗\n");
        if (stack->first)
            printf("\t║%d║\n", stack->first->value);
        printf("\t╟──╢\n");
        if (stack->first)
            printf("\t║ %d║\n", stack->first->nb_point);
        printf("\t╚══╝\n");
        printf("\n\n");
        stack = stack->nxt;
    }
}

void    free_lst(Liste *lst) {

    t_elm   *tmp;
    t_stack *stack;

    while (lst->premier) {
        tmp = lst->premier;
        lst->premier = lst->premier->next;
        tmp->next = NULL;
        free(tmp);
    }
    while (lst->stack) {
        stack = lst->stack;
        lst->stack = lst->stack->nxt;
        free_stack(stack);
    }
    free(lst);
}

void    display_pickominos(Liste *lst) {

    t_elm *elm;

    printf("\n");
    for (int i = 0; i < lst->size; i++)
        printf("╔══╗ ");
    printf("\n");
    elm = lst->premier;
    while (elm) {
        printf("║%d║ ", elm->value);
        elm = elm->next;
    }
    printf("\n");
    for (int i = 0; i < lst->size; i++)
        printf("╟──╢ ");
    printf("\n");
    elm = lst->premier;
    while (elm) {
        printf("║ %d║ ", elm->nb_point);
        elm = elm->next;
    }
    printf("\n");
    for (int i = 0; i < lst->size; i++)
        printf("╚══╝ ");
    printf("\n\n");
}

Liste		*init_game() {

    int     nbJ;
    char    *str;
    Liste   *liste;
    t_stack *stack;

    fflush(stdin);
    nbJ = 0;
    while (!nbJ) {
        str = prompt("Veuillez saisir le nombre de joueur[1,7] : ", 2, 0);
        nbJ = atoi(str);
        free(str);
        if (nbJ <= 0 || nbJ > 7)
            printf("\n\t\t \033[31m==== nombre de joueur doit être entre [1,7] ====\033[00m\n");
    }
    liste = init_lst();
    liste->nbplayer = nbJ;
    if (liste->nbplayer >= 2) {
        for (int i = 0; i < liste->nbplayer; i++) {
            stack = init_stack(i + 1);
            add_lst_stack(liste, stack);
        }
    }
    return (liste);
}

void    printer(int nb_d, int *d) {

    printf("\n\t Les numéros gagnants ==> ");
    for (int i = 0; i < nb_d; i++){
        if (d[i] == 6)
            printf("[V] ");
        else
            printf("[%d] ", d[i]);
    }
    printf("\n");
}

t_bool  checkSaisi(char *saisi) {

    if (!strcmp(saisi, "1") || !strcmp(saisi, "p") ||
        !strcmp(saisi, "2") || !strcmp(saisi, "s") ||
        !strcmp(saisi, "3") || !strcmp(saisi, "4") ||
        !strcmp(saisi, "5") || !strcmp(saisi, "v") ||
        !strcmp(saisi, "V") || !strcmp(saisi, "P") ||
        !strcmp(saisi, "a") || !strcmp(saisi, "A") ||
        !strcmp(saisi, "S"))
        return (true);
    return (false);
}

t_bool  valuExist(int *d, int val, int nb_d) {

    for (int i = 0; i < nb_d; i++) {
        if (d[i] == val)
            return (true);
    }
    return (false);
}

t_bool      printerSave(Liste *liste, t_stack *stack, char *saisi) {

    if (!strcmp(saisi, "p") || !strcmp(saisi, "P")) {
        print_elm(liste);
        return (true);
    }
    if (!strcmp(saisi, "a") || !strcmp(saisi, "A")) {
        print_stack(stack);
        return (true);
    }
    if (!strcmp(saisi, "o") || !strcmp(saisi, "O")) {
        display_pickominos(liste);
        return (true);
    }
    if (!strcmp(saisi, "s") || !strcmp(saisi, "S")) {
        return (true);
    }
    return (false);
}

int    getValue(Liste *liste, int nb_d, int *stock, int *d, t_stack *stack) {

    int     j;
    int     jet;
    char    *saisi;

    j = 0;
    while (true)
    {
        printf("\n\t\t === Veuillez choisir la valeur à conserver ===\n\n");
        saisi = prompt(stack->name_player , 2, 2);
        if (checkSaisi(saisi) == false) {
            printf("\n\t\t \033[31m===== Saisie incorrecte!!! =====\033[00m\n");
            printer(nb_d, d);
            free(saisi);
            continue ;
        }
        if (printerSave(liste, stack, saisi) == true) {
            printer(nb_d, d);
            free(saisi);
            continue ;
        }
        if (!strcmp(saisi, "v") || !strcmp(saisi, "V"))
            jet = 6;
        else
            jet = atoi(saisi);
        free(saisi);
        if (valuExist(d, jet, nb_d) == false) {
            printf("\n\t\t \033[31m===== Choix impossible!!! =====\033[00m\n");
            printer(nb_d, d);
            continue ;
        }
        if (stock[jet - 1]) {
            printf("\n\t\t \033[31m===== Valeur déjà choisie, choisir une autre =====\033[00m\n");
            printer(nb_d, d);
        }
        else
            break;
    }
    for (int i = 0; i < nb_d; i++) {
        if (d[i] == jet) {
            stock[jet - 1] += 1;
            j++;
        }
    }
    return (j);
}

t_bool  valExist(int *d, int *stock, int nb_d) {

    for (int i = 0; i < nb_d; i++) {
        if (stock[d[i] - 1] == 0)
            return (true);
    }
    return (false);
}

t_bool  start(Liste *liste, t_stack *stack, char *mess) {

    char *rp;

    while (true) {
        rp = prompt(mess, 2, 2);
        if (strcmp(rp, "l") == 0 || strcmp(rp, "L") == 0) {
            free(rp);
            return (true);
        }
        if (strcmp(rp, "r") == 0 || strcmp(rp, "R") == 0) {
            free(rp);
            return (false);
        }
        if (printerSave(liste, stack, rp) == true) {
            free(rp);
            continue ;
        }
        free(rp);
        printf("\n\t\t \033[31m==== Erreure de saisie!!! ====\033[00m\n\n");
    }
}

int     calculValue(int *stock) {

    int res;

    res = 0;
    for (int i = 0; i < 6; i++) {
        if (i == 5)
            res += i * stock[i];
        else
            res += (i + 1) * stock[i];
    }
    return (res);
}

void    printStock(int *stock) {

    printf("\n\t ");
    for (int i = 0; i < 6; i++) {
        if (stock[i] == 6)
            printf("[V] ");
        else
            printf("[%d] ", stock[i]);
    }
    printf("\n\n\t ");
}

int    jets_d(Liste *liste, t_stack *stack) {

    int     jet;
    int     nb_d;
	int		stock[6];
	int		d[8];

    nb_d = 8;
    memset(stock, 0, sizeof(int) * 6);
    srand( time( NULL ) );
    while (nb_d) {
        memset(d, 0, sizeof(int) * 8);
        if (start(liste, stack, stack->name_player) == false)
            break ;
        for (int i = 0; i < nb_d; i++) {
            jet = rand() % (6 + 1 - 1) + 1;
            d[i] = jet;
        }
        printer(nb_d, d);
        if (valExist(d, stock, nb_d) == false) {
            printf("\n\t\t\033[31m === Pas de nouvelle valeure, tour raté [pass] ===\033[00m\n");
            return (0);
        }
        jet = getValue(liste, nb_d, stock, d, stack);
        nb_d -= jet;
    }
    printStock(stock);
    if (stock[5] == 0) {
        printf("\n\t\t \033[31m=== Pas de [V] dans les valeurs conservées, tour raté [pass] ===\033[00m\n");
        return (0);
    }
    jet = calculValue(stock);
    printf("Valeur =>> [%d]\n", jet);
    return (jet);
}


void    spillElm(t_elm *elm) {

    t_elm *curr;
    t_elm *prev;

    if (elm->next) {
        prev = elm;
        curr = elm->next;
        while (curr->next) {
            curr = curr->next;
            prev = prev->next;
        }
        free(curr);
        prev->next = NULL;
    }
}

t_bool  checkAdversary(Liste *liste, t_stack *stack, int value) {

    t_elm   *elm;
    t_stack *tmp;

    tmp = liste->stack;
    while (tmp) {
        if (tmp != stack) {
            if (tmp->first && tmp->first->value == value) {
                elm = pull_stack(tmp);
                push_stack(elm, stack);
                return (true);
            }
        }
        tmp = tmp->nxt;
    }
    return (false);
}

void game_winner(Liste *liste) {

    t_stack *stack;
    t_stack *tmp;

    stack = liste->stack;
    printf("\n====================================================\n\n");
    printf("@%s\n", stack->name_player);
    print_stack(stack);
    tmp = stack->nxt;
    while (tmp) {
        if (stack->total < tmp->total)
            stack = tmp;
        printf("\n====================================================\n\n");
        printf("@%s\n", tmp->name_player);
        print_stack(tmp);
        tmp = tmp->nxt;
    }
    printf("\n\t====================================================\n");
    printf("\n\t ======== Le gagnant est %s avec %d points =======\n", stack->name_player, stack->total);
    printf("\n\t====================================================\n\n");
}

void    start_game() {
    
    int       value;
    Liste     *liste;
    t_stack   *stack;
    t_elm     *elm;

    liste = init_game();
    stack = liste->stack;
    while(liste->premier) {
        display_pickominos(liste);
        value = jets_d(liste, stack);
        if (!value || value < liste->premier->value ) {
            elm = pull_stack(stack);
            if (elm) {
                insert_elm(liste, elm);
                spillElm(elm);
            }
        }
        else {
            if (checkAdversary(liste, stack, value) == false) {
                elm = pull_lst(liste, value);
                if (elm)
                    push_stack(elm, stack);
            }
        }
        if (stack->nxt)
            stack = stack->nxt;
        else
            stack = liste->stack;
    }
    game_winner(liste);
    free_lst(liste);
}
