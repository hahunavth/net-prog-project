#include <jni.h>
// #include "../../lib/cgen/bkcgen.h"

// typedef struct
// {
//   char *name;
//   char *sig; // (type)
// } cls_field_info;

// typedef struct
// {
//   char *class_name;
//   struct hmap *attrs;
// } cls_info;

// uint mock_hash(gtype in)
// {
//   return 1;
// }

// cls_info *create_cls(char *cls_name)
// {
//   char *_cls_name = (char *)calloc(strlen(cls_name), sizeof(char));
//   memcpy(_cls_name, cls_name, strlen(cls_name));

//   cls_info *info = (cls_info *)calloc(1, sizeof(cls_info));
//   info->class_name = cls_name;
//   info->attrs = hmap_create(mock_hash, gtype_cmp_s, NULL, NULL);

//   return info;
// }