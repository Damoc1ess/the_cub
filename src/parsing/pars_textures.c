#include "../../include/cub.h"
#include "../../libft/includes/ft_array.h"
#include "../../libft/includes/get_next_line.h"

char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	char	*result;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != '"')
		{
			result[j] = str[i];
			j++;
		}
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	check_xpm_size(const char *texture_path)
{
	int		fd;
	char	*line;
	char	**tokens;
	int		width;
	int		height;
	int		found_dimensions;
	char	*cleaned_line;

	found_dimensions = 0;
	fd = open(texture_path, O_RDONLY);
	if (fd < 0)
		map_error("Error: Failed to open texture file.", NULL, NULL);
	while ((line = get_next_line(fd)) != NULL)
	{
		if (line[0] == '"' && !found_dimensions)
		{
			cleaned_line = remove_quotes(line);
			if (!cleaned_line)
			{
				free(line);
				close(fd);
				map_error("Error: Allocation memory failed.", NULL, NULL);
			}
			tokens = ft_split(cleaned_line, ' ');
			free(cleaned_line);
			if (!tokens)
			{
				free(line);
				close(fd);
				map_error("Error: Allocation memory failed.", NULL, NULL);
			}
			if (tokens[0] && tokens[1])
			{
				width = ft_atoi(tokens[0]);
				height = ft_atoi(tokens[1]);
				printf("Texture XPM \"%s\" size: %dx%d\n", texture_path, width,
					height);
				if (width > 64 || height > 64)
				{
					printf("Erreur: La texture XPM \"%s\" est trop grande (%dx%d). La taille maximale autorisée est 64x64.\n",
						texture_path, width, height);
					ft_free_split(tokens);
					free(line);
					close(fd);
					exit(EXIT_FAILURE);
				}
				found_dimensions = 1;
			}
			ft_free_split(tokens);
		}
		free(line);
		if (found_dimensions)
			break ;
	}
	close(fd);
	if (!found_dimensions)
	{
		printf("Erreur: Impossible de trouver les dimensions de la texture XPM \"%s\".\n",
			texture_path);
		exit(EXIT_FAILURE);
	}
	close(fd);
	if (!found_dimensions)
	{
		printf("Erreur: Impossible de trouver les dimensions de la texture XPM \"%s\".\n",
			texture_path);
		exit(EXIT_FAILURE);
	}
}

void	validate_texture(const char *texture_path, const char *identifier)
{
	int	fd;

	if (!texture_path || *texture_path == '\0')
	{
		printf("Error: %s texture path is missing or invalid.\n", identifier);
		exit(EXIT_FAILURE);
	}
	fd = open(texture_path, O_RDONLY);
	if (fd < 0)
	{
		printf("Error: Failed to open texture file: %s\n", texture_path);
		exit(EXIT_FAILURE);
	}
	close(fd);
	printf("Texture %s: %s\n", identifier, texture_path);
	check_xpm_size(texture_path);
}

void	validate_scene_textures(t_scene *scene)
{
	validate_texture(scene->north_texture, "North");
	validate_texture(scene->south_texture, "South");
	validate_texture(scene->west_texture, "West");
	validate_texture(scene->east_texture, "East");
}