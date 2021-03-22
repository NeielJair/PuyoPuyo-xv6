// Single-linked List
typedef struct slist {
  void *data;
  struct slist *next;
} SList;

SList *slist_create(void);
void slist_add(SList *list, void *data);

unsigned int slist_length(SList *list);
void *slist_get(SList *list, unsigned int n);
int slist_find(SList *list, void *data);

void slist_destroy(SList *list, void (*data_destroy)(void*));
