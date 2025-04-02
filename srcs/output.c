/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pboucher <pboucher@42student.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/01 17:02:53 by pboucher          #+#    #+#             */
/*   Updated: 2025/04/01 17:47:50 by pboucher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	error_msg(char *str, char *detail)
{
	ft_printf(RED"ERROR!\n"GRN);
	if (!detail)
		ft_printf(str, PROG_NAME);
	else
		ft_printf(str, PROG_NAME, detail);
	ft_printf(RST);
	exit(EXIT_FAILURE);
}
