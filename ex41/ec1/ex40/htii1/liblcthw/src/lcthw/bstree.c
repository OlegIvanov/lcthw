#include <lcthw/dbg.h>
#include <lcthw/bstree.h>
#include <stdlib.h>
#include <lcthw/bstrlib.h>
#include <lcthw/hashmap_algos.h>

char BSTREE_ERROR;

static int default_compare(void *a, void *b)
{
	return bstrcmp((bstring)a, (bstring)b);
}

BSTree *BSTree_create(BSTree_compare compare, Hashmap_hash hash_func)
{
	BSTree *map = calloc(1, sizeof(BSTree));
	check_mem(map);

	map->compare = compare == NULL ? default_compare : compare;
	map->hash_func = hash_func == NULL ? Hashmap_djb_hash : hash_func;

	return map;

error:
	if(map) {
		BSTree_destroy(map);
	}
	return NULL;
}

static int BSTree_destroy_cb(BSTreeNode *node)
{
	free(node);
	return 0;
}

void BSTree_destroy(BSTree *map)
{
	if(map) {
		BSTree_traverse(map, BSTree_destroy_cb);
		free(map);
	}
}

static inline BSTreeNode *BSTreeNode_create(BSTree *map, BSTreeNode *parent, void *key, void *data)
{
	check(key, "key can't be NULL");

	BSTreeNode *node = calloc(1, sizeof(BSTreeNode));
	check_mem(node);

	node->key = key;
	node->data = data;
	node->hash = map->hash_func(node->key);
	node->parent = parent;
	return node;

error:
	return NULL;
}

static inline int BSTree_setnode(BSTree *map, BSTreeNode *node, void *key, void *data)
{
	check(map, "map can't be NULL");
	check(node, "node can't be NULL");
	check(key, "key can't be NULL");

	int cmp = map->compare(node->key, key);
	
	if(cmp <= 0) {
		if(node->left) {
			BSTree_setnode(map, node->left, key, data);
		} else {
			node->left = BSTreeNode_create(map, node, key, data);
		}
	} else {
		if(node->right) {
			BSTree_setnode(map, node->right, key, data);
		} else {
			node->right = BSTreeNode_create(map, node, key, data);
		}
	}
	
	return 0;

error:
	return -1;
}

int BSTree_set(BSTree *map, void *key, void *data)
{
	check(map, "map can't be NULL");
	check(key, "key can't be NULL");

	uint32_t hash = 0;

	if(map->root == NULL) {
		// first so just make it and get out
		map->root = BSTreeNode_create(map, NULL, key, data);
		check_mem(map->root);
	} else {
		hash = map->hash_func(key);
		BSTree_setnode(map, map->root, key, data);
	}

	return 0;
error:
	return -1;
}

static inline BSTreeNode *BSTree_getnode(BSTree *map, BSTreeNode *node, void *key)
{
	check(map, "map can't be NULL");
	check(node, "node can't be NULL");
	check(key, "key can't be NULL");

	int cmp = map->compare(node->key, key);

	if(cmp == 0) {
		return node;
	} else if(cmp < 0) {
		if(node->left) {
			return BSTree_getnode(map, node->left, key);
		} else {
			return NULL;
		}
	} else {
		if(node->right) {
			return BSTree_getnode(map, node->right, key);
		} else {
			return NULL;
		}
	}

error:
	return BSTREE_ERROR_POINTER;
}

void *BSTree_get(BSTree *map, void *key)
{
	check(map, "map can't be NULL");
	check(key, "key can't be NULL");

	uint32_t hash = 0;

	if(map->root == NULL) {
		return NULL;
	} else {
		hash = map->hash_func(key);
		BSTreeNode *node = BSTree_getnode(map, map->root, key);
		return node == NULL ? NULL : node->data;
	}

error:
	return BSTREE_ERROR_POINTER;
}

static inline int BSTree_traverse_nodes(BSTreeNode *node, BSTree_traverse_cb traverse_cb)
{
	int rc = 0;

	if(node->left) {
		rc = BSTree_traverse_nodes(node->left, traverse_cb);
		if(rc != 0) return rc;
	}

	if(node->right) {
		rc = BSTree_traverse_nodes(node->right, traverse_cb);
		if(rc != 0) return rc;
	}

	return traverse_cb(node);
}

int BSTree_traverse(BSTree *map, BSTree_traverse_cb traverse_cb)
{
	check(map, "map can't be NULL");

	if(map->root) {
		return BSTree_traverse_nodes(map->root, traverse_cb);
	}

	return 0;

error:
	return -1;
}

static inline BSTreeNode *BSTree_find_min(BSTreeNode *node)
{
	while(node->left) {
		node = node->left;
	}

	return node;
}

static inline int BSTree_replace_node_in_parent(BSTree *map, BSTreeNode *node, BSTreeNode *new_value)
{
	check(map, "map can't be NULL");

	if(node->parent) {
		if(node == node->parent->left) {
			node->parent->left = new_value;
		} else {
			node->parent->right = new_value;
		}
	} else {
		// this is the root so gotta change it
		map->root = new_value;
	}

	if(new_value) {
		new_value->parent = node->parent;
	}

	return 0;

error:
	return -1;
}

static inline void BSTree_swap(BSTreeNode *a, BSTreeNode *b)
{
	void *temp = NULL;
	temp = b->key; b->key = a->key; a->key = temp;
	temp = b->data; b->data = a->data; a->data = temp;

	uint32_t temp_hash = 0;
	temp_hash = b->hash; b->hash = a->hash; a->hash = temp_hash;
}

static inline BSTreeNode *BSTree_node_delete(BSTree *map, BSTreeNode *node, void *key)
{
	int cmp = map->compare(node->key, key);

	if(cmp < 0) {
		if(node->left) {
			return BSTree_node_delete(map, node->left, key);
		} else {
			// not found
			return NULL;
		}
	} else if(cmp > 0) {
		if(node->right) {
			return BSTree_node_delete(map, node->right, key);
		} else {
			// not found
			return NULL;
		}
	} else {
		if(node->left && node->right) {
			// swap this node for the smallest node that is bigger than us
			BSTreeNode *successor = BSTree_find_min(node->right);
			BSTree_swap(successor, node);

			// this leaves the old successor with possibly a right child
			// so replace it with the right child
			BSTree_replace_node_in_parent(map, successor, successor->right);

			// finally it's swapped, so return successor instead of node
			return successor;
		} else if(node->left) {
			BSTree_replace_node_in_parent(map, node, node->left);
		} else if(node->right) {
			BSTree_replace_node_in_parent(map, node, node->right);
		} else {
			BSTree_replace_node_in_parent(map, node, NULL);
		}

		return node;
	}
}

void *BSTree_delete(BSTree *map, void *key)
{
	check(map, "map can't be NULL");
	check(key, "key can't be NULL");

	void *data = NULL;
	
	if(map->root) {
		BSTreeNode *node = BSTree_node_delete(map, map->root, key);

		if(node) {
			data = node->data;
			free(node);
		}
	}

	return data;

error:
	return BSTREE_ERROR_POINTER;
}
