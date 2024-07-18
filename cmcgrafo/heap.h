typedef struct heap Heap;

Heap* heap_cria (int max);
void heap_libera (Heap *h);
void heap_insere (Heap *h, int distancia, int idno);
int heap_remove (Heap *h);
void heap_debug_show (Heap *h, char* title);
void heap_corrige (Heap* h, int novadist, int idno);