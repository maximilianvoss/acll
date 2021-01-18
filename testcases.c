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

#include <casserts.h>
#include "acll.h"

static int test_acll_append_0(void *data) {
    acll_t *list = NULL;
    list = acll_append(list, NULL);
    ASSERTNULL(list);
    return 0;
}

static int test_acll_append_1(void *data) {
    acll_t *list = NULL;
    list = acll_append(list, "element 0");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTNULL(list->prev);
    ASSERTNULL(list->next);
    return 0;
}

static int test_acll_append_2(void *data) {
    acll_t *list = NULL;
    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);
    ASSERTSTR("element 0", (char *) list->next->prev->payload);
    ASSERTNULL(list->prev);
    ASSERTNULL(list->next->next);
    return 0;
}

static int test_acll_concat_0(void *data) {
    acll_t *list1 = NULL;
    acll_t *list2 = NULL;
    acll_t *result;
    result = acll_concat(list1, list2);
    ASSERTNULL(result);
    return 0;
}

static int test_acll_concat_1(void *data) {
    acll_t *list1 = NULL;
    acll_t *list2 = NULL;
    acll_t *result;
    list1 = acll_append(list1, "element 0");
    result = acll_concat(list1, list2);
    ASSERTNOTNULL(result);
    ASSERTSTR("element 0", (char *) result->payload);
    ASSERTNULL(result->next);
    ASSERTNULL(result->prev);
    return 0;
}

static int test_acll_concat_2(void *data) {
    acll_t *list1 = NULL;
    acll_t *list2 = NULL;
    acll_t *result;
    list2 = acll_append(list2, "element 1");
    result = acll_concat(list1, list2);
    ASSERTNOTNULL(result);
    ASSERTSTR("element 1", (char *) result->payload);
    ASSERTNULL(result->next);
    ASSERTNULL(result->prev);
    return 0;
}

static int test_acll_concat_3(void *data) {
    acll_t *list1 = NULL;
    acll_t *list2 = NULL;
    acll_t *result;
    list1 = acll_append(list1, "element 0");
    list2 = acll_append(list2, "element 1");
    result = acll_concat(list1, list2);
    ASSERTNOTNULL(result);
    ASSERTSTR("element 0", (char *) result->payload);
    ASSERTSTR("element 1", (char *) result->next->payload);
    ASSERTNULL(result->next->next);
    ASSERTNULL(result->next->next);
    ASSERTNULL(result->prev);
    ASSERTNOTNULL(result->next->prev);
    ASSERTPTREQUAL(result->next->prev, result);
    return 0;
}

static int test_acll_first_0(void *data) {
    acll_t *list = acll_first(NULL);
    ASSERTNULL(list);
    return 0;
}

static int test_acll_first_1(void *data) {
    acll_t *list = NULL;
    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");

    acll_t *ptr = acll_first(list);
    ASSERTNOTNULL(ptr);
    ASSERTSTR("element 0", (char *) ptr->payload);
    return 0;
}

static int test_acll_first_2(void *data) {
    acll_t *list = NULL;
    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");

    acll_t *ptr = acll_first(list->next);
    ASSERTNOTNULL(ptr);
    ASSERTSTR("element 0", (char *) ptr->payload);
    return 0;
}

static int test_acll_last_0(void *data) {
    acll_t *list = acll_last(NULL);
    ASSERTNULL(list);
    return 0;
}

static int test_acll_last_1(void *data) {
    acll_t *list = NULL;
    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");

    acll_t *ptr = acll_last(list);
    ASSERTNOTNULL(ptr);
    ASSERTSTR("element 1", (char *) ptr->payload);
    return 0;
}

static int test_acll_last_2(void *data) {
    acll_t *list = NULL;
    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");

    acll_t *ptr = acll_last(list->next);
    ASSERTNOTNULL(ptr);
    ASSERTSTR("element 1", (char *) ptr->payload);
    return 0;
}

static int test_acll_pop_0(void *data) {
    char *payload;
    acll_t *list = NULL;

    list = acll_pop(list, (void **) &payload);
    ASSERTNULL(list);
    ASSERTNULL(payload);

    return 0;
}

static int test_acll_pop_1(void *data) {
    char *payload;
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_pop(list, (void **) &payload);
    ASSERTNOTNULL(list);
    ASSERTNOTNULL(payload);
    ASSERTSTR("element 0", (char *) payload);
    ASSERTSTR("element 1", (char *) list->payload);

    list = acll_pop(list, (void **) &payload);
    ASSERTNOTNULL(list);
    ASSERTNOTNULL(payload);
    ASSERTSTR("element 1", (char *) payload);
    ASSERTSTR("element 2", (char *) list->payload);

    list = acll_pop(list, (void **) &payload);
    ASSERTNULL(list);
    ASSERTNOTNULL(payload);
    ASSERTSTR("element 2", (char *) payload);

    return 0;
}

static int test_acll_push_0(void *data) {
    acll_t *list = NULL;
    list = acll_push(list, NULL);
    ASSERTNULL(list);
    return 0;
}

static int test_acll_push_1(void *data) {
    acll_t *list = NULL;
    list = acll_push(list, "element 0");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTNULL(list->prev);
    ASSERTNULL(list->next);
    return 0;
}

static int test_acll_push_2(void *data) {
    acll_t *list = NULL;
    list = acll_push(list, "element 0");
    list = acll_push(list, "element 1");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 1", (char *) list->payload);
    ASSERTSTR("element 0", (char *) list->next->payload);
    ASSERTSTR("element 1", (char *) list->next->prev->payload);
    ASSERTNULL(list->prev);
    ASSERTNULL(list->next->next);
    return 0;
}

static int test_acll_count_0(void *data) {
    acll_t *list = NULL;
    uint32_t count = acll_count(list);
    ASSERTINT(0, count);
    return 0;
}

static int test_acll_count_1(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    uint32_t count = acll_count(list);
    ASSERTINT(3, count);
    return 0;
}

static int test_acll_remove_0(void *data) {
    acll_t *list = NULL;
    list = acll_remove(list, list);
    ASSERTNULL(list);
    return 0;
}

static int test_acll_remove_1(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);
    ASSERTSTR("element 2", (char *) list->next->next->payload);

    acll_t *tmp = list;
    list = acll_remove(list, tmp);

    ASSERTSTR("element 1", (char *) list->payload);
    ASSERTSTR("element 2", (char *) list->next->payload);

    ASSERTNULL(tmp->prev);
    ASSERTNULL(tmp->next);
    ASSERTSTR("element 0", (char *) tmp->payload);

    return 0;
}

static int test_acll_remove_2(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);
    ASSERTSTR("element 2", (char *) list->next->next->payload);

    acll_t *tmp = list->next;
    list = acll_remove(list, tmp);

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 2", (char *) list->next->payload);

    ASSERTNULL(tmp->prev);
    ASSERTNULL(tmp->next);
    ASSERTSTR("element 1", (char *) tmp->payload);

    return 0;
}

static int test_acll_remove_3(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);
    ASSERTSTR("element 2", (char *) list->next->next->payload);

    acll_t *tmp = list->next->next;
    list = acll_remove(list, tmp);

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);

    ASSERTNULL(tmp->prev);
    ASSERTNULL(tmp->next);
    ASSERTSTR("element 2", (char *) tmp->payload);

    return 0;
}

static int test_acll_remove_4(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    ASSERTSTR("element 0", (char *) list->payload);

    acll_t *tmp = list;
    list = acll_remove(list, tmp);

    ASSERTNULL(list);
    ASSERTNULL(tmp->prev);
    ASSERTNULL(tmp->next);
    ASSERTSTR("element 0", (char *) tmp->payload);

    return 0;
}

static int test_acll_delete_0(void *data) {
    acll_t *list = NULL;
    list = acll_delete(list, list, NULL);
    ASSERTNULL(list);
    return 0;
}

static int test_acll_delete_1(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);
    ASSERTSTR("element 2", (char *) list->next->next->payload);

    list = acll_delete(list, list, NULL);

    ASSERTSTR("element 1", (char *) list->payload);
    ASSERTSTR("element 2", (char *) list->next->payload);

    return 0;
}

static int test_acll_delete_2(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);
    ASSERTSTR("element 2", (char *) list->next->next->payload);

    list = acll_delete(list, list->next, NULL);

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 2", (char *) list->next->payload);

    return 0;
}

static int test_acll_delete_3(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);
    ASSERTSTR("element 2", (char *) list->next->next->payload);

    list = acll_delete(list, list->next->next, NULL);

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);

    return 0;
}

static int test_acll_delete_4(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    ASSERTSTR("element 0", (char *) list->payload);

    list = acll_delete(list, list, NULL);
    ASSERTNULL(list);
    return 0;
}

static int test_acll_clone_0(void *data) {
    acll_t *list = NULL;
    list = acll_clone(list, sizeof(char), NULL);
    ASSERTNULL(list);
    return 0;
}

static int test_acll_clone_1(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    acll_t *clonedList = acll_clone(list, sizeof(char) * 10, NULL);

    ASSERTNOTNULL(clonedList);

    ASSERTSTR("element 0", (char *) clonedList->payload);
    ASSERTSTR("element 1", (char *) clonedList->next->payload);
    ASSERTSTR("element 2", (char *) clonedList->next->next->payload);

    ASSERTNULL(list->prev);
    ASSERTNULL(clonedList->prev);

    ASSERTPTRNOTEQUAL(list, clonedList);
    ASSERTPTRNOTEQUAL(list->next, clonedList->next);
    ASSERTPTRNOTEQUAL(list->next->prev, clonedList->next->prev);
    ASSERTPTRNOTEQUAL(list->next->next, clonedList->next->next);
    ASSERTPTRNOTEQUAL(list->next->next->prev, clonedList->next->next->prev);

    return 0;
}

static void test_acll_clone_2_sub(void *payload) {
    strcpy(payload, "element 3");
}

static int test_acll_clone_2(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    acll_t *clonedList = acll_clone(list, sizeof(char) * 10, test_acll_clone_2_sub);

    ASSERTNOTNULL(clonedList);

    ASSERTSTR("element 3", (char *) clonedList->payload);
    ASSERTSTR("element 3", (char *) clonedList->next->payload);
    ASSERTSTR("element 3", (char *) clonedList->next->next->payload);

    ASSERTNULL(list->prev);
    ASSERTNULL(clonedList->prev);

    ASSERTPTRNOTEQUAL(list, clonedList);
    ASSERTPTRNOTEQUAL(list->next, clonedList->next);
    ASSERTPTRNOTEQUAL(list->next->prev, clonedList->next->prev);
    ASSERTPTRNOTEQUAL(list->next->next, clonedList->next->next);
    ASSERTPTRNOTEQUAL(list->next->next->prev, clonedList->next->next->prev);

    return 0;
}

static int test_acll_in_0(void *data) {
    uint8_t in;
    in = acll_in(NULL, NULL);
    ASSERTINT(0, in);
    return 0;
}

static int test_acll_in_1(void *data) {
    uint8_t in;
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    in = acll_in(list, NULL);
    ASSERTINT(0, in);
    return 0;
}

static int test_acll_in_2(void *data) {
    uint8_t in;
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    in = acll_in(list, list);
    ASSERTINT(1, in);
    return 0;
}

static int test_acll_in_3(void *data) {
    uint8_t in;
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    in = acll_in(list, list->next);
    ASSERTINT(1, in);
    return 0;
}

static int test_acll_in_4(void *data) {
    uint8_t in;
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    in = acll_in(list, list->next->next);
    ASSERTINT(1, in);
    return 0;
}

static int test_acll_in_5(void *data) {
    uint8_t in;
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    acll_t test;
    test.payload = "test";
    test.prev = NULL;
    test.next = NULL;

    in = acll_in(list, &test);
    ASSERTINT(0, in);
    return 0;
}

static int test_acll_sort_0_sub(void *payload1, void *payload2) {
    return strcmp((char *) payload1, (char *) payload2);
}

static int test_acll_sort_0(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 2");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 0");

    ASSERTSTR("element 2", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);
    ASSERTSTR("element 0", (char *) list->next->next->payload);

    list = acll_sort(list, test_acll_sort_0_sub);

    ASSERTSTR("element 0", (char *) list->payload);
    ASSERTSTR("element 1", (char *) list->next->payload);
    ASSERTSTR("element 2", (char *) list->next->next->payload);

    return 0;
}

static int test_acll_find_sub(void *payload, void *input) {
    if (!strcmp((char *) payload, (char *) input)) {
        return 1;
    }
    return 0;
}

static int test_acll_find_0(void *data) {
    acll_t *list = NULL;

    list = acll_find(NULL, test_acll_find_sub, "element 1");
    ASSERTNULL(list);

    return 0;
}

static int test_acll_find_1(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_find(list, test_acll_find_sub, "element 1");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 1", (char *) list->payload);

    return 0;
}

static int test_acll_find_2(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 2");
    list = acll_append(list, "element 3");

    list = acll_find(list, test_acll_find_sub, "element 1");
    ASSERTNULL(list);

    return 0;
}

static int test_acll_find_3(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_find(list, NULL, "element 1");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 0", (char *) list->payload);

    return 0;
}

static int test_acll_nextFilter_sub(void *payload, void *input) {
    if (!strcmp((char *) payload, "element 2")) {
        return 1;
    }
    return 0;
}

static int test_acll_nextFilter_0(void *data) {
    acll_t *list = NULL;
    list = acll_nextFilter(NULL, NULL, NULL);
    ASSERTNULL(list);

    return 0;
}

static int test_acll_nextFilter_1(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_nextFilter(list, NULL, NULL);
    ASSERTNOTNULL(list);
    ASSERTSTR("element 1", (char *) list->payload);

    return 0;
}

static int test_acll_nextFilter_2(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_nextFilter(list, test_acll_nextFilter_sub, NULL);
    ASSERTNOTNULL(list);
    ASSERTSTR("element 2", (char *) list->payload);

    return 0;
}

static int test_acll_prevFilter_sub(void *payload, void *input) {
    if (!strcmp((char *) payload, (char *) input)) {
        return 1;
    }
    return 0;
}

static int test_acll_prevFilter_0(void *data) {
    acll_t *list = NULL;
    list = acll_prevFilter(NULL, NULL, "element 0");
    ASSERTNULL(list);

    return 0;
}

static int test_acll_prevFilter_1(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_prevFilter(list->next->next, NULL, "element 0");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 1", (char *) list->payload);

    return 0;
}

static int test_acll_prevFilter_2(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_prevFilter(list->next->next, test_acll_prevFilter_sub, "element 0");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 0", (char *) list->payload);

    return 0;
}

static int test_acll_firstFilter_sub(void *payload, void *input) {
    if (!strcmp((char *) payload, "element 2")) {
        return 1;
    }
    return 0;
}

static int test_acll_firstFilter_0(void *data) {
    acll_t *list = NULL;
    list = acll_firstFilter(NULL, NULL, NULL);
    ASSERTNULL(list);

    return 0;
}

static int test_acll_firstFilter_1(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_firstFilter(list, NULL, NULL);
    ASSERTNOTNULL(list);
    ASSERTSTR("element 0", (char *) list->payload);

    return 0;
}

static int test_acll_firstFilter_2(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_firstFilter(list->next->next, test_acll_firstFilter_sub, NULL);
    ASSERTNOTNULL(list);
    ASSERTSTR("element 2", (char *) list->payload);

    return 0;
}

static int test_acll_lastFilter_sub(void *payload, void *input) {
    if (!strcmp((char *) payload, (char *) input)) {
        return 1;
    }
    return 0;
}

static int test_acll_lastFilter_0(void *data) {
    acll_t *list = NULL;
    list = acll_lastFilter(NULL, NULL, "element 0");
    ASSERTNULL(list);

    return 0;
}

static int test_acll_lastFilter_1(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_lastFilter(list, NULL, "element 0");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 2", (char *) list->payload);

    return 0;
}

static int test_acll_lastFilter_2(void *data) {
    acll_t *list = NULL;

    list = acll_append(list, "element 0");
    list = acll_append(list, "element 1");
    list = acll_append(list, "element 2");

    list = acll_lastFilter(list, test_acll_lastFilter_sub, "element 0");
    ASSERTNOTNULL(list);
    ASSERTSTR("element 0", (char *) list->payload);

    return 0;
}

int main(int argc, char **argv) {
    TESTCALL("test_acll_append_0", test_acll_append_0, NULL);
    TESTCALL("test_acll_append_1", test_acll_append_1, NULL);
    TESTCALL("test_acll_append_2", test_acll_append_2, NULL);
    TESTCALL("test_acll_concat_0", test_acll_concat_0, NULL);
    TESTCALL("test_acll_concat_1", test_acll_concat_1, NULL);
    TESTCALL("test_acll_concat_2", test_acll_concat_2, NULL);
    TESTCALL("test_acll_concat_3", test_acll_concat_3, NULL);
    TESTCALL("test_acll_first_0", test_acll_first_0, NULL);
    TESTCALL("test_acll_first_1", test_acll_first_1, NULL);
    TESTCALL("test_acll_first_2", test_acll_first_2, NULL);
    TESTCALL("test_acll_last_0", test_acll_last_0, NULL);
    TESTCALL("test_acll_last_1", test_acll_last_1, NULL);
    TESTCALL("test_acll_last_2", test_acll_last_2, NULL);
    TESTCALL("test_acll_pop_0", test_acll_pop_0, NULL);
    TESTCALL("test_acll_pop_1", test_acll_pop_1, NULL);
    TESTCALL("test_acll_push_0", test_acll_push_0, NULL);
    TESTCALL("test_acll_push_1", test_acll_push_1, NULL);
    TESTCALL("test_acll_push_2", test_acll_push_2, NULL);
    TESTCALL("test_acll_count_0", test_acll_count_0, NULL);
    TESTCALL("test_acll_count_1", test_acll_count_1, NULL);
    TESTCALL("test_acll_remove_0", test_acll_remove_0, NULL);
    TESTCALL("test_acll_remove_1", test_acll_remove_1, NULL);
    TESTCALL("test_acll_remove_2", test_acll_remove_2, NULL);
    TESTCALL("test_acll_remove_3", test_acll_remove_3, NULL);
    TESTCALL("test_acll_remove_4", test_acll_remove_4, NULL);
    TESTCALL("test_acll_delete_0", test_acll_delete_0, NULL);
    TESTCALL("test_acll_delete_1", test_acll_delete_1, NULL);
    TESTCALL("test_acll_delete_2", test_acll_delete_2, NULL);
    TESTCALL("test_acll_delete_3", test_acll_delete_3, NULL);
    TESTCALL("test_acll_delete_4", test_acll_delete_4, NULL);
    TESTCALL("test_acll_clone_0", test_acll_clone_0, NULL);
    TESTCALL("test_acll_clone_1", test_acll_clone_1, NULL);
    TESTCALL("test_acll_clone_2", test_acll_clone_2, NULL);
    TESTCALL("test_acll_in_0", test_acll_in_0, NULL);
    TESTCALL("test_acll_in_1", test_acll_in_1, NULL);
    TESTCALL("test_acll_in_2", test_acll_in_2, NULL);
    TESTCALL("test_acll_in_3", test_acll_in_3, NULL);
    TESTCALL("test_acll_in_4", test_acll_in_4, NULL);
    TESTCALL("test_acll_in_5", test_acll_in_5, NULL);
    TESTCALL("test_acll_sort_0", test_acll_sort_0, NULL);
    TESTCALL("test_acll_find_0", test_acll_find_0, NULL);
    TESTCALL("test_acll_find_1", test_acll_find_1, NULL);
    TESTCALL("test_acll_find_2", test_acll_find_2, NULL);
    TESTCALL("test_acll_find_3", test_acll_find_3, NULL);
    TESTCALL("test_acll_nextFilter_0", test_acll_nextFilter_0, NULL);
    TESTCALL("test_acll_nextFilter_1", test_acll_nextFilter_1, NULL);
    TESTCALL("test_acll_nextFilter_2", test_acll_nextFilter_2, NULL);
    TESTCALL("test_acll_prevFilter_0", test_acll_prevFilter_0, NULL);
    TESTCALL("test_acll_prevFilter_1", test_acll_prevFilter_1, NULL);
    TESTCALL("test_acll_prevFilter_2", test_acll_prevFilter_2, NULL);
    TESTCALL("test_acll_firstFilter_0", test_acll_firstFilter_0, NULL);
    TESTCALL("test_acll_firstFilter_1", test_acll_firstFilter_1, NULL);
    TESTCALL("test_acll_firstFilter_2", test_acll_firstFilter_2, NULL);
    TESTCALL("test_acll_lastFilter_0", test_acll_lastFilter_0, NULL);
    TESTCALL("test_acll_lastFilter_1", test_acll_lastFilter_1, NULL);
    TESTCALL("test_acll_lastFilter_2", test_acll_lastFilter_2, NULL);
    return 0;
}