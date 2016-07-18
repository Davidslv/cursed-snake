#include <ncurses.h>
 
#define TICKRATE 100

#define WORLD_WIDTH 50
#define WORLD_HEIGHT 20
 
#define SNAKE_LENGTH 40

enum direction { UP, RIGHT, DOWN, LEFT };

typedef struct spart
{
  int x;
  int y;
} snake_part;

int move_snake(WINDOW *win, int direction, snake_part snake[]);

int main(int argc, char *argv[])
{

  WINDOW *world;
  int offsetx, offsety, i, ch;

  initscr();
  noecho();
  cbreak();
  timeout(TICKRATE);
  keypad(stdscr, TRUE);

  printw("Snake v. 0.1 - Press x to quit...");

  refresh();

  offsetx = (COLS - WORLD_WIDTH) / 2;
  offsety = (LINES - WORLD_HEIGHT) / 2;

  world = newwin(WORLD_HEIGHT, 
                 WORLD_WIDTH, 
                 offsety, 
                 offsetx); 

  snake_part snake[SNAKE_LENGTH];

  int sbegx = (WORLD_WIDTH - SNAKE_LENGTH) / 2;
  int sbegy = (WORLD_HEIGHT - 1) / 2;

  for (i = 0; i < SNAKE_LENGTH; i++)
  {
    snake[i].x = sbegx + i;
    snake[i].y = sbegy;
  }

  int current_direction = RIGHT;


  while((ch = getch()) != 'x')
  {
    move_snake(world, current_direction, snake);

    if (ch != ERR)
    {
      switch(ch)
      {
        case KEY_UP:
          current_direction = UP;
          break;
        case KEY_DOWN:
          current_direction = DOWN;
          break;
        case KEY_RIGHT:
          current_direction = RIGHT;
          break;
        case KEY_LEFT:
          current_direction = LEFT;
          break;
        default:
          break;
      }
    }
  }

  delwin(world);
  
  endwin();

  return 0;
}


int move_snake(WINDOW *win, int direction, snake_part snake[])
{
  wclear(win);

  for(int i = 0; i < SNAKE_LENGTH - 1; i++)
  {
    snake[i] = snake[i + 1];
    mvwaddch(win, snake[i].y, snake[i].x, '#');
  }

  int x = snake[SNAKE_LENGTH - 1].x;
  int y = snake[SNAKE_LENGTH - 1].y;

  switch(direction)
  {
    case UP:
      y - 1 == 0 ? y = WORLD_HEIGHT - 2 : y--;
      break;
    case DOWN:
      y + 1 == WORLD_HEIGHT - 1 ? y = 1 : y++;
      break;
    case RIGHT:
      x + 1 == WORLD_WIDTH - 1 ? x = 1 : x++;
      break;
    case LEFT:
      x - 1 == 0 ? x = WORLD_WIDTH - 2 : x--;
      break;
    default:
      break;
  }

  snake[SNAKE_LENGTH - 1].x = x;
  snake[SNAKE_LENGTH - 1].y = y;

  mvwaddch(win, y, x, '#');

  box(win, 0, 0);

  wrefresh(win);
 
  return 0;
}
