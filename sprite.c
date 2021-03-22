#include "types.h"
#include "stat.h"
#include "user.h"
#include "slist.h"
#include "sprite.h"

Bitmap*
spritesheet_get(Spritesheet *sprsht, char *index)
{
  assert(sprsht != 0, "spritesheet_get: invalid arguments");
  for(uint i = 0; i < sprsht->size; i++)
    if(strcmp(sprsht->indexes[i], index) == 0)
      return sprsht->bitmaps[i];
  return 0;
}

Layer*
layer_create(int x, int y, Flip flip, int visible, Bitmap *bitmap)
{
  Layer *layer = malloc(sizeof(Layer));
  layer->x_offset = x;
  layer->y_offset = y;
  layer->flip = flip;
  layer->visible = visible;
  layer->bitmap = bitmap;
  return layer;
}

void
layer_destroy(Layer *layer)
{
  if(layer)
    free(layer);
}

Composition
composition_create(void)
{
  return slist_create();
}

void
composition_make(
  Composition comp, int x, int y, Flip flip, int visible, Bitmap *bitmap
) {
  assert(comp != 0, "composition_make: unexistant composition\n");

  slist_add(comp,
    layer_create(x, y, flip, visible, bitmap)
  );
}

Layer*
composition_get(Composition comp, uint n)
{
  return slist_get(comp, n);
}

void
composition_destroy(Composition comp)
{
  slist_destroy(comp, (void(*)(void*))layer_destroy);
}
