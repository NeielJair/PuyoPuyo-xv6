#include "types.h"
#include "stat.h"
#include "user.h"
#include "keyinput.h"
#include "slist.h"
#include "sprite.h"
#include "puyopuyo.h"

//region Spritesheets
Spritesheet sprsht_puyo = {
  .size = 19,
  .indexes = (char*[19]) {
    "main",
    "mainland",
    "sec",
    "secland",
    "down",
    "up",
    "updown",
    "right",
    "rightdown",
    "upright",
    "uprightdown",
    "left",
    "downleft",
    "upleft",
    "updownleft",
    "rightleft",
    "rightdownleft",
    "uprightleft",
    "uprightdownleft"
  },
  .bitmaps = (Bitmap*[19]) {
    &bmp_puyo,
    &bmp_puyo_land,
    &bmp_puyo_sec,
    &bmp_puyo_sec_land,
    &bmp_puyo_down,
    &bmp_puyo_up,
    &bmp_puyo_up_down,
    &bmp_puyo_right,
    &bmp_puyo_right_down,
    &bmp_puyo_up_right,
    &bmp_puyo_up_right_down,
    &bmp_puyo_left,
    &bmp_puyo_down_left,
    &bmp_puyo_up_left,
    &bmp_puyo_up_down_left,
    &bmp_puyo_right_left,
    &bmp_puyo_right_down_left,
    &bmp_puyo_up_right_left,
    &bmp_puyo_up_right_down_left
  }
};
Spritesheet sprsht_font = {
  .size = 37,
  .indexes = (char*[37]) {
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "mult"
  },
  .bitmaps = (Bitmap*[37]) {
    &bmp_font_0,
    &bmp_font_1,
    &bmp_font_2,
    &bmp_font_3,
    &bmp_font_4,
    &bmp_font_5,
    &bmp_font_6,
    &bmp_font_7,
    &bmp_font_8,
    &bmp_font_9,
    &bmp_font_mult
  }
};
//endregion

//region Tools
int
abs_mod(int i, int n)
{
  return (i % n + n) % n;
}

int
dir_to_hor(Dir dir)
{
  switch(dir){
  case RIGHT:
    return 1;
  case LEFT:
    return -1;
  default:
    return 0;
  }
}

int
dir_to_ver(Dir dir)
{
  switch(dir){
  case UP:
    return -1;
  case DOWN:
    return 1;
  default:
    return 0;
  }
}

Dir
dir_rotate(Dir dir, Dir rotation)
{
  Dir newDir = dir;
  if(rotation == CLOCKWISE)
    newDir++;
  else if(rotation == C_CLOCKWISE)
    newDir--;
  return newDir % 4;
}

char*
dir_to_string(Dir dir)
{
  switch(dir){
  case UP:
    return "up";
  case RIGHT:
    return "right";
  case DOWN:
    return "down";
  case LEFT:
    return "left";
  default:
    return "";
  }
}

char*
strcat(char *s1, char *s2)
{
  free(s1); //optional
  char *str = malloc(strlen(s1) + strlen(s2) + 1);
  char *ret = str;
  while(*s1)
    *str++ = *s1++;
  while(*s2)
    *str++ = *s2++;
  *str = 0;
  return ret;
}

char
random()
{
   randomState = randomState * 1664525 + 1013904223;
   return randomState >> 24;
}

int**
random_color()
{
  return palettes[abs_mod(random(), PUYO_COLOR_COUNT)];
}

int
bound(int n, int min, int max)
{
  return (n >= max) ? max :
         (n <= min) ? min :
                      n;
}

int
array_find(int *array, int size, int element)
{
  for(int i = 0; i < size; i++)
    if(array[i] == element)
      return i;
  return -1;
}

int
array_find_ptr(void **array, int size, void *element)
{
  for(int i = 0; i < size; i++)
    if((int)array[i] == (int)element)
      return i;
  return -1;
}

int
array_count(int *array, int size, int element)
{
  int ret = 0;
  for(int i = 0; i < size; i++)
    if(array[i] == element)
      ret++;
  return ret;
}

int
int_get_digits(int i)
{
  int ret = 0;
  while(i != 0){
    i = i / 10;
    ret++;
  }
  return ret;
}

char*
int_to_string(int i, int digits)
{
  int d = (digits <= 0) ? int_get_digits(i) : digits;
  char *str = malloc((d + 1)*sizeof(char));
  str += d + 1;
  *str-- = '\0';
  while(d-- > 0){
    *str-- = (i % 10) + '0';
    i = i / 10;
  }
  return str + 1;
}

int
power(int i, int n)
{
  int ret = 1;
  for(int k = 1; k <= n; k++)
    ret *= i;
  return ret;
}
//endregion

//region Input System
void
input_update(void)
{
  int prev = input;
  input = getkey();
  if(input >= 0x80)
    input = prev;
}

int
input_get(void)
{
  return input;
}

int
input_check(int key)
{
  return input == key;
}

int
input_check_released(int key)
{
  return input == key + 0x80;
}
//endregion

//region Puyo TAD
Puyo*
puyo_new(void)
{
  Puyo *puyo = malloc(sizeof(Puyo));
  puyo->i = 0;
  puyo->j = 0;
  puyo->palette = random_color();
  memset(puyo->chained, 0, 4*sizeof(Puyo*));
  puyo->bitmap = spritesheet_get(&sprsht_puyo, "main");
  return puyo;
}

Puyo*
puyo_get(int i, int j)
{
  if(i < 0 || i >= BOARD_WIDTH || j < 0 || j >= BOARD_HEIGHT)
    return 0;
  return board[i][j];
}

void
puyo_set(Puyo *puyo, int i, int j)
{
  assert(
    (puyo != 0 && i >= 0 && i < BOARD_WIDTH && j >= 0 && j < BOARD_HEIGHT),
    "puyo_set: invalid arguments\n"
  );
  puyo->i = i;
  puyo->j = j;
  board[i][j] = puyo;
}

void
puyo_register(Puyo *puyo)
{
  assert(puyo != 0, "puyo_register: invalid arguments\n");
  board[puyo->i][puyo->j] = puyo;
}

void
puyo_unregister(Puyo *puyo)
{
  assert(puyo != 0, "puyo_unregister: invalid arguments\n");
  board[puyo->i][puyo->j] = 0;
}

Puyo*
puyo_at(Puyo *puyo, Dir pos)
{
  assert(puyo != 0, "puyo_at: invalid arguments\n");

  Puyo *target;
  switch(pos){
  case UP:
    if(puyo->j == 0)
      return 0;
    target = puyo_get(puyo->i, puyo->j - 1);
    break;
  case RIGHT:
    if(puyo->i == BOARD_WIDTH - 1)
      return 0;
    target = puyo_get(puyo->i + 1, puyo->j);
    break;
  case DOWN:
    if(puyo->j == BOARD_HEIGHT - 1)
      return 0;
    target = puyo_get(puyo->i, puyo->j + 1);
    break;
  case LEFT:
    if(puyo->i == 0)
      return 0;
    target = puyo_get(puyo->i - 1, puyo->j);
    break;
  default:
    assert(0, "puyo_at: invalid arguments\n");
  }
  return target;
}

int
puyo_free_at(Puyo *puyo, Dir pos)
{
  int ret = puyo_at(puyo, pos) == 0;
  switch(pos){
  case UP:
    ret = ret && puyo->j > 0;
    break;
  case RIGHT:
    ret = ret && puyo->i < BOARD_WIDTH - 1;
    break;
  case DOWN:
    ret = ret && puyo->j < BOARD_HEIGHT - 1;
    break;
  case LEFT:
    ret = ret && puyo->i > 0;
    break;
  default:
    exit(); //Already asserted
  }
  return ret;
}

Puyo*
puyo_chained_at(Puyo *puyo, Dir dir)
{
  assert(puyo != 0, "puyo_chained_at: invalid arguments\n");
  return puyo->chained[dir];
}

int
puyo_is_chained(Puyo *puyo)
{
  assert(puyo != 0, "puyo_is_chained: invalid arguments\n");
  return puyo_chained_at(puyo, UP)    != 0
      || puyo_chained_at(puyo, RIGHT) != 0
      || puyo_chained_at(puyo, DOWN)  != 0
      || puyo_chained_at(puyo, LEFT)  != 0;
}

void
puyo_chain(Puyo *puyo)
{
  assert(puyo != 0, "puyo_chain: invalid arguments\n");
  Puyo *adjPuyo;
  for(int d = 0; d < 4; d++){
    adjPuyo = puyo_at(puyo, d);
    if(adjPuyo && puyo->palette == adjPuyo->palette)
      puyo->chained[d] = adjPuyo;
  }
}

void
puyo_unchain(Puyo *puyo)
{
  assert(puyo != 0, "puyo_unchain: invalid arguments\n");
  memset(puyo->chained, 0, 4*sizeof(Puyo*));
}

void
puyo_pop(Puyo *puyo)
{
  assert(puyo != 0, "puyo_pop: invalid arguments\n");
  puyo_unregister(puyo);
  free(puyo);
}

int
puyo_move(Puyo *puyo, Dir dir)
{
  assert(puyo != 0, "puyo_move: invalid arguments\n");
  if(!puyo_free_at(puyo, dir))
    return 0;
  switch(dir){
  case RIGHT:
    puyo_unregister(puyo);
    puyo_set(puyo, puyo->i + 1, puyo->j);
    break;
  case DOWN:
    puyo_unregister(puyo);
    puyo_set(puyo, puyo->i, puyo->j + 1);
    break;
  case LEFT:
    puyo_unregister(puyo);
    puyo_set(puyo, puyo->i - 1, puyo->j);
    break;
  default:
    return 0;
  }
  return 1;
}

void
puyo_update_bitmap(Puyo *puyo)
{
  assert(puyo != 0, "puyo_update_bitmap: invalid arguments\n");
  if(puyo == mypuyo.sec){
    puyo->bitmap = spritesheet_get(&sprsht_puyo, "sec");
    return;
  } else if(puyo == mypuyo.main || !puyo_is_chained(puyo)){
    puyo->bitmap = spritesheet_get(&sprsht_puyo, "main");
    return;
  }
  char *index = malloc(sizeof(char));
  *index = 0;
  for(int d = 0; d < 4; d++)
    if(puyo_chained_at(puyo, d)){
      index = strcat(index, dir_to_string(d));
    }
  puyo->bitmap = spritesheet_get(&sprsht_puyo, index);
  free(index);
}
//endregion

//region MyPuyo Functions
void
mypuyo_update(void)
{
  mypuyo.sec->i = mypuyo.main->i + dir_to_hor(mypuyo.rotation);
  mypuyo.sec->j = mypuyo.main->j + dir_to_ver(mypuyo.rotation);
}

void
mypuyo_reset(void)
{
  mypuyo.main = (mypuyo_next.main != 0) ? mypuyo_next.main : puyo_new();
  mypuyo.main->i = 2;
  mypuyo.sec  = (mypuyo_next.sec != 0) ? mypuyo_next.sec : puyo_new();
  mypuyo.rotation = RIGHT;

  mypuyo_next.main = puyo_new();
  mypuyo_next.sec = puyo_new();
  mypuyo_update();
  puyo_update_bitmap(mypuyo.sec);
}

int
mypuyo_move(Dir dir)
{
  Puyo *puyo = (mypuyo.rotation == dir || mypuyo.rotation == DOWN)
             ? mypuyo.sec
             : mypuyo.main;
  Puyo *otherPuyo = (puyo == mypuyo.sec) ? mypuyo.main : mypuyo.sec;
  int rot = mypuyo.rotation;
  if(!puyo_free_at(puyo, dir)
  || ((rot == LEFT || rot == RIGHT) && !puyo_free_at(otherPuyo, DOWN)))
    return 0;
  switch(dir){
  case RIGHT:
    mypuyo.main->i++;
    break;
  case DOWN:
    mypuyo.main->j++;
    break;
  case LEFT:
    mypuyo.main->i--;
    break;
  default:
    exit(); //Already asserted
  }
  mypuyo_update();
  return 1;
}

void
mypuyo_rotate(Dir rotation)
{
  Dir resultingDir = dir_rotate(mypuyo.rotation, rotation);
  if(!puyo_free_at(mypuyo.main, resultingDir)){
    mypuyo.main->i -= dir_to_hor(resultingDir);
    mypuyo.main->j -= dir_to_ver(resultingDir);
  }
  mypuyo.rotation = resultingDir;
  mypuyo_update();
}
//endregion

//region Draw Events
void
draw_puyo(Puyo *puyo)
{
  plotbitmap(
    BOARD_XPOS + puyo->i * CELL_SIZE,
    BOARD_YPOS + puyo->j * CELL_SIZE,
    puyo->bitmap,
    puyo->palette
  );
}

void
draw_mypuyo(void)
{
  draw_puyo(mypuyo.main);
  draw_puyo(mypuyo.sec);
}

void
draw_allpuyo(void)
{
  int i, j, foundPuyo;
  Puyo *puyo;
  for(j = BOARD_HEIGHT - 1; j >= 0; j--){
    foundPuyo = 0;

    for(i = 0; i < BOARD_WIDTH; i++)
      if((puyo = puyo_get(i, j))){
        foundPuyo = 1;
        puyo_update_bitmap(puyo);
        draw_puyo(puyo);
      }

    if(!foundPuyo && gameState != FALLING)
      break;
  }
}

void
draw_mypuyo_next(void)
{
  int x = BOARD_XPOS + (BOARD_WIDTH + 2) * CELL_SIZE;
  int y = BOARD_YPOS + 2 * CELL_SIZE;
  int w = 4 * bmp_puyo.width;
  int h = 3 * bmp_puyo.height;
  plotrectangle(x, y, x + w, y + h, 0);
  plotbitmap(
    x + bmp_puyo.width, y + bmp_puyo.height,
    &bmp_puyo, mypuyo_next.main->palette
  );
  plotbitmap(
    x + 2 * bmp_puyo.width, y + bmp_puyo.height,
    &bmp_puyo_sec, mypuyo_next.sec->palette
  );
}

void
draw_score(void)
{
  int x = BOARD_XPOS + (BOARD_WIDTH + 2) * CELL_SIZE;
  int y = BOARD_YPOS + 8 * CELL_SIZE;
  int w = (SCORE_MAX_DIGITS + 2) * bmp_font_0.width;
  int h = 2 * bmp_font_0.height;
  char *scorestr = int_to_string(score, SCORE_MAX_DIGITS);
  Composition scoregfx = composition_create();

  char *c = malloc(2 * sizeof(char));
  *(c + 1) = '\0';
  for(int k = 0; k < SCORE_MAX_DIGITS; k++){
    *c = *(scorestr + k);
    Bitmap *bitmap = spritesheet_get(&sprsht_font, c);
    composition_make(scoregfx, bmp_font_0.width * k, 0, NONE, 1, bitmap);
  }
  free(c);

  plotrectangle(x, y, x + w, y + h, 0);
  plotcomposition(x + bmp_font_0.width, y + h/4, &scoregfx, &plt_font_yellow);
  free(scorestr-1);
  composition_destroy(scoregfx);
}

void
clear_board(void)
{
  plotrectangle(
    BOARD_XPOS, BOARD_YPOS,
    BOARD_XPOS + CELL_SIZE*BOARD_WIDTH, BOARD_YPOS + CELL_SIZE*BOARD_HEIGHT,
    0
  );
}

void
redraw_board(void)
{
  clear_board();
  plotbitmap(BOARD_XPOS + 2 * CELL_SIZE, BOARD_YPOS, &bmp_cross, 0);
  plotbitmap(BOARD_XPOS + 3 * CELL_SIZE, BOARD_YPOS, &bmp_cross, 0);
  draw_allpuyo();
}
//endregion

//region Game Events
void
switch_state(State state)
{
  switch(gameState){
  case DROP:
    break;
  default:
    break;
  }
  gameState = state;
  timer = 0;
  switch(gameState){
  case DROP:
    chainNumber = 0;
    memset(colorsInChain, 0, PUYO_COLOR_COUNT);
    mypuyo_reset();
    draw_mypuyo_next();
    break;
  case FALLING:
    makefall_loop = 1;
    break;
  default:
    break;
  }
}

void
score_add(int points)
{
  score = bound(score + points, 0, power(10, SCORE_MAX_DIGITS + 1) - 1);
  draw_score();
}

void
drop_puyo(void)
{
  const int framesToDrop = 6;
  int shouldRegister = 0;
  switch(input_get()){
  case KEY_RIGHT:
    mypuyo_move(RIGHT);
    break;
  case KEY_LEFT:
    mypuyo_move(LEFT);
    break;
  case KEY_UP:
  case KEY_S:
    mypuyo_rotate(CLOCKWISE);
    break;
  case KEY_A:
    mypuyo_rotate(C_CLOCKWISE);
    break;
  case KEY_DOWN:
    timer = framesToDrop; //Fall instantly
    break;
  case KEY_W:
    shouldRegister = 1;
    break;
  }
  if(timer >= framesToDrop){
    timer = 0;
    if(!mypuyo_move(DOWN))
      shouldRegister = 1;
  }
  if(shouldRegister){
    puyo_register(mypuyo.main);
    puyo_register(mypuyo.sec);
    switch_state(FALLING);
  }
}

void
makefall_puyo(void)
{
  if(!makefall_loop){
    switch_state(CHAIN);
    return;
  }
  int i, j;
  Puyo *puyo;
  makefall_loop = 0;
  for(j = BOARD_HEIGHT - 1; j >= 0; j--)
    for(i = 0; i < BOARD_WIDTH; i++)
      if((puyo = puyo_get(i, j)) && puyo_move(puyo, DOWN))
        makefall_loop = 1;
}

void
connect_puyo(void)
{
  int i, j, foundPuyo;
  Puyo *puyo;
  for(j = BOARD_HEIGHT - 1; j >= 0; j--){
    foundPuyo = 0;

    for(i = 0; i < BOARD_WIDTH; i++)
      if((puyo = puyo_get(i, j))){
        foundPuyo = 1;
        puyo_chain(puyo);
        if(j == 0 && (i == 2 || i == 3))
          gameOver = &bmp_cross;
      }

    if(!foundPuyo)
      break;
  }
}

void
add_puyo_to_chain(SList *chain, Puyo *puyo)
{
  if(!puyo || slist_find(chain, puyo) >= 0)
    return;
  slist_add(chain, puyo);
  for(int d = 0; d < 4; d++){
    add_puyo_to_chain(chain, puyo_chained_at(puyo, d));
  }
}

void
chain_puyo(void)
{
  int i, j, foundPuyo, poppedPuyo;
  Puyo *puyo;

  if(timer < 3)
    return;

  poppedPuyo = 0;
  for(j = BOARD_HEIGHT - 1; j >= 0; j--){
    foundPuyo = 0;

    for(i = 0; i < BOARD_WIDTH; i++)
      if((puyo = puyo_get(i, j))){
        foundPuyo = 1;

        SList *chain = slist_create();
        add_puyo_to_chain(chain, puyo);

        int length = slist_length(chain);
        if(length >= PUYO_CHAIN_GROUP_MIN){
          poppedPuyo = 1;
          chainNumber++;
          colorsInChain[
            array_find_ptr((void**)palettes, PUYO_COLOR_COUNT, puyo->palette)
          ] = 1;

          int tableBonuses = 0;
          //Chain power
          tableBonuses += (chainNumber >= PUYO_CHAIN_POWER_MAX)
                        ? PUYO_CHAIN_POWER_TOP
                        : tbl_chain_power[chainNumber];
          //Color bonus
          tableBonuses += tbl_color_bonus[
            array_count(colorsInChain, PUYO_COLOR_COUNT, 1)
          ];
          //Group bonus
          tableBonuses += (length >= PUYO_GROUP_BONUS_MAX)
                        ? PUYO_GROUP_BONUS_TOP
                        : tbl_group_bonus[length - PUYO_CHAIN_GROUP_MIN];
          score_add(10 * length * bound(tableBonuses, 1, 999));
          slist_destroy(chain, (void(*)(void*))puyo_pop);
        } else
          slist_destroy(chain, 0);
      }

    if(!foundPuyo)
      break;
  }
  switch_state((poppedPuyo) ? FALLING : DROP);
}

void
end_game(Bitmap *bitmap)
{
  gameOver = bitmap;
  switch_state(END);
}

void
init(void)
{
  randomState = (char)uptime();
  //Init Palettes
  plt_puyo_red    = malloc(sizeof(int) * PLT_PUYO_COUNT);
  plt_puyo_green  = malloc(sizeof(int) * PLT_PUYO_COUNT);
  plt_puyo_blue   = malloc(sizeof(int) * PLT_PUYO_COUNT);
  plt_puyo_yellow = malloc(sizeof(int) * PLT_PUYO_COUNT);
  plt_font_yellow = malloc(sizeof(int) * 6);

  plt_puyo_red[0]    = 4;
  plt_puyo_green[0]  = 18;
  plt_puyo_blue[0]   = 1;
  plt_puyo_yellow[0] = 54;
  plt_puyo_purple[0] = 13;

  int excluded = random() % PUYO_TOTAL_COLOR_COUNT;
  int i = 0;
  for(int k = 0; k < PUYO_TOTAL_COLOR_COUNT; k++)
    if(k != excluded){
      int **palette = 0;
      switch(k){
      case 0:
        palette = &plt_puyo_red;
        break;
      case 1:
        palette = &plt_puyo_green;
        break;
      case 2:
        palette = &plt_puyo_blue;
        break;
      case 3:
        palette = &plt_puyo_yellow;
        break;
      case 4:
        palette = &plt_puyo_purple;
        break;
      }
      palettes[i] = palette;
      i++;
    }

  plt_font_yellow[0] = 55;
  plt_font_yellow[1] = 62;
  plt_font_yellow[2] = 54;
  plt_font_yellow[3] = 38;
  plt_font_yellow[4] = 14;
  plt_font_yellow[5] = 6;

  //Set up HUD
  plotrectangle(0, 0, 320, 200, 7);
  redraw_board();
  draw_score();

  switch_state(DROP);
}

void
update(void)
{
  switch(gameState){
  case DROP:
    drop_puyo();
    break;
  case FALLING:
    makefall_puyo();
    break;
  case CHAIN:
    connect_puyo();
    chain_puyo();
    break;
  default:
    break;
  }
}

void
draw(void)
{
  redraw_board();
  if(gameState == DROP)
    draw_mypuyo();
}
//endregion

int
main(void)
{
  modeswitch(1);

  init();

  uint prev_t, t;
  prev_t = 0;
  for(t = uptime();; t = uptime()){
    input_update();
    if(t >= prev_t + TICKS_PER_FRAME){
      prev_t = t;
      timer++;
      update();
      draw();
      if(input == KEY_ESCAPE)
        gameOver = &bmp_puyo_up_right_down_left;
      input = 0;
    }
    if(gameOver)
      break;
  }

  //Finish
  for(t = uptime();; t = uptime()){
    if(t >= prev_t + 1){
      prev_t = t;
      static int x_pos = 0;
      static int y_pos = 0;
      plotbitmap(
        x_pos * bmp_cross.width, y_pos * bmp_cross.height,
        gameOver, &plt_puyo_red
      );
      x_pos++;
      if(x_pos >= (int)320/bmp_cross.width){
        x_pos = 0;
        y_pos++;
      }
      if(y_pos >= 200/bmp_cross.height + 1)
        exit();
    }
  }

  exit();
}
