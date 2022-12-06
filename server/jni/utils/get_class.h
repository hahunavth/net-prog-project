#include <jni.h>
#include "../../lib/cgen/bkcgen.h"

typedef struct
{
  char *name;
  char *sig; // (type)
} cls_field_info;

typedef struct
{
  char *class_name;

} cls_info;