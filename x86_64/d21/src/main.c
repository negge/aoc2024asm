// cc -Os main.c -o main
// ./main < ../data/input.txt

#include <unistd.h>

char numpad[] = {
  '.', '0', 'A',
  '1', '2', '3',
  '4', '5', '6',
  '7', '8', '9'
};

char keypad[] = {
  '<', 'v', '>',
  '.', '^', 'A',
};

int find(char *pad, char c) {
  for (int i = 0; ; i++) if (pad[i] == c) return i;
}

#define X(x) ((x)%3)
#define Y(x) ((x)/3)
#define ABS(x) ((x) > 0 ? (x) : -(x))

int path(char *p, int s, int e, char *pad) {
  int m = 0;
  for (int i = 0; i < ABS(X(s) - X(e)); i++) p[m++] = X(s) > X(e) ? '<' : '>';
  for (int i = 0; i < ABS(Y(s) - Y(e)); i++) p[m++] = Y(s) > Y(e) ? 'v' : '^';
  if ((X(e) > X(s) && pad[Y(e)*3 + X(s)] != '.') || pad[Y(s)*3 + X(e)] == '.')
   for (char *h = p, *t = p + m, v; h < t; v = *h, *h++ = *--t, *t = v);
  p[m++] = 'A';
  return m;
}

long solve(char *c, int m, int n, int d, long *map) {
  if (n == d) return m;
  else {
    long ret = 0;
    char *pad = n == 0 ? numpad : keypad;
    int s = find(pad, 'A');
    for (int j = 0; j < m; j++) {
      char p[8];
      int e = find(pad, c[j]);
      int i = n*16*16 + s*16 + e;
      if (map[i]) ret += map[i];
      else ret += map[i] = solve(p, path(p, s, e, pad), n + 1, d, map);
      s = e;
    }
    return ret;
  }
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
  for (int r = 1; r; ) {
    char c[8];
    int m = 0;
    int n = 0;
    while ((r = read(0, &c[m], 1)) && c[m++] != '\n');
    if (r == 0) break;
    for (int i = 0; c[i] != 'A'; i++) n = n*10 + c[i] - '0';
    part1 += n*solve(c, m - 1, 0,  3, map1);
    part2 += n*solve(c, m - 1, 0, 26, map2);
  }
  println(part1);
  println(part2);
}
