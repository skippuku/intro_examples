#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Welcome to introcity!
// To build this project you may will need to edit the config.mk in the root directory with the location of the introcity repository as well as the intro parser executable.

// You should include the intro.h header before declaring any types.
#include <intro.h>

// intro parses global types in the file and generates information about them.

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

// intro assumes includes that end with ".intro" are to be generated and ignores them
#include "01_intro_to_intro.c.intro"

// if the file name does not end with ".intro", intro will try to include the file and fail
// #include "types.intro.h" // Error: File not found.
//
// if you want to use a different file extension, you can make intro ignore the include.
// the __INTRO__ macro is defined by the intro preprocessor.
// #ifndef __INTRO__
// #include "types.intro.h"
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

    // To get information about a type, use the ITYPE macro.
    // This evaluates to a pointer of type (IntroType *)
    const IntroType * Person_type = ITYPE(Person);
    printf("Person type name: %s\n", Person_type->name);

    // A common thing you may want to do is iterate through the fields of a struct type.
    // Structure information is contained in the i_struct field.
    // Be warned that this field is actually part of a union that is used for different purposes
    // by different categories of types. You may want to check for the expected category.
    assert(ITYPE(Person)->category == INTRO_STRUCT);

    printf("Person struct members:\n");
    for (int member_i=0; member_i < ITYPE(Person)->count; member_i++) {
        const IntroMember * member = &ITYPE(Person)->members[member_i];

        printf("   (name '%-16s') (offset %-8u) (type name '%-16s'),\n",
                   member->name, member->offset, member->type->name);
    }
    printf("\n");

    // introlib has a builtin function for printing information about a value
    // values with type information are always passed as a (const void *) to the value and a (const IntroType *)
    printf("jon = ");
    intro_print(&jon, ITYPE(Person), NULL); // intro_print takes a pointer for extra options, NULL indicates default options
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

    // Try changing around the data types and recompiling with make. Watch the program adapt to these changes.
    // Also, try taking a look at the generated .intro file. It's a bit ugly, but can give you an idea of how this works.

    return 0;
}
