#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <term.h>
#include <curses.h>

#define REFRESH_INTERVAL 0.3
#define INIT_LEN 5 // mustnt be greater than half the terminal height
#define BORDER_LOOP // should the snake loop on the border?

#define HEAD  'O'
#define TAIL  '#'
#define FRUIT 'o'

int score = 0;

struct snake {
	int x, y;
	snake* tail;

};

void nconfig(void){
	cbreak();
	noecho();
	curs_set(0);
	intrflush(stdscr, FALSE);
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);

	srand(time(NULL));
}

void endgame(void){
	endwin();
	printf("%d\n", score);
}

int main(void){
	initscr();
	nconfig();

	struct snake head;
	struct snake *curr=&head;
	for(int i = 0; i < INIT_LEN; i++){
		curr->x = LINES / 2 + i;
		curr->y = COLS / 2;
		curr->tail = (struct snake*)malloc(sizeof(struct snake));
		curr = curr->tail;
	}
	
	struct snake fruit;
	fruit.x = ((double)rand() / RAND_MAX) * LINES;
	fruit.y = ((double)rand() / RAND_MAX) * COLS;
	for(struct snake *i = &head; i; i = i->tail)
		if(i->x == fruit.x && i->y == fruit.y){
			fruit.x = ((double)rand() / RAND_MAX) * LINES;
			fruit.y = ((double)rand() / RAND_MAX) * COLS;
			i = &head;
		}

	curr = NULL;
	int x = -1, y = 0;
	while(true){
		clear();
		clock_t timer = clock();
		int inp = ERR;
		inp = getch();
		switch(inp){
			case 'q':
				endgame();
				return 0;
			break;
			case 'w':
			if(x!=1){
				x = -1;
				y =  0;
			}
			break;
			case 'a':
			if(y!=1){
				x =  0;
				y = -1;
			}
			break;
			case 's':
			if(x!=-1){
				x =  1;
				y =  0;
			}
			break;
			case 'd':
			if(y!=-1){
				x =  0;
				y =  1;
			}
			break;
		}
	
		struct snake last = head;
		struct snake *i;
		for(i = &head; i; i=i->tail){
			if(i == &head){
				i->x+=x;
				i->y+=y;
				
				#ifdef BORDER_LOOP
				if(head.x < 0) head.x = LINES - 1;
				if(head.x > LINES - 1) head.x = 0;

				if(head.y < 0) head.y = COLS - 1;
				if(head.y > COLS - 1) head.y = 0;
				#else
				if(head.x < 0 ||
				   head.x > LINES - 1 ||
				   head.y < 0 ||
				   head.y > COLS - 1){
					endgame();
					return 0;
				}
				#endif
				if(i->x == fruit.x && i->y == fruit.y){
					struct snake *j;
					for(j = &head; j->tail; j=j->tail);
					j->tail = (struct snake*)malloc(sizeof(struct snake));
					score++;
					fruit.x = ((double)rand() / RAND_MAX) * LINES;
					fruit.y = ((double)rand() / RAND_MAX) * COLS;
					for(j = &head; j; j = j->tail)
						if(j->x == fruit.x && j->y == fruit.y){
							fruit.x = ((double)rand() / RAND_MAX) * LINES;
							fruit.y = ((double)rand() / RAND_MAX) * COLS;
							j = &head;
					}
				}

			}
			else{
				struct snake holder;
				holder = *i;
				i->x = last.x;
				i->y = last.y;
				last = holder;
			}
		}

		for(i = &head; i; i=i->tail){
			if(i != &head && head.x == i->x && head.y == i->y){
				endgame();
				return 0;
			}
		}

		mvaddch(fruit.x, fruit.y, FRUIT);
		for(struct snake *i = &head; i->tail; i=i->tail){
			mvaddch(i->x, i->y, (i==&head?HEAD:TAIL));
		}
		refresh();
		while((float)(clock() - timer) / CLOCKS_PER_SEC < REFRESH_INTERVAL);
	}
	return 0;
}

