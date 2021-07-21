#include "myConio.h"

char symbol, input, keyPress;
int x, y, firstX, firstY;
int lock;
int isRunning, isPause, direction;

#define X_RANGE 40
#define Y_RANGE 5
#define Y_BELOW_RANGE 20


void* takeInput(void* arg) {
  while (1) {
    input = getch();
    if (input == 27) break;
    switch (input) {
      case 'w':
        direction = 0;
        break;
      case 'a':
        direction = 1;
        break;
      case 's':
        direction = 2;
        break;
      case 'd':
        direction = 3;
        break;
      case '1':
        isPause = 1;
        break;
      case '2':
        isPause = 0;
        break;
    }
  }
}

void* printScreen(void* arg) {
  while (1) {
    if (input == 27) break;
    printf("%c", symbol);
    fflush(stdout);
    gotoxy(x, y);
    if (!isPause) {
      nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
      printf(" ");
      fflush(stdout);
      switch (direction) {
        case 0:
          if (y > Y_RANGE)
            --y;
          else {
            x = firstX;
            y = firstY;
          }
          break;
        case 1:
          if (x > 0)
            --x;
          else {
            x = firstX;
            y = firstY;
          }
          break;
        case 2:
          if (y < Y_BELOW_RANGE)
            ++y;
          else {
            x = firstX;
            y = firstY;
          }
          break;
        case 3:
          if (x < X_RANGE)
            ++x;
          else {
            x = firstX;
            y = firstY;
          }
          break;
      }
    }
    gotoxy(x, y);
  }
}

// nanosleep((const struct timespec[]){{0, 500000000L}}, NULL);
int main() {
  // Initialize default parameter
  direction = 3;
  isPause = 0;
  symbol = '\0';
  input = 'a';
  system("clear");

  // Get user's paramenters
  printf("Enter your symbol: ");
  symbol = getchar();
  printf("Enter x coordinate: ");
  scanf("%d", &x);
  printf("Enter y coordinate: ");
  scanf("%d", &y);

  // normalize value
  if (x < 0) x = 0;
  if (y < 4) y = 4;
  if (x > X_RANGE) x = X_RANGE;
  if (y < Y_RANGE) y = Y_RANGE;
  if (y > Y_BELOW_RANGE) y = Y_BELOW_RANGE;

  // Store initial position
  firstX = x;
  firstY = y;

  printf("\e[?25l");  // For hiding cursor blinking
  fflush(stdout);

  // Thread declaration
  pthread_t thread1, thread2;
  gotoxy(x, y);
  pthread_create(&thread1, NULL, takeInput, NULL);
  pthread_create(&thread2, NULL, printScreen, NULL);

  // Wait for all threads to exit
  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  system("clear");
  gotoxy(0, 0);
  printf("\e[?25h");  // For enabling cursor back
  return 0;
}
