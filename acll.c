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

#include <memory.h>
#include "acll.h"

static inline acll_t *buildPayloadWrapper(const void *payload);

static inline acll_t *buildPayloadWrapper(const void *payload) {
    acll_t *payloadWrapper = calloc(1, sizeof(acll_t));
    payloadWrapper->payload = (void *) payload;
    return payloadWrapper;
}

acll_t *acll_append(const acll_t *acll, const void *payload) {
    acll_t *ptr = (acll_t *) acll;

    if (payload == NULL) {
        return ptr;
    }

    acll_t *payloadWrapper = buildPayloadWrapper(payload);
    if (ptr == NULL) {
        return payloadWrapper;
    }

    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = payloadWrapper;
    payloadWrapper->prev = ptr;
    return (acll_t *) acll;
}

acll_t *acll_first(const acll_t *acll) {
    acll_t *ptr = (acll_t *) acll;
    while (ptr != NULL) {
        if (ptr->prev == NULL) {
            return ptr;
        }
        ptr = ptr->prev;
    }
    return NULL;
}

acll_t *acll_last(const acll_t *acll) {
    acll_t *ptr = (acll_t *) acll;
    if (ptr == NULL) {
        return NULL;
    }
    if (ptr->next == NULL) {
        return ptr;
    }

    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    return ptr;
}

acll_t *acll_pop(const acll_t *acll, void **payload) {
    if (acll == NULL) {
        *payload = NULL;
        return NULL;
    }

    acll_t *element = acll_first(acll);
    acll_t *tmp = element->next;
    if (tmp != NULL) {
        tmp->prev = NULL;
    }
    element->next = NULL;
    *payload = element->payload;
    return tmp;
}

acll_t *acll_push(const acll_t *acll, const void *payload) {
    if (payload == NULL) {
        return (acll_t *) acll;
    }

    acll_t *payloadWrapper = buildPayloadWrapper(payload);
    if (acll == NULL) {
        return payloadWrapper;
    }

    acll_t *ptr = acll_first(acll);
    payloadWrapper->next = ptr;
    ptr->prev = payloadWrapper;
    return payloadWrapper;
}

uint32_t acll_count(const acll_t *acll) {
    uint32_t count = 0;
    acll_t *ptr = acll_first(acll);
    while (ptr != NULL) {
        count++;
        ptr = ptr->next;
    }
    return count;
}

acll_t *acll_remove(const acll_t *acll, acll_t *element) {
    acll_t *ptr = (acll_t *) acll;

    while (ptr != NULL && element != NULL) {
        if (ptr == element) {
            acll_t *tmpNext = element->next;
            acll_t *tmpPrev = element->prev;

            element->prev = NULL;
            element->next = NULL;

            if (tmpPrev != NULL) {
                tmpPrev->next = tmpNext;
            }
            if (tmpNext != NULL) {
                tmpNext->prev = tmpPrev;
            }

            if (tmpPrev == NULL) {
                return tmpNext;
            }

            return (acll_t *) acll;
        }
        ptr = ptr->next;
    }
    return (acll_t *) acll;
}

acll_t *acll_delete(const acll_t *acll, acll_t *element, void (*payloadFreeFunction)(void *payload)) {
    acll_t *ptr = element;
    acll_t *tmp;

    if (ptr == NULL) {
        return NULL;
    }

    if (ptr->prev == NULL) {
        tmp = ptr->next;
        if (payloadFreeFunction != NULL) {
            payloadFreeFunction(ptr);
        }
        free(ptr);
        if (tmp != NULL) {
            tmp->prev = NULL;
        }
        return tmp;
    }

    if (ptr->next == NULL) {
        tmp = ptr->prev;
        if (payloadFreeFunction != NULL) {
            payloadFreeFunction(ptr);
        }
        free(ptr);
        tmp->next = NULL;
        return (acll_t *) acll;
    }

    tmp = ptr;
    ptr->prev->next = tmp->next;
    ptr->next->prev = tmp->prev;
    if (payloadFreeFunction != NULL) {
        payloadFreeFunction(tmp->payload);
    }
    free(tmp);
    return (acll_t *) acll;
}

acll_t *acll_clone(const acll_t *acll, size_t payloadSize, void (*payloadCloneFunction)(void *payload)) {
    if (acll == NULL) {
        return NULL;
    }
    acll_t *ptr = acll_first(acll);
    acll_t *clone = NULL;

    while (ptr != NULL) {
        void *clonedPayload = calloc(1, payloadSize);
        memcpy (clonedPayload, ptr->payload, payloadSize);

        if (payloadCloneFunction != NULL) {
            payloadCloneFunction(clonedPayload);
        }
        clone = acll_append(clone, clonedPayload);

        ptr = ptr->next;
    }
    return clone;
}

void acll_free(acll_t *acll, void (*payloadFreeFunction)(void *payload)) {
    if (acll == NULL) {
        return;
    }
    acll_t *next = acll->next;
    acll_free(next, payloadFreeFunction);

    if (payloadFreeFunction != NULL) {
        payloadFreeFunction(acll->payload);
    }
    free(acll);
}

uint8_t acll_in(const acll_t *acll, acll_t *element) {
    if (element == NULL) {
        return 0;
    }
    if (acll == NULL) {
        return 0;
    }

    acll_t *ptr = acll_first(acll);
    while (ptr != NULL) {
        if (ptr == element) {
            return 1;
        }
        ptr = ptr->next;
    }
    return 0;
}

acll_t *acll_sort(acll_t *acll, int (*payloadComperatorFunction)(void *payload1, void *payload2)) {
    if (acll == NULL) {
        return NULL;
    }

    acll_t *list = acll_first(acll);

    acll_t *cur = list->next;
    while (cur != NULL) {

        acll_t *check = cur->prev;
        while (check != NULL) {
            if (payloadComperatorFunction(check->payload, cur->payload) < 0) {
                break;
            }
            check = check->prev;
        }

        if (check == NULL) {
            acll_t *prev = cur->prev;
            acll_t *next = cur->next;

            if (prev != NULL) {
                prev->next = next;
            }
            if (next != NULL) {
                next->prev = prev;
            }

            if (cur != list) {
                cur->next = list;
            }
            cur->prev = NULL;
            list->prev = cur;
            list = cur;

            cur = next;
        } else {
            acll_t *prev = cur->prev;
            acll_t *next = cur->next;

            if (cur->prev != check) {

                cur->next = check->next;
                if (cur->next != NULL) {
                    cur->next->prev = cur;
                }

                check->next = cur;
                cur->prev = check;

                if (prev != NULL) {
                    prev->next = next;
                }
                if (next != NULL) {
                    next->prev = prev;
                }
            }

            cur = next;
        }
    }
    return acll_first(list);
}

acll_t *acll_find(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input) {
    if (acll == NULL) {
        return NULL;
    }

    if (payloadFilter == NULL) {
        return (acll_t *) acll;
    }

    acll_t *ptr = acll_first(acll);
    while (ptr != NULL) {
        if (payloadFilter(ptr->payload, input)) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

acll_t *acll_nextFilter(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input) {
    if (acll == NULL) {
        return NULL;
    }
    if (payloadFilter == NULL) {
        return acll->next;
    }

    acll_t *ptr = (acll_t *) acll;
    while (ptr != NULL) {
        if (payloadFilter(ptr->payload, input)) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

acll_t *acll_prevFilter(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input) {
    if (acll == NULL) {
        return NULL;
    }
    if (payloadFilter == NULL) {
        return acll->prev;
    }

    acll_t *ptr = (acll_t *) acll;
    while (ptr != NULL) {
        if (payloadFilter(ptr->payload, input)) {
            return ptr;
        }
        ptr = ptr->prev;
    }
    return NULL;
}

acll_t *acll_firstFilter(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input) {
    if (acll == NULL) {
        return NULL;
    }

    if (payloadFilter == NULL) {
        return acll_first(acll);;
    }

    acll_t *ptr = acll_first(acll);
    while (ptr != NULL) {
        if (payloadFilter(ptr->payload, input)) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

acll_t *acll_lastFilter(const acll_t *acll, int (*payloadFilter)(void *payload, void *input), void *input) {
    if (acll == NULL) {
        return NULL;
    }

    if (payloadFilter == NULL) {
        return acll_last(acll);;
    }

    acll_t *ptr = acll_first(acll);
    acll_t *last = NULL;
    while (ptr != NULL) {
        if (payloadFilter(ptr->payload, input)) {
            last = ptr;
        }
        ptr = ptr->next;
    }
    return last;
}