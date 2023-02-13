#include "../mlx/mlx.h"
#include <math.h>

int worldMap[8][8]= 
{
	{1,3,2,3,2,3,2,1},
	{2,0,0,0,0,0,0,3},
	{1,0,3,0,0,0,0,3},
	{2,0,0,0,0,0,0,3},
	{1,0,0,0,0,0,0,3},
	{2,0,0,0,0,3,0,3},
	{2,0,0,0,0,0,0,3},
	{1,3,1,3,1,3,1,1}
};

typedef struct s_game
{
	void	*mlx;
	void	*mlx_win;

	double posX;
	double posY;
	double dirX;
	double dirY;
	double planeX;
	double planeY;
	double cameraX;
	double rayDirX;
	double rayDirY;
	int	mapX;
	int mapY;
	double sideDistX;
	double sideDistY;
	double deltaDistX;
	double deltaDistY;
}		t_game;

void	verLine(t_game *info, int x, int y1, int y2, int color)
{
	int	y;

	y = y1;
	while (y <= y2)
	{
		mlx_pixel_put(info->mlx, info->mlx_win, x, y, color);
		y++;
	}
}

int	main(void)
{
	t_game game;

	game.posX = 5;
	game.posY = 4;
	game.dirX = -1;
	game.dirY = 0;
	game.planeX = 0;
	game.planeY = 0.66;

	game.mlx = mlx_init();
	game.mlx_win = mlx_new_window(game.mlx, 640, 480, "Cub3d");
	while(1)
	{
		sleep(1);
		mlx_clear_window(game.mlx, game.mlx_win);
		for(int x = 0; x < 640; x++)
		{
			game.cameraX = 2 * x / 640.0f - 1;
			game.rayDirX = game.dirX + game.planeX * game.cameraX;
			game.rayDirY = game.dirY + game.planeY * game.cameraX;
			game.mapX = (int)(game.posX);
			game.mapY = (int)(game.posY);
			game.deltaDistX = sqrt(1 + (game.rayDirY * game.rayDirY) / (game.rayDirX * game.rayDirX));
			game.deltaDistY = sqrt(1 + (game.rayDirX * game.rayDirX) / (game.rayDirY * game.rayDirY));
			double perpWallDist;
			int stepX;
			int stepY;
			int	hit = 0;
			int side;
			if (game.rayDirX < 0)
			{
				stepX = -1;
				game.sideDistX = (game.posX - game.mapX) * game.deltaDistX;
			}
			else
			{
				stepX = 1;
				game.sideDistX = (game.mapX + 1.0 - game.posX) * game.deltaDistX;
			}
			if (game.rayDirY < 0)
			{
				stepY = -1;
				game.sideDistY = (game.posY - game.mapY) * game.deltaDistY;
			}
			else
			{
				stepY = 1;
				game.sideDistY = (game.mapY + 1.0 - game.posY) * game.deltaDistY;
			}
			while(hit == 0)
			{
				if(game.sideDistX < game.sideDistY)
				{
					game.sideDistX += game.deltaDistX;
					game.mapX += stepX;
					side = 0;
				}
				else
				{
					game.sideDistY += game.deltaDistY;
					game.mapY += stepY;
					side = 1;
				}
				printf("mapX :%i, mapY :%i.\n",game.mapX,game.mapY);
				if(worldMap[game.mapX][game.mapY] > 0)
					hit = 1;
			}
			if (side == 0)
				perpWallDist = (game.sideDistX - game.deltaDistX);
			else
				perpWallDist = (game.sideDistY - game.deltaDistY);
			int lineHeight = (int)(480 / perpWallDist);
			int drawStart = -lineHeight / 2 + 480 / 2;
			if(drawStart < 0) drawStart = 0;
			int drawEnd = lineHeight / 2 + 480 / 2;
			if(drawEnd >= 480) drawEnd = 480 - 1;
			int color;
			switch(worldMap[game.mapX][game.mapY])
			{
				case 1:  color = 255;	break;
				case 2:  color = 65535;  break;
				case 3:  color = 16711935; break;
				default: color = 100;	break;
			}
			if(side == 1) {color = color / 2;}

			verLine(&game, x, drawStart, drawEnd, color);
		}
		double oldDirX = game.dirX;
		game.dirX = game.dirX * cos(-100) - game.dirY * sin(-100);
		game.dirY = oldDirX * sin(-100) + game.dirY * cos(-100);
		double oldPlaneX = game.planeX;
		game.planeX = game.planeX * cos(-100) - game.planeY * sin(-100);
		game.planeY = oldPlaneX * sin(-100) + game.planeY * cos(-100);
	}
//	mlx_loop_hook(game.mlx, raycasting, game);
	mlx_loop(game.mlx);
	return (0);
}
