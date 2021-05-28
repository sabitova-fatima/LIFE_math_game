#include <stdio.h>
// # include <opengl/mlx.h>
# include <math.h>
# include <time.h>
# include <fcntl.h>
# include <unistd.h>
#include <stdlib.h>

// в пустой (мёртвой) клетке, рядом с которой ровно три живые клетки, зарождается жизнь;
// если у живой клетки есть две или три живые соседки, то эта клетка продолжает жить; 
// в противном случае, если соседей меньше двух или больше трёх, клетка умирает («от одиночества» или «от перенаселённости»)

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

int count_friends(char **map, int i, int j, char alive)
{
    int num_friends;
    num_friends = 0;

    // if (map[i-1][j-1] && map[i-1][j-1] == alive)
    //     num_friends++;
    // if (map[i][j-1] && map[i][j-1] == alive)
    //     num_friends++;
    // if (map[i+1][j-1] && map[i+1][j-1] == alive)
    //     num_friends++;
    // if (map[i-1][j] && map[i-1][j] == alive)
    //     num_friends++;
    // if (map[i+1][j] && map[i+1][j] == alive)
    //     num_friends++;
    // if (map[i-1][j+1] && map[i-1][j+1] == alive)
    //     num_friends++;
    // if (map[i][j+1] && map[i][j+1] == alive)
    //     num_friends++;
    // if (map[i+1][j+1] && map[i+1][j+1] == alive)
    //     num_friends++;
    // return(num_friends);

    if (map[i-1][j-1] == alive)
        num_friends++;
    if (map[i][j-1] == alive)
        num_friends++;
    if (map[i+1][j-1] == alive)
        num_friends++;
    if (map[i-1][j] == alive)
        num_friends++;
    if (map[i+1][j] == alive)
        num_friends++;
    if (map[i-1][j+1] == alive)
        num_friends++;
    if (map[i][j+1] == alive)
        num_friends++;
    if (map[i+1][j+1] == alive)
        num_friends++;
    return(num_friends);
    
}

int count_lines(char **map)
{
    int i = 0;
    int j = 0;

    while (map[i])
    {
        j = 0;
        while(map[i][j])
            j++;
        i++;
    }
    return(i);
}


int count_line_len(char **map)
{
    int i = 0;
    int j = 0;

    while (map[i])
    {
        j = 0;
        while(map[i][j])
            j++;
        i++;
    }
    return(j);
}

void print_map(char **map)
{
    int i = 0;
    while(map[i])
        printf("%s\n", map[i++]);
}

int main (int argc, char **argv)
{
    int		fd;
	int		from_read;
	char	*line;
    char    **map;
    int     i;
    int     j;
    char    alive = 'o';
    char    dead = '.';

    int     num_friends;

    i = 0;
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

    int num_lines = 0;
    int line_len = 0;

    while (map[num_lines])
    {
        line_len = 0;
        while(map[num_lines][line_len])
            line_len++;
        num_lines++;
    }


    i = 1;
    j = 1;

    printf("%d\n", num_lines-1);
    printf("%d\n", line_len-1);

    while (i < num_lines-1)
    {
        j = 1;
        while(j < line_len-1)
        {
            // printf("%c", map[i][j]);
            num_friends = count_friends(map, i, j, alive);
            if(map[i][j] == dead && num_friends == 3)
                map[i][j] = alive;
            printf("num_friends for %d, %d, is %d\n", i, j, num_friends);
            j++;
        }
        // printf("\n");
        num_friends = count_friends(map, i, j, alive);
        printf("num_friends for %d, %d, is %d\n", i, j, num_friends);
        i++;
    }

    // num_friends = count_friends(map, i, j, alive);
    // printf("num_frinds for %d, %d, is %d\n", i, j, num_friends);
    
    print_map(map);
    
}
