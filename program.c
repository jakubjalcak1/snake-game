#define _POSIX_C_SOURCE 200809L
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>

#define WIDTH  40
#define HEIGHT 20
#define MAX_LENGTH (WIDTH * HEIGHT)

void save_highscore(int score) {
    FILE *fp = fopen("highscore.txt", "w");
    if (fp == NULL){
       return;
    }
    fprintf(fp, "%d\n", score);
    fclose(fp);
}

int load_highscore() {
    FILE *fp = fopen("highscore.txt", "r");
    if (fp == NULL) {
       return EXIT_FAILURE;
    }
    int s = 0;
    fscanf(fp, "%d", &s);
    fclose(fp);
    return s;
}

int main() {
    WINDOW *win;
    int ch;
    int game_over = 0;

    initscr();
    start_color();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    srand((unsigned int) time(NULL));

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);

    int highscore = load_highscore();

    int startx = (COLS - WIDTH) / 2;
    int starty = (LINES - HEIGHT) / 2;

    win = newwin(HEIGHT, WIDTH, starty, startx);
    box(win, 0, 0);
    wrefresh(win);

    int snake_x[MAX_LENGTH];
    int snake_y[MAX_LENGTH];
    int length = 3;
    int dir_hor = 1;
    int dir_ver = 0;

    snake_x[0] = WIDTH / 2;
    snake_y[0] = HEIGHT / 2;
    snake_x[1] = snake_x[0] - 1;
    snake_y[1] = snake_y[0];
    snake_x[2] = snake_x[1] - 1;
    snake_y[2] = snake_y[1];

    int food_x;
    int food_y;
    food_x = (rand() % (WIDTH - 2)) + 1;
    food_y = (rand() % (HEIGHT - 2)) + 1;

    int score = 0;
    int delay = 120;
    int level = 1;

    while (game_over == 0){
        werase(win);
        box(win, 0, 0);

        mvprintw(starty - 2, startx, "Highscore: %d", highscore);
        mvprintw(starty - 1, startx, "Score: %d  Level: %d", score, level);
        refresh();

        wattron(win, COLOR_PAIR(1));
        mvwaddch(win, food_y, food_x, '*');
        wattroff(win, COLOR_PAIR(1));

        for (int i = 0; i < length; i++) {
           wattron(win, COLOR_PAIR(2));
            mvwaddch(win, snake_y[i], snake_x[i], i == 0 ? 'O' : 'o');
            wattroff(win, COLOR_PAIR(2));
        }

        wrefresh(win);

        ch = getch();
        switch (ch) {
            case KEY_UP:
                if (dir_ver != 1) {
                   dir_hor = 0;
                   dir_ver = -1;
                }
                break;
            case KEY_DOWN:
                if (dir_ver != -1) {
                   dir_hor = 0;
                   dir_ver = 1;
                }
                break;
            case KEY_LEFT:
                if (dir_hor != 1) {
                   dir_hor = -1;
                   dir_ver = 0;
                }
                break;
            case KEY_RIGHT:
                if (dir_hor != -1) {
                   dir_hor = 1;
                   dir_ver = 0;
                }
                break;
            case 'q': case 'Q':
                game_over = 1;
                break;
        }

        for (int i = length - 1; i > 0; i--) {
            snake_x[i] = snake_x[i - 1];
            snake_y[i] = snake_y[i - 1];
        }

        snake_x[0] += dir_hor;
        snake_y[0] += dir_ver;

        if (snake_x[0] <= 0 || snake_x[0] >= WIDTH - 1 ||
            snake_y[0] <= 0 || snake_y[0] >= HEIGHT - 1)
            game_over = 1;

        for (int i = 1; i < length; i++) {
            if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i])
                game_over = 1;
        }

        if (score >= 50 && level == 1) {
            level = 2;
        }

        if (level == 2) {
            int fx = food_x;
            int fy = food_y;
            int mdir = rand() % 4;

            if (mdir == 0 && fx > 1) fx--;
            if (mdir == 1 && fx < WIDTH - 2) fx++;
            if (mdir == 2 && fy > 1) fy--;
            if (mdir == 3 && fy < HEIGHT - 2) fy++;

            food_x = fx;
            food_y = fy;
        }

        if (snake_x[0] == food_x && snake_y[0] == food_y) {
            if (length < MAX_LENGTH)
                length++;
            score += 10;

            int valid = 0;
            while (valid == 0) {
                valid = 1;
                food_x = (rand() % (WIDTH - 2)) + 1;
                food_y = (rand() % (HEIGHT - 2)) + 1;
                for (int i = 0; i < length; i++)
                    if (snake_x[i] == food_x && snake_y[i] == food_y)
                        valid = 0;
            }

            if (delay > 40) delay -= 5;
        }

        struct timespec ts;
        ts.tv_sec = delay / 1000;
        ts.tv_nsec = (delay % 1000) * 1000000L;
        nanosleep(&ts, NULL);
    }

    if (score > highscore) {
        highscore = score;
        save_highscore(highscore);
    }

    nodelay(stdscr, FALSE);
    mvprintw(starty + HEIGHT + 1, startx, "GAME OVER! Final score: %d", score);
    mvprintw(starty + HEIGHT + 2, startx, "Najlepsie skore: %d", highscore);
    mvprintw(starty + HEIGHT + 3, startx, "Stlac ENTER pre ukoncenie...");
    refresh();

    while ((ch = getch()) != '\n' && ch != '\r');

    endwin();
    return 0;
}
