#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Welcome to intro!
// To build this project you will need to edit the Makefile in the root directory.

// The intro.h header has necessary type information used by generated headers. It also includes function prototypes for introlib.
#include <intro.h>

// intro parses all types in the file and generates information about them.

enum Color {
    COLOR_BLACK,
    COLOR_RED,
    COLOR_ORANGE,
    COLOR_YELLOW,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_VIOLET,
    COLOR_WHITE,
    COLOR_COUNT
};

typedef enum {
    FIELD_NONE = 0,
    FIELD_CONSTRUCTION,
    FIELD_MEDICINE,
    FIELD_LEGAL,
    FIELD_TECHNOLOGY,
    FIELD_EDUCATION,
    FIELD_ENTERTAINMENT,
    FIELD_COUNT
} Field;

typedef struct {
    char * name;
    enum Color color;
    uint8_t age;
} Cat;

typedef struct {
    int id_number;
    Field field;
    unsigned char age;
    char * name;
    Cat * pet;
} Person;

// the generated file must be included *after* any type declarations.
// this is because the header must reference the types to get offset/size information.

// intro ignores includes that end with ".intro"
#include "01_intro_to_intro.c.intro"

// if the file does not end with ".intro", intro will try to include the file and fail
// #include "01.intro_to_intro.intro.h" // Error: File not found.
//
// if you want to use a different file extension, you can make intro ignore the include.
// the __INTRO__ macro is always defined by the intro preprocessor.
// #ifndef __INTRO__
// #include "01_intro_to_intro.intro.h"
// #endif

int
main() {
    Cat garfield;
    garfield.name = "Garfield";
    garfield.color = COLOR_ORANGE;
    garfield.age = 7; // ??

    Person jon;
    jon.id_number = 12;
    jon.field = FIELD_ENTERTAINMENT;
    jon.age = 29;
    jon.name = "Jon Arbuckle";
    jon.pet = &garfield;

    // The easiest way to get information on a type is with the ITYPE macro
    const IntroType * Person_type = ITYPE(Person);
    printf("Person type name: %s\n", Person_type->name);

    // A common thing you may want to do is iterate through the fields of a struct type.
    // Structure information is contained in the i_struct field.
    // Be warned that this field is actually part of a union that is used for different purposes
    // by different categories of types. You may want to check for the expected category to
    // avoid undefined behavior.
    assert(ITYPE(Person)->category == INTRO_STRUCT);

    printf("Person struct members:\n");
    for (int member_i=0; member_i < ITYPE(Person)->i_struct->count_members; member_i++) {
        const IntroMember * member = &ITYPE(Person)->i_struct->members[member_i];

        printf("   (name '%-16s') (offset %-8u) (type name '%-16s'),\n",
                   member->name, member->offset, member->type->name);
    }
    printf("\n");

    // introlib has a builtin function for printing information about a value
    // values with type information are always passed as (void *) to value and (const IntroType *)
    printf("jon = ");
    intro_print(&jon, ITYPE(Person), NULL); // intro_print takes a pointer for extra options, NULL indicates default
    printf(";\n\n");

    // of course this works on any type you have information about
    printf("ITYPE(Person) = ");
    intro_print(ITYPE(Person), ITYPE(IntroType), NULL);
    printf(";\n\n");

    Cat neal;
    neal.name = "Neal";
    neal.color = COLOR_BLACK;
    neal.age = 1;

    // I don't know what this implies about Jon Arbuckle, ok.
    jon.age += 2;
    jon.field = FIELD_MEDICINE;
    jon.id_number = -1234;
    jon.name = "Timothy Alan";
    jon.pet = &neal;

    printf("jon = ");
    intro_print(&jon, ITYPE(Person), NULL);
    printf(";\n");

    // a fun exercise: change around the values and data types, recompile, watch the program adapt without you changing implementation code!

    return 0;
}
