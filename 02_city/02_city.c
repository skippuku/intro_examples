#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <intro.h>

typedef struct {
    float x, y;
} Vector2;

typedef struct Node Node;
struct Node {
    Vector2 v;
    int id;
    Node * next;
};

typedef struct {
    int32_t version;
    char * name;
    Node * node;
} SerData_v1;

typedef struct {
    int32_t version I(1);
    char * name I(2, length name_len, ~cstring);
    int name_len;
    Node * base I(3, alias node);
    int count_nodes;
} SerData_v2;

#include "02_city.c.intro"

Node *
new_node(float x, float y) {
    static int counter = 0;
    Node * node = malloc(sizeof(*node));
    node->v.x = x;
    node->v.y = y;
    node->id = counter++;
    node->next = NULL;
    return node;
}

Node *
insert_node(Node * base, Node * node) {
    node->next = base->next;
    base->next = node;
    return node;
}

void
report_list(Node * node) {
    while (node) {
        printf("%i(%g, %g) -> ", node->id, node->v.x, node->v.y);
        node = node->next;
    }
    printf("<null>\n");
}

int
main() {
    SerData_v1 data1;
    data1.version = 1234;
    data1.name = "John Calandar of March III";
    
    Node * base = new_node(4, -5), * node;

    node = insert_node(base, new_node(-1, 2));
    insert_node(node, new_node(-300, 10.5));
    insert_node(node, new_node(87.234, 200));

    data1.node = base;

    printf("data1: ");
    intro_print(&data1, ITYPE(SerData_v1), NULL);
    printf("\nlist: ");
    report_list(data1.node);

    if (!intro_create_city_file("ser_data.cty", &data1, ITYPE(SerData_v1))) {
        printf("failed to write file.\n");
        return 1;
    }

    printf("ser_data.cty was written.\n");

    SerData_v2 data2;
    void * data_handle = intro_load_city_file(&data2, ITYPE(SerData_v2), "ser_data.cty");
    if (!data_handle) {
        printf("failed to load file.\n");
        return 1;
    }

    printf("ser_data.cty was read.\n");

    printf("data2: ");
    intro_print(&data2, ITYPE(SerData_v2), NULL);
    printf("\nlist: ");
    report_list(data2.base);

    free(data_handle);

    return 0;
}
