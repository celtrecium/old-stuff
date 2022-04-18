#include "../include/hashtable.h"
#include <sbvector.h>
#include <stdbool.h>

hashtable_t
hashtable (size_t type_size)
{
  size_t i = 0;
  hashtable_t ret;

  ret.keys = sbvector (sizeof (sbvector_t));
  ret.data = sbvector (type_size);

  sbv_reserve (&ret.keys, BUCKETS_NUM);

  while (i++ < BUCKETS_NUM)
    sbv_push (&ret.keys, sbvector_t, sbvector (sizeof (node_t)));

  return ret;
}

static hash_t
_pow (hash_t f, hash_t s)
{
  while (--s != 0)
    f *= f;

  return f;
}

hash_t
ht_hash_function (char *key)
{
  hash_t i = 1;
  hash_t hash = (hash_t)*key++;

  for (; *key != 0; ++i, ++key)
    hash += (hash_t)*key * _pow (i, 2);

  return hash;
}

static node_t *
_find_node (hashtable_t ht, hash_t hash)
{
  size_t i = 0;
  sbvector_t nodes = *sbv_get (&ht.keys, sbvector_t, hash % ht.keys.length);
  node_t *node;

  if (!nodes.length)
    return NULL;

  node = sbv_get (&nodes, node_t, 0);

  if (node->key == hash)
    return node;

  for (i = 1; i < nodes.length; ++i)
    {
      node = sbv_get (&nodes, node_t, i);

      if (node->key == hash)
        return node;
    }

  return NULL;
}

static void *
_push_new (hashtable_t *ht, hash_t hash)
{
  node_t new_node;

  new_node.index = ht->data.length;
  new_node.key = hash;

  sbv_push (sbv_get (&ht->keys, sbvector_t, hash % ht->keys.length), node_t,
            new_node);

  sbv_resize (&ht->data, ht->data.length + 1);

  return __sbv_get_f (&ht->data, new_node.index);
}

void *
__ht_update (hashtable_t *ht, char *key)
{
  hash_t hash = ht_hash_function (key);
  node_t *node = _find_node (*ht, hash);

  if (!node)
    return _push_new (ht, hash);

  return __sbv_get_f (&ht->data, node->index);
}

void *
__ht_get (hashtable_t ht, char *key)
{
  node_t *node = _find_node (ht, ht_hash_function (key));

  if (!node)
    return NULL;

  return __sbv_get_f (&ht.data, node->index);
}

bool
ht_free (hashtable_t *ht)
{
  while (ht->keys.length)
    sbv_free (sbv_pop (&ht->keys, sbvector_t));

  sbv_free (&ht->keys);
  sbv_free (&ht->data);

  return true;
}
