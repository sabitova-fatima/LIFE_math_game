#include <stdio.h>
// # include <opengl/mlx.h>
# include <math.h>
# include <time.h>
# include <fcntl.h>
# include <unistd.h>
#include <stdlib.h>

int     get_next_line(int fd, char **line)
{
    char    c;
    char    *buf;
    int     rv;
    int     i = 0;

    if (!(buf = (char*)malloc(10000)))
        return (-1);
    *line = buf;
    while ((rv = read(fd, &c, 1)) > 0 && c != '\n' && c != '\0')
    {
        buf[i] = c;
        i++;
    }
    buf[i] = '\0';
    if (rv == -1)
        return (-1);
    return (rv ? 1 : 0);
}

int main (int argc, char **argv)
{
    int		fd;
	int		from_read;
	char	*line;
    char    **map;
    int i = 0;

    map = malloc(100 * 100);

    fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		printf("I do not see any map file\n");

    while (from_read != -1)
	{
		from_read = get_next_line(fd, &line);
		
        map[i] = line;

		if (from_read == 0)
			break ;
        i++;
	}

    // i = 0;
    // while (map[i])
    // {
    //     printf("%s\n", map[i]);
    //     i++;
    // }

}