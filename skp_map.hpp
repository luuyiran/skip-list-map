#ifndef __SKIP_LIST_MAP__
#define __SKIP_LIST_MAP__


#include <stdio.h>
#include <stdlib.h>
#include <initializer_list>

#define SKIPLIST_MAXLEVEL   32


template<typename keyType, typename valueType>
class skp_map {
public:
    struct skpNode {
        keyType     key;
        valueType   value;
        skpNode     *next[];
    };

    skp_map() : levels(0) {
        head = createNode(keyType(), valueType(), SKIPLIST_MAXLEVEL);
    }

    ~skp_map() {
        skpNode *pre = head;
        while (pre) {
            head = pre->next[0];
            free(pre);
            pre = head;
        }
    }


    skpNode *insert(const keyType &key, const valueType &value) {
        skpNode     *update[SKIPLIST_MAXLEVEL];
        skpNode     *node = head;
        skpNode     *next = nullptr;


        // record previous path
        for (int i = levels - 1; i >= 0; i--) {
            while ((next = node->next[i]) && (next->key < key)) {
                node = next;
            }
            update[i] = node;
        }

        // if already in list, update it
        if (next && next->key == key) {
            next->value = value;
            return next;
        }

        int lvl = randomLevel();
        if (lvl > levels) {
            for (int i = levels; i < lvl; i++)
                update[i] = head;
            levels = lvl;
        }

        node = createNode(key, value, lvl);
        for (int i = 0; i < lvl; i++) {
            node->next[i] = update[i]->next[i];
            update[i]->next[i] = node;
        }
        return node;
    }

    void erase(const keyType &key) {
        skpNode     *update[SKIPLIST_MAXLEVEL];
        skpNode     *node = head;
        skpNode     *next = nullptr;

        for (int i = levels - 1; i >= 0; i--) {
            while ((next = node->next[i]) && (next->key < key)) {
                node = next;
            }
            update[i] = node;
        }

        if (next && next->key == key) {
            for (int i = 0; i < levels; i++) {
                if (update[i]->next[i] == next)
                    update[i]->next[i] = next->next[i];
            }
            free(next);
            for (int i = levels - 1; i >= 0; i--)
                if (head->next[i] == nullptr)
                    levels--;
        }

    }

    skpNode *find(const keyType &key) {
        skpNode     *node = head;
        skpNode     *next = nullptr;
        for (int i = levels - 1; i >= 0; i--) {
            while ((next = node->next[i]) && (next->key <= key)) {
                if (next->key == key) return next;
                node = next;
            }
        }
        return nullptr;
    }

    void print() {
        skpNode *node;
        printf("levels: %d\n", levels);
        for (int i = levels - 1; i >= 0; i--) {
            node = head->next[i];
            while (node) {
                printf("[%d,%d]->", node->key, node->value);
                node = node->next[i];
            }
            printf("\n");
        }
    }


private:

    skpNode *createNode(const keyType &k, const valueType &v, int level) {
        skpNode *res = (skpNode *)calloc(1, sizeof(skpNode) + level * sizeof(skpNode*));
        res->key = k;
        res->value = v;
        return res;
    }

    int randomLevel(void) {
        int level = 1;
        // 16383 = 65535 >> 2;  only 1/4 random nums less than 16383
        while ((random()&65535) < 16383)
            level += 1;
        return level < SKIPLIST_MAXLEVEL ? level : SKIPLIST_MAXLEVEL;
    }


    int levels;
    skpNode *head;

};

#endif // !__SKIP_LIST_MAP__
