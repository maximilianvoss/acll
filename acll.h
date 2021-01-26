/*
 * Copyright 2021 Maximilian Voss (maximilian@voss.rocks)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _ACLL_H
#define _ACLL_H

#include <stdlib.h>
#include <stdint.h>

typedef struct acll_s {
    struct acll_s *prev;
    struct acll_s *next;
    void *payload;
} acll_t;

acll_t *acll_append(const acll_t *acll, const void *payload);

acll_t *acll_concat(acll_t *acll1, acll_t *acll2);

acll_t *acll_first(const acll_t *acll);

acll_t *acll_last(const acll_t *acll);

acll_t *acll_pop(const acll_t *acll, void **payload);

acll_t *acll_push(const acll_t *acll, const void *payload);

uint32_t acll_count(const acll_t *acll);

acll_t *acll_remove(const acll_t *acll, acll_t *element);

acll_t *acll_delete(const acll_t *acll, acll_t *element, void (*payloadFreeFunction)(void *payload));

acll_t *acll_clone(const acll_t *acll, size_t payloadSize, void (*payloadCloneFunction)(void *payload));

void acll_free(acll_t *acll, void (*payloadFreeFunction)(void *payload));

uint8_t acll_in(const acll_t *acll, acll_t *element);

acll_t *acll_sort(acll_t *acll, int (*payloadComparatorFunction)(void *payload1, void *payload2));

acll_t *acll_find(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input);

acll_t *acll_nextFilter(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input);

acll_t *acll_prevFilter(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input);

acll_t *acll_firstFilter(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input);

acll_t *acll_lastFilter(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input);

#endif