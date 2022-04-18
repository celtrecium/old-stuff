#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <sbvector.h>
#include <stdlib.h>
#include <u8string.h>

#define BUCKETS_NUM 32

typedef unsigned long long int hash_t;

typedef struct node
{
  hash_t key;
  size_t index;
} node_t;

typedef struct hashtable
{
  sbvector_t keys;
  sbvector_t data;
} hashtable_t;

hashtable_t hashtable (size_t type_size);
void *__ht_update (hashtable_t *ht, char *key);
void *__ht_get (hashtable_t ht, char *key);
hash_t ht_hash_function (char *key);
bool ht_free (hashtable_t *ht);

#define ht_update(ht, type, key, value)                                       \
  (*((type *)__ht_update (ht, key)) = value)
#define ht_get(ht, type, key) ((type *)__ht_get (ht, key))

#endif  /* HASHTABLE_H */
