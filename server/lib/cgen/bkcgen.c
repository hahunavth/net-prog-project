/* (C) Nguyen Ba Ngoc 2022 */
/* Version: 1.1.6 */

#include "bkcgen.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***********************************
 * base/hashes.c
 ***********************************/

const int prime_mod [] =
{
  1,          /* 1 << 0 */
  2,
  3,
  7,
  13,
  31,
  61,
  127,
  251,
  509,
  1021,
  2039,
  4093,
  8191,
  16381,
  32749,
  65521,      /* 1 << 16 */
  131071,
  262139,
  524287,
  1048573,
  2097143,
  4194301,
  8388593,
  16777213,
  33554393,
  67108859,
  134217689,
  268435399,
  536870909,
  1073741789,
  2147483647  /* 1 << 31 */
};

/***********************************
 * algo/sort.c
 ***********************************/

void selsort(long n, gtype *a, gtype_cmp_t cmp) {
  for (int i = 0; i < n - 1; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (cmp(a[i], a[j]) > 0) {
        gtype_swap(a[i], a[j]);
      }
    }
  }
}

void insort(long n, gtype *a, gtype_cmp_t cmp) {
  // a[0] == min
  for (int i = n - 1; i > 0; --i) {
    if (cmp(a[i - 1], a[i]) > 0) {
      gtype_swap(a[i - 1], a[i]);
    }
  }
  for (int i = 2; i < n; ++i) {
    int j = i;
    gtype v = a[i];
    while (cmp(a[j - 1], v) > 0) {
      a[j] = a[j - 1];
      --j;
    }
    a[j] = v;
  }
}

void bubsort(long n, gtype *a, gtype_cmp_t cmp) {
  for (int i = 0; i < n - 1; ++i) {
    for (int j = n - 1; j > i; --j) {
      if (cmp(a[j - 1], a[j]) > 0) {
        gtype_swap(a[j - 1], a[j]);
      }
    }
  }
}

void q2sort(long n, gtype *a, gtype_cmp_t cmp) {
  if (n <= 1) {
    return;
  }
  gtype *left = a, *right = a + n - 2;
  gtype v = a[n - 1];
  for (;;) {
    while (cmp(v, *left) > 0) {
      ++left;
    }
    while (cmp(*right, v) > 0) {
      if (right == a) {
        break;
      }
      --right;
    }
    if (left >= right) {
      break;
    }
    gtype_swap(*left, *right);
    ++left;
    --right;
  }
  gtype_swap(*left, a[n - 1]);
  right = left + 1;
  left = left - 1;
  q2sort(left - a + 1, a, cmp);
  q2sort(a + n - right, right, cmp);
}

void q2m3sort(long n, gtype *a, gtype_cmp_t cmp) {
  if (n <= 1) {
    return;
  }
  gtype *left = a, *right = a + n - 1,
        *mid = left + ((right - left) >> 1);
  if (cmp(*left, *mid) > 0) {
    gtype_swap(*left, *mid);
  }
  if (cmp(*mid, *right) > 0) {
    gtype_swap(*mid, *right);
    if (cmp(*left, *mid) > 0) {
      gtype_swap(*left, *mid);
    }
  }
  ++left;
  --right;
  gtype v = *mid;
  do {
    while (cmp(v, *left) > 0) {
      ++left;
    }
    while (cmp(*right, v) > 0) {
      --right;
    }
    if (left < right) {
      gtype_swap(*left, *right);
      ++left;
      --right;
    } else if (left == right) {
      ++left;
      --right;
    }
  } while (left <= right);
  q2m3sort(right - a + 1, a, cmp);
  q2m3sort(a + n - left, left, cmp);
}

void q3sort(long n, gtype *a, gtype_cmp_t cmp) {
  if (n <= 1) {
    return;
  }
  gtype *left = a, *right = a + n - 2;
  gtype v = a[n - 1];
  gtype *p = a, *q = a + n - 2;
  for (;;) {
    while (cmp(v, *left) > 0) {
      ++left;
    }
    while (cmp(*right, v) > 0) {
      if (right == a) {
        break;
      }
      --right;
    }
    if (left >= right) {
      break;
    }
    gtype_swap(*left, *right);
    if (cmp(*left, v) == 0) {
      gtype_swap(*p, *left);
      ++p;
    }
    if (cmp(*right, v) == 0) {
      gtype_swap(*q, *right);
      --q;
    }
    ++left;
    --right;
  }
  gtype_swap(*left, a[n - 1]);
  right = left + 1;
  left = left - 1;
  for (gtype *k = a; k < p; ++k, --left) {
    gtype_swap(*k, *left);
  }
  for (gtype *k = a + n - 2; k > q; --k, ++right) {
    gtype_swap(*k, *right);
  }
  q3sort(left - a + 1, a, cmp);
  q3sort(a + n - right, right, cmp);
}

/***********************************
 * algo/qinsort.c
 ***********************************/

#define Q2M3_THRESH 8

static void _q2m3thresh(long n, gtype *a, gtype_cmp_t cmp) {
  if (n <= Q2M3_THRESH) {
    return;
  }
  gtype *left = a, *right = a + n - 1,
        *mid = left + ((right - left) >> 1);
  if (cmp(*left, *mid) > 0) {
    gtype_swap(*left, *mid);
  }
  if (cmp(*mid, *right) > 0) {
    gtype_swap(*mid, *right);
    if (cmp(*left, *mid) > 0) {
      gtype_swap(*left, *mid);
    }
  }
  ++left;
  --right;
  gtype v = *mid;
  do {
    while (cmp(v, *left) > 0) {
      ++left;
    }
    while (cmp(*right, v) > 0) {
      --right;
    }
    if (left < right) {
      gtype_swap(*left, *right);
      ++left;
      --right;
    } else if (left == right) {
      ++left;
      --right;
    }
  } while (left <= right);
  _q2m3thresh(right - a + 1, a, cmp);
  _q2m3thresh(a + n - left, left, cmp);
}

void qinsort(long n, gtype *a, gtype_cmp_t cmp) {
  _q2m3thresh(n, a, cmp);
  insort(n, a, cmp);
}

#undef Q2M3_THRESH

/***********************************
 * algo/quicksort.c
 ***********************************/

/* Giá trị bí ẩn, ngưỡng chuyển sang xếp chèn */
#define QUICKSORT_THRESH 10

struct stack_node {
  gtype *lo;
  gtype *hi;
};

#define QS_STACK_SIZE  (CHAR_BIT * sizeof(size_t))
#define QS_PUSH(low, high)  ((void) ((top->lo = (low)), (top->hi = (high)), ++top))
#define QS_POP(low, high)  ((void) (--top, (low = top->lo), (high = top->hi)))
#define QS_STACK_NOT_EMPTY  (stack < top)

void quicksort(long n, gtype *a, gtype_cmp_t cmp) {
  if (n <= 0) {
    return;
  }
  if (n > QUICKSORT_THRESH) {
    gtype *lo = a;
    gtype *hi = a + n - 1;
    struct stack_node stack[QS_STACK_SIZE];
    struct stack_node *top = stack;
    QS_PUSH(NULL, NULL);

    while (QS_STACK_NOT_EMPTY) {
      gtype *left_ptr;
      gtype *right_ptr;

      /* Tìm số trung vị của LO, MID, và HI */
      gtype *mid = lo + ((hi - lo) >> 1);

      if (cmp(*mid, *lo) < 0) {
        gtype_swap(*mid, *lo);
      }
      if (cmp(*hi, *mid) < 0) {
        gtype_swap(*hi, *mid);
        if (cmp(*mid, *lo) < 0) {
          gtype_swap(*mid, *lo);
        }
      }
      left_ptr = lo + 1;
      right_ptr = hi - 1;
      gtype v = *mid;
      do {
        while (cmp(v, *left_ptr) > 0) ++left_ptr;
        while (cmp(*right_ptr, v) > 0) --right_ptr;
        if (left_ptr < right_ptr) {
          gtype_swap(*left_ptr, *right_ptr);
          ++left_ptr;
          --right_ptr;
        } else if (left_ptr == right_ptr) {
          ++left_ptr;
          --right_ptr;
          break;
        }
      } while (left_ptr <= right_ptr);

      if (right_ptr - lo <= QUICKSORT_THRESH) {
        if (hi - left_ptr <= QUICKSORT_THRESH) {
          QS_POP(lo, hi);
        } else {
          lo = left_ptr;
        }
      } else if (hi - left_ptr <= QUICKSORT_THRESH) {
        hi = right_ptr;
      } else if (right_ptr - lo > hi - left_ptr) {
        QS_PUSH(lo, right_ptr);
        lo = left_ptr;
      } else {
        QS_PUSH(left_ptr, hi);
        hi = right_ptr;
      }
    }
  }

  // Sắp xếp chèn
  {
    long thresh = n - 1;
    if (thresh > QUICKSORT_THRESH) {
      thresh = QUICKSORT_THRESH;
    }
    long i, j = 0;
    for (i = 1; i <= thresh; ++i) {
      if (cmp(a[i], a[j]) < 0) {
        j = i;
      }
    }
    if (j != 0) {
      gtype_swap(a[0], a[j]);
    }
    for (i = 2; i < n; ++i) {
      gtype c = a[i];
      j = i;
      while (cmp(a[j - 1], c) > 0) {
        a[j] = a[j - 1];
        --j;
      }
      a[j] = c;
    }
  }
}

#undef QUICKSORT_THRESH
#undef QS_STACK_SIZE
#undef QS_PUSH
#undef QS_POP
#undef QS_STACK_NOT_EMPTY

/***********************************
 * algo/heapsort.c
 ***********************************/

void heapsort(long n, gtype *a, gtype_cmp_t cmp) {
  heap_make(n, a, cmp);
  for (long i = n - 1; i > 0; --i) {
    gtype_swap(a[0], a[i]);
    heap_shift_down(0, i, a, cmp);
  }
}

/***********************************
 * vec/gvec.c
 ***********************************/

struct gvector *gvec_create(long n, gtype_free_t free_value) {
  struct gvector *v = malloc(sizeof(struct gvector));
  v->free_value = free_value;
  v->sz = n;
  v->cap = n;

  /* Mặc định x 2 dung lượng */
  v->scale = 2.0;
  if (n == 0) {
    v->elems = NULL;
    return v;
  }
  v->elems = calloc(n, sizeof(gtype));
  return v;
}

struct gvector *gvec_create_full(long size, long cap, gtype value,
      gtype_free_t free_value) {
  if (size > cap) {
    return NULL;
  }
  struct gvector *v = gvec_create(size, free_value);
  if (cap > size) {
    gvec_reserve(v, cap);
  }
  gvec_fill(v, value);
  return v;
}

struct gvector *gvec_clone(struct gvector *v) {
  struct gvector *v2 = malloc(sizeof(struct gvector));
  memcpy(v2, v, sizeof(struct gvector));
  if (v->cap == 0) {
    v2->elems = NULL;
    return v2;
  }
  size_t elems_size = v2->cap * sizeof(gtype);
  v2->elems = malloc(elems_size);
  memcpy(v2->elems, v->elems, elems_size);
  return v2;
}

int gvec_identical(struct gvector *v1, struct gvector *v2) {
  if (v1->sz != v2->sz || v1->cap != v2->cap) {
    return 0;
  }
  size_t elems_size = v1->cap * sizeof(gtype);
  char *p = v1->elems, *q = v2->elems;
  for (size_t i = 0; i < elems_size; ++i) {
    if (p[i] != q[i]) {
      return 0;
    }
  }
  return 1;
}

void gtype_free_gvec(gtype value) {
  gvec_free(value.gvec);
}

void gvec_pprint(struct gvector *v, gtype_print_t pp) {
  gvec_traverse(cur, v) {
    pp(*cur);
  }
}

struct gvector *gvec_rand_l(long n) {
  struct gvector *v = gvec_create(n, NULL);
  gvec_traverse(cur, v) {
    cur->l = rand();
  }
  return v;
}

/***********************************
 * vec/heap.c
 ***********************************/

/**
 * Triển khai các hàm vun đống
 */

void heap_shift_down(long i, long n, gtype *a, gtype_cmp_t cmp) {
  for (;;) {
    long lc = HLEFT(i), rc = HRIGHT(i), root = i;
    if (lc < n && cmp(a[root], a[lc]) < 0) {
      root = lc;
    }
    if (rc < n && cmp(a[root], a[rc]) < 0) {
      root = rc;
    }
    if (root == i) {
      break;
    }
    gtype_swap(a[i], a[root]);
    i = root;
  }
}

void heap_shift_up(long i, gtype *a, gtype_cmp_t cmp) {
  long j = HTOP(i);
  while (i > 0 && cmp(a[j], a[i]) < 0) {
    gtype_swap(a[i], a[j]);
    i = j;
    j = HTOP(i);
  }
}

void heap_make(long n, gtype *a, gtype_cmp_t cmp) {
  for (long i = n / 2; i >= 0; --i) {
    heap_shift_down(i, n, a, cmp);
  }
}

/***********************************
 * vec/hmap.c
 ***********************************/

static void hmap_setup_storage(struct hmap *tab);
static inline int hmap_lookup_node(struct hmap *tab, gtype key, uint *hash_return);
static inline int hmap_maybe_realloc(struct hmap *tab);
static void hmap_remove_node(struct hmap *tab, int i);

struct hmap *hmap_create(gtype_hash_t hash_func, gtype_cmp_t cmp,
          gtype_free_t free_key, gtype_free_t free_value) {
  struct hmap *tab = malloc(sizeof(struct hmap));
  tab->size = 0;
  tab->noccupied = 0;
  tab->hash_func = hash_func;
  tab->cmp = cmp;
  tab->free_key = free_key;
  tab->free_value = free_value;
  hmap_setup_storage(tab);
  return tab;
}

static void hmap_set_shift(struct hmap *tab, int shift) {
  tab->capacity = 1 << shift;
  tab->mod = prime_mod[shift];
  tab->mask = tab->capacity - 1;
}

static void hmap_setup_storage(struct hmap *tab) {
  hmap_set_shift(tab, HASH_MIN_SHIFT);
  tab->hashes = arr_create(tab->capacity, uint);
  memset(tab->hashes, 0, tab->capacity * sizeof(uint));
  tab->keys = arr_create(tab->capacity, gtype);
  tab->values = arr_create(tab->capacity, gtype);
}

void gtype_free_hmap(gtype value) {
  hmap_free(value.hmap);
}

gtype *hmap_value(struct hmap *tab, gtype key) {
  int node_index = hmap_lookup_node(tab, key, NULL);
  return (HASH_IS_REAL(tab->hashes[node_index]))?
          tab->values + node_index: NULL;
}

struct hmap_ires hmap_insert(struct hmap *tab, gtype key, gtype value) {
  uint key_hash;
  int node_index = hmap_lookup_node(tab, key, &key_hash);
  uint curr_hash = tab->hashes[node_index];
  int already_exists = HASH_IS_REAL(curr_hash);
  if (already_exists) {
    return (struct hmap_ires){tab->values + node_index, 0};
  }
  tab->hashes[node_index] = key_hash;
  tab->keys[node_index] = key;
  tab->values[node_index] = value;
  tab->size++;
  if (HASH_IS_UNUSED(curr_hash)) {
    tab->noccupied++;
    if (hmap_maybe_realloc(tab) == 1) {
      node_index = hmap_lookup_node(tab, key, NULL);
    }
  }
  return (struct hmap_ires){tab->values + node_index, 1};
}

gtype *hmap_put(struct hmap *tab, gtype key, gtype value) {
  uint key_hash;
  int node_index = hmap_lookup_node(tab, key, &key_hash);
  uint curr_hash = tab->hashes[node_index];
  int already_exists = HASH_IS_REAL(curr_hash);
  if (already_exists) {
    return tab->values + node_index;
  }
  tab->hashes[node_index] = key_hash;
  tab->keys[node_index] = key;
  tab->values[node_index] = value;
  tab->size++;
  if (HASH_IS_UNUSED(curr_hash)) {
    tab->noccupied++;
    hmap_maybe_realloc(tab);
  }
  return NULL;
}

int hmap_remove(struct hmap *tab, gtype key) {
  int node_index = hmap_lookup_node(tab, key, NULL);
  if (HASH_IS_NOTREAL(tab->hashes[node_index])) {
    return 0;
  }
  hmap_remove_node(tab, node_index);
  hmap_maybe_realloc(tab);
  return 1;
}

void hmap_clear(struct hmap *tab) {
  int capacity = (tab)->capacity;
  for (int i = 0; i < capacity; ++i) {
    if (HASH_IS_REAL(tab->hashes[i])) {
      if (tab->free_key) {
        tab->free_key(tab->keys[i]);
      }
      if (tab->free_value) {
        tab->free_value(tab->values[i]);
      }
    }
  }
  tab->size = 0;
  tab->noccupied = 0;
  arr_resize(tab->keys, 0);
  arr_resize(tab->values, 0);
  arr_resize(tab->hashes, 0);
  memset(tab->hashes, 0, tab->capacity * sizeof(uint));
  hmap_maybe_realloc(tab);
}

gtype *hmap_next_pkey(struct hmap *map, gtype* curr) {
  gtype * r;
  hashes_next_pkey_or_pvalue(map, curr, keys, r);
  return r;
}

gtype *hmap_next_pvalue(struct hmap *map, gtype* curr) {
  gtype * r;
  hashes_next_pkey_or_pvalue(map, curr, values, r);
  return r;
}

static int hmap_find_closest_shift(int n) {
  int i = 0;
  for (; n; ++i) {
    n >>= 1;
  }
  return i;
}

static void hmap_set_shift_from_capacity(struct hmap *tab, int capacity) {
  int shift = hmap_find_closest_shift(capacity);
  shift = MAX(shift, HASH_MIN_SHIFT);
  hmap_set_shift(tab, shift);
}

#define hmap_hash_to_index(tab, hash)((hash * 11) % tab->mod)

static inline int hmap_lookup_node(struct hmap *tab, gtype key, uint *hash_return) {
  uint lookup_hash = tab->hash_func(key);
  if (HASH_IS_NOTREAL(lookup_hash)) {
    lookup_hash = MIN_HASH_VALUE;
  }
  if (hash_return) {
    *hash_return = lookup_hash;
  }
  int node_index = hmap_hash_to_index(tab, lookup_hash);
  uint node_hash = tab->hashes[node_index];
  int first_deleted = -1;
  int step = 0;
  while (HASH_IS_INUSED(node_hash)) {
    if (node_hash == lookup_hash) {
      gtype node_key = tab->keys[node_index];
      if (tab->cmp(node_key, key) == 0) {
        return node_index;
      }
    } else if (HASH_IS_DELETED(node_hash) && first_deleted < 0) {
      first_deleted = node_index;
    }
    ++step;
    node_index += step;
    node_index &= tab->mask;
    node_hash = tab->hashes[node_index];
  }
  if (first_deleted >= 0) {
    return first_deleted;
  }
  return node_index;
}

static void hmap_remove_node(struct hmap *tab, int i) {
  gtype key = tab->keys[i], value = tab->values[i];
  tab->hashes[i] = DELETED_HASH_VALUE;
  tab->size--;
  if (tab->free_key) {
    tab->free_key(key);
  }
  if (tab->free_value) {
    tab->free_value(value);
  }
}

static void hmap_realloc_arrays(struct hmap *tab) {
  arr_reserve(tab->hashes, tab->capacity);
  arr_reserve(tab->keys, tab->capacity);
  arr_reserve(tab->values, tab->capacity);
}

static void relocate_map(struct hmap *tab, uint old_capacity, uint32 *reallocated_buckets_bitmap) {
  for (int i = 0; i < old_capacity; ++i) {
    uint node_hash = tab->hashes[i];
    gtype key, value;
    if (HASH_IS_NOTREAL(node_hash)) {
      tab->hashes[i] = UNUSED_HASH_VALUE;
      continue;
    }
    if (get_status_bit(reallocated_buckets_bitmap, i)) {
      continue;
    }
    tab->hashes[i] = UNUSED_HASH_VALUE;
    key = tab->keys[i];
    value = tab->values[i];
    for (;;) {
      uint idx, replaced_hash, step = 0;
      idx = hmap_hash_to_index(tab, node_hash);
      while (get_status_bit(reallocated_buckets_bitmap, idx)) {
        ++step;
        idx += step;
        idx &= tab->mask;
      }
      set_status_bit(reallocated_buckets_bitmap, idx);
      replaced_hash = tab->hashes[idx];
      tab->hashes[idx] = node_hash;
      if (HASH_IS_NOTREAL(replaced_hash)) {
        tab->keys[idx] = key;
        tab->values[idx] = value;
        break;
      }
      node_hash = replaced_hash;
      evict_key_or_value(tab->keys, idx, key, key);
      evict_key_or_value(tab->values, idx, value, value);
    }
  }
}

static void hmap_realloc(struct hmap *tab) {
  uint32 *reallocated_buckets_bitmap;
  int old_capacity;
  old_capacity = tab->capacity;
  hmap_set_shift_from_capacity(tab, tab->size * 1.333);
  if (tab->capacity > old_capacity) {
    hmap_realloc_arrays(tab);
    memset(tab->hashes + old_capacity, 0, (tab->capacity - old_capacity) * sizeof(uint));
    reallocated_buckets_bitmap = calloc((tab->capacity + 31) / 32, sizeof(uint32));
  } else {
    reallocated_buckets_bitmap = calloc((old_capacity + 31) / 32, sizeof(uint32));
  }

  relocate_map(tab, old_capacity, reallocated_buckets_bitmap);
  free(reallocated_buckets_bitmap);
  if (tab->capacity < old_capacity) {
    hmap_realloc_arrays(tab);
  }
  tab->noccupied = tab->size;
}

static inline int hmap_maybe_realloc(struct hmap *tab) {
  uint noccupied = tab->noccupied, capacity = tab->capacity;
  if ((capacity > tab->size * 4 && capacity > 1 << HASH_MIN_SHIFT) ||
      (capacity <= noccupied + (noccupied/16))) {
    hmap_realloc(tab);
    return 1;
  }
  return 0;
}

/***********************************
 * vec/hset.c
 ***********************************/

static void hset_setup_storage(struct hset *hs);
static inline int hset_lookup_node(struct hset *hs, gtype key, uint *hash_return);
static inline int hset_maybe_realloc(struct hset *hs);
static void hset_remove_node(struct hset *hs, int i);
static void hset_free_nodes(struct hset *hs);

struct hset *hset_create(gtype_hash_t hash_func, gtype_cmp_t cmp,
          gtype_free_t free_key) {
  struct hset *hs = malloc(sizeof(struct hset));
  hs->size = 0;
  hs->noccupied = 0;
  hs->hash_func = hash_func;
  hs->cmp = cmp;
  hs->free_key = free_key;
  hset_setup_storage(hs);
  return hs;
}

static void hset_set_shift(struct hset *hs, int shift) {
  hs->capacity = 1 << shift;
  hs->mod = prime_mod[shift];
  hs->mask = hs->capacity - 1;
}

static void hset_setup_storage(struct hset *hs) {
  hset_set_shift(hs, HASH_MIN_SHIFT);
  hs->hashes = arr_create(hs->capacity, uint);
  memset(hs->hashes, 0, hs->capacity * sizeof(uint));
  hs->keys = arr_create(hs->capacity, gtype);
}

int hset_index_of(struct hset *hs, gtype key) {
  int node_index = hset_lookup_node(hs, key, NULL);
  return (HASH_IS_REAL(hs->hashes[node_index]))?
          node_index: INDEX_NOT_FOUND;
}

int hset_insert(struct hset *hs, gtype key) {
  uint key_hash;
  int node_index = hset_lookup_node(hs, key, &key_hash);
  uint curr_hash = hs->hashes[node_index];
  int already_exists = HASH_IS_REAL(curr_hash);
  if (already_exists) {
    return 0;
  }
  hs->hashes[node_index] = key_hash;
  hs->keys[node_index] = key;
  hs->size++;
  if (HASH_IS_UNUSED(curr_hash)) {
    hs->noccupied++;
    hset_maybe_realloc(hs);
  }
  return 1;
}

int hset_remove(struct hset *hs, gtype key) {
  int node_index = hset_lookup_node(hs, key, NULL);
  if (HASH_IS_NOTREAL(hs->hashes[node_index])) {
    return 0;
  }
  hset_remove_node(hs, node_index);
  hset_maybe_realloc(hs);
  return 1;
}

void hset_clear(struct hset *hs) {
  hset_free_nodes(hs);
  hset_setup_storage(hs);
}

void hset_free(struct hset *hs) {
  hset_free_nodes(hs);
  free(hs);
}

void gtype_free_hset(gtype value) {
  hset_free(value.hset);
}

gtype *hset_next_pkey(struct hset *hs, gtype* curr) {
  gtype * r;
  hashes_next_pkey_or_pvalue(hs, curr, keys, r);
  return r;
}

static int hset_find_closest_shift(int n) {
  int i = 0;
  for (; n; ++i) {
    n >>= 1;
  }
  return i;
}

static void hset_set_shift_from_capacity(struct hset *hs, int capacity) {
  int shift = hset_find_closest_shift(capacity);
  shift = MAX(shift, HASH_MIN_SHIFT);
  hset_set_shift(hs, shift);
}

#define hset_hash_to_index(hs, hash)((hash * 11) % hs->mod)

static inline int hset_lookup_node(struct hset *hs, gtype key, uint *hash_return) {
  uint lookup_hash = hs->hash_func(key);
  if (HASH_IS_NOTREAL(lookup_hash)) {
    lookup_hash = MIN_HASH_VALUE;
  }
  if (hash_return) {
    *hash_return = lookup_hash;
  }
  int node_index = hset_hash_to_index(hs, lookup_hash);
  uint node_hash = hs->hashes[node_index];
  int first_deleted = -1;
  int step = 0;
  while (HASH_IS_INUSED(node_hash)) {
    if (node_hash == lookup_hash) {
      gtype node_key = hs->keys[node_index];
      if (hs->cmp(node_key, key) == 0) {
        return node_index;
      }
    } else if (HASH_IS_DELETED(node_hash) && first_deleted < 0) {
      first_deleted = node_index;
    }
    ++step;
    node_index += step;
    node_index &= hs->mask;
    node_hash = hs->hashes[node_index];
  }
  if (first_deleted >= 0) {
    return first_deleted;
  }
  return node_index;
}

static void hset_remove_node(struct hset *hs, int i) {
  gtype key = hs->keys[i];
  hs->hashes[i] = DELETED_HASH_VALUE;
  hs->size--;
  if (hs->free_key) {
    hs->free_key(key);
  }
}

static void hset_free_nodes(struct hset *hs) {
  int capacity = hs->capacity;
  for (int i = 0; i < capacity; ++i) {
    if (HASH_IS_REAL(hs->hashes[i])) {
      if (hs->free_key) {
        hs->free_key(hs->keys[i]);
      }
    }
  }
  arr_free(hs->keys);
  arr_free(hs->hashes);

  hs->size = 0;
  hs->noccupied = 0;
}

static void hset_realloc_arrays(struct hset *hs) {
  arr_reserve(hs->hashes, hs->capacity);
  arr_reserve(hs->keys, hs->capacity);
}

static void relocate_set(struct hset *hs, uint old_capacity, uint32 *reallocated_buckets_bitmap) {
  for (int i = 0; i < old_capacity; ++i) {
    uint node_hash = hs->hashes[i];
    gtype key;
    if (HASH_IS_NOTREAL(node_hash)) {
      hs->hashes[i] = UNUSED_HASH_VALUE;
      continue;
    }
    if (get_status_bit(reallocated_buckets_bitmap, i)) {
      continue;
    }
    hs->hashes[i] = UNUSED_HASH_VALUE;
    key = hs->keys[i];
    for (;;) {
      uint idx, replaced_hash, step = 0;
      idx = hset_hash_to_index(hs, node_hash);
      while (get_status_bit(reallocated_buckets_bitmap, idx)) {
        ++step;
        idx += step;
        idx &= hs->mask;
      }
      set_status_bit(reallocated_buckets_bitmap, idx);
      replaced_hash = hs->hashes[idx];
      hs->hashes[idx] = node_hash;
      if (HASH_IS_NOTREAL(replaced_hash)) {
        hs->keys[idx] = key;
        break;
      }
      node_hash = replaced_hash;
      evict_key_or_value(hs->keys, idx, key, key);
    }
  }
}

static void hset_realloc(struct hset *hs) {
  uint32 *reallocated_buckets_bitmap;
  int old_capacity;
  old_capacity = hs->capacity;
  hset_set_shift_from_capacity(hs, hs->size * 1.333);
  if (hs->capacity > old_capacity) {
    hset_realloc_arrays(hs);
    memset(hs->hashes + old_capacity, 0, (hs->capacity - old_capacity) * sizeof(uint));
    reallocated_buckets_bitmap = calloc((hs->capacity + 31) / 32, sizeof(uint32));
  } else {
    reallocated_buckets_bitmap = calloc((old_capacity + 31) / 32, sizeof(uint32));
  }

  relocate_set(hs, old_capacity, reallocated_buckets_bitmap);
  free(reallocated_buckets_bitmap);
  if (hs->capacity < old_capacity) {
    hset_realloc_arrays(hs);
  }
  hs->noccupied = hs->size;
}

static inline int hset_maybe_realloc(struct hset *hs) {
  uint noccupied = hs->noccupied, capacity = hs->capacity;
  if ((capacity > hs->size * 4 && capacity > 1 << HASH_MIN_SHIFT) ||
      (capacity <= noccupied + (noccupied/16))) {
    hset_realloc(hs);
    return 1;
  }
  return 0;
}

/***********************************
 * vec/p1w.c
 ***********************************/

/**
 * Triển khai hàng đợi ưu tiên
 */

struct p1way *p1w_create(gtype_cmp_t cmp) {
  struct p1way *h = malloc(sizeof(struct p1way));
  h->elems = arr_create(0, gtype);
  h->cmp = cmp;
  return h;
}

gtype p1w_peek(struct p1way *h) {
  return p1w_arr(h)[0];
}

gtype p1w_dequeue(struct p1way *h) {
  long sz = p1w_size(h);
  gtype *a = p1w_arr(h);
  gtype tmp = a[0];
  gtype_swap(a[0], a[sz - 1]);
  arr_resize(h->elems, sz - 1);
  heap_shift_down(0, sz - 1, a, h->cmp);
  return tmp;
}

void p1w_enqueue(struct p1way *h, gtype value) {
  arr_append(h->elems, value);
  long j = p1w_size(h) - 1;
  gtype *a = p1w_arr(h);
  heap_shift_up(j, a, h->cmp);
}

void p1w_root(struct p1way *h, gtype value) {
  long sz = p1w_size(h);
  gtype *a = p1w_arr(h);
  a[0] = value;
  heap_shift_down(0, sz, a, h->cmp);
}

void gtype_free_p1w(gtype value) {
  p1w_free(value.p1w);
}

/***********************************
 * vec/p2w.c
 ***********************************/

/**
 * Hàng đợi ưu tiên truy cập 2 chiều dựa trên Heap
 */

#define arr_set_value_with_index(a, v, i) \
  do {\
    if ((i) >= arr_capacity(a)) { \
      arr_reserve(a, (i) + 1); \
    } \
    (a)[(i)] = (v); \
  } while (0)

struct p2ways *p2w_create(gtype_cmp_t cmp) {
  struct p2ways *h = malloc(sizeof(struct p2ways));
  h->data = arr_create(0, gtype);
  h->index = arr_create(0, long);
  h->index2 = arr_create(0, long);
  h->cmp = cmp;
  return h;
}

void gtype_free_p2w(gtype value) {
  p2w_free(value.p2w);
}

void p2w_switch(struct p2ways *h, long e1, long e2) {
  if (e1 != e2) {
    long tmp1, tmp2;
    gtype_swap(h->data[e1], h->data[e2]);

    tmp1 = h->index[e1];
    tmp2 = h->index[e2];

    arr_set_value_with_index(h->index2, e2 + 2, tmp1);
    arr_set_value_with_index(h->index2, e1 + 2, tmp2);

    arr_set_value_with_index(h->index, tmp2, e1);
    arr_set_value_with_index(h->index, tmp1, e2);
  }
}

void p2w_shift_up(struct p2ways *h, long elem) {
  if (elem == 0 ||
      h->cmp(h->data[elem], h->data[HTOP(elem)]) < 0) {
    /* at the top */
  } else {
    p2w_switch(h, elem, HTOP(elem));
    p2w_shift_up(h, HTOP(elem));
  }
}

void p2w_sink(struct p2ways *h, long head) {
  long size = p2w_size(h);
  if (HLEFT(head) >= size) {
    /* no subtrees */
  } else if (HRIGHT(head) == size ||
         h->cmp(h->data[HLEFT(head)], h->data[HRIGHT(head)]) >= 0) {
    /* sink to the left if needed */
    if (h->cmp(h->data[head], h->data[HLEFT(head)]) < 0) {
      p2w_switch(h, head, HLEFT(head));
      p2w_sink(h, HLEFT(head));
    }
  } else {
    /* sink to the right */
    if (h->cmp(h->data[head], h->data[HRIGHT(head)]) < 0) {
      p2w_switch(h, head, HRIGHT(head));
      p2w_sink(h, HRIGHT(head));
    }
  }
}

int p2w_push_with_index(struct p2ways *h, long idx, gtype elem) {
  long size = arr_size(h->data);
  arr_append(h->data, elem);
  arr_append(h->index, idx);
  arr_set_value_with_index(h->index2, size + 2, idx);

  p2w_shift_up(h, size);
  return 0;
}

gtype p2w_max(const struct p2ways *h) {
  return h->data[0];
}

long p2w_max_index(const struct p2ways *h) {
  return h->index[0];
}

int p2w_has_elem(const struct p2ways *h, long idx) {
  return h->index2[idx] != 0;
}

int p2w_has_active(const struct p2ways *h, long idx) {
  return h->index2[idx] > 1;
}

gtype p2w_get(const struct p2ways *h, long idx) {
  long i = h->index2[idx] - 2;
  return h->data[i];
}

gtype p2w_delete_max(struct p2ways *h) {
  gtype tmp = h->data[0];
  long tmpidx = h->index[0];
  p2w_switch(h, 0, p2w_size(h) - 1);
  arr_resize(h->data, arr_size(h->data) - 1);
  arr_resize(h->index, arr_size(h->index) - 1);
  arr_set_value_with_index(h->index2, 0, tmpidx);
  p2w_sink(h, 0);

  return tmp;
}

gtype p2w_deactivate_max(struct p2ways *h) {
  gtype tmp = h->data[0];
  long tmpidx = h->index[0];
  p2w_switch(h, 0, p2w_size(h) - 1);
  arr_resize(h->data, arr_size(h->data) - 1);
  arr_resize(h->index, arr_size(h->index) - 1);
  arr_set_value_with_index(h->index2, 1, tmpidx);
  p2w_sink(h, 0);

  return tmp;
}

gtype p2w_delete_max_index(struct p2ways *h, long *idx) {
  gtype tmp = h->data[0];
  long tmpidx = h->index[0];
  p2w_switch(h, 0, p2w_size(h) - 1);
  arr_resize(h->data, arr_size(h->data) - 1);
  arr_resize(h->index, arr_size(h->index) - 1);
  arr_set_value_with_index(h->index2, 0, tmpidx);
  p2w_sink(h, 0);

  if (idx) {
    *idx = tmpidx;
  }
  return tmp;
}

int p2w_modify(struct p2ways *h, long idx, gtype elem) {
  long pos = h->index2[idx] - 2;

  h->data[pos] = elem;
  p2w_sink(h, pos);
  p2w_shift_up(h, pos);

  return 0;
}

int p2w_check(struct p2ways *h) {
  long size = p2w_size(h);
  int ecode = 0;
  for (long i = 0; i < size; i++) {
    if (HLEFT(i) >= size) {
      break;
    }
    if (h->cmp(h->data[HLEFT(i)], h->data[i]) > 0) {
      ecode = 1;
      break;
    }
    if (HRIGHT(i) >= size) {
      break;
    }
    if (h->cmp(h->data[HRIGHT(i)], h->data[i]) > 0) {
      ecode = 1;
      break;
    }
  }

  return ecode == 0;
}

/***********************************
 * vec/spec/d2w.c
 ***********************************/

int d2w_push_with_index(struct p2ways *h, long idx, double elem) {
  return p2w_push_with_index(h, idx, gtype_value(d, elem));
}

double d2w_max(const struct p2ways *h) {
  return p2w_max(h).d;
}

double d2w_get(const struct p2ways *h, long idx) {
  return p2w_get(h, idx).d;
}

double d2w_delete_max(struct p2ways *h) {
  return p2w_delete_max(h).d;
}

double d2w_deactivate_max(struct p2ways *h) {
  return p2w_deactivate_max(h).d;
}

double d2w_delete_max_index(struct p2ways *h, long *idx) {
  return p2w_delete_max_index(h, idx).d;
}

int d2w_modify(struct p2ways *h, long idx, double elem) {
  return p2w_modify(h, idx, gtype_value(d, elem));
}

/***********************************
 * list/dll.c
 ***********************************/

struct dlnode *dll_create_node() {
  return calloc(1, sizeof(struct dlnode));
}

struct dllist *dll_create_list() {
  return calloc(1, sizeof(struct dllist));
}

void dll_push_back(struct dllist *list, struct dlnode *nn) {
  if (list->back == NULL) {
    list->front = list->back = nn;
  } else {
    list->back->next = nn;
    nn->prev = list->back;
    list->back = nn;
  }
  ++list->length;
}

void dll_push_front(struct dllist *list, struct dlnode *nn) {
  if (list->front == NULL) {
    list->front = list->back = nn;
  } else {
    list->front->prev = nn;
    nn->next = list->front;
    list->front = nn;
  }
  ++list->length;
}

void dll_pop_back(struct dllist *list) {
  if (dll_is_empty(list)) {
    return;
  }
  struct dlnode *tmp = list->back;
  list->back = tmp->prev;
  if (list->back) {
    list->back->next = NULL;
  } else {
    list->front = NULL;
  }
  free(tmp);
  --list->length;
}

void dll_pop_front(struct dllist *list) {
  if (dll_is_empty(list)) {
    return;
  }
  struct dlnode *tmp = list->front;
  list->front = tmp->next;
  if (list->front) {
    list->front->prev = NULL;
  } else {
    list->back = NULL;
  }
  free(tmp);
  --list->length;
}

/* insert nn after pos in list. push_back if pos == NULL */
void dll_inserta(struct dllist *list, struct dlnode *pos, struct dlnode *nn) {
  if (!pos) {
    dll_push_back(list, nn);
    return;
  }

  struct dlnode *tmp = pos->next;
  pos->next = nn;
  nn->prev = pos;
  nn->next = tmp;
  if (tmp) {
    tmp->prev = nn;
  } else {
    list->back = nn;
  }
  ++list->length;
}

/* insert nn before pos in list. push_front is pos == NULL */
void dll_insertb(struct dllist *list, struct dlnode *pos, struct dlnode *nn) {
  if (!pos) {
    dll_push_front(list, nn);
    return;
  }

  struct dlnode *tmp = pos->prev;
  pos->prev = nn;
  nn->next = pos;
  nn->prev = tmp;
  if (tmp) {
    tmp->next = nn;
  } else {
    list->front = nn;
  }
  ++list->length;
}

void dll_erase(struct dllist *list, struct dlnode *pos) {
  if (pos == list->front) {
    dll_pop_front(list);
    return;
  }
  if (pos == list->back) {
    dll_pop_back(list);
    return;
  }

  struct dlnode *p1 = pos->prev,
             *p2 = pos->next;
  p1->next = p2;
  p2->prev = p1;
  free(pos);
  --list->length;
}

void dll_node_print_address(struct dlnode *node) {
  printf("[%p]", node);
}

void dll_pprint(struct dllist *list, dll_node_print_t npp) {
  dll_traverse(cur, list) {
    npp(cur);
    printf(" ");
  }
  printf("\n");
}

/***********************************
 * list/gdl.c
 ***********************************/

struct gdlnode *gdl_create_node(gtype value) {
  struct dlnode *tmp = dll_create_node();
  struct gdlnode *nn = realloc(tmp, sizeof(struct gdlnode));
  nn->value = value;
  return nn;
}

struct gdllist *gdl_create_list(gtype_free_t free_value) {
  struct dllist *tmp = dll_create_list();
  struct gdllist *list = realloc(tmp, sizeof(struct gdllist));
  list->free_value = free_value;
  return list;
}

long gdl_size(struct gdllist *list) {
  long cc = 0;
  gdl_traverse(cur, list) {
    ++cc;
  }
  return cc;
}

void gtype_free_gdl(gtype value) {
  gdl_free(value.gdl);
}

void gdl_pprint(struct gdllist *list, gtype_print_t pp) {
  gdl_traverse(cur, list) {
    pp(*cur);
    printf(" ");
  }
  printf("\n");
}

/***********************************
 * list/sll.c
 ***********************************/

struct slnode *sll_create_node() {
  return calloc(1, sizeof(struct slnode));
}

struct sllist *sll_create_list() {
  return calloc(1, sizeof(struct sllist));
}

void sll_push_back(struct sllist *list, struct slnode *node) {
  node->next = NULL;
  if (list->front == NULL) {
    list->front = list->back = node;
  } else {
    list->back->next = node;
    list->back = node;
  }
  ++list->length;
}

void sll_push_front(struct sllist *list, struct slnode *node) {
  if (list->front == NULL) {
    list->front = list->back = node;
    node->next = NULL;
  } else {
    node->next = list->front;
    list->front = node;
  }
  ++list->length;
}

void sll_inserta(struct sllist *list, struct slnode *pos, struct slnode *nn) {
  if (!pos) {
    sll_push_back(list, nn);
    return;
  }

  struct dlnode *tmp = pos->next;
  pos->next = nn;
  nn->next = tmp;
  if (!tmp) {
    list->back = nn;
  }
  ++list->length;
}

void sll_pop_front(struct sllist *list) {
  if (sll_is_empty(list)) {
    return;
  }
  struct slnode *tmp = list->front;
  list->front = list->front->next;
  if (list->front == NULL) {
    list->back = NULL;
  }
  free(tmp);
  --list->length;
}

void sll_node_print_address(struct slnode *node) {
  printf("[%p]", node);
}

void sll_pprint(struct sllist *list, sll_node_print_t npp) {
  sll_traverse(cur, list) {
    npp(cur);
    printf(" ");
  }
  printf("\n");
}

/***********************************
 * list/gsl.c
 ***********************************/

struct gslnode *gsl_create_node(gtype value) {
  struct slnode *tmp = sll_create_node();
  struct gslnode *nn = realloc(tmp, sizeof(struct gslnode));
  nn->value = value;
  return nn;
}

struct gsllist *gsl_create_list(gtype_free_t free_value) {
  struct sllist *tmp = sll_create_list();
  struct gsllist *l = realloc(tmp, sizeof(struct gsllist));
  l->free_value = free_value;
  return l;
}

void gtype_free_gsl(gtype value) {
  gsl_free(value.gsl);
}

void gsl_pprint(struct gsllist *l, gtype_print_t pp) {
  gsl_traverse(cur, l) {
    pp(*cur);
    printf(" ");
  }
  printf("\n");
}

/***********************************
 * list/que.c
 ***********************************/

struct gsllist *que_enq(struct gsllist *q, gtype value) {
  gsl_push_back(q, value);
  return q;
}

struct gsllist *que_deq(struct gsllist *q) {
  gsl_pop_front(q);
}

gtype que_peek(struct gsllist *q) {
  return gsl_node_value(gsl_front(q));
}

/***********************************
 * list/stk.c
 ***********************************/

struct gsllist *stk_push(struct gsllist *stk, gtype value) {
  gsl_push_front(stk, value);
  return stk;
}

struct gsllist *stk_pop(struct gsllist *stk) {
  gsl_pop_front(stk);
  return stk;
}

gtype stk_top(struct gsllist *stk) {
  return gsl_node_value(gsl_front(stk));
}

/***********************************
 * list/spec/idl.c
 ***********************************/

struct idlnode *idl_create_node(int value) {
  struct idlnode *n = malloc(sizeof(struct idlnode));
  n->base.prev = n->base.next = NULL;
  n->value = value;
  return n;
}

void idl_push_back(struct dllist *list, int value) {
  struct idlnode *nn = idl_create_node(value);
  dll_push_back(list, dll_node(nn));
}

void idl_push_front(struct dllist *list, int value) {
  struct idlnode *nn = idl_create_node(value);
  dll_push_front(list, dll_node(nn));
}

int idl_pop_back(struct dllist *list) {
  int value = idl_back_value(list);
  dll_pop_back(list);
  return value;
}

int idl_pop_front(struct dllist *list) {
  int value = idl_front_value(list);
  dll_pop_front(list);
  return value;
}

void idl_insert_a(struct dllist *list, struct idlnode *pos, int value) {
  dll_inserta(list, dll_node(pos), dll_node(idl_create_node(value)));
}

void idl_insertb(struct dllist *list, struct idlnode *pos, int value) {
  dll_insertb(list, dll_node(pos), dll_node(idl_create_node(value)));
}

void idn_pprint(struct idlnode *node) {
  printf("[%d]", idl_node_value(node));
}

void idl_pprint(struct dllist *list) {
  dll_pprint(list, idn_pprint);
}

/***********************************
 * list/spec/isl.c
 ***********************************/

struct slnode *isl_create_node(long value) {
  struct islnode *nn = malloc(sizeof(struct islnode));
  nn->base.next = NULL;
  nn->value = value;
  return (struct slnode *)nn;
}

void isl_stack_push(struct sllist *list, long value) {
  struct slnode *nn = isl_create_node(value);
  return sll_push_front(list, nn);
}

long isl_stack_pop(struct sllist *list) {
  long value = isl_stack_top(list);
  sll_pop_front(list);
  return value;
}

long isl_stack_top(struct sllist *list) {
  struct slnode *tmp = sll_front(list);
  return isl_node_value(tmp);
}

void isn_pprint(struct slnode *node) {
  printf("[%ld]", isl_node_value(node));
}

void isl_pprint(struct sllist *list) {
  sll_pprint(list, isn_pprint);
}

/* Triển khai giao diện queue số nguyên */
void isl_fifo_enq(struct sllist *list, long value) {
  struct slnode *nn = isl_create_node(value);
  return sll_push_back(list, nn);
}

long isl_fifo_deq(struct sllist *list) {
  long value = isl_fifo_peek(list);
  sll_pop_front(list);
  return value;
}

long isl_fifo_peek(struct sllist *list) {
  struct slnode *tmp = sll_front(list);
  return isl_node_value(tmp);
}

/***********************************
 * mix/s2w.c
 ***********************************/

struct si2ways *s2w_create() {
  struct si2ways *col = malloc(sizeof(struct si2ways));
  col->is = gvec_create(0, NULL);
  col->si = s2i_create();
  return col;
}

struct rbm_ires s2w_insert(struct si2ways *col, const char *s) {
  long id = gvec_size(col->is);
  struct rbm_ires r = s2i_insert(col->si, s, id);
  if (r.inserted) {
    char *key = rbm_node(
      container_of(r.value, struct rbmnode, value))->key.s;
        gvec_append(col->is, gtype_s(key));
  }
  return r;
}

long *s2w_put(struct si2ways *col, const char *s) {
  long id = gvec_size(col->is);
  struct rbm_ires r = s2i_insert(col->si, s, id);
  if (!r.inserted) {
    return r.value;
  }
  char *key = rbm_node(
    container_of(r.value, struct rbmnode, value))->key.s;
  gvec_append(col->is, gtype_s(key));
  return NULL;
}

int s2w_remove(struct si2ways *col, const char *s) {
  long id = s2w_id(col, s);
  if (id < 0) {
    return 0;
  }
  gvec_elem(col->is, id).s = NULL;
  return s2i_remove(col->si, s);
}

long s2w_id(struct si2ways *col, const char *s) {
  long *tmp = s2i_value(col->si, s);
  if (!tmp) {
    return -1;
  }
  return *tmp;
}

char *s2w_str(struct si2ways *col, const long id) {
  long sz = gvec_size(col->is);
  if (id < 0 || id >= sz) {
    return NULL;
  }
  return gvec_elem(col->is, id).s;
}

void s2w_free(struct si2ways *col) {
  s2i_free(col->si);
  gvec_free(col->is);
  free(col);
}

/***********************************
 * tree/bn.c
 ***********************************/

struct bnnode *bn_create_node() {
  return calloc(1, sizeof(struct bnnode));
}

struct bntree *bn_create_tree(struct bnnode *root) {
  struct bntree *tree = malloc(sizeof(struct bntree));
  tree->root = root;
  return tree;
}

void bn_free_tree(struct bntree *t) {
  struct bnnode *tmp = NULL;
  bn_traverse_lrn(cur, t) {
    bn_free_node(tmp);
    tmp = cur;
  }
  bn_free_node(tmp);
  free(t);
}

struct bnnode *bn_left_deepest_node(struct bnnode *node) {
  if (!node) {
    return NULL;
  }
  for (;;) {
    if (node->left) {
      node = node->left;
    } else if (node->right) {
      node = node->right;
    } else {
      return node;
    }
  }
}

struct bnnode *bn_next_postorder(struct bnnode *node) {
  if (!node) {
    return NULL;
  }
  struct bnnode *top = node->top;

  /* If we're sitting on node, we've already seen our children */
  if (top && node == top->left && top->right) {
    /* If we are the top's left node, go to the top's right
     * node then all the way down to the left */
    return bn_left_deepest_node(top->right);
  } else
    /* Otherwise we are the top's right node, and the parent
     * should be next */
    return top;
}

struct bnnode *bn_first_postorder(struct bnnode *n) {
  return bn_left_deepest_node(n);
}

void bn_foreach_lrn(struct bntree *t, bn_callback_t op, void *u) {
  struct bnnode *n = bn_first_postorder(t->root);
  for (; n != NULL; n = bn_next_postorder(n)) {
    if (op(n, u)) {
      break;
    }
  }
}

struct bnnode *bn_left_most(struct bnnode *x) {
  if (!x) {
    return NULL;
  }
  struct bnnode *y;
  #define bn_MOST(x, child, out) \
  (out) = (x); \
  do { \
    while ((out)->child != NULL) { \
      (out) = (out)->child; \
    } \
  } while (0)
  bn_MOST(x, left, y);
  return y;
}

struct bnnode *bn_right_most(struct bnnode *x) {
  if (!x) {
    return NULL;
  }
  struct bnnode *y;
  bn_MOST(x, right, y);
  return y;
}

struct bnnode *bn_next_inorder(struct bnnode *x)  {
  if (!x) {
    return NULL;
  }
  struct bnnode *y;
#define BS_NEAREST(x, left, right, out) \
  do { \
    if ((x)->right != NULL) { \
      (out) = bn_ ##left ##_most((x)->right); \
    } else { \
      (out) = (x)->top; \
      while ((out) != NULL && (x) == (out)->right) {\
        (x) = (out); \
        (out) = (out)->top; \
      } \
    } \
  } while (0)
  BS_NEAREST(x, left, right, y);
  return y;
}

struct bnnode *bn_prev_inorder(struct bnnode *x) {
  if (!x) {
    return NULL;
  }
  struct bnnode *y;
  BS_NEAREST(x, right, left, y);
  return y;
}

void bn_foreach_lnr(struct bntree *t, bn_callback_t op, void *u) {
  if (!t->root) {
    return;
  }
  struct bnnode *n = bn_left_most(t->root);
  for (; n != NULL; n = bn_next_inorder(n)) {
    if (op(n, u)) {
      break;
    }
  }
}

void bn_foreach_rnl(struct bntree *t, bn_callback_t op, void *u) {
  if (!t->root) {
    return;
  }
  struct bnnode *nd = bn_right_most(t->root);
  for (; nd != NULL; nd = bn_prev_inorder(nd)) {
    if (op(nd, u)) {
      break;
    }
  }
}

void bn_pprint_internal(struct bnnode *root, bn_node_print_t nprt,
      int spaces, int step) {
  if (!root) {
    return;
  }
  if (root->right) {
    bn_pprint_internal(root->right, nprt, spaces + step, step);
  }
  for (int i = 0; i < spaces; ++i) {
    printf(" ");
  }
  nprt(root);
  if (root->left) {
    bn_pprint_internal(root->left, nprt, spaces + step, step);
  }
}

int g_bn_pprint_spaces_at_begin = 0;
int g_bn_pprint_step = 3;

void bn_pprint(struct bntree *t, bn_node_print_t p) {
  bn_pprint_internal(t->root, p,
    g_bn_pprint_spaces_at_begin, g_bn_pprint_step);
}

long bn_size(struct bntree *t) {
  long cc = 0;
  bn_traverse_lrn(cur, t)  {
    ++cc;
  }
  return cc;
}

long bn_distance(struct bnnode *n) {
  long cc = -1;
  while (n != NULL) {
    n = n->top;
    ++cc;
  }
  return cc;
}

long bn_edge_height(struct bntree *t) {
  long max = 0;
  bn_traverse_lnr(n, t) {
    if (n && !n->left && !n->right) {
      long tmp = bn_distance(n);
      if (tmp > max) {
        max = tmp;
      }
    }
  }
  return max;
}

/***********************************
 * tree/bs.c
 ***********************************/

#define BS_INSERT_TPL(...) \
  struct bnnode *top = NULL; \
  struct bnnode *x = t->root; \
  int rl = 0; \
  while (x) { \
    rl = cmp(nn, x, t); \
    __VA_ARGS__ \
    top = x; \
    x = bs_child(top, rl); \
  } \
  struct bnnode * *loc = top? bs_child_ref(top, rl): &t->root; \
  bn_insert(nn, loc, top); \
  return (struct bs_ires){nn, 1}

struct bs_ires bs_insert(struct bntree *t, struct bnnode *nn, bn_compare_t cmp) {
  BS_INSERT_TPL();
}

struct bs_ires bs_insert_unique(struct bntree *t, struct bnnode *nn, bn_compare_t cmp) {
  BS_INSERT_TPL(
    if (!rl) {
      return (struct bs_ires){x, 0};
    }
  );
}

#undef BS_INSERT_TPL

struct bnnode *bs_search(struct bntree *t, struct bnnode *sn, bn_compare_t cmp) {
  int rl;
  struct bnnode *x = t->root;
  while (x) {
    rl = cmp(sn, x, t);
    if (!rl) {
      return x;
    }
    x = bs_child(x, rl);
  }
  return NULL;
}

struct bnnode *bs_search_gte(struct bntree *t, struct bnnode *sn, bn_compare_t cmp) {
  int rl;
  struct bnnode *x = t->root;
  struct bnnode *o = NULL;
  while (x) {
    rl = cmp(sn, x, t);
    if (!rl) {
      return x;
    }
    if (rl < 0) {
      o = x;
      x = x->left;
      continue;
    }
    x = x->right;
  }
  return o;
}

struct bnnode *bs_search_lte(struct bntree *t, struct bnnode *sn, bn_compare_t cmp) {
  int rl;
  struct bnnode *x = t->root;
  struct bnnode *o = NULL;
  while (x) {
    rl = cmp(sn, x, t);
    if (!rl) {
      return x;
    }
    if (rl > 0) {
      o = x;
      x = x->right;
      continue;
    }
    x = x->left;
  }
  return o;
}

int bs_delete(struct bntree *t, struct bnnode *dn) {
  struct bnnode *lc = dn->left,
            *rc = dn->right,
            *top = dn->top;
  if (!lc) {
    bn_change_child(dn, rc, top, t);
    if (rc) {
      bn_connect1(rc, top, top);
    }
  } else {
    if (!rc) {
      bn_change_child(dn, lc, top, t);
      bn_connect1(lc, top, top);
    } else {
      struct bnnode *successor = bn_left_most(rc);
      struct bnnode *rc2 = successor->right,
                *top2 = successor->top;
      bn_change_child(successor, rc2, top2, t);
      if (rc2) {
        bn_connect1(rc2, top, top2);
      }
      successor->left = dn->left;
      successor->right = dn->right;
      successor->top = dn->top;
      bn_change_child(dn, successor, top, t);
    }
  }
  free(dn);
  return 1;
}

/***********************************
 * tree/rb.c
 ***********************************/

const char * color_names[] = {"Đỏ", "Đen"};

struct rbnode *rb_create_node() {
  struct bnnode *tmp = bn_create_node();
  struct rbnode *nn = realloc(tmp, sizeof(struct rbnode));
  nn->color = RB_RED;
  return nn;
}

static void rb_insert_fixup(struct bntree *t, struct bnnode *n, struct bnnode *p) {
  /*
   * Các biến:
   * t - con trỏ tới cây (tree)
   * n - ban đầu là nút mới được thêm vào (node)
   * p - là đỉnh của n (parent, n->top)
   * u - nút đối xứng của p trong cây, chú bác của n (uncle)
   * gp - ông của n, là đỉnh của p (grandparent, p->top)
   *
   * Trong các ví dụ minh họa thì nút có tên được viết hoa là nút đen,
   *    nút có tên viết thường là nút đỏ, nút có thể là đen hoặc đỏ
   *    (không ảnh hưởng đển tính đúng đắn) thì được đặt trong dấu ()
   */
  while (true) {
    /* Các bất biến của vòng lặp:
     *  + p là đỉnh của n, tính chất cây đỏ đen chỉ bị vi phạm ở đoạn
     *    p-n: n và p là các nút đỏ (tính chất 4). Vấn đề này được
     *    khắc phục trong quá trình n được đẩy lên phía gốc.
     * Ban đầu n là nút mới được thêm vào, sau mỗi vòng lặp n tiến
     * gần hơn về phía gốc của cây. Vòng lặp dừng lại khi p ==
     * NULL (n là gốc của cây) hoặc p được tô mầu đen.
     *
     * Trong vòng lặp chúng ta có
     *  + n->top != NULL và p->top != NULL (bởi vì n và p
     * là các nút đỏ)
     */

    if (p == p->top->left) {
#define IMPL_INSERT_FIXUP(left, right) \
      struct bnnode *_u = p->top->right; \
      if (rb_is_red(_u)) { \
        /*     GP                gp  <- n mới                      \
             p   u  thành>>>   P    U                              \
          ->n <-     có thể vi phạm tính chất 4 nếu gp->top là đỏ,\
                     n có thể là con trái hoặc con phải của p     \
         */ \
        rb_set_black(p); \
        rb_set_black(_u); \
        rb_set_red(p->top); \
        n = p->top; \
        p = n->top; \
        if (p == NULL) { \
          /* n là gốc của cây */ \
          rb_set_black(n); \
          break; \
        } \
        if (rb_is_black(p)) { \
          /* Các tính chất đã được thỏa mãn */ \
          break; \
        } \
      } else { \
        if (n == n->top->right) { \
          /*     GP                  GP           \
               p    U  thành>>>   n <-p  U        \
                n               p  <-n mới        \
           */ \
          bn_rotate(t, p, right, left); \
          n = p; \
          p = n->top; \
        } \
        /*   \
         + n là con trái của p                        \
                GP                   gp               \
             p     U  lật mầu >>   P   U              \
           n                      n                   \
          >>> & sau khi xoay phải ở GP thành =>>>     \
              P                                       \
            n    gp                                   \
                    U                                 \
            Thỏa mãn các tính chất của cây đỏ đen     \
         */                                           \
        rb_set_color(p, RB_BLACK); \
        rb_set_color(p->top, RB_RED); \
        p = p->top; \
        bn_rotate(t, p, left, right); \
        break;  \
      }
      IMPL_INSERT_FIXUP(left, right)
    } else {
      IMPL_INSERT_FIXUP(right, left)
    }
  }
}

#undef IMPL_INSERT_FIXUP

#define RB_INSERT_TPL(bs_interface, ...) \
  struct bs_ires ires = bs_interface(t, nn, cmp); \
  __VA_ARGS__ \
  struct bnnode *par = bn_node(nn)->top; \
  if (par == NULL) { \
    rb_set_black(nn); \
  } else if (rb_is_red(par)) { \
    /* vi phạm tính chất 4 (sau thao tác thêm vào chỉ có tính chất 4) \
       có thể bị vi phạm. */ \
    rb_insert_fixup(bn_tree(t), bn_node(nn), bn_node(par)); \
    /* Tìm vị trí */ \
  } \
  return ires

struct bs_ires rb_insert(struct bntree *t, struct rbnode *nn, bn_compare_t cmp) {
  RB_INSERT_TPL(bs_insert);
}

struct bs_ires rb_insert_unique(struct bntree *t, struct rbnode *nn, bn_compare_t cmp) {
  RB_INSERT_TPL(bs_insert_unique,
    if (!ires.inserted) {
      return ires;
    }
  );
}

#undef RB_INSERT_TPL

static void rb_delete_fix_color(struct bntree *t, struct bnnode *parent) {
  struct bnnode *node = NULL, *sibling,
          *cn,  // Con của sibling ở phía gần node (close nephew)
          *dn;  // Con của sibling ở phía xa node (distant nephew)
  while (true) {
    /*
    * Các tính chất bất biến trong vòng lặp:
    * - node là nút đen (hoặc NULL trong lần lặp đầu tiên)
    * - node không phải là nút gốc (top của nó khác NULL)
    * - Tất cả các đường dẫn tới lá đi qua parent va node có số
    *   lượng nút đen ít hơn 1 so với các đường dẫn khác.
    */
    sibling = parent->right;
    if (node != sibling) {  // node == parent->left
#define ERASE_COLOR_SYMMETRY(left, right) \
      /* Trong các ký hiệu cây chữ cái đầu viết thường là nút đỏ, \
       *    chữ cái đầu viết hoa là nút đen, \
       *    nút được để trong ngoặc có thể là đỏ hoặc đen. \
      */ \
      if (rb_is_red(sibling)) { \
        /* \
         * Trường hợp 1 - Xoay trái ở parent      \
         *                                        \
         *     P               S                  \
         *    / \             / \                 \
         *   N   s    -->    p   Dn               \
         *      / \         / \                   \
         *     Cn  Dn      N   Cn <- sibling mới  \
         */                                       \
        bn_rotate(t, parent, right, left); \
        rb_set_red(parent); \
        rb_set_black(sibling); \
        sibling = parent->right; \
      } \
      dn = sibling->right; \
      if (rb_is_black(dn)) { \
        cn = sibling->left; \
        if (rb_is_black(cn)) { \
          /*  \
           * Trường hợp 2 - Đảo mầu sibling, p có thể có mầu bất kỳ  \
           *                                                         \
           *    (p)           (p)                                    \
           *    / \           / \                                    \
           *   N   S    -->  N   s                                   \
           *      / \           / \                                  \
           *     Cn  Dn        Cn  Dn                                \
           *                                                         \
           * Điều này dẫn tới vi phạm ràng buộc 5), vi phạm này có   \
           * thể được khắc phục bằng cách đảo mầu p thành đen nếu nó \
           * là nút đỏ, hoặc đệ quy tại p nếu ngược lại. Nút p có    \
           * mầu đỏ sau khi xử lý trường hợp 1.                      \
           */                                                        \
          rb_set_color(sibling, RB_RED); \
          if (rb_is_red(parent)) { \
            rb_set_black(parent); \
          } else { \
            node = parent; \
            parent = node->top; \
            if (parent) { \
              continue; \
            } \
          } \
          break; \
        } \
        /* \
         * Trường hợp 3 - Xoay phải tại sibling (p có thể có mầu bất \
         * kỳ)                                                       \
         *                                                           \
         *              (p)           (p)                            \
         *              / \           / \                            \
         *             N   S    -->  N   cn                          \
         *                / \             \                          \
         *               cn  Dn            S                         \
         *                                  \                        \
         *                                   Dn                      \
         * Lưu ý: + p có thể là nút đỏ, và nếu như vậy thì cả p và   \
         * Cn đều là các nút đỏ sau khi xoay (vi phạm ràng buộc 4).  \
         *                                                           \
         * + Đường đi từ p qua cn sau đó rẽ về phía N bị giảm một    \
         * nút đen (S, vi phạm tính chất 5).                         \
         *                                                           \
         * Các vấn đề  này được xử lý trong trường hợp 4: Sau khi    \
         * xoay trái tại parent, cn được tô bằng mầu của p, dn và p  \
         * được tô mầu đen)                                          \
         *                                                           \
         *   (p)            (cn)                                     \
         *   / \            /  \                                     \
         *  N   cn   -->   P    S                                    \
         *       \        /      \                                   \
         *        S      N        Dn                                 \
         *         \                                                 \
         *          Dn                                               \
         */                                                          \
        bn_rotate(t, sibling, left, right); \
        sibling = parent->right; \
      } \
      /* Trường hợp 4 - Xoay trái ở parent + đảo mầu các nút         \
       * (p và cn có thể có mầu bất kỳ trước khi xoay. Sau khi xoay  \
       * p và dn được tô mầu đen, s có mầu của p,                    \
       * còn cn giữ nguyênmầu của nó)                                \
       *                                                             \
       *      (p)             (s)                                    \
       *      / \             / \                                    \
       *     N   S     -->   P   Dn                                  \
       *        / \         / \                                      \
       *      (cn) dn      N  (cn)                                   \
       */ \
      dn = sibling->right; \
      bn_rotate(t, parent, right, left); \
      rb_set_color(sibling, rb_color(parent)); \
      rb_set_black(parent); \
      rb_set_black(dn); \
      break
      ERASE_COLOR_SYMMETRY(left, right);
    } else {
      sibling = parent->left;
      ERASE_COLOR_SYMMETRY(right, left);
#undef ERASE_COLOR_SYMMETRY
    }
  }
}

#define rb_set_parent_color(n, parent, color) \
  n->top = parent; \
  rb_set_color(n, color)

int rb_delete(struct bntree *t, struct rbnode *dn) {
  struct bnnode *node = bn_node(dn);
  struct bnnode *child = node->right,
            *tmp = node->left,
            *parent, *rebalance;
  struct bnnode *p;
  enum rbnode_color c;
  if (!tmp) {
    /* Trường hợp 1: Nếu nút đang xóa có không quá 1 nút con (dễ)
     *
     * Nếu có một con thì nút con phải là nút đỏ do tính chất 5,
     * và nó phải là nút đen theo tính chất 4. Chúng ta điều chỉnh
     * mầu trong lân cận để tránh gọi hàm sửa mầu sau này.
     */
    p = node->top;
    c = rb_color(node);
    parent = p;
    bn_change_child(node, child, parent, t);
    if (child) {
      rb_set_parent_color(child, p, c);
      rebalance = NULL;
    } else {
      rebalance = c == RB_BLACK? parent: NULL;
    }
    tmp = parent;
  } else if (!child) {
    // Vẫn trường hợp 1 nhưng nút con là node->left
    p = node->top;
    c = rb_color(node);
    rb_set_parent_color(tmp, p, c);
    parent = p;
    bn_change_child(node, tmp, parent, t);
    rebalance = NULL;
    tmp = parent;
  } else {
    struct bnnode *successor = child, *child2;
    tmp = child->left;
    if (!tmp) {
      /* Trường hợp 2: Nút liền sau node là con phải của node.
       *
       *    (n)          (s)
       *    / \          / \
       *  (x) (s)  ->  (x) (c)
       *        \
       *        (c)
       */
      parent = successor;
      child2 = successor->right;
    } else {
      /* Trường hợp 3: Nút liền sau node là nút trái nhất trong
       * cây con phải của node
       *
       *    (n)          (s)
       *    / \          / \
       *  (x) (y)  ->  (x) (y)
       *      /            /
       *    (p)          (p)
       *    /            /
       *  (s)          (c)
       *    \
       *    (c)
       */
      do {
        parent = successor;
        successor = tmp;
        tmp = tmp->left;
      } while (tmp);
      child2 = successor->right;
      parent->left = child2;
      successor->right = child;
      child->top = successor;
    }
    tmp = node->left;
    successor->left = tmp;
    tmp->top = successor;
    p = node->top;
    c = rb_color(node);
    tmp = p;
    bn_change_child(node, successor, tmp, t);
    if (child2) {
      rb_set_parent_color(child2, parent, RB_BLACK);
      rebalance = NULL;
    } else {
      enum rbnode_color c2 = rb_color(successor);
      rebalance = c2 == RB_BLACK? parent: NULL;
    }
    rb_set_parent_color(successor, p, c);
    tmp = successor;
  }
  if (rebalance) {
    rb_delete_fix_color(t, rebalance);
  }
  free(dn);
  return 1;
}

#undef rb_set_parent_color

/***********************************
 * tree/rbm.c
 ***********************************/

/**
 * Cấu trúc map dựa trên cây đỏ đen (rb)
*/

#define tm_cmp_conv(u, x) cmp(u, rbm_node_key(x))

struct rbmnode *rbm_create_node(gtype key, gtype value) {
  struct rbnode *tmp = rb_create_node();
  struct rbmnode *n = realloc(tmp, sizeof(struct rbmnode));
  rbm_node_key(n) = key;
  rbm_node_value(n) = value;
  return n;
}

int rbm_cmp_node(struct bnnode *n1, struct bnnode *n2, struct bntree *t) {
  return rbm_tree(t)->cmp(rbm_node(n1)->key, rbm_node(n2)->key);
}

struct rbmtree *rbm_create(gtype_cmp_t cmp, gtype_free_t fk, gtype_free_t fv) {
  struct bntree *tmp = bn_create_tree(NULL);
  struct rbmtree *m = realloc(tmp, sizeof(struct rbmtree));
  m->cmp = cmp;
  m->fk = fk;
  m->fv = fv;
  m->size = 0;
  return m;
}

struct rbm_ires rbm_insert(struct rbmtree *t, gtype key, gtype value) {
  struct rbmnode *nn = rbm_create_node(key, value);
  struct bs_ires ires = rb_insert_unique(bn_tree(t), nn, rbm_cmp_node);
  if (!ires.inserted) {
    free(nn);
    return (struct rbm_ires){&rbm_node_value(ires.nn), 0};
  }
  ++t->size;
  return (struct rbm_ires){&rbm_node_value(ires.nn), 1};
}

gtype *rbm_put(struct rbmtree *t, gtype key, gtype value) {
  struct rbmnode *nn = rbm_create_node(key, value);
  struct bs_ires ires = rb_insert_unique(bn_tree(t), bn_node(nn), rbm_cmp_node);
  if (!ires.inserted) {
    free(nn);
    return &rbm_node_value(ires.nn);
  }
  ++t->size;
  return NULL;
}

struct rbmnode *rbm_search(struct rbmtree *t, gtype key) {
  struct rbmnode sn = {.key = key};
  return rbm_node(bs_search(bn_tree(t), bn_node(&sn), rbm_cmp_node));
}

gtype *rbm_value(struct rbmtree *t, gtype key) {
  struct rbmnode *n = rbm_search(t, key);
  if (n) {
    return &rbm_node_value(n);
  }
  return NULL;
}

int rbm_remove(struct rbmtree *t, gtype key) {
  struct rbmnode *n = rbm_search(t, key);
  if (!n) {
    return 0;
  }
  if (t->fk) {
    t->fk(n->key);
  }
  if (t->fv) {
    t->fv(n->value);
  }
  rb_delete((struct bntree *)t, bn_node(n));
  --(t->size);
  return 1;
}

void gtype_free_rbm(gtype value) {
  rbm_free(value.rbm);
}

/***********************************
 * tree/rbs.c
 ***********************************/

struct rbstree *rbs_create(gtype_cmp_t cmp, gtype_free_t fk) {
  struct grbtree *tmp = grb_create_tree(NULL, cmp, fk);
  struct rbstree *s = realloc(tmp, sizeof(struct rbstree));
  s->size = 0;
  return s;
}

int rbs_insert(struct rbstree *s, gtype elem) {
  struct bs_ires ires = grb_insert_unique(grb_tree(s), elem);
  if (ires.inserted) {
    ++(s->size);
  }
  return ires.inserted;
}

struct grbnode *rbs_search(struct rbstree *s, gtype elem) {
  return grb_search(grb_tree(s), elem);
}

int rbs_remove(struct rbstree *s, gtype elem) {
  struct grbnode *n = rbs_search(s, elem);
  if (!n) {
    return 0;
  }
  grb_delete(grb_tree(s), n);
  --s->size;
  return 1;
}

void gtype_free_rbs(gtype value) {
  rbs_free(value.rbs);
}

/***********************************
 * tree/spec/gbs.c
 ***********************************/

/* Giao diện hỗ trợ gtype */

struct gbsnode *gbs_create_node(gtype key) {
  struct bnnode *tmp = bn_create_node();
  struct gbsnode *nn = realloc(tmp, sizeof(struct gbsnode));
  nn->key = key;
  return nn;
}

struct gbstree *gbs_create_tree(struct gbsnode *root, gtype_cmp_t cmp, gtype_free_t fk) {
  struct bntree *tmp = bn_create_tree(bn_node(root));
  struct gbstree *t = realloc(tmp, sizeof(struct gbstree));
  t->cmp = cmp;
  t->fk = fk;
  return t;
}

int gbs_cmp_node(struct bnnode *n1, struct bnnode *n2, struct bntree *t) {
  return gbs_tree(t)->cmp(gbs_node(n1)->key, gbs_node(n2)->key);
}

struct bs_ires gbs_insert(struct gbstree *t, gtype key) {
  struct gbsnode *nn = gbs_create_node(key);
  return bs_insert(t, bn_node(nn), gbs_cmp_node);
}

struct bs_ires gbs_insert_unique(struct gbstree *t, gtype key) {
  struct gbsnode *nn = gbs_create_node(key);
  struct bs_ires r = bs_insert_unique(t, bn_node(nn), gbs_cmp_node);
  if (!r.inserted) {
    free(nn);
  }
  return r;
}

struct gbsnode *gbs_search(struct gbstree *t, gtype key) {
  struct gbsnode sn = {.key = key};
  return gbs_node(bs_search(bn_tree(t), bn_node(&sn), gbs_cmp_node));
}

struct gbsnode *gbs_search_gte(struct gbstree *t, gtype key) {
  struct gbsnode sn = {.key = key};
  return gbs_node(bs_search_gte(bn_tree(t), bn_node(&sn), gbs_cmp_node));
}

struct gbsnode *gbs_search_lte(struct gbstree *t, gtype key) {
  struct gbsnode sn = {.key = key};
  return gbs_node(bs_search_lte(bn_tree(t), bn_node(&sn), gbs_cmp_node));
}

int gbs_delete(struct gbstree *t, struct gbsnode *dn) {
  if (t->fk) {
    t->fk(dn->key);
  }
  return bs_delete(bn_node(t), bn_node(dn));
}

void gbs_pprint(struct gbstree *t, gtype_print_t gpp) {
  extern int g_bn_pprint_spaces_at_begin;
  extern int g_bn_pprint_step;

#define bs_gpp(n) gpp(gbs_node(n)->key)

  struct bntree *bnt = bn_tree(t);
  bn_pprint_inline(bnt, g_bn_pprint_spaces_at_begin, g_bn_pprint_step, bs_gpp);

#undef bs_gpp
}

/***********************************
 * tree/spec/grb.c
 ***********************************/

struct grbnode *grb_create_node(gtype key) {
  struct rbnode *tmp = rb_create_node();
  struct grbnode *nn = realloc(tmp, sizeof(struct grbnode));
  nn->key = key;
  return nn;
}

int grb_cmp_node(struct bnnode *n1, struct bnnode *n2, struct bntree *t) {
  return grb_tree(t)->cmp(grb_node(n1)->key, grb_node(n2)->key);
}

struct grbtree *grb_create_tree(struct grbnode *root, gtype_cmp_t cmp, gtype_free_t fk) {
  struct bntree *tmp = bn_create_tree(bn_node(root));
  struct grbtree *t = realloc(tmp, sizeof(struct grbtree));
  t->cmp = cmp;
  t->fk = fk;
  return t;
}

struct bs_ires grb_insert(struct grbtree *t, gtype key) {
  struct grbnode *nn = grb_create_node(key);
  return rb_insert(bn_tree(t), nn, grb_cmp_node);
}

struct bs_ires grb_insert_unique(struct grbtree *t, gtype key) {
  struct grbnode *nn = grb_create_node(key);
  struct bs_ires r = rb_insert_unique(bn_tree(t), nn, grb_cmp_node);
  if (!r.inserted) {
    free(nn);
  }
  return r;
}

struct grbnode *grb_search(struct grbtree *t, gtype key) {
  struct grbnode sn = {.key = key};
  return grb_node(bs_search(bn_tree(t), bn_node(&sn), grb_cmp_node));
}

struct grbnode *grb_search_gte(struct grbtree *t, gtype key) {
  struct grbnode sn = {.key = key};
  return grb_node(bs_search_gte(bn_tree(t), bn_node(&sn), grb_cmp_node));
}

struct grbnode *grb_search_lte(struct grbtree *t, gtype key) {
  struct grbnode sn = {.key = key};
  return grb_node(bs_search_lte(bn_tree(t), bn_node(&sn), grb_cmp_node));
}

int grb_delete(struct grbtree *t, struct grbnode *dn) {
  if (t->fk) {
    t->fk(dn->key);
  }
  return rb_delete(bn_tree(t), rb_node(dn));
}

/***********************************
 * tree/spec/s2i.c
 ***********************************/

struct rbmtree *s2i_create() {
  return rbm_create(gtype_cmp_s, gtype_free_s, NULL);
}

struct rbm_ires s2i_insert(struct rbmtree *si, const char *key, long value) {
  char *tmp = strdup(key);
  struct rbm_ires r = rbm_insert(si, gtype_s(tmp), gtype_l(value));
  if (!r.inserted) {
    free(tmp);
  }
  return r;
}

long *s2i_put(struct rbmtree *si, const char *key, long value) {
  char *tmp = strdup(key);
  gtype *res = rbm_put(si, gtype_s(tmp), gtype_l(value));
  if (res) {
    free(tmp);
  }
  return (long*)res;
}

long *s2i_value(struct rbmtree *si, const char *key) {
  return (long*)rbm_value(si, gtype_s(key));
}

int s2i_remove(struct rbmtree *si, const char *key) {
  return rbm_remove(si, gtype_s(key));
}

void s2i_print_node(struct bnnode *n) {
  printf("(%s, %ld)\n", rbm_node_key(n).s, rbm_node_value(n).l);
}

/***********************************
 * utils/str.c
 ***********************************/

const char* ascii_spaces = "\t\n\v\f\r ";

struct gvector *str_tokens(char *line, const char *delims) {
  struct gvector *tokens = gvec_create(0, NULL);
  tok_traverse(tk, line, delims) {
    gvec_append(tokens, gtype_s(tk));
  }
  return tokens;
}
