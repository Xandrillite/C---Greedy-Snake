#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <synchapi.h>
#include <windows.h>
#include <time.h>

#define PAUSE 'p'
#define RIGHT 'r'
#define LEFT 'l'
#define UP 'u'
#define DOWN 'd'
#define EXIT 'e'

typedef struct Fruit {
    COORD pos;
    int status;
} FRUIT;

typedef struct Snake {
    COORD section[500];
    int len;
    char direction;
    char last_direction;
} SNAKE;

char key_control(int *ctrl, SNAKE snake);

void set_cursor(COORD coord);

SNAKE *init_snake();

void background();

void draw_snake(SNAKE *snake);

void move(SNAKE *snake);

void judge(SNAKE snake, COORD end);

void *init_fruit();

void level_up(SNAKE *snake, FRUIT *fruit);

void create_fruit(FRUIT *fruit);

int main() {
    srand((unsigned) time(NULL));
    background();
    SNAKE *snake = init_snake();
    FRUIT *fruit = init_fruit();
    int ctrl;
    COORD end;
    end.X = 0;
    end.Y = 28;
    do {
        draw_snake(snake);
        snake->direction = key_control(&ctrl, *snake);
        if (snake->direction == EXIT) {
            return 0;
        } else if (snake->direction == PAUSE) {
            set_cursor(end);
            system("pause");
            set_cursor(end);
            printf("                                                    ");
            draw_snake(snake);
            snake->direction = snake->last_direction;
        } else if (snake->direction == '\0') {
            snake->direction = snake->last_direction;
        }
        snake->last_direction = snake->direction;
        move(snake);
        judge(*snake, end);
        level_up(snake, fruit);
        create_fruit(fruit);
    } while (1);
}

char key_control(int *ctrl, SNAKE snake) {
    int status = kbhit();
    if (status != 0) {
        *ctrl = getch();
        switch (*ctrl) {
            case 'w':
            case 'W':
            case 72:
                if (snake.direction != DOWN) return UP;
                break;
            case 's':
            case 'S':
            case 80:
                if (snake.direction != UP) return DOWN;
                break;
            case 'a':
            case 'A':
            case 75:
                if (snake.direction != RIGHT) return LEFT;
                break;
            case 'd':
            case 'D':
            case 77:
                if (snake.direction != LEFT) return RIGHT;
                break;
            case 27:
                return EXIT;
            case ' ':
                return PAUSE;
            default:;
        }
    }
    return '\0';
}

void set_cursor(COORD coord) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos;
    pos.X = (short) coord.X;
    pos.Y = (short) coord.Y;
    SetConsoleCursorPosition(handle, pos);
    CONSOLE_CURSOR_INFO *cursorInfo;
    cursorInfo = (CONSOLE_CURSOR_INFO *) malloc(sizeof(COORD));
    cursorInfo->dwSize = 1;
    cursorInfo->bVisible = 0;
    SetConsoleCursorInfo(handle, cursorInfo);
}

SNAKE *init_snake() {
    SNAKE *snake;
    snake = (SNAKE *) malloc(sizeof(SNAKE));
    snake->section[0].X = (short) 13;
    snake->section[0].Y = (short) 10;
    snake->section[1].X = (short) 11;
    snake->section[1].Y = (short) 10;
    snake->section[2].X = (short) 9;
    snake->section[2].Y = (short) 10;
    snake->section[3].X = (short) 7;
    snake->section[3].Y = (short) 10;
    snake->section[4].X = (short) 5;
    snake->section[4].Y = (short) 10;
    snake->len = 5;
    snake->direction = RIGHT;
    snake->last_direction = snake->direction;
    return snake;
}

void background() {
    system("color 7e");
    for (int _y = 0; _y < 28; ++_y) {
        printf("| |");
        for (int _x = 0; _x < 100; ++_x) {
            if (_x % 2 == 0) {
                if (_y == 0 || _y == 27 || _y == 1 || _y == 26) {
                    printf("-");
                } else {
                    printf(" ");
                }
            } else if (_x != 99) {
                printf(" ");
            }
        }
        printf("| |\n");
    }
}

void draw_snake(SNAKE *snake) {
    for (int i = 0; i < snake->len; ++i) {
        set_cursor(snake->section[i]);
        printf("*");
    }
}

void move(SNAKE *snake) {
    set_cursor(snake->section[snake->len - 1]);
    printf(" ");
    for (int i = snake->len - 1; i > 0; --i) {
        snake->section[i] = snake->section[i - 1];
    }
    switch (snake->direction) {
        case RIGHT:
            snake->section[0].X++;
            snake->section[0].X++;
            Sleep(200);
            break;
        case LEFT:
            snake->section[0].X--;
            snake->section[0].X--;
            Sleep(200);
            break;
        case UP:
            snake->section[0].Y--;
            Sleep(200);
            break;
        case DOWN:
            snake->section[0].Y++;
            Sleep(200);
            break;
        default:;
    }
}

void judge(SNAKE snake, COORD end) {
    for (int i = 1; i < snake.len; ++i) {
        if (snake.section[0].X == snake.section[i].X && snake.section[0].Y == snake.section[i].Y) {
            set_cursor(end);
            printf("YOU DEAD!\n");
            printf("Your score is %d point.", snake.len - 5);
            system("pause");
            exit(1);
        }
    }
    if (snake.section[0].X == 1 || snake.section[0].X == 103 || snake.section[0].Y == 1 || snake.section[0].Y == 26) {
        set_cursor(end);
        printf("YOU DEAD!\n");
        printf("Your score is %d point.", snake.len - 5);
        system("pause");
        exit(2);
    }
}

void *init_fruit() {
    FRUIT *fruit = (FRUIT *) malloc(sizeof(FRUIT));
    fruit->pos.X = (short) (rand() % 50 * 2 + 3);
    fruit->pos.Y = (short) (rand() % 24 + 2);
    fruit->status = 1;
    set_cursor(fruit->pos);
    printf("#");
    return fruit;
}

void level_up(SNAKE *snake, FRUIT *fruit) {
    if (snake->section[0].X == fruit->pos.X && snake->section[0].Y == fruit->pos.Y) {
        snake->len++;
        snake->section[snake->len - 1] = snake->section[snake->len - 2];
        fruit->status = 0;
    }
}

void create_fruit(FRUIT *fruit) {
    if (fruit->status == 0) {
        fruit->pos.X = (short) (rand() % 50 * 2 + 3);
        fruit->pos.Y = (short) (rand() % 24 + 2);
        fruit->status = 1;
        set_cursor(fruit->pos);
        printf("#");
    }
}