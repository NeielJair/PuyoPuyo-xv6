typedef enum flip {
  NONE, HORIZONTAL, VERTICAL, HOR_VER
} Flip;

typedef int Color;

typedef struct bitmap {
  unsigned int width;
  unsigned int height;
  int x_center;
  int y_center;
  int *data;
} Bitmap;

/*typedef struct sprite {

}*/

//An array of bitmaps is a spritesheet
typedef struct spritesheet {
  int size;
  char **indexes;
  Bitmap **bitmaps;
} Spritesheet;

typedef struct layer {
  int x_offset;
  int y_offset;
  Flip flip;
  int visible;
  Bitmap *bitmap;
} Layer;

//Initially called Sprite, but it confused me
//Composition is a list of layers
struct slist;
typedef struct slist* Composition;

Bitmap *spritesheet_get(Spritesheet *sprsht, char *index);

Layer *layer_create(int x, int y, Flip flip, int visible, Bitmap *bitmap);
void layer_destroy(Layer *layer);

Composition composition_create(void);
void composition_make(
  Composition comp, int x, int y, Flip flip, int visible, Bitmap *bitmap
);
Layer *composition_get_layer(Composition comp, uint n);
void composition_destroy(Composition comp);

/*
  Note: in bitmaps, negative numbers are also used as 'colors':
    -1: transparent
    -2: color1
    -3: color2
    ...
*/
