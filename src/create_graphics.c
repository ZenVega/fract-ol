/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_graphics.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: uschmidt <uschmidt@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 14:20:32 by uschmidt          #+#    #+#             */
/*   Updated: 2025/02/17 14:07:24 by uschmidt         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fdf.h"

void	pixel_put(t_img *img, int x, int y, unsigned int color)
{
	int		offset;
	char	*dst;

	if (x > 0 && x < WIN_WIDTH && y > 0 && y < WIN_HEIGHT)
	{
		offset = y * img->line_length + x * (img->bits_per_pixel / 8);
		dst = img->addr + offset;
		*(unsigned int *)dst = color;
	}
}

t_vector	get_vector(t_point a, t_point b)
{
	t_vector		v;

	v.delta_x = b.proj_x - a.proj_x;
	v.delta_y = b.proj_y - a.proj_y;
	if (v.delta_x > v.delta_y)
		v.pixels = v.delta_x;
	else
		v.pixels = v.delta_y;
	v.delta_x /= v.pixels;
	v.delta_y /= v.pixels;
	v.delta_z = ((b.z - a.z) / (double)v.pixels);
	v.pixel_x = a.proj_x;
	v.pixel_y = a.proj_y;
	return (v);
}
void	draw_line(t_p *p, t_point a, t_point b)
{
	t_vector		v;
	unsigned int	color;
	int				i;

	if (
		(a.proj_x < 0 && b.proj_x < 0)
		|| (a.proj_x > p->width && b.proj_x > p->width)
		|| (a.proj_y < 0 && b.proj_y < 0)
		|| (a.proj_y > p->height && b.proj_y > p->height))
		return ;
	v = get_vector(a, b);
	i = 0;
	while (i < v.pixels)
	{
		if (p->syscol == 1)
			color = get_grad_col(i, v.pixels, a.color, b.color);
		else
			color = get_grad_col(i, v.pixels, a.color_height, b.color_height);
		pixel_put(&p->img, (int)v.pixel_x, (int)v.pixel_y, color);
		v.pixel_x += v.delta_x;
		v.pixel_y += v.delta_y;
		i++;
	}
}

void	gen_noise(t_p *p)
{
	int	i;
	int	x;
	int	y;
	int	color;

	i = 0;
	x = 0;
	y = 0;
	while (i++ < p->width * p->height)
	{
		color = get_grad_col(x, p->width, 0xFFFF0000, 0xFF0000FF);
		if (random() > RAND_MAX / 1.15)
			pixel_put(&p->img, x, y, color);
		x++;
		if (x > p->width)
		{
			y++;
			x = 0;
		}
	}
}

void	draw_noisy_square(t_p p)
{
	int	x;
	int	y;

	x = p.width / 3;
	y = p.height / 3;
	while (y < p.height - (p.height / 3))
	{
		if (x > p.width / 3 && x < p.width - (p.width / 3))
			if (random() > RAND_MAX / 1.15)
				pixel_put(&p.img, x, y, 0xFF00FF00);
		x++;
		if (x > p.width)
		{
			y++;
			x = 0;
		}
	}
}
