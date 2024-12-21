// cc -Os main.c -o main
// ./main < ../data/input.txt

#include <unistd.h>

char numpad[] = {
  '.', '0', 'A', '.',
  '1', '2', '3', '.',
  '4', '5', '6', '.',
  '7', '8', '9', '.'
};

char keypad[] = {
  '<', 'v', '>', '.',
  '.', '^', 'A', '.',
};

int find(char *pad, char c) {
  for (int i = 0; ; i++) if (pad[i] == c) return i;
}

#define X(x) ((x) & 3)
#define Y(x) ((x) >> 2)
#define ABS(x) ((x) > 0 ? (x) : -(x))

long solve_pair(int s, int e, int n, int d, long *map);

long solve_path(int s, int e, int n, int d, long *map) {
  char *pad = n == 0 ? numpad : keypad;
  char p[8];
  int dx = X(s) - X(e);
  int dy = Y(s) - Y(e);
  char h = dx < 0 ? dx = -dx, find(keypad, '>') : find(keypad, '<');
  char v = dy < 0 ? dy = -dy, find(keypad, '^') : find(keypad, 'v');
  int cond = (X(e) > X(s) && pad[Y(e)*4 + X(s)] != '.') || pad[Y(s)*4 + X(e)] == '.';
  long ret = 0;
  s = find(keypad, 'A');
  n++;
  if (cond) {
    for (int i = 0; e = v, i < dy; s = e, i++) ret += solve_pair(s, e, n, d, map);
    for (int i = 0, e = h; i < dx; s = e, i++) ret += solve_pair(s, e, n, d, map);
  }
  else {
    for (int i = 0, e = h; i < dx; s = e, i++) ret += solve_pair(s, e, n, d, map);
    for (int i = 0; e = v, i < dy; s = e, i++) ret += solve_pair(s, e, n, d, map);
  }
  ret += solve_pair(s, find(keypad, 'A'), n, d, map);
  return ret;
}

long solve_pair(int s, int e, int n, int d, long *map) {
  int i = n*16*16 + s*16 + e;
  if (n == d) return 1;
  if (!map[i]) map[i] = solve_path(s, e, n, d, map);
  return map[i];
}

void println(long n) {
  char c[20];
  int i = 0;
  for (c[i++] = '\n'; n; c[i++] = '0' + n%10, n/=10);
  while (i--) write(1, &c[i], 1);
}

#define MAX_DEPTH (30)

long map1[MAX_DEPTH*16*16];
long map2[MAX_DEPTH*16*16];

void main() {
  long part1 = 0;
  long part2 = 0;
  int r;
  for (;;) {
    int n = 0;
    long t1 = 0;
    long t2 = 0;
    for (char e, s = find(numpad, 'A'); (r = read(0, &e, 1)) && e != '\n'; s = e) {
      if (e != 'A') n = n*10 + e - '0';
      e = find(numpad, e);
      t1 += solve_pair(s, e, 0,  3, map1);
      t2 += solve_pair(s, e, 0, 26, map2);
    }
    if (r == 0) break;
    part1 += n*t1;
    part2 += n*t2;
  }
  println(part1);
  println(part2);
}
