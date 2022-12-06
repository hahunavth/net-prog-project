#ifndef BKCGEN_H_
#define BKCGEN_H_

/* (C) Nguyen Ba Ngoc 2022 */
/* Version: 1.1.6 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/***********************************
 * tdef.h
 ***********************************/

#ifndef TDEF_H_
#define TDEF_H_

/** @file
 * @brief Quy tắc sử dụng kiểu con trỏ ở dạng đủ thông tin, không tạo
 * tên tương đương cho kiểu con trỏ (typedef) được áp dụng trong
 * phạm vi thư viện này.
 *
 * Tuy nhiên để tiếp tục hỗ trợ các mã nguồn đã sử dụng các tên tương
 * đương (rút gọn) như gvec_t, các typedef sẽ được tập hợp ở đây.
 * Các lựa chọn là riêng tư, người dùng cũng có thể tự quy ước các tên
 * rút gọn với typedef bên ngoài phạm vi thư viện nếu muốn.
 *
 * Sử dụng kiểu con trỏ ở dạng đầy đủ sẽ dễ đọc hơn, ví dụ
 * khi sử dụng kèm const, phép gán và các toán tử khác, v.v..
 * Người dùng nên chuyển sang dạng đầy đủ thay cho các tên tương đương
 * và không nên quy ước các tên rút gọn cho kiểu con trỏ.
 */

typedef struct gvector *gvec_t;
typedef struct dlnode *dln_t;
typedef struct dllist *dll_t;
typedef struct idlnode *idn_t;
typedef struct slnode *sln_t;
typedef struct sllist *sll_t;
typedef struct islnode *isn_t;
typedef struct gdlnode *gdn_t;
typedef struct gdllist *gdl_t;
typedef struct gslnode *gsn_t;
typedef struct gsllist *gsl_t;
typedef struct si2ways *si2w_t;
typedef struct bnnode *bnn_t;
typedef struct bntree *bnt_t;
typedef struct rbnode *rbn_t;
typedef struct gbsnode *gbn_t;
typedef struct gbstree *gbt_t;
typedef struct grbnode *grn_t;
typedef struct grbtree *grt_t;
typedef struct rbmnode *rbmn_t;
typedef struct rbmtree *rbm_t;
typedef struct rbstree *rbs_t;
typedef struct hmap *hmap_t;
typedef struct hset *hset_t;
typedef struct p1way *p1w_t;
typedef struct p2ways *p2w_t;

#endif  // TDEF_H_

/***********************************
 * base/core.h
 ***********************************/

#ifndef BASE_CORE_H_
#define BASE_CORE_H_

#define container_of(ptr, type, member) \
  ((type *)((void*)(ptr) - offsetof(type, member)))

#define New(TYPE, ...) TYPE ## _create(  __VA_ARGS__ )

#define Delete(TYPE, ptr) do        \
        {       \
          TYPE ## _free(ptr); \
          /*free(ptr);*/    \
        }       \
        while(0)

#define BENCH(NAME, ITER, ...)    do { \
        double _sum = 0, _start, _stop; \
        for (int _i = 0; _i < (ITER); ++_i) { \
          _start = clock(); \
          { __VA_ARGS__; } \
          _stop = clock(); \
          _sum += _stop - _start; \
        } \
        if ((ITER) > 1) { \
          printf("%s (trung bình %d lượt) = %.5g s\n", \
                (NAME), (ITER),  (_sum/CLOCKS_PER_SEC) / (ITER)); \
        } else { \
          printf("%s: %5g s\n", (NAME), _sum/CLOCKS_PER_SEC); \
        }\
    } while (0)

#define BENCH1_START() \
   long _bench1_start = clock()
#define BENCH1_END() \
   long _bench1_end = clock(); \
   printf("Time: %.3f\n", (double)(_bench1_end - _bench1_start)/CLOCKS_PER_SEC)

#endif  // BASE_CORE_H_

/***********************************
 * base/gtype.h
 ***********************************/

#ifndef BASE_GTYPE_H_
#define BASE_GTYPE_H_

/** @file
 * @brief Định nghĩa kiểu ::gtype và các thành phần bổ trợ.
 */

/**
 * \headerfile "all.h"
 * Kiểu dữ liệu cơ bản của các cấu trúc dữ liệu được triển khai.
 * ::gtype có thể thay thế cho 1 nhóm kiểu dữ liệu.
 */
typedef union generic_type {
  long l;
  double d;
  char *s;
  void *v;
  union generic_type *g;
  struct gdllist *gdl;
  struct gsllist *gsl;
  struct gvector *gvec;
  struct rbmtree *rbm;
  struct rbstree *rbs;
  struct hmap *hmap;
  struct hset *hset;
  struct p1way *p1w;
  struct p2ways *p2w;
} gtype;

#define gtype_zero (gtype_l(0l))

#define gtype_value(type, val) ((gtype){.type = (val)})
#define gtype_l(value) gtype_value(l, value)
#define gtype_d(value) gtype_value(d, value)
#define gtype_s(value) gtype_value(s, value)
#define gtype_v(value) gtype_value(v, value)
#define gtype_g(value) gtype_value(g, value)
#define gtype_gdl(value) gtype_value(gdl, value)
#define gtype_gsl(value) gtype_value(gsl, value)
#define gtype_gvec(value) gtype_value(gvec, value)
#define gtype_rbm(value) gtype_value(rbm, value)
#define gtype_rbs(value) gtype_value(rbs, value)
#define gtype_hmap(value) gtype_value(hmap, value)
#define gtype_hset(value) gtype_value(hset, value)
#define gtype_p1w(value) gtype_value(p1w, value)
#define gtype_p2w(value) gtype_value(p2w, value)

#define gtype_swap(v1, v2) \
  do { \
    gtype _tmp = (v1); \
    (v1) = (v2); \
    (v2) = _tmp; \
  } while (0)

typedef int (*gtype_cmp_t)(gtype, gtype);
typedef void (*gtype_free_t)(gtype);
typedef int (*gtype_print_t)(gtype);

static inline int gtype_cmp_l(gtype v1, gtype v2) {
  return v1.l - v2.l;
}

static inline int gtype_rcmp_l(gtype v1, gtype v2) {
  return v2.l - v1.l;
}

static inline int gtype_cmp_d(gtype v1, gtype v2) {
  if (v1.d < v2.d) {
    return -1;
  } else if (v1.d > v2.d) {
    return 1;
  }
  return 0;
}

static inline int gtype_rcmp_d(gtype v1, gtype v2) {
  if (v2.d < v1.d) {
    return -1;
  } else if (v2.d > v1.d) {
    return 1;
  }
  return 0;
}

/**
 * Hàm so sánh các giá trị gtype chứa con trỏ chuỗi ký tự.
 *
 * @param v1 Đối số thứ nhất.
 * @param v2 Đỗ số thứ 2.
 * @return Giá trị trả về theo định dạng strcmp <br>
 *   -1 nếu v1.s < v2.s, <br>
 *    1 nếu ngược lại và v1.s > v2.s, <br>
 *    0 nếu ngược lại (v1.s == v2.s). <br>
 *
 * \memberof generic_type
 */
static inline int gtype_cmp_s(gtype v1, gtype v2) {
  return strcmp(v1.s, v2.s);
}

static inline int gtype_rcmp_s(gtype v1, gtype v2) {
  return strcmp(v2.s, v1.s);
}

static inline int gtype_qsort_l(const void *v1, const void *v2) {
  return ((const gtype*)v1)->l - ((const gtype*)v2)->l;
}

static inline int gtype_qsort_d(const void *v1, const void *v2) {
  if (((const gtype*)v1)->l > ((const gtype*)v2)->l) {
    return 1;
  } else if (((const gtype*)v1)->l < ((const gtype*)v2)->l) {
    return -1;
  }
  return 0;
}

static inline int gtype_qsort_s(const void *v1, const void *v2) {
  return strcmp(((const gtype*)v1)->s, ((const gtype*)v2)->s);
}

static int gtype_print_l(gtype value) {
  printf("%ld\n", value.l);
  return 0;  // Tương thích với foreach
}

static int gtype_print_d(gtype value) {
  printf("%f\n", value.d);
  return 0;
}

static int gtype_print_s(gtype value) {
  printf("%s\n", value.s);
  return 0;
}

/**
 * Giải phóng chuỗi được trỏ đến bởi con trỏ v.s
 *
 * @param v Giá trị gtype chứa con trỏ tới chuỗi.
 * @return Hàm không trả về giá trị.
 */
static void gtype_free_s(gtype v) {
  free(v.s);
}

static void gtype_free_v(gtype v) {
  free(v.v);
}

#endif  // BASE_GTYPE_H_

/***********************************
 * base/hashes.h
 ***********************************/

#ifndef BASE_HASHES_H_
#define BASE_HASHES_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
  Triển khai hmap và hset
*/
#define HASH_MIN_SHIFT 3
#define UNUSED_HASH_VALUE 0
#define DELETED_HASH_VALUE 1
#define MIN_HASH_VALUE 2
#define HASH_IS_UNUSED(h_) ((h_) == UNUSED_HASH_VALUE)
#define HASH_IS_INUSED(h_) (!HASH_IS_UNUSED(h_))
#define HASH_IS_DELETED(h_) ((h_) == DELETED_HASH_VALUE)
#define HASH_IS_REAL(h_) ((h_) >= MIN_HASH_VALUE)
#define HASH_IS_NOTREAL(h_) (!HASH_IS_REAL(h_))
#define MAX(a, b) ((a) > (b)? (a): (b))

typedef unsigned int uint;
typedef uint32_t uint32;
typedef uint (*gtype_hash_t) (gtype);

#define hashes_next_pkey_or_pvalue(m, c, kv, o) \
  do { \
    (o) = NULL; \
    int _idx = (c)? (c) - (m)->kv + 1: 0; \
    for (int _i = _idx; _i < (m)->capacity; ++_i) { \
      if (HASH_IS_REAL((m)->hashes[_i])) { \
        (o) = (m)->kv + _i; \
        break; \
      } \
    } \
  } while (0)

extern const int prime_mod [];

#define get_status_bit(bitmap, index) (((bitmap)[(index)/32] >> ((index) %32)) & 1)
#define set_status_bit(bitmap, index) ((bitmap)[(index) / 32] |= 1U << ((index) % 32))

#define evict_key_or_value(a, i, v, ov) \
  { \
    gtype _tmp = (a)[(i)]; \
    (a)[(i)] = (v); \
    (ov) = _tmp; \
  }

// Một hàm băm khái quát
static uint32_t hgen(const void *data, long length) {
  register long i = length;
  register uint32_t hv = 0;
  register const unsigned char *s = (const unsigned char *)data;
  while (i--) {
    hv += *s++;
    hv += (hv << 10);
    hv ^= (hv >> 6);
  }
  hv += (hv << 3);
  hv ^= (hv >> 11);
  hv += (hv << 15);

  return hv;
}

static inline uint gtype_hash_s(gtype v) {
  const char *p;
  uint h = 5381;
  for (p = v.s; *p != '\0'; ++p) {
    h = (h << 5) + h + *p;
  }
  return h;
}

static inline uint gtype_hash_d(gtype v) {
  return (uint) v.d;
}

static inline uint gtype_hash_l(gtype v) {
  return (uint) v.l;
}

#ifdef __cplusplus
}
#endif

#endif  // BASE_HASHES_H_

/***********************************
 * algo/sort.h
 ***********************************/

#ifndef ALGO_SORT_H_
#define ALGO_SORT_H_

void selsort(long n, gtype *a, gtype_cmp_t cmp);
void insort(long n, gtype *a, gtype_cmp_t cmp);
void bubsort(long n, gtype *a, gtype_cmp_t cmp);

/* Sắp xếp nhanh, phân mảnh 2 phần */
void q2sort(long n, gtype *a, gtype_cmp_t cmp);

/* Sắp xếp nhanh, phân mảnh 2 phần, sử dụng trung vị của lo, mid, hi làm chốt */
void q2m3sort(long n, gtype *a, gtype_cmp_t cmp);

/* Kết hợp q2m3 và sắp xếp chèn */
void qinsort(long n, gtype *a, gtype_cmp_t cmp);

/* Sắp xếp nhanh, phân mảnh 3 phần */
void q3sort(long n, gtype *a, gtype_cmp_t cmp);

/**
 * Tùy chỉnh glibc cho gtype:
 * Phân mảnh 2 phần, sử dụng trung vị của lo, mid, hi,
 *   bỏ qua khối nhỏ, xếp chèn dãy gần hoàn thiện và khử đệ quy.
 */
void quicksort(long n, gtype *a, gtype_cmp_t cmp);

/**
 * Sắp xếp vun đống.
 */
void heapsort(long n, gtype *a, gtype_cmp_t cmp);

#endif  // ALGO_SORT_H_

/***********************************
 * vec/arr.h
 ***********************************/

#ifndef VEC_ARR_H_
#define VEC_ARR_H_

/**
 * Macro đánh dấu kiểu phần tử của arr.
 * atype tuy không làm thay đổi ý nghĩa của elemtype nhưng
 * có thể giúp đánh dấu con trỏ được tạo bởi arr, hỗ trợ đọc
 * mã nguồn.
 * Nên sử dụng atype(elemtype) * thay cho elemtype * đối với
 * các con trỏ được sử dụng cho arr.
 */
#define atype(elemtype) elemtype

struct arr_info {
  long size;
  long cap;
  long esz;
  double scale;
};

static inline void *arr_create_internal(long n, long elem_size, double scale) {
  struct arr_info *info = calloc(1, sizeof(struct arr_info) + n * elem_size);
  info->size = n;
  info->cap = n;
  info->esz = elem_size;
  info->scale = scale;
  return (void*)(info + 1);
}

#define arr_info(a) ((struct arr_info *)((void*)(a) - sizeof(struct arr_info)))
#define arr_size(a) (arr_info(a)->size)
#define arr_capacity(a) (arr_info(a)->cap)
#define arr_elemsize(a) (arr_info(a)->esz)
#define arr_scale(a) (arr_info(a)->scale)

/* Mặc định scale = 2 */
#define arr_create(count, elemtype) \
   (atype(elemtype) *)arr_create_internal(count, sizeof(elemtype), 2.0)

#define arr_make(name, count, elemtype) \
   atype(elemtype) *name = arr_create(count, elemtype)

#define arr_reserve(a, newcap) \
  do { \
    if ((newcap) < arr_size(a)) { \
      break; \
    } \
    struct arr_info *_info = \
      realloc(arr_info(a), sizeof(struct arr_info) + (newcap) * arr_elemsize(a)); \
    _info->cap = (newcap); \
    (a) = (void*)(_info + 1); \
  } while (0)

#define arr_resize(a, newsize) \
  do { \
    if (newsize > arr_capacity(a)) {  \
      arr_reserve((a), newsize); \
    } \
    arr_size(a) = (newsize); \
  } while (0)

#define arr_free(a) \
    free(arr_info(a)); \

#define arr_clear(a) \
  do { \
    arr_resize(a, 0); \
    arr_reserve(a, 0); \
  } while (0)

#define arr_append(a, elem) \
   do { \
     struct arr_info *_info = arr_info(a); \
     if (_info->size >= _info->cap) { \
       long _newcap = _info->cap < 10? 10: _info->cap * _info->scale; \
       arr_reserve((a), _newcap); \
       _info = arr_info(a); \
     } \
     (a)[_info->size] = (elem); \
     ++(_info->size); \
   } while (0)

#define arr_qsort(a, cmp) \
   qsort(a, arr_size(a), arr_elemsize(a), cmp)

/**
 * Duyệt tuần tự từng phần tử của mảng a,
 * typeof ban đầu là 1 từ khóa trong mở rộng của GNU,
 * và đến nay đã chính thức trở thành 1 từ khóa của ISO C kể từ C23
 */
#define arr_traverse(cur, a) \
  for (typeof(a) cur = a; cur < a + arr_size(a); ++cur)

/**
 * Gán tất cả các giá trị của a = value.
 */
#define arr_fill(a, value) \
  do { \
    arr_traverse(_cur, a) { \
      *_cur = value; \
    } \
  } while (0)

/**
 * Gán max trong mảng a cho biến m, m phải được khai báo bên ngoài macro
 */
#define arr_assign_max(m, a) \
  do { \
    (m) = (a)[0]; \
    for (long _j = 1; _j < arr_size(a); ++_j) { \
      if ((a)[_j] > (m)) { \
        (m) = (a)[_j]; \
      } \
    } \
  } while (0)

/**
 * Gán min trong mảng a cho biến m, m phải được khai báo bên ngoài macro
 */
#define arr_assign_min(m, a) \
  do { \
    (m) = (a)[0]; \
    for (long _j = 1; _j < arr_size(a); ++_j) { \
      if ((a)[_j] < (m)) { \
        (m) = (a)[_j]; \
      } \
    } \
  } while (0)

#endif  // VEC_ARR_H_

/***********************************
 * vec/gvec.h
 ***********************************/

#ifndef VEC_GVEC_H_
#define VEC_GVEC_H_

/** @file
 * @brief Vec-tơ của các giá trị ::gtype.
 *
 * Có thể truy cập ngẫu nhiên đến 1 phần tử bất kỳ trong vec-tơ
 * theo chỉ số và sắp xếp nhanh vec-tơ tương tự như mảng.
 * Kích thước của vec-tơ có thể thay đổi sau khi được tạo
 * (dựa trên realloc) vì vậy linh động hơn mảng thông thường và
 * cũng có phạm vi ứng dụng rộng hơn.
 */

/**
 * \headerfile "all.h"
 * Cấu trúc biểu diễn vec-tơ của các giá trị ::gtype.
 *
 * Các macros điều khiển (v phải có kiểu con trỏ ::struct gvector *):
 *
 *   #gvec_size(v) - Kích thước của v.
 *
 *   #gvec_capacity(v) - Dung lượng của v.
 *
 *   #gvec_arr(v) - Mảng các phần tử của v.
 *
 *   #gvec_elem(v, i) - Phần tử thứ i của v.
 *
 *   #gvec_idx_of(v, elem_ptr) - Chỉ số của phần tử được trỏ tới bởi
 *   elem_ptr trong v.
 *
 *   #gvec_append(v, val) - Thêm val vào sau phần tử cuối cùng trong v.
 *
 *   #gvec_remove(v, idx) - Xóa phần tử có chỉ số idx khỏi v.
 *
 *   #gvec_reserve(v, cap) - Chủ động thiết lập dung lượng cho v.
 *
 *   #gvec_free(v) - Giải phóng bộ nhớ được cấp phát cho v.
 *
 *   #gvec_clear(v) - Làm rỗng v
 *
 *   #gvec_qsort(v, cmp) - Sắp xếp các phần tử của v bằng qsort với hàm so sánh cmp.
 *
 *   #gvec_traverse(cur, v) - Duyệt các phần tử của v theo chiều thuận.
 */
struct gvector {
  /**
   * Mảng cơ sở lưu các đối tượng ::gtype
   */
  gtype *elems;

  /**
   * Kích thước: Số lượng phần tử đang có trong vec-tơ
   */
  long sz;

  /**
   * Dung lượng: Số lượng phần tử đã được cấp phát
   */
  long cap;

  /**
   * Con trỏ tới hàm giải phóng dữ liệu được quản lý qua đối tượng
   * ::gtype trong mảng. Nếu != NULL thì hàm được gọi khi giải phóng
   * bộ nhớ của vec-tơ, nếu ngược lại (== NULL) thì các gọi hàm được
   * bỏ qua.
   */

  /**
   * Tốc độ tăng dung lượng khi append (> 1, mặc định = 2):
   *   dung lượng mới = dung lượng cũ * scale
   */
  double scale;

  /**
   * Con trỏ hàm giải phóng bộ nhớ động của các phần tử.
   */
  gtype_free_t free_value;
};

/**
 * Hàm tạo đối tượng vec-tơ, khởi tạo các phần tử = 0.
 *
 * @param n Kích thước & dung lượng ban đầu của vec-tơ.
 * @param free_value con trỏ hàm giải phóng bộ nhớ bên ngoài được gắn
 * với đối tượng ::gtype. Sử dụng NULL nếu không có bộ nhớ bên ngoài.
 * @return Trả về đối tượng tạo được nếu thành công hoặc NULL nếu thất bại.
 * \memberof gvector
 */
struct gvector *gvec_create(long n, gtype_free_t free_value);

/**
 * Hàm tạo đối tượng vec-tơ ở dạng đầy đủ, khởi tạo các phần tử với tham số value.
 *
 * @param size Kích thước ban đầu của vec-tơ.
 * @param cap Dung lượng ban đầu của vec-tơ, phía gọi cần đảm bảo size <= cap.
 * @param value Giá trị ban đầu để khởi tạo các phần tử.
 * @param free_value con trỏ hàm giải phóng bộ nhớ bên ngoài được gắn
 * với đối tượng ::gtype. Sử dụng NULL nếu không có bộ nhớ bên ngoài.
 * @return Trả về đối tượng tạo được nếu thành công hoặc NULL nếu thất bại.
 * \memberof gvector
 */
struct gvector *gvec_create_full(long size, long cap, gtype value,
        gtype_free_t free_value);

/**
 * Hàm tạo bản sao đầy đủ của vec-tơ
 *
 * @param v Con trỏ vec-tơ
 * @return Con trỏ tới bản sao đầy đủ của vec-tơ, kiểu ::struct gvector *
 */
struct gvector *gvec_clone(struct gvector *v);

/**
 * Hàm kiểm tra 2 vec-tơ giống hệt nhau
 *
 * @param v1 Con trỏ vec-tơ thứ nhất
 * @param v2 Con trỏ vec-tơ thứ 2
 * @return 1 nếu giống nhau, 0 nếu ngược lại
 */
int gvec_identical(struct gvector *v1, struct gvector *v2);

/**
 * Trong vec-tơ size là số lượng phần tử mảng đã sử dụng,
 * còn capacity là số lượng phần tử đã được cấp phát cho
 * mảng cơ sở (elems), size luôn luôn <= cap.
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @return Trả về kích thước (size) của vec-tơ, giá trị có kiểu long.
 */
#define gvec_size(v) ((v)->sz)

/**
 * Trong vec-tơ size là số lượng phần tử mảng đã sử dụng,
 * còn capacity là số lượng phần tử đã được cấp phát cho
 * mảng cơ sở (arr), size luôn luôn <= cap.
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @return Trả về dung lượng (capacity) của vec-tơ, giá trị có kiểu long.
 */
#define gvec_capacity(v) ((v)->cap)

/**
 * Tỉ lệ tăng dung lượng của vec-tơ khi append
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *)
 * @return Trả về tỉ lệ tăng dung lượng (scale), có kiểu double.
 */
#define gvec_scale(v) ((v)->scale)

/**
 * Giao diện mảng của vec-tơ.
 * \code{.c}
 * gvec_arr(v)[0];  \\ Phần tử đầu tiên của vec-tơ v.
 * \endcode
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @return Trả về con trỏ tới phần tử đầu tiên của mảng (kiểu ::gtype *)
 */
#define gvec_arr(v) ((v)->elems)

/**
 * Chỉ định phần tử của vec-tơ bằng chỉ số.
 * \code{.c}
 *  gvec_elem(v, 0);  \\ Phần tử đầu tiên của vec-tơ v.
 * \endcode
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param i Chỉ số của phần tử, là số nguyên và < #gvec_size(v).
 * @return Phần tử có chỉ số i trong vec-tơ v, kết quả là lvalue có kiểu ::gtype.
 */
#define gvec_elem(v, i) (gvec_arr(v)[(i)])

/**
 * Chỉ số của phần tử trong vec-tơ.
 * \code{.c}
 * gvec_idx_of(&gvec_elem(v, i)) == i;
 * \endcode
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param elem_ptr Con trỏ tới 1 phần tử hợp lệ trong vec-tơ.
 * @return Chỉ số của phần tử được trỏ tới bởi elem_ptr.
 */
#define gvec_idx_of(v, elem_ptr) ((long)((elem_ptr) - gvec_arr(v)))

/**
 * Thay đổi dung lượng của vec-tơ, có thể tăng hoặc giảm
 * nhưng không giảm xuống < kích thước hiện có
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param newcap Dung lượng mới
 * @return Không trả về kết quả
 */
#define gvec_reserve(v, newcap) \
  do { \
    if ((newcap) < gvec_size(v)) { \
      break; \
    } \
    (v)->elems = realloc((v)->elems, newcap * sizeof(gtype)); \
    (v)->cap = newcap; \
  } while (0)

/**
 * Thay đổi kích thước của vec-tơ, có thể tăng hoặc giảm.
 * Kéo theo thay đổi dung lượng nếu kích thước mới >
 * dung lượng hiện tại.
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param newsz Kích thước mới
 * @return Không trả về kết quả
 */
#define gvec_resize(v, newsz) \
  do {\
    if (newsz > gvec_capacity(v)) { \
      gvec_reserve(v, newsz); \
    } else if (newsz < gvec_size(v) && (v)->free_value) { \
      for (long _j = newsz; _j < gvec_size(v); ++_j) { \
        (v)->free_value(gvec_elem(v, _j)); \
      }\
    }\
    gvec_size(v) = (newsz); \
  } while (0)

/**
 * Thêm giá trị val vào sau phần tử cuối cùng trong v và tăng kích thước lên 1.
 * Nếu vec-tơ đang đầy thì được tăng dung lượng trước khi thêm val.
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param val Giá trị được thêm vào vec-tơ v.
 * @return Không trả về giá trị.
 */
#define gvec_append(v, val) \
  do { \
    if (gvec_size(v) == 0) { \
      gvec_reserve(v, 10); \
    } else if (gvec_size(v) == gvec_capacity(v)) {\
      gvec_reserve(v, gvec_scale(v) * gvec_size(v)); \
    } \
    gvec_elem(v, gvec_size(v)) = val; \
    gvec_resize(v, gvec_size(v) + 1); \
  } while (0)

/**
 * Thêm giá trị vào vị trí theo chỉ số, có thể mở rộng vec-tơ nếu cần
 *
 * @param i Chỉ số
 * @param v Con trỏ tới vec-tơ, có kiểu ::struct gvector*
 * @param e Phần tử được đưa vào
 * @return Không trả về giá trị
 */
#define gvec_force_set(v, i, e) \
  do { \
    if ((i) >= gvec_size(v)) { \
      gvec_resize((v), (i) + 1); \
    } \
    gvec_elem(v, i) = e; \
  } while (0)

/**
 * Xóa phần tử có chỉ số idx khỏi vec-tơ v. Nếu v->free_value != NULL
 * thì gọi hàm v->free_value(gvec_elem(v, i)) - Giải phóng bộ nhớ
 * được gắn với đối tượng được xóa.
 * Nếu idx là chỉ số không hợp lệ thì không có thay đổi gì, nếu ngược lại
 * thì các phần tử có chỉ số > idx được dịch sang trái 1 vị trí, và kích
 * thước vec-tơ được giảm đi 1 đơn vị.
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param idx Chỉ số phần tử được xóa.
 */
#define gvec_remove(v, idx) \
  do { \
    gtype *_arr = gvec_arr(v); \
    long _sz = gvec_size(v); \
    if ((idx) >= _sz || (idx) < 0) { \
      break; \
    } \
    gtype _tmp = _arr[(idx)]; \
    for (long _i = (idx); _i < _sz - 1; ++_i) { \
      _arr[_i] = _arr[_i + 1]; \
    } \
    _arr[_sz - 1] = _tmp; \
    gvec_resize(v, _sz - 1); \
  } while (0)

/**
 * Duyệt tuần tự các phần tử của vec-tơ
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param cur Con trỏ tới phần tự hiện tại của vec-tơ trong vòng lặp,
 * có kiểu ::gtype *.
 */
#define gvec_traverse(cur, v) \
  for (gtype *cur = gvec_arr(v), *end = gvec_arr(v) + gvec_size(v); cur < end; ++cur)

/**
 * Làm rỗng vec-tơ: Giải phóng các vùng nhớ được cấp phát cho mảng
 * và các phần tử của v. Sau đó thiết lập lại kích thước vec-tơ = 0,
 * dung lượng vec-tơ không thay đổi.
 *
 * @param v Con trỏ tới đối tượng kiểu vec-tơ
 * @return Không trả về giá trị
 *
 * Tham khảo: #gvec_reserve(v, cap) - Thay đổi dung lượng
 */
#define gvec_clear(v) \
  gvec_resize(v, 0); \

/**
 * Giải phóng bộ nhớ được cấp phát cho v và các vùng nhớ ngoài
 * được gắn với các phần tử của vec-tơ nếu có (v->free_value != NULL).
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @return Không trả về giá trị.
 */
#define gvec_free(v) \
  do{ \
    gvec_clear(v); \
    free((v)->elems); \
    free(v); \
  } while (0)

/**
 * Hàm giải phóng bộ nhớ cho trường hợp con trỏ tới đối tượng vec-tơ
 * được lưu trong một cấu trúc lưu trữ khác.
 *
 * @param value Giá trị gtype đang chứa con trỏ tới vec-tơ.
 * Trước tiên value.v được ép kiểu thành struct gvector *,
 * sau đó con trỏ struct gvector * được truyền cho gvec_free.
 * @return Hàm không trả về giá trị.
 */
void gtype_free_gvec(gtype value);

/**
 * Sắp xếp các phần tử của vec-tơ sử dụng stdlib.h qsort.
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param cmp Con trỏ tới hàm so sách các đối tượng ::gtype được lưu trong vec-tơ.
 * Đối số cmp có kiểu như hàm so sánh cho qsort (stdlib.h).
 * Có nguyên mẫu là int (*)(const void *, const void *);
 * @return Không trả về giá trị.
 */
#define gvec_qsort(v, cmp) \
  qsort(gvec_arr(v), gvec_size(v), sizeof(gtype), cmp)

/**
 * Sắp xếp các phần tử của vec-tơ sử dụng quicksort (tự phát triển).
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param cmp Con trỏ tới hàm so sách các đối tượng ::gtype được lưu trong vec-tơ.
 * Có nguyên mẫu là int (*)(gtype, gtype), có kiểu là ::gtype_cmp_t.
 * @return Không trả về giá trị.
 */
#define gvec_quicksort(v, cmp) \
  quicksort(gvec_size(v), gvec_arr(v), cmp)

/**
 * Sắp xếp các phần tử của vec-tơ bằng giải thuật sắp xếp chọn.
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param cmp Con trỏ tới hàm so sách các đối tượng ::gtype được lưu trong vec-tơ.
 * Đối số cmp có kiểu gtype_cmp_t.
 * @return Không trả về giá trị.
 */
#define gvec_selsort(v, cmp) \
  selsort(gvec_size(v), gvec_arr(v), cmp)

/**
 * Sắp xếp các phần tử của vec-tơ bằng giải thuật sắp xếp chèn.
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param cmp Con trỏ tới hàm so sách các đối tượng ::gtype được lưu trong vec-tơ.
 * Đối số cmp có kiểu gtype_cmp_t.
 * @return Không trả về giá trị.
 */
#define gvec_insort(v, cmp) \
  insort(gvec_size(v), gvec_arr(v), cmp)

/**
 * Sắp xếp các phần tử của vec-tơ bằng giải thuật sắp xếp nổi bọt.
 *
 * @param v Con trỏ tới đối tượng vec-tơ (có kiểu struct gvector *).
 * @param cmp Con trỏ tới hàm so sách các đối tượng ::gtype được lưu trong vec-tơ.
 * Đối số cmp có kiểu gtype_cmp_t.
 * @return Không trả về giá trị.
 */
#define gvec_bubsort(v, cmp) \
  bubsort(gvec_size(v), gvec_arr(v), cmp);

/**
 * Xuất các phần tử của vec-tơ
 *
 * @param v Con trỏ tới đối tượng vec-tơ
 * @param pp Hàm xuất giá trị gtype
 */
void gvec_pprint(struct gvector *v, gtype_print_t pp);

/**
 * Sinh ngẫu nhiên 1 vec-tơ chứa giá trị kiểu long
 *
 * @param n Số lượng phần tử
 * @return Con trỏ tới vec-tơ được tạo, có kiểu ::struct gvector *
 */
struct gvector *gvec_rand_l(long n);

#define gvec_fill(v, value) \
  do { \
    gvec_traverse(_cur, v) { \
      *_cur = (value); \
    } \
  } while (0)

#endif  // VEC_GVEC_H_

/***********************************
 * vec/heap.h
 ***********************************/

#ifndef VEC_HEAP_H_
#define VEC_HEAP_H_

#define HTOP(i) (((i) - 1) >> 1)
#define HLEFT(i) (((i) << 1) + 1)
#define HRIGHT(i) (((i) << 1) + 2)

void heap_shift_down(long i, long n, gtype *a, gtype_cmp_t cmp);
void heap_shift_up(long i, gtype *a, gtype_cmp_t cmp);

/**
 * Sử dụng hàm so sánh thuận cho kết quả là đống cực đại,
 * Sử dụng hàm so sánh nghịch cho kết quả là đống cực tiểu.
 */
void heap_make(long n, gtype *a, gtype_cmp_t cmp);

#endif  // VEC_HEAP_H_

/***********************************
 * vec/hmap.h
 ***********************************/

#ifndef VEC_HMAP_H_
#define VEC_HMAP_H_

/** @file
 * @brief Bảng băm khái quát với khóa & giá trị kểu ::gtype
 *
 * Bảng băm là 1 dạng ánh xạ khóa => giá trị dựa trên hàm băm.
 * Trong triển khai này dữ liệu được lưu trong 1 mảng cấp phát
 * động. Xung đột chỉ số (khi 2 khóa khác nhau được quy chiếu
 * về cùng 1 chỉ số) trong triển khai này được xử lý theo cơ chế
 * địa chỉ mở (Open addressing).
 */

/**
 * \headerfile "all.h"
 * Cấu trúc điều khiển đối tượng bảng băm. Được tạo bằng hàm
 * hmap_create(gtype_hash_t hash_func, gtype_cmp_t cmp, gtype_free_t free_key, gtype_free_t free_value)
 *
 * Các macro hỗ trợ:
 *   #hmap_size(map) - Kích thước của map
 *
 *   #hmap_traverse(k, v, map) - Duyệt tuần tự các cặp trong map.
 *
 *   #hmap_free(map) - Giải phóng bộ nhớ của map
 *
 */
struct hmap {
  int capacity;
  int mod;
  uint mask;
  int size;
  int noccupied;
  atype(gtype) *keys;
  atype(gtype) *values;
  atype(uint) *hashes;
  gtype_hash_t hash_func;
  gtype_cmp_t cmp;
  gtype_free_t free_key;
  gtype_free_t free_value;
};

/**
 * \headerfile "all.h"
 * Kiểu trả về của hàm hmap_insert(struct hmap *tab, gtype key, gtype value)
 */
struct hmap_ires {
  /**
   * Con trỏ tới giá trị đang được lưu trong bảng băm.
   */
  gtype *value;

  /**
   * Trạng thái: <br>
   *    = 1 Nếu đã thêm vào, <br>
   *    = 0 Nếu khóa đã tồn tại nên chưa được thêm vào.
   */
  int inserted;
};

#define hmap_hash_at(map, idx) ((map)->hashes[(idx)])
#define hmap_key_at(map, idx) ((map)->keys[(idx)])
#define hmap_value_at(map, idx) ((map)->values[(idx)])

/**
 * Hàm tạo đối tượng bảng băm.
 *
 * @param hash_func Hàm băm khóa, phải != NULL.
 * Tham khảo: gtype_cmp_s(gtype v1, gtype v2).
 * @param cmp Hàm so sánh khóa, phải != NULL.
 * @param free_key Hàm giải phóng bộ nhớ động bên ngoài được gắn với khóa.
 * Được gọi khi xóa dữ liệu nếu != NULL. Trong trường hợp không
 * có bộ nhớ ngoài hoặc không cần xóa bộ nhớ ngoài (như tình huống đang chia sẻ bộ nhớ
 * với 1 câu trúc lưu trữ khác) thì để = NULL.
 * @param free_value Hàm giải phóng bộ nhớ động bên ngoài được gắn với giá trị.
 * Tương tự free_key.
 * @return Con trỏ tới đối tượng bảng băm được tạo. (Hiện chưa xử lý kịch)
 * bản lỗi cấp phát bộ nhớ.
 *
 * \memberof hmap
 *
 * Tham khảo: rbm_create(gtype_cmp_t cmp, gtype_free_t free_key, gtype_free_t free_value)
 */
struct hmap *hmap_create(gtype_hash_t hash_func, gtype_cmp_t cmp,
          gtype_free_t free_key, gtype_free_t free_value);

/**
 * Thêm cặp (key, value) vào bảng tab. Nếu key đã tồn tại thì
 * bỏ qua, có thể truy cập giá trị đang được gắn với khóa đã có
 * trong bảng băm bằng con trỏ ::struct hmap_ires::value trong kết quả
 * được trả về.
 *
 * @param tab Con trỏ tới bảng băm
 * @param key Khóa được thêm vào
 * @param value Giá trị giá trị được thêm vào
 * @return Trả về đối tượng ::struct hmap_ires
 *
 * \memberof hmap
 *
 * Tham khảo: rbm_insert(struct rbmtree *t, gtype key, gtype value)
 */
struct hmap_ires hmap_insert(struct hmap *tab, gtype key, gtype value);

/**
 * Thêm cặp (key, value) vào bảng tab. Nếu key đã tồn tại thì
 * bỏ qua và trả về con trỏ tới giá trị đang có trong tab.
 *
 * @param tab Con trỏ tới bảng băm
 * @param key Khóa được thêm vào
 * @param value Giá trị giá trị được thêm vào
 * @return Trả về con trỏ tới giá trị đang được gắn với key
 * trong tab nếu đã có, hoặc trả về NULL nếu không.
 * Giá trị trả về có kiểu ::gtype*
 *
 * \memberof hmap
 *
 * Tham khảo: rbm_put(struct rbmtree *t, gtype key, gtype value)
 */
gtype *hmap_put(struct hmap *tab, gtype key, gtype value);

/**
 * Tra cứu giá trị trong bảng tab theo key.
 *
 * @param tab Con trỏ tới bảng băm.
 * @param key Khóa được sử dụng để tìm kiếm.
 * @return Trả về con trỏ tới giá trị đang được gắn với key trong tab
 * nếu tồn tại, hoặc NULL nếu ngược lại.
 *
 * \memberof hmap
 *
 * Tham khảo: rbm_value(struct rbmtree *t, gtype key)
 */
gtype *hmap_value(struct hmap *tab, gtype key);

/**
 * Nếu key không có trong tab thì bỏ qua, nếu ngược lại thì xóa cặp
 * khóa & giá trị tương ứng trong tab. Các hàm free_key và free_value được
 * gọi nếu != NULL.
 *
 * @param tab Con trỏ tới bảng băm
 * @param key Khóa cần xóa.
 * @return 1 Nếu tồn tại khóa sau khi xóa dữ liệu, 0 nếu ngược lại.
 *
 * \memberof hmap
 *
 * Tham khảo: rbm_remove(struct rbmtree *t, gtype key)
 */
int hmap_remove(struct hmap *tab, gtype key);

/**
 * Số lượng cặp khóa & giá trị đang được lưu trong hmap.
 *
 * @param tab Con trỏ tới đối tượng bảng băm.
 * @return Trả về số lượng cặp khóa/giá trị đang được lưu trong tab.
 *
 */
#define hmap_size(tab) (tab->size)

/**
 * Giải phóng bộ nhớ được cấp phát cho bảng băm tab. Các hàm free_key và
 * free_value được gọi cho từng khóa và từng giá trị nếu != NULL.
 *
 * @param tab Con trỏ tới bảng băm.
 */
#define hmap_free(tab) \
  do { \
    int _capacity = (tab)->capacity; \
    for (int _i = 0; _i < _capacity; ++_i) { \
      if (HASH_IS_REAL(tab->hashes[_i])) { \
        if ((tab)->free_key) { \
          (tab)->free_key(hmap_key_at(tab, _i)); \
        } \
        if ((tab)->free_value) { \
          (tab)->free_value(hmap_value_at(tab, _i)); \
        } \
      } \
    } \
    arr_free((tab)->keys); \
    arr_free((tab)->values); \
    arr_free((tab)->hashes); \
    free(tab); \
  } while (0)

/**
 * Làm rỗng bảng băm tab. Các hàm free_key và free_value được gọi
 * cho các cặp khóa và giá trị nếu != NULL.
 *
 * @param tab Con trỏ tới bảng băm.
 * @return Hàm không trả về giá trị.
 *
 * \memberof hmap
 */
void hmap_clear(struct hmap *tab);

/**
 * Hàm giải phóng bộ nhớ cho trường hợp con trỏ tới đối tượng bảng bắm
 * được lưu trong một cấu trúc lưu trữ khác.
 *
 * @param value Giá trị gtype đang chứa con trỏ tới bảng băm.
 * Trước tiên value.v được ép kiểu thành ::struct hmap *, sau đó con trỏ ::struct hmap *
 * được truyền cho hmap_free.
 * @return Hàm không trả về giá trị.
 *
 * \memberof gtype
 */
void gtype_free_hmap(gtype value);

gtype *hmap_next_pkey(struct hmap *, gtype*);
gtype *hmap_next_pvalue(struct hmap *, gtype*);

/**
 * Duyệt tuần tự các cặp khóa và giá trị trong map. Các tham số key và value
 * là các định danh do người sử dụng tự đặt và sẽ có kiểu ::gtype *
 *
 * @param key Định danh sẽ được sử dụng như con trỏ tới khóa.
 * @param value Định danh sẽ được sử dụng như giá trị tới khóa.
 * @param map Con trỏ tới bảng băm.
 *
 * Tham khảo: #rbm_traverse(k, v, map)
 */
#define hmap_traverse(key, value, map) \
  for(gtype *key = hmap_next_pkey(map, NULL), *value = hmap_next_pvalue(map, NULL); \
     key && value; key = hmap_next_pkey(map, key), value = hmap_next_pvalue(map, value)) \

#endif  // VEC_HMAP_H_

/***********************************
 * vec/hset.h
 ***********************************/

#ifndef VEC_HSET_H_
#define VEC_HSET_H_

/**
 * Tập hợp dựa trên hàm băm (hash hs)
 */

struct hset {
  int capacity;
  int mod;
  uint mask;
  int size;
  int noccupied;
  atype(gtype) *keys;
  atype(uint) *hashes;
  gtype_hash_t hash_func;
  gtype_cmp_t cmp;
  gtype_free_t free_key;
};

#define hset_hash_at(set, idx) ((set)->hashes[idx])
#define hset_key_at(set, idx) ((set)->keys[idx])
#define INDEX_NOT_FOUND -1
#define hset_contains(hs, value) (hset_index_of(hs, value) != INDEX_NOT_FOUND)

struct hset *hset_create(gtype_hash_t hash_func, gtype_cmp_t cmp,
          gtype_free_t free_key);
int hset_insert(struct hset *hs, gtype key);
int hset_index_of(struct hset *hs, gtype key);
int hset_remove(struct hset *hs, gtype key);
void hset_clear(struct hset *hs);
void hset_free(struct hset *hs);

/**
 * Hàm giải phóng bộ nhớ cho trường hợp con trỏ tới đối tượng hset
 * được lưu trong một cấu trúc lưu trữ khác.
 *
 * @param value Giá trị gtype đang chứa con trỏ tới đối tượng hset.
 * Con trỏ value.hset (kiểu ::struct hset *) được truyền cho hset_free.
 * @return Hàm không trả về giá trị.
 */
void gtype_free_hset(gtype value);

#define hset_size(hs) (hs->size)

gtype *hset_next_pkey(struct hset *, gtype*);
#define hset_traverse(key, map) \
  for(gtype *key = hset_next_pkey(map, NULL); \
     key; key = hset_next_pkey(map, key)) \

#endif  // VEC_HSET_H_

/***********************************
 * vec/p1w.h
 ***********************************/

#ifndef VEC_P1W_H_
#define VEC_P1W_H_

/**
 * @file
 * @brief Triển khai hàng đợi ưu tiên 1 chiều dựa trên cấu trúc đống.
 */

struct p1way {
  atype(gtype) *elems;
  gtype_cmp_t cmp;
};

/**
 * Hàm tạo hàng đợi ưu tiên, sử dụng hàm so sánh thuận cho kết quả
 * là hàng đợi ưu tiên giá trị cực đại (đống cực đại),
 * sử dụng hàm so sánh nghịch cho kết quả là hàng đợi ưu tiên
 * giá trị cực tiểu
 *
 * @param cmp Hàm so sánh
 * @return Con trỏ hàng đợi ưu tiên, có kiểu struct p1way*
 */
struct p1way *p1w_create(gtype_cmp_t cmp);
gtype p1w_peek(struct p1way *h);
gtype p1w_dequeue(struct p1way *h);
void p1w_enqueue(struct p1way *h, gtype value);
void p1w_root(struct p1way *h, gtype value);

#define p1w_size(h) \
  (arr_size((h)->elems))

#define p1w_is_empty(h) \
  (p1w_size(h) == 0)

#define p1w_free(h) \
  do { \
    arr_free((h)->elems); \
    free(h); \
  } while (0)

#define p1w_clear(h) \
  arr_clear((h)->elems)

/**
 * Hàm giải phóng bộ nhớ cho trường hợp con trỏ tới đối tượng p1w
 * được lưu trong một cấu trúc lưu trữ khác.
 *
 * @param value Giá trị gtype đang chứa con trỏ tới đối tượng p1w.
 * Con trỏ value.p1w (kiểu ::struct p1way *) được truyền cho p1w_free.
 * @return Hàm không trả về giá trị.
 */
void gtype_free_p1w(gtype value);

#define p1w_arr(h) ((h)->elems)

#endif  // VEC_P1W_H_

/***********************************
 * vec/p2w.h
 ***********************************/

#ifndef VEC_P2W_H_
#define VEC_P2W_H_

struct p2ways {
  atype(gtype) *data;
  atype(long) *index;
  atype(long) *index2;
  gtype_cmp_t cmp;
};

struct p2ways *p2w_create(gtype_cmp_t cmp);

#define p2w_free(h) \
  do { \
    arr_free((h)->data); \
    arr_free((h)->index); \
    arr_free((h)->index2); \
    free(h); \
  } while (0)

#define p2w_size(h) arr_size((h)->data)

#define p2w_clear(h) \
  do { \
    arr_clear((h)->data); \
    arr_clear((h)->index); \
    arr_clear((h)->index2); \
  } while (0)

#define p2w_is_empty(h) (p2w_size(h) == 0)

int p2w_push_with_index(struct p2ways *h, long idx, gtype elem);
gtype p2w_max(const struct p2ways *h);
long p2w_max_index(const struct p2ways *h);
gtype p2w_get(const struct p2ways *h, long idx);
gtype p2w_delete_max(struct p2ways *h);
gtype p2w_deactivate_max(struct p2ways *h);
gtype p2w_delete_max_index(struct p2ways *h, long *idx);
int p2w_modify(struct p2ways *h, long idx, gtype elem);
int p2w_check(struct p2ways *h);

/**
 * Hàm giải phóng bộ nhớ cho trường hợp con trỏ tới đối tượng p2w
 * được lưu trong một cấu trúc lưu trữ khác.
 *
 * @param value Giá trị gtype đang chứa con trỏ tới đối tượng p2w.
 * Con trỏ value.p2w (kiểu ::struct p2ways *) được truyền cho p2w_free.
 * @return Hàm không trả về giá trị.
 */
void gtype_free_p2w(gtype value);

#endif  // VEC_P2W_H_

/***********************************
 * vec/spec/d2w.h
 ***********************************/

#ifndef VEC_SPEC_D2W_H_
#define VEC_SPEC_D2W_H_

/**
 * Hàng đợi ưu tiên truy cập 2 chiều với kiểu double
 */

int d2w_push_with_index(struct p2ways *h, long idx, double elem);
double d2w_max(const struct p2ways *h);
double d2w_get(const struct p2ways *h, long idx);
double d2w_delete_max(struct p2ways *h);
double d2w_deactivate_max(struct p2ways *h);
double d2w_delete_max_index(struct p2ways *h, long *idx);
int d2w_modify(struct p2ways *h, long idx, double elem);

#endif  // VEC_SPEC_D2W_H_

/***********************************
 * list/dll.h
 ***********************************/

#ifndef LIST_DLL_H_
#define LIST_DLL_H_

/** @file
 * @brief Triển khai danh sách móc nối kép chỉ bao gồm cấu trúc liên kết.
 * Có thể kế thừa triển khai này để tạo danh sách chứa dữ liệu trong các nút.
 * Generic double Linked List (DLL/dll)
 */

/**
 * Cấu trúc mô tả nút của danh sách móc nối kép.
 * dlnode = double linked node
 */
struct dlnode {
  struct dlnode *next;
  struct dlnode *prev;
};

/**
 * Cấu trúc mô tả danh sách móc nối kép
 * dllist = double linked list
 */
struct dllist {
  struct dlnode *front;
  struct dlnode *back;
  long length;
};

typedef void (*dll_node_print_t)(struct dlnode *);

#define dll_node(n) ((struct dlnode *)(n))
#define dll_list(list) ((struct dllist *)(list))
#define dll_front(list) (dll_list(list)->front)
#define dll_back(list) (dll_list(list)->back)
#define dll_length(list) (dll_list(list)->length)

#define dll_traverse(cur, list) \
  for (struct dlnode *cur = (list)->front; cur != NULL; cur = cur->next)

#define dll_rtraverse(cur, list) \
  for (struct dlnode *cur = (list)->back; cur != NULL; cur = cur->prev)

#define dll_is_empty(list) \
  (dll_front(list) == NULL && dll_back(list) == NULL)

#define dll_free(list) \
  do {\
    while (!dll_is_empty(list)) { \
      dll_pop_front(list); \
    } \
    free(list); \
  } while(0)

#define dll_clear(list) \
  do {\
    while (!dll_is_empty(list)) { \
      dll_pop_front(list); \
    } \
  } while (0)

struct dlnode *dll_create_node();
struct dllist *dll_create_list();
void dll_push_back(struct dllist *list, struct dlnode *nn);
void dll_push_front(struct dllist *list, struct dlnode *nn);
void dll_pop_front(struct dllist *list);
void dll_pop_back(struct dllist *list);
void dll_inserta(struct dllist *list, struct dlnode *pos, struct dlnode *nn);
void dll_insertb(struct dllist *list, struct dlnode *pos, struct dlnode *nn);
void dll_erase(struct dllist *list, struct dlnode *pos);
void dll_node_print_address(struct dlnode *);
void dll_pprint(struct dllist *, dll_node_print_t);

#endif  // LIST_DLL_H_

/***********************************
 * list/gdl.h
 ***********************************/

#ifndef LIST_GDL_H_
#define LIST_GDL_H_

/** @file
 * @brief Triển khai danh sách móc nối kép cho kiểu gtype
 * Kế thừa ::struct dllist
 */

/**
 * gtype double linked list
 */

/**
 * Cấu trúc nút của danh sách móc nối kép với kiểu gtype.
 * gdlnode = gtype double linked (list) node
 */
struct gdlnode {
  struct dlnode base;
  gtype value;
};

/**
 * Cấu trúc điều khiển của danh sách móc nối kép với kiểu gtype
 * gdllist = gtype double linked list
 */
struct gdllist {
  struct dllist base;
  gtype_free_t free_value;
};

#define gdl_node(n) ((struct gdlnode *)(n))
#define gdl_list(lst) ((struct gdllist *)(lst))

#define gdl_node_value(n) (gdl_node(n)->value)
#define gdl_node_next(n) (gdl_node(dll_node(n)->next))
#define gdl_node_prev(n) (gdl_node(dll_node(n)->prev))

#define gdl_front(list) (gdl_node(dll_front(list)))
#define gdl_back(list) (gdl_node(dll_back(list)))
#define gdl_is_empty(list) (gdl_front(list) == NULL && gdl_back(list) == NULL)
#define gdl_push_front(list, value) \
  do { \
    struct gdlnode *_nn = gdl_create_node(value); \
    dll_push_front(dll_list(list), dll_node(_nn)); \
  } while (0)

#define gdl_push_back(list, value) \
  do { \
    struct gdlnode *_nn = gdl_create_node(value); \
    dll_push_back(dll_list(list), dll_node(_nn)); \
  } while (0)

#define gdl_pop_front(list) \
  do { \
    struct gdlnode *_tmp = gdl_front(list); \
    if (!_tmp) { \
      break; \
    } \
    if ((list)->free_value) { \
      (list)->free_value(gdl_node_value(_tmp)); \
    } \
    dll_pop_front(list); \
  } while (0)

#define gdl_pop_back(list) \
  do { \
    struct gdlnode *_tmp = gdl_back(list); \
    if (!_tmp) { \
      break; \
    } \
    if ((list)->free_value) { \
      (list)->free_value(gdl_node_value(_tmp)); \
    } \
    dll_pop_back(list); \
  } while (0)

#define gdl_clear(list) \
  do { \
    while (!gdl_is_empty(list)) { \
      gdl_pop_front(list); \
    } \
  } while (0)

#define gdl_free(list) \
  do { \
    gdl_clear(list); \
    free(list); \
  } while (0)

#define gdl_inserta(list, pos, value) \
  do { \
    struct gdlnode *_nn = gdl_create_node(value); \
    dll_inserta(dll_list(list), dll_node(pos), dll_node(_nn)); \
  } while (0)

#define gdl_insertb(list, pos, value) \
  do { \
    struct gdlnode *_nn = gdl_create_node(value); \
    dll_insertb(dll_list(list), dll_node(pos), dll_node(_nn)); \
  } while (0)

static inline gtype* gdl_first_value(struct gdllist *lst) {
  struct gdlnode *n = gdl_front(lst);
  return n? &n->value: NULL;
}

static inline gtype* gdl_next_value(gtype *cur) {
  struct gdlnode *nn = gdl_node_next(container_of(cur, struct gdlnode, value));
  return nn? &nn->value: NULL;
}

#define gdl_traverse(cur, list) \
  for (gtype *cur = gdl_first_value(list); cur; cur = gdl_next_value(cur))

struct gdlnode *gdl_create_node(gtype value);
struct gdllist *gdl_create_list(gtype_free_t free_value);
long gdl_size(struct gdllist *list);
void gtype_free_gdl(gtype value);
void gdl_pprint(struct gdllist *list, gtype_print_t pp);

#endif  // LIST_GDL_H_

/***********************************
 * list/sll.h
 ***********************************/

#ifndef LIST_SLL_H_
#define LIST_SLL_H_

/** @file
 * @brief Triển khai khái quát danh sách móc nối đơn chỉ bao gồm
 * cấu trúc liên kết.
 * Có thể kế thừa để lưu dữ liệu trong các nút.
 *
 */

/**
 * Cấu trúc mô tả nút của danh sách móc nối đơn
 * slnode = single linked node
 */
struct slnode {
  struct slnode *next;
};

/**
 * Cấu trúc mô tả danh sách móc nối đơn
 * sll = single linked list
 */
struct sllist {
  struct slnode *front;
  struct slnode *back;
  long length;
};

typedef void (*sll_node_print_t)(struct slnode *);

#define sll_node(n) ((struct slnode *)(n))
#define sll_list(list) ((struct sllist *)(list))
#define sll_front(list) (sll_list(list)->front)
#define sll_back(list) (sll_list(list)->back)
#define sll_length(list) (sll_list(list)->length)

#define sll_traverse(cur, list) \
  for (struct slnode *cur = list->front; cur != NULL; cur = cur->next)

#define sll_free(list) \
  do { \
    while (!sll_is_empty(list)) { \
      sll_pop_front(list); \
    } \
    free(list); \
  } while (0)

#define sll_is_empty(list) \
  (sll_front(list) == NULL && sll_back(list) == NULL)

struct slnode *sll_create_node();
struct sllist *sll_create_list();
void sll_push_back(struct sllist *list, struct slnode *node);
void sll_push_front(struct sllist *list, struct slnode *node);
void sll_inserta(struct sllist *list, struct slnode *pos, struct slnode *nn);
void sll_pop_front(struct sllist *list);
void sll_node_print_address(struct slnode *node);
void sll_pprint(struct sllist *list, sll_node_print_t npp);

#endif  // LIST_SLL_H_

/***********************************
 * list/gsl.h
 ***********************************/

#ifndef LIST_GSL_H_
#define LIST_GSL_H_

/**
 * gtype single linked list
 */

/** @file
 * @brief Triển khai danh sách móc nối đơn cho gtype
 * Kế thừa ::struct sllist
 */

/**
 * Cấu trúc nút của danh sách móc nối đơn với kiểu gtype
 * gslnode = gtype single linked (list) node
 */
struct gslnode {
  struct slnode base;
  gtype value;
};

/**
 * Cấu trúc điều khiển của danh sách móc nối kép của kiểu gtype
 * gsllist = gtype single linked list
 */
struct gsllist {
  struct sllist base;
  gtype_free_t free_value;
};

#define gsl_node(n) ((struct gslnode *)(n))
#define gsl_list(l) ((struct gsllist *)(l))

#define gsl_node_value(n) (gsl_node(n)->value)
#define gsl_node_next(n) (gsl_node(sll_node(n)->next))
#define gsl_front(list) (gsl_node(sll_front(list)))
#define gsl_back(list) (gsl_node(sll_back(list)))
#define gsl_length(list) (sll_length(list))
#define gsl_push_back(list, value) \
  do { \
    struct gslnode *_nn = gsl_create_node(value); \
    sll_push_back(sll_list(list), sll_node(_nn)); \
  } while (0)

#define gsl_push_front(list, value) \
  do { \
    struct gslnode *_nn = gsl_create_node(value); \
    sll_push_front(sll_list(list), sll_node(_nn)); \
  } while (0)

#define gsl_pop_front(list) \
  do { \
    struct gslnode *_nn = gsl_front(list); \
    if (!_nn) { \
      break; \
    } \
    if ((list)->free_value) { \
      (list)->free_value(_nn->value); \
    } \
    sll_pop_front(list); \
  } while (0)

#define gsl_inserta(list, pos, value) \
  do { \
    struct gslnode *_nn = gsl_create_node(value); \
    sll_inserta(sll_list(list), sll_node(pos), sll_node(_nn)); \
  } while (0)

#define gsl_is_empty(list) (sll_is_empty(sll_list(list)))
#define gsl_clear(list) \
  do {\
    while (!gsl_is_empty(list)) { \
      gsl_pop_front(list); \
    } \
  } while (0)

#define gsl_free(list) \
  do {\
      gsl_clear(list); \
      free(list); \
  } while (0)

static inline gtype *gsl_front_value(struct gsllist *l) {
  struct gslnode *tmp = gsl_front(l);
  return tmp? &tmp->value: NULL;
}

static inline gtype *gsl_next_value(gtype *cur) {
  struct gslnode *nn = gsl_node_next(container_of(cur, struct gslnode, value));
  return nn? &nn->value: NULL;
}

#define gsl_traverse(cur, list) \
  for (gtype *cur = gsl_front_value(list); cur; cur = gsl_next_value(cur))

struct gslnode *gsl_create_node(gtype value);
struct gsllist *gsl_create_list(gtype_free_t free_value);
void gtype_free_gsl(gtype value);
void gsl_pprint(struct gsllist *l, gtype_print_t pp);

#endif  // LIST_GSL_H_

/***********************************
 * list/que.h
 ***********************************/

#ifndef LIST_QUE_H_
#define LIST_QUE_H_

/* Triển khai hàng đợi FIFO dựa trên danh sách móc nối đơn
   cho gtype */

struct gsllist *que_enq(struct gsllist *q, gtype value);
struct gsllist *que_deq(struct gsllist *q);
gtype que_peek(struct gsllist *q);

#define que_size(q) (gsl_length(q))
#define que_is_empty(q) (gsl_is_empty(q))

#endif  // LIST_QUE_H_

/***********************************
 * list/stk.h
 ***********************************/

#ifndef LIST_STK_H_
#define LIST_STK_H_

/* Triển khai khái quát ngăn xếp dựa trên danh sách móc nối đơn
   với gtype */

struct gsllist *stk_push(struct gsllist *stk, gtype value);
struct gsllist *stk_pop(struct gsllist *stk);
gtype stk_top(struct gsllist *stk);

#define stk_is_empty(stk) (gsl_is_empty(stk))
#define stk_size(stk) (gsl_length(stk))

#endif  // LIST_STK_H_

/***********************************
 * list/spec/idl.h
 ***********************************/

#ifndef LIST_SPEC_IDL_H_
#define LIST_SPEC_IDL_H_

/** @file
 * @brief Kế thừa dllist cho kiểu int
 * Int Double Linked List
 */

/**
 * Cấu trúc nút của danh sách móc nối kép
 * với kiểu int.
 * idlnode = int double linked (list) node
 */
struct idlnode {
  struct dlnode base;
  int value;
};

#define idl_node(n) ((struct idlnode *)(n))
#define idl_node_value(n) (idl_node(n)->value)
#define idl_front(list) (idl_node(dll_front(list)))
#define idl_back(list) (idl_node(dll_back(list)))
#define idl_front_value(list) (idl_front(list)->value)
#define idl_back_value(list) (idl_back(list)->value)

struct idlnode *idl_create_node(int value);

void idl_push_back(struct dllist *list, int value);
void idl_push_front(struct dllist *list, int value);
int idl_pop_back(struct dllist *list);
int idl_pop_front(struct dllist *list);
void idl_insert_a(struct dllist *list, struct idlnode *pos, int value);
void idl_insertb(struct dllist *list, struct idlnode *pos, int value);

void idn_pprint(struct idlnode *);
void idl_pprint(struct dllist *);

#endif // LIST_SPEC_IDL_H_

/***********************************
 * list/spec/isl.h
 ***********************************/

#ifndef LIST_SPEC_ISL_H_
#define LIST_SPEC_ISL_H_

/**
 * LIFO và FIFO với kiểu long (đóng gói sllist)
 */

/**
 * Cấu trúc nút của danh sách móc nối đơn kiêu int
 * islnode = int single linked (list) node
 */
struct islnode {
  struct slnode base;
  long value;
};

void isn_pprint(struct slnode *node);
void isl_pprint(struct sllist *list);

#define isl_node(n) ((struct islnode *)(n))
#define isl_node_value(n) (isl_node(n)->value)

struct slnode *isl_create_node(long value);
void isl_stack_push(struct sllist *list, long value);
long isl_stack_pop(struct sllist *list);
long isl_stack_top(struct sllist *list);

/* Giao diện queue kiểu long dựa trên giao diện sll */
void isl_fifo_enq(struct sllist *list, long value);
long isl_fifo_deq(struct sllist *list);
long isl_fifo_peek(struct sllist *list);

#endif  // LIST_SPEC_ISL_H_

/***********************************
 * tree/bn.h
 ***********************************/

#ifndef TREE_BN_H_
#define TREE_BN_H_

/** @file
 * @brief Triển khai cây nhị phân khái quát chỉ bao gồm cấu trúc liên kết
 * Có thể được kế thừa để lưu giữ liệu trong các nút.
 */

/**
 * Cấu trúc nút của cây nhị phân
 * bnnode = binary (tree) node
 */
struct bnnode {
  struct bnnode *left;
  struct bnnode *right;
  struct bnnode *top;
};

#define bn_node(n) ((struct bnnode *)(n))
#define bn_connect1(n1, link, n2) bn_node(n1)->link = bn_node(n2)
#define bn_connect2(n1, link1, n2, link2) bn_connect1(n1, link1, n2); \
    bn_connect1(n2, link2, n1)

/**
 * Cấu trúc điều khiển của cây nhị phân
 * bntree = binary tree
 */
struct bntree {
  struct bnnode *root;
};

#define bn_tree(t) ((struct bntree *)(t))

typedef int (*bn_callback_t)();
typedef int (*bn_compare_t)(struct bnnode *, struct bnnode *, struct bntree *);
typedef void (*bn_node_print_t)();

struct bnnode *bn_create_node();
#define bn_free_node(n) free(n)

struct bntree *bn_create_tree(struct bnnode *root);
void bn_free_tree(struct bntree *t);
void bn_foreach_lrn(struct bntree *t, bn_callback_t op, void *u);
void bn_foreach_lnr(struct bntree *t, bn_callback_t op, void *u);
void bn_foreach_rnl(struct bntree *t, bn_callback_t op, void *u);
void bn_pprint(struct bntree *t, bn_node_print_t p);
long bn_size(struct bntree *t);
long bn_distance(struct bnnode *n);  // số lượng cạnh tới gốc
long bn_edge_height(struct bntree *t);

struct bnnode *bn_left_deepest_node(struct bnnode *node);
struct bnnode *bn_next_postorder(struct bnnode *node);
struct bnnode *bn_first_postorder(struct bnnode *n);
struct bnnode *bn_left_most(struct bnnode *x);
struct bnnode *bn_right_most(struct bnnode *x);
struct bnnode *bn_next_inorder(struct bnnode *x);
struct bnnode *bn_prev_inorder(struct bnnode *x);

#define bn_traverse_lrn(cur, tree) \
  for (struct bnnode *cur = bn_first_postorder(tree->root); \
       cur != NULL; cur = bn_next_postorder(cur))

#define bn_traverse_lnr(cur, tree) \
  for (struct bnnode *cur = bn_left_most(tree->root); \
       cur != NULL; cur = bn_next_inorder(cur))

#define bn_traverse_rnl(cur, tree) \
  for (struct bnnode *cur = bn_right_most(tree->root); \
       cur != NULL; cur = bn_prev_inorder(cur))

#define bn_pprint_inline(t, pad, step, npp) \
    bn_traverse_rnl(cur, t) { \
      printf("%*s", (int)(pad + step * bn_distance(cur)), ""); \
      npp(cur); \
    }

#define bn_clear_tree(t) \
  do { \
    struct bnnode *_tmp = NULL; \
    bn_traverse_lrn(_cur, (t)) { \
      free(_tmp); \
      _tmp = _cur; \
    } \
    free(_tmp); \
    (t)->root = NULL; \
  } while (0)

#define bn_is_empty(t) ((t)->root == NULL)

#define bn_change_child(old_node, new_node, parent, t) \
  do { \
    if (parent) { \
      if (parent->left == old_node) { \
        parent->left = new_node; \
      } else { \
        parent->right = new_node; \
      } \
    } else { \
      t->root = new_node; \
    } \
  } while (0)

#define bn_insert(nn, loc, par) \
    *loc = nn; \
    nn->top = par \

/* Xoay từ trái sang phải hoặc xoay từ phải sang trái */
/* x là trục xoay */
#define bn_rotate(t, x, right, left) \
  do { \
    struct bnnode *_y = (x)->right; \
    (x)->right = _y->left; \
    if (_y->left != NULL) { \
      _y->left->top = (x); \
    } \
    _y->top = (x)->top; \
    bn_change_child(x, _y, (x)->top, t); \
    _y->left = (x); \
    (x)->top = _y; \
  } while (0)

#define bn_ntref(n, t) \
 (bn_node(n)->top? \
    (bn_node(n)->top->left == n? \
      &(bn_node(n)->top->left): \
      &(bn_node(n)->top->right)): \
    &bn_tree(t)->root)

#define bn_recover(o, n, t) \
   do { \
     if (bn_node(n)->top) { \
       if (bn_node(n)->top->left == bn_node(o)) { \
         bn_node(n)->top->left = bn_node(n); \
       } else { \
         bn_node(n)->top->right = bn_node(n); \
       } \
     } else { \
       bn_tree(t)->root = bn_node(n); \
     } \
     if (bn_node(n)->left) { \
       bn_node(n)->left->top = bn_node(n); \
     } \
     if (bn_node(n)->right) { \
       bn_node(n)->right->top = bn_node(n); \
     } \
   }  while (0)

#endif  // TREE_BN_H_

/***********************************
 * tree/bs.h
 ***********************************/

#ifndef TREE_BS_H_
#define TREE_BS_H_

#define bs_child(n, rl) ((rl) < 0? bn_node(n)->left: bn_node(n)->right)
#define bs_child_ref(n, rl) ((rl) < 0? &(bn_node(n)->left): &(bn_node(n)->right))

struct bs_ires {
  struct bnnode *nn;
  int inserted;
};

struct bs_ires bs_insert(struct bntree *t, struct bnnode *nn, bn_compare_t cmp);
struct bs_ires bs_insert_unique(struct bntree *t, struct bnnode *nn, bn_compare_t cmp);
struct bnnode *bs_search(struct bntree *t, struct bnnode *nn, bn_compare_t cmp);
struct bnnode *bs_search_gte(struct bntree *t, struct bnnode *nn, bn_compare_t cmp);
struct bnnode *bs_search_lte(struct bntree *t, struct bnnode *nn, bn_compare_t cmp);
int bs_delete(struct bntree *t, struct bnnode *dn);

#endif  // TREE_BS_H_

/***********************************
 * tree/rb.h
 ***********************************/

/**
 * Cài đặt khái quát của cây đỏ-đen,
 * tương thích với các hàm cho cây nhị phân và cây nhị phân tìm kiếm
*/

#ifndef TREE_RB_H_
#define TREE_RB_H_

/*
 * Các tính chất của cây đỏ đen:
 * 1) Mỗi nút chỉ có thể là đỏ hoặc đen
 * 2) Nút gốc là nút đen
 * 3) Tất cả các nút lá (NULL) là các nút đen
 * 4) Cả hai con của nút đỏ là các nút đen
 * 5) Tất cả các đường đi đơn giản từ nút gốc tới các nút lá đều có
 *    cùng số lượng nút đen.
 */

// đỏ = 0, đen = 1 như vậy chúng ta có tổng giá trị mầu = số lượng nút đen
enum rbnode_color {
  RB_RED = 0,
  RB_BLACK = 1
};

extern const char * color_names[];

/**
 * Cấu trúc nút của cây đỏ đen
 * rbnode = red black node
 */
struct rbnode {
  struct bnnode base;
  enum rbnode_color color;
};

#define rb_node(n) ((struct rbnode *)(n))
#define rb_color(n) ((n)? rb_node(n)->color: RB_BLACK)
#define rb_color_str(n) color_names[(int)rb_color(n)]
#define rb_set_color(n, new_color) rb_node(n)->color = (new_color)
#define rb_is_red(node) (rb_color(node) == RB_RED)
#define rb_is_black(node) (rb_color(node) == RB_BLACK)
#define rb_set_black(node) rb_set_color(node, RB_BLACK)
#define rb_set_red(node) rb_set_color(node, RB_RED)

/*
  Trong triển khai này NULL được sử dụng thay vì lính canh để tương
  thích tốt hơn với các hàm ngoại.*

  Nút NULL được quy ước là nút đen
*/

struct rbnode *rb_create_node();
#define rb_free_node(n) free(n)

struct bs_ires rb_insert(struct bntree *t, struct rbnode *nn, bn_compare_t cmp);
struct bs_ires rb_insert_unique(struct bntree *t, struct rbnode *nn, bn_compare_t cmp);
int rb_delete(struct bntree *t, struct rbnode *dn);

#endif  // TREE_RB_H_

/***********************************
 * tree/spec/gbs.h
 ***********************************/

#ifndef SPEC_TREE_GBS_H_
#define SPEC_TREE_GBS_H_

/**
 * Cấu trúc nút của cây nhị phân tìm kiếm với kiểu gtype
 * gbsnode = gtype binary search node
 */
struct gbsnode {
  struct bnnode base;
  gtype key;
};

/**
 * Cấu trúc điều khiển của cây nhị phân tìm kiếm với kiểu gtype
 * gbstree = gtype binary search tree
 */
struct gbstree {
  struct bntree base;
  gtype_cmp_t cmp;
  gtype_free_t fk;
};

#define gbs_node(n) ((struct gbsnode *)(n))
#define gbs_tree(t) ((struct gbstree *)(t))

struct gbsnode *gbs_create_node(gtype key);
int gbs_cmp_node(struct bnnode *n1, struct bnnode *n2, struct bntree *t);
struct gbstree *gbs_create_tree(struct gbsnode *root, gtype_cmp_t cmp, gtype_free_t fk);

struct bs_ires gbs_insert(struct gbstree *t, gtype key);
struct bs_ires gbs_insert_unique(struct gbstree *t, gtype key);
struct gbsnode *gbs_search(struct gbstree *t, gtype key);
struct gbsnode *gbs_search_gte(struct gbstree *t, gtype key);
struct gbsnode *gbs_search_lte(struct gbstree *t, gtype key);
int gbs_delete(struct gbstree *t, struct gbsnode *n);

#define gbs_free_node(n, fk) \
  do { \
    if (fk) { \
      (fk)(gbs_node(n)->key); \
    } \
    bn_free_node(n); \
  } while (0)

#define gbs_free_tree(t) \
  do { \
    if (gbs_tree(t)->fk) { \
      bn_traverse_lnr(_cur, bn_tree(t)) { \
        gbs_tree(t)->fk(gbs_node(_cur)->key); \
      }  \
    } \
    bn_free_tree(bn_tree(t)); \
  } while (0)

void gbs_pprint(struct gbstree *, gtype_print_t gpp);

#endif  // SPEC_TREE_GBS_H_

/***********************************
 * tree/spec/grb.h
 ***********************************/

/**
 * Cài đặt khái quát của cây đỏ-đen,
 * tương thích với các hàm cho cây nhị phân và cây nhị phân tìm kiếm
*/

#ifndef TREE_SPEC_GRB_H_
#define TREE_SPEC_GRB_H_

/**
 * Cấu trúc nút đỏ đen với kiểu gtype, kế thừa ::struct rbnode
 * grbnode = gtype red black node
 */
struct grbnode {
  struct rbnode base;
  gtype key;
};

/**
 * Cấu trúc điều khiển của cây đỏ đen với kiểu gtype
 * grbtree = gtype red black tree
 */
struct grbtree {
  struct bntree base;
  gtype_cmp_t cmp;
  gtype_free_t fk;
};

#define grb_node(n) ((struct grbnode *)(n))
#define grb_tree(t) ((struct grbtree *)(t))

struct grbnode *grb_create_node(gtype key);
#define grb_free_node(n, t) gbs_free_node(n, gbs_tree(t)->fk)
int grb_cmp_node(struct bnnode *, struct bnnode *, struct bntree *);

struct grbtree *grb_create_tree(struct grbnode *root, gtype_cmp_t cmp, gtype_free_t fk);

#define grb_free_tree(t) \
  do { \
    if (grb_tree(t)->fk) { \
      bn_traverse_lnr(_cur, bn_tree(t)) { \
        grb_tree(t)->fk(grb_node(_cur)->key); \
      }  \
    } \
    bn_free_tree(bn_tree(t)); \
  } while (0)

struct bs_ires grb_insert(struct grbtree *t, gtype key);
struct bs_ires grb_insert_unique(struct grbtree *t, gtype key);
struct grbnode *grb_search(struct grbtree *t, gtype key);
struct grbnode *grb_search_gte(struct grbtree *t, gtype key);
struct grbnode *grb_search_lte(struct grbtree *t, gtype key);
int grb_delete(struct grbtree *t, struct grbnode *dn);

#endif  // TREE_SPEC_GRB_H_

/***********************************
 * tree/rbm.h
 ***********************************/

#ifndef TREE_RBM_H_
#define TREE_RBM_H_

/** @file
 * @brief Ánh xạ khóa=>giá trị khái quát dựa trên cây đỏ-đen
 *
 * rbm - Red-black tree map là một triển khai ánh xạ khái quát
 * dựa trên cây đỏ đen (gọi ngắn gọn là bảng cây). Khóa và giá
 * trị được lưu trong cùng nút của cây và có kiểu ::gtype. Các
 * khóa được so sánh bằng hàm do người dùng cung cấp.
 */

/**
 * \headerfile "all.h"
 * Cấu trúc nút của rbmtree, là mở rộng nút của cây đỏ đen.
 * rbmnode = red black map node
 *
 * \private Người sử dụng không cần thao tác với kiểu này.
 */
struct rbmnode {
  struct rbnode base;

  /** \private
   * Người sử dụng không cần trực tiếp truy cập tới
   * các thuộc tính của nút.
   */
  gtype key;
  gtype value;
};

#define rbm_node(n) ((struct rbmnode *)(n))
#define rbm_node_key(n) (rbm_node(n)->key)
#define rbm_node_value(n) (rbm_node(n)->value)

/**
 * \headerfile "all.h"
 * Cấu trúc điều khiển của bảng cây rbmtree, được tạo bằng hàm
 * rbmtree = red black map tree
 *
 * rbm_create(gtype_cmp_t cmp, gtype_free_t free_key, gtype_free_t free_value).
 *
 * Các macro hỗ trợ: <br>
 *   #rbm_size(map) - Kích thước của map.
 *
 *   #rbm_traverse(k, v, map) - Duyệt tuần tự các cặp trong map.
 *
 *   #rbm_free(map) - Giải phóng bộ nhớ của map
 *
 *   #rbm_clear(map) - Làm rỗng map
 */
struct rbmtree {
  struct bntree t;
  gtype_cmp_t cmp;
  gtype_free_t fk, fv;
  long size;
};

/**
 * Cấu trúc kết quả trả về của hàm rbm_insert(struct rbmtree *t, gtype key, gtype value),
 * cho biết trạng thái kết thúc thao tác thêm dữ liệu vào rbmtree.
 */
struct rbm_ires {
  gtype *value;
  int inserted;
};

#define rbm_tree(t) ((struct rbmtree *)(t))

struct rbmnode *rbm_create_node(gtype key, gtype value);
int rbm_cmp_node(struct bnnode *n1, struct bnnode *n2, struct bntree *t);

/**
 * Hàm tạo đối tượng điều khiển bảng cây.
 *
 * @param cmp Hàm so sánh các giá trị khóa, phải != NULL.
 * Tham khảo: gtype_cmp_s(gtype v1, gtype v2).
 * @param free_key Con trỏ tới hàm giải phóng bộ nhớ ngoài được gắn với khóa, tham khảo.
 * gtype_free_s(gtype v). Được gọi khi xóa dữ liệu nếu != NULL. Trong trường hợp không
 * có bộ nhớ ngoài hoặc không cần xóa bộ nhớ ngoài (như tình huống đang chia sẻ bộ nhớ
 * với 1 câu trúc lưu trữ khác) thì để = NULL.
 * @param free_value Con trỏ tới hàm giải phóng bộ nhớ ngoài được gắn với giá trị, tương
 * tự free_key.
 * @return Trả về đối tượng điều khiển bảng cây.
 *
 * \memberof rbmtree
 *
 * Tham khảo: hmap_create(gtype_hash_t hash_func, gtype_cmp_t cmp, gtype_free_t free_key, gtype_free_t free_value)
 */
struct rbmtree *rbm_create(gtype_cmp_t cmp, gtype_free_t fk, gtype_free_t fv);

/**
 * Thêm cặp (key, value) vào bảng t. Nếu key đã tồn tại thì
 * bỏ qua, có thể truy cập giá trị đang được gắn với khóa
 * đã có trong t bằng con trỏ ::rbm_ires::value trong kết quả
 * được trả về.
 *
 * @param t Con trỏ tới bảng cây.
 * @param key Khóa được thêm vào.
 * @param value Giá trị được thêm vào.
 * @return Trả về đối tượng ::rbm_ires
 *
 * \memberof rbmtree
 *
 * Tham khảo: hmap_insert(struct hmap *tab, gtype key, gtype value)
 */
struct rbm_ires rbm_insert(struct rbmtree *t, gtype key, gtype value);

/**
 * Thêm cặp (key, value) vào bảng t. Nếu key đã tồn tại thì
 * bỏ qua và trả về con trỏ tới giá trị đang có trong t.
 * Nếu ngược lại thì thêm (key, value) và trả về NULL.
 *
 * @param t Con trỏ tới bảng cây.
 * @param key Khóa được thêm vào.
 * @param value Giá trị được thêm vào.
 * @return Trả về con trỏ tới giá trị nếu khóa đã tồn tại
 * hoặc trả về NULL nếu ngược lại, kiểu ::gtype*.
 *
 * \memberof rbmtree
 *
 * Tham khảo: hmap_put(struct hmap *tab, gtype key, gtype value)
 */
gtype *rbm_put(struct rbmtree *t, gtype key, gtype value);

/**
 * Tra cứu giá trị trong t theo key.
 *
 * @param t Con trỏ tới đối tượng bảng cây.
 * @param key Khóa được sử dụng để tìm kiếm.
 * @return Trả về con trỏ tới giá trị đang được gắn với key trong t
 * nếu tồn tại, hoặc NULL nếu ngược lại.
 *
 * \memberof rbmtree
 *
 * Tham khảo: hmap_value(struct hmap *tab, gtype key)
 */
gtype *rbm_value(struct rbmtree *t, gtype key);

/**
 * Có thể sẽ xóa hàm này.
 * \private
 */
struct rbmnode *rbm_search(struct rbmtree *t, gtype key);

/**
 * Nếu key không có trong t thì bỏ qua, nếu ngược lại thì xóa cặp
 * khóa & giá trị tương ứng trong t. Các hàm free_key và free_value
 * được gọi nếu != NULL.
 *
 * @param t Con trỏ tới bảng cây.
 * @param key Khóa cần xóa.
 * @return 1 Nếu tồn tại khóa sau khi xóa dữ liệu, 0 nếu ngược lại.
 *
 * \memberof rbmtree
 *
 * Tham khảo: hmap_remove(struct hmap *tab, gtype key)
 */
int rbm_remove(struct rbmtree *t, gtype key);

/**
 * Số lượng cặp khóa & giá trị đang được lưu trong bảng.
 *
 * @param t Con trỏ tới đối tượng bảng cây.
 * @return Trả về số lượng cặp khóa/giá trị đang được lưu trong t.
 *
 */
#define rbm_size(t) ((t)->size)

static inline void _rbm_move_next(gtype **k, gtype **v) {
  struct rbmnode *nd = rbm_node(container_of(*k, struct rbmnode, key));
  struct bnnode *tmp = bn_next_inorder(bn_node(nd));
  if (!tmp) {
    *k = NULL;
    *v = NULL;
    return;
  }
  *k = &(rbm_node_key(tmp));
  *v = &(rbm_node_value(tmp));
}

/**
 * Duyệt tuần tự các cặp khóa & giá trị trong bảng map. Các tham số k và v
 * là các định danh do người sử dụng tự đặt và sẽ có kiểu ::gtype *
 *
 * @param k Định danh sẽ được sử dụng như con trỏ tới khóa hiện hành.
 * @param v Định danh sẽ được sử dụng như con trỏ tới giá trị hiện hành.
 * @param map Con trỏ tới bảng cây.
 *
 * Tham khảo: #hmap_traverse(key, value, map)
 */
#define rbm_traverse(k, v, map) \
  for (gtype *k = (rbm_size(map))? &(rbm_node_key(bn_left_most(bn_tree(map)->root))): NULL, \
             *v = (rbm_size(map))? &(rbm_node_value(bn_left_most(bn_tree(map)->root))): NULL; \
       k != NULL && v != NULL; _rbm_move_next(&k, &v)) \

/**
 * Giải phóng bộ nhớ được cấp phát cho bảng map. Các hàm free_key và
 * free_value được gọi cho từng khóa và giá trị nếu != NULL.
 *
 * @param map Con trỏ tới bảng cây.
 */
#define rbm_free(map) \
  do { \
    if (rbm_tree(map)->fk || rbm_tree(map)->fv) { \
      rbm_traverse(_k, _v, (map)) { \
        if (rbm_tree(map)->fk) { \
          rbm_tree(map)->fk(*_k); \
        } \
        if (rbm_tree(map)->fv) { \
          rbm_tree(map)->fv(*_v); \
        } \
      } \
    } \
    bn_free_tree(bn_tree(map)); \
  } while (0)

/**
 * Làm rỗng bảng map. Các hàm free_key và free_value sẽ được gọi
 * cho từng cặp khóa và giá trị nếu != NULL.
 *
 * @param map Con trỏ tới bảng cây.
 */
#define rbm_clear(map) \
  do { \
    if (rbm_tree(map)->fk || rbm_tree(map)->fv) { \
      rbm_traverse(_k, _v, (map)) { \
        if (rbm_tree(map)->fk) { \
          rbm_tree(map)->fk(*_k); \
        } \
        if (rbm_tree(map)->fv) { \
          rbm_tree(map)->fv(*_v); \
        } \
      } \
    } \
    struct bntree *_t = (struct bntree *)(map); \
    bn_clear_tree(_t); \
    rbm_tree(map)->size = 0; \
  } while (0)

/**
 * Hàm giải phóng bộ nhớ cho trường hợp con trỏ tới đối tượng rbmtree
 * được lưu trong một cấu trúc lưu trữ khác.
 *
 * @param value Giá trị gtype đang chứa con trỏ tới đối tượng rbmtree.
 * Con trỏ value.rbm (kiểu ::struct rbmtree *) được truyền cho rbm_free.
 * @return Hàm không trả về giá trị.
 *
 * \memberof gtype
 */
void gtype_free_rbm(gtype value);

#endif  // TREE_RBM_H_

/***********************************
 * tree/rbs.h
 ***********************************/

#ifndef TREE_RBS_H_
#define TREE_RBS_H_

/**
 * Cấu trúc điều khiển của tập hợp với kiểu gtype dựa trên
 * cây đỏ đen
 * rbstree = red black set tree
 */
struct rbstree {
  struct grbtree base;
  long size;
};

#define rbs_tree(t) ((struct rbstree *)(t))

#define rbs_node_key(n) (grb_node(n)->key)
#define rbs_contains(s, v) (rbs_search(s, v) != NULL)

struct rbstree *rbs_create(gtype_cmp_t cmp, gtype_free_t fk);
int rbs_insert(struct rbstree *s, gtype elem);
struct grbnode *rbs_search(struct rbstree *s, gtype elem);
int rbs_remove(struct rbstree *s, gtype elem);

static inline void _rbs_move_next(gtype **cur) {
  struct grbnode *nd = container_of(*cur, struct grbnode, key);
  struct bnnode *tmp = bn_next_inorder(bn_node(nd));
  if (!tmp) {
    *cur = NULL;
    return;
  }
  *cur = &(rbs_node_key(tmp));
}

#define rbs_size(s) ((s)->size)

#define rbs_traverse(cur, s) \
  for (gtype *cur = (rbs_size(s))? &(rbs_node_key(bn_left_most(bn_tree(s)->root))): NULL; \
        cur != NULL; _rbs_move_next(&cur))

#define rbs_free(s) \
    do {  \
      if (grb_tree(s)->fk) { \
        rbs_traverse(_k, (s)) { \
          grb_tree(s)->fk(*_k); \
        } \
      } \
      bn_free_tree(bn_tree(s)); \
    } while (0)

#define rbs_clear(s) \
    do {  \
      if (grb_tree(s)->fk) { \
        rbs_traverse(_k, (s)) { \
          grb_tree(s)->fk(*_k); \
        } \
      } \
      bn_clear_tree(bn_tree(s)); \
      rbs_tree(s)->size = 0; \
    } while (0)

/**
 * Hàm giải phóng bộ nhớ cho trường hợp con trỏ tới đối tượng rbstree
 * được lưu trong một cấu trúc lưu trữ khác.
 *
 * @param value Giá trị gtype đang chứa con trỏ tới đối tượng rbstree.
 * Con trỏ value.rbs (kiểu ::struct rbstree * được truyền cho rbs_free.
 * @return Hàm không trả về giá trị.
 */
void gtype_free_rbs(gtype value);

#endif  // TREE_RBS_H_

/***********************************
 * tree/spec/s2i.h
 ***********************************/

/**
 * Ánh xạ chuỗi => số (đóng gói cây đỏ-đen)
 */

#ifndef TREE_SPEC_S2I_H_
#define TREE_SPEC_S2I_H_

struct rbmtree *s2i_create();

struct rbm_ires s2i_insert(struct rbmtree *si, const char *key, long value);

/**
 * Lưu cặp key & value, bỏ qua nếu key đã tồn tại.
 * Mặc định tạo bản sao của khóa
 *
 * @param si Con trỏ tới bảng cây
 * @param key Chuỗi khóa
 * @param value Mã số được gắn với khóa
 * @return NULL nếu key chưa tồn tại
 *         Con trỏ tới value nếu ngược lại.
 */
long *s2i_put(struct rbmtree *si, const char *key, long value);

/**
 * Đọc giá trị được gắn với key
 *
 * @param si Con trỏ tới bảng cây
 * @param key Chuỗi khóa
 * @return NULL nếu key chưa tồn tại
 *         Con trỏ tới value nếu ngược lại.
 */
long *s2i_value(struct rbmtree *si, const char *key);
int s2i_remove(struct rbmtree *si, const char *key);
void s2i_print_node(struct bnnode *n);

#define s2i_node_key(n) (rbm_node(n)->key)
#define s2i_node_value(n) (rbm_node(n)->value)

#define s2i_free(si) rbm_free(si)

#endif  // TREE_SPEC_S2I_H_

/***********************************
 * utils/io.h
 ***********************************/

#ifndef UTILS_IO_H_
#define UTILS_IO_H_

#ifndef CGEN_IO_BUF_SIZE
  #define CGEN_DEFAULT_IO_BUFF_SIZE
  #define CGEN_IO_BUF_SIZE 1024
#endif  // CGEN_IO_BUF_SIZE

/*
 * Hàm cgetline đọc 1 dòng từ luồng nhập dữ liệu văn bản inp
 * (ví dụ tệp văn bản). Hàm đọc dữ liệu cho tới khi gặp ký tự
 * LF ('\n') hoặc kết thúc tệp, tự cấp phát hoặc cấp phát lại bộ
 * nhớ đủ để lưu dữ liệu đọc được.
 *
 * Các tham số:
 *    lineptr: con trỏ tới vùng nhớ lưu kết quả đọc dữ liệu
 *             (được phép NULL).
 *    n: là kích thước vùng nhớ đã được cấp phát cho *lineptr
 *       (nếu chưa cấp phát thì n == 0).
 *    inp: con trỏ tới luồng nhập văn bản.
 *
 *    Nếu *lineptr == NULL thì n == 0 và ngược lại, hàm sẽ tự
 *    cấp phát bộ nhớ động trong trường hợp này. Người sử dụng
 *    tự giải phóng bộ nhớ sau khi sử dụng.
 *    Nếu *lineptr != NULL thì *ptr phải trỏ vào vùng nhớ đã
 *    được cấp phát, và n là kích thước vùng nhớ đã được cấp phát
 *    cho *lineptr
 *
 * Giá trị trả về:
 *    Trả về chuỗi ký tự lưu dòng đọc được (*lineptr). Trong trường
 *    hợp không thể đọc được dữ liệu từ inp (ví dụ kết thúc tệp)
 *    hàm trả về NULL và giá trị *lineptr không thay đổi.
 *    Trong trường hợp không thể cấp phát được bộ nhớ để lưu
 *    dòng, hàm trả về NULL và *lineptr == NULL.
 */
static char *cgetline(char **lineptr, long *nptr, FILE *inp) {
  char buff[CGEN_IO_BUF_SIZE];
  long idx = 0, len, tmp;
  long n = nptr? *nptr: 0;
  while (fgets(buff, CGEN_IO_BUF_SIZE, inp)) {
    len = strlen(buff);
    tmp = idx + len + 1;
    if (tmp > n) {
      *lineptr = realloc(*lineptr, tmp);
      if (*lineptr == NULL) {
        return NULL;  // Lỗi cấp phát bộ nhớ
      }
      n = tmp;
    }
    if (idx == 0) {
      *lineptr[0] = 0;
    }
    strcat(*lineptr, buff);
    idx += len;
    if (buff[len - 1] == '\n') {
      break;
    }
  }
  if (idx == 0) {
    return NULL;
  }
  if (nptr) {
    *nptr = n;
  }
  return *lineptr;
}

#define clear_stdin() while (getchar() != '\n')

static char *remove_tail_lf(char *s) {
  if (!s) {
    return s;
  }
  char *p = s + strlen(s) - 1;
  if (*p == '\n') {
    *p = '\0';
  }
  return s;
}

#ifdef CGEN_DEFAULT_IO_BUFF_SIZE
  #undef CGEN_IO_BUF_SIZE
#endif  // CGEN_DEFAULT_IO_BUFF_SIZE

#endif  // UTILS_IO_H_

/***********************************
 * utils/io.h
 ***********************************/

#ifndef UTILS_IO_H_
#define UTILS_IO_H_

#ifndef CGEN_IO_BUF_SIZE
  #define CGEN_DEFAULT_IO_BUFF_SIZE
  #define CGEN_IO_BUF_SIZE 1024
#endif  // CGEN_IO_BUF_SIZE

/*
 * Hàm cgetline đọc 1 dòng từ luồng nhập dữ liệu văn bản inp
 * (ví dụ tệp văn bản). Hàm đọc dữ liệu cho tới khi gặp ký tự
 * LF ('\n') hoặc kết thúc tệp, tự cấp phát hoặc cấp phát lại bộ
 * nhớ đủ để lưu dữ liệu đọc được.
 *
 * Các tham số:
 *    lineptr: con trỏ tới vùng nhớ lưu kết quả đọc dữ liệu
 *             (được phép NULL).
 *    n: là kích thước vùng nhớ đã được cấp phát cho *lineptr
 *       (nếu chưa cấp phát thì n == 0).
 *    inp: con trỏ tới luồng nhập văn bản.
 *
 *    Nếu *lineptr == NULL thì n == 0 và ngược lại, hàm sẽ tự
 *    cấp phát bộ nhớ động trong trường hợp này. Người sử dụng
 *    tự giải phóng bộ nhớ sau khi sử dụng.
 *    Nếu *lineptr != NULL thì *ptr phải trỏ vào vùng nhớ đã
 *    được cấp phát, và n là kích thước vùng nhớ đã được cấp phát
 *    cho *lineptr
 *
 * Giá trị trả về:
 *    Trả về chuỗi ký tự lưu dòng đọc được (*lineptr). Trong trường
 *    hợp không thể đọc được dữ liệu từ inp (ví dụ kết thúc tệp)
 *    hàm trả về NULL và giá trị *lineptr không thay đổi.
 *    Trong trường hợp không thể cấp phát được bộ nhớ để lưu
 *    dòng, hàm trả về NULL và *lineptr == NULL.
 */
static char *cgetline(char **lineptr, long *nptr, FILE *inp) {
  char buff[CGEN_IO_BUF_SIZE];
  long idx = 0, len, tmp;
  long n = nptr? *nptr: 0;
  while (fgets(buff, CGEN_IO_BUF_SIZE, inp)) {
    len = strlen(buff);
    tmp = idx + len + 1;
    if (tmp > n) {
      *lineptr = realloc(*lineptr, tmp);
      if (*lineptr == NULL) {
        return NULL;  // Lỗi cấp phát bộ nhớ
      }
      n = tmp;
    }
    if (idx == 0) {
      *lineptr[0] = 0;
    }
    strcat(*lineptr, buff);
    idx += len;
    if (buff[len - 1] == '\n') {
      break;
    }
  }
  if (idx == 0) {
    return NULL;
  }
  if (nptr) {
    *nptr = n;
  }
  return *lineptr;
}

#define clear_stdin() while (getchar() != '\n')

static char *remove_tail_lf(char *s) {
  if (!s) {
    return s;
  }
  char *p = s + strlen(s) - 1;
  if (*p == '\n') {
    *p = '\0';
  }
  return s;
}

#ifdef CGEN_DEFAULT_IO_BUFF_SIZE
  #undef CGEN_IO_BUF_SIZE
#endif  // CGEN_DEFAULT_IO_BUFF_SIZE

#endif  // UTILS_IO_H_

/***********************************
 * utils/str.h
 ***********************************/

#ifndef UTILS_STR_H_
#define UTILS_STR_H_

extern const char* ascii_spaces;

struct gvector *str_tokens(char *line, const char *delims);

#define str_split(line) str_tokens(line, ascii_spaces)
#define tok_traverse(cur, line, delims) \
  for (char *cur = strtok(line, delims); cur; cur = strtok(NULL, delims))

/*
  Trả về bản sao của chuỗi s hoặc NULL nếu không thể
  cấp phát bộ nhớ.
  Ghi chú: strdup là một hàm trong mở rộng, không có
  trong thư viện tiêu chuẩn.
*/
static inline char * cstrdup (const char *s) {
  size_t len = strlen (s) + 1;
  void *snew = malloc (len);

  if (snew == NULL) {
    return NULL;
  }

  return (char *) memcpy (snew, s, len);
}

#endif  // UTILS_STR_H_

/***********************************
 * mix/s2w.h
 ***********************************/

#ifndef MIX_S2W_H_
#define MIX_S2W_H_

/**
 * Cấu trúc điều khiển của ánh xạ 2 chiều chuỗi <=> chỉ số nguyên.
 * Các chỉ số được tăng dần tự động 0, 1, ...
 * quy ước -1 là chỉ số không hợp lệ.
 * si2ways = string id 2 ways (mapping)
 */
struct si2ways {
  struct gvector *is;
  struct rbmtree *si;
};

struct si2ways *s2w_create();
struct rbm_ires s2w_insert(struct si2ways *col, const char *s);
long *s2w_put(struct si2ways *col, const char *s);
int s2w_remove(struct si2ways *col, const char *s);
long s2w_id(struct si2ways *col, const char *s);
char *s2w_str(struct si2ways *col, const long id);
void s2w_free(struct si2ways *col);

#endif  // MIX_S2W_H_

#endif  // BKCGEN_H_
