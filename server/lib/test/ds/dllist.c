#include "../../data_structures/bkcgen.h"

int main()
{
  struct gdllist *list = gdl_create_list(NULL);

  // struct gdlnode *value;

  gtype t;
  t.d = 4;
  gdl_inserta(list, 0, t);

  t.d = 5;
  gdl_inserta(list, 0, t);

  t.d = 6;
  gdl_inserta(list, 0, t);

  t.d = 1;
  gdl_inserta(list, 0, t);

  t.d = 2;
  gdl_inserta(list, 0, t);

  gdl_pprint(list, gtype_print_d);

  gdl_free(list);
  return 0;
}