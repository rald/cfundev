#include <ncurses.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int w,h;

void center(char *s,int y) {
	int n=strlen(s);
	move(y,(w-n)/2); 
	printw("%s",s);
}

int main(void) {

	int x[6];
	int r;
	int wn[2]={-1,-1};
	int wi=0;					

	srand(time(NULL));	
			
	initscr();
	curs_set(0);
	
	getmaxyx(stdscr,h,w);
	
	
	center("press enter to continue...",7);
	refresh();
	
	getchar();
	
	clear();
	
	for(int i=0;i<6;i++) {
		x[i]=0;
		move(i,0);
		printw("%d",i+1);
		move(i,w-1);
		addch('|');
	}
	
	refresh();

	sleep(1);

	center("   ready  ",7); refresh(); sleep(1);
	center("  get set ",7); refresh(); sleep(1);
	center("    go!   ",7); refresh(); sleep(1);
	center("          ",7); refresh();
	
	do {
		do {
			r=rand()%6;
		} while(r==wn[0]);	
		move(r,x[r]);
		addch(' ');
		x[r]++;
		move(r,x[r]);
		if(x[r]>=w-1) wn[wi++]=r;
		printw("%d",r+1);
		refresh();
		napms(10);
	} while(wi<2);
	
	move(7,(w-8)/2); printf("win %d - %d",wn[0]+1,wn[1]+1); refresh();
			
	getchar();
	
	endwin();
	
	return 0;	
}