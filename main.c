#include <stdio.h>
// # include <opengl/mlx.h>
# include <math.h>
# include <time.h>
# include <fcntl.h>
# include <unistd.h>
#include <stdlib.h>

#include <time.h>
#include <errno.h>    

/* msleep(): Sleep for the requested number of milliseconds. */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

#define GREEN(string) "\x1b[32m" string "\x1b[0m"

// в пустой (мёртвой) клетке, рядом с которой ровно три живые клетки, зарождается жизнь;
// если у живой клетки есть две или три живые соседки, то эта клетка продолжает жить; 
// в противном случае, если соседей меньше двух или больше трёх, 
// клетка умирает («от одиночества» или «от перенаселённости»)

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


void print_map_char(char **map, int dead, int alive)
{
    int i = 0;
    int j = 0;

    while(map[i])
    {
        j = 0;

        while (map[i][j])
        {
            if (map[i][j] == alive)
                printf("" GREEN("o") "");
                // printf("%c", map[i][j]);
            else    
                printf("%c", map[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

char **move(char **map, int dead, int alive)
{
    int i = 1;
    int j = 1;

    int num_lines = 0;
    int line_len = 0;
    int num_friends = 0;
    while (map[num_lines])
    {
        line_len = 0;
        while(map[num_lines][line_len])
            line_len++;
        num_lines++;
    }

    while (i < num_lines-1)
    {
        j = 1;
        while(j < line_len-1)
        {
            num_friends = count_friends(map, i, j, alive);
            if (map[i][j] == dead && num_friends == 3)
                map[i][j] = alive;
            // else if (map[i][j] == alive && (num_friends == 2 || num_friends == 3))
            //     map[i][j] = alive;
            else if (map[i][j] == alive && (num_friends < 2 || num_friends > 3))
                map[i][j] = dead;
            // printf("num_friends for %d, %d, is %d\n", i, j, num_friends);
            j++;
        }
        num_friends = count_friends(map, i, j, alive);
        // printf("num_friends for %d, %d, is %d\n", i, j, num_friends);
        i++;
    }
    return(map);
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

    print_map_char(map, dead, alive);
    printf("\n");
    sleep(1);
    printf("\e[1;1H\e[2J");

    while (1)
    {
        map = move(map, dead, alive);
        print_map_char(map, dead, alive);
        printf("\n");
        msleep(200);
        printf("\e[1;1H\e[2J"); 
    }
}
