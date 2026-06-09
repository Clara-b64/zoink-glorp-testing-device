/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_combn.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uzanchi <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 20:21:13 by uzanchi           #+#    #+#             */
/*   Updated: 2024/03/13 20:21:17 by uzanchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	print_comb(int *digit, int n)
{
	int		i;
	char	c;

	i = 0;
	while (i < n)
	{
		c = digit[i] + '0';
		write(1, &c, 1);
		i++;
	}
}

void	next_comb(int *digit, int n)
{
	int	i;

	i = n - 1;
	while (i >= 0 && digit[i] == 10 - n + i)
		i--;
	if (i < 0)
		return ;
	digit[i]++;
	i++;
	while (i < n)
	{
		digit[i] = digit[i - 1] + 1;
		i++;
	}
}

void	ft_print_combn(int n)
{
	int	digit[10];
	int	i;
	int	last;

	i = 0;
	while (i < n)
	{
		digit[i] = i;
		i++;
	}
	last = 0;
	while (!last)
	{
		print_comb(digit, n);
		last = (digit[0] == 10 - n);
		if (!last)
		{
			write(1, ", ", 2);
			next_comb(digit, n);
		}
	}
}
