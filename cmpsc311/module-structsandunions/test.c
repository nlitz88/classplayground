#include <stdio.h>

int main() {

    // STRUCTS

    // As far as I can gather right now:
    // When compiler reaches this struct definition, it will generate assembly instructions
    // to allocate memory on the stack for each variable contained within the struct.
    // Basically, the struct gets it's own "block" of memory where each of its elements are allocated.

    // Then, it seems that the struct "user" just acts as a label to the memory in which
    // the struct's elements are contained. For the programmer's purposes, this just means
    // it serves as a convenient means of grouping data?

    // In memory, a struct's elements are stored inside a section of memory that is GREATER THAN
    // or EQUAL TO the size of all the members combined. Due to different sizes, there may be 
    // PADDING between elements. Therefore, the elements aren't necessarily contiguous.

    // DEFINITION of a struct called "user"
    // struct keyword <struct tag>

    // This is a struct declaration. This is basically just a reference that the compiler uses to determine
    // what kind of assembly instructions it'll need to generate to allocate enough memory for a struct
    // of the type "user".
    struct user {
        char *username;
        int age;
        float balance;
    } u3;

    // Important Note: Struct members can NOT be initialized within struct 
    // Again, this is because the struct hasn't yet been defined, and therefore there'd be nowhere to put the value.

    // Here's just a DEFINITION of a struct. This is where memory is allocated for it on the stack.
    struct user u1;

    // Then, you can access teh values of u1 using dot notation.
    u1.username = "nlitz88";
    u1.age = 20;
    u1.balance = 1.99;

    printf("u1.sername: %s", u1.username);
    printf("u1.age: %d", u1.age);

    // Initializing the values of the "user" struct using curly brace notation.

    struct user u2 = {"Ryaguya", 17, 21.20};
    // NOTE: how "user" basically serves as the an alias for the declaration for the struct type "user" above!.
    //       Thus, this is how we can make anonymous struct declarations, by just not calling them anything
    //       and using the declaration in the definition!

    
    // Also, here's an example of a named enum, which is effectively like a struct.
    // I know they can be typdeffed, I'm not really sure of the utility in naming them yet.
    typedef enum {
        READY,
        NOTREADY
    } STATUS;
    STATUS reactorStatus = READY;

    // Here's a named example, but as you can see, it makes a lot more sense just to typedef
    // the type "enum status2" as something like "STATUS2" so you don't have to write enum status2 every time.
    enum status2 {
        UNINITIALIZED,
        INITIALIZED
    };
    enum status2 reactor2Status = UNINITIALIZED;



    // NESTED Struct examples.

    // Here's an anonymous struct declaration and some corresponding definitions using the anonymous declaration.
    
    struct vehicle {
        char name[128]; // Make and model
        int mileage; // The current mileage
        // Here, one of our elements will an enum of enum status2.
        enum status2 secondStatus;
        // Another one will more cleanly be STATUS.
        STATUS status1;
        // Or, our structure elements can have their declarations "inline" with the definition of the member variable itself!
        struct {
            int cylinders;
            int horsepower;
            int hours_smoh;
        } engine;
    } gremlin, cayman, cessna180, montauk;

    // The problem with the above structure definition style is that, what if we have another structure that also has an engine?
    // Well, now that struct can't use the anonymously declared struct within our anonymous vehicle struct.
    
    // One solution: We could give our struct for simpleEngine a name like this:
    struct simpleEngine {
        int cylinders;
        int horsepower;
        int hours_smoh;
    };
    // And then, it could be referenced as a member in another struct like this:
    struct {
        char* name;
        struct simpleEngine;
    };
    // This is fine, but it requires that we refer to our simpleEngine struct as "struct simpleEngine" as the type...every time.
    // This exposes too much detail/complexity about our simpleEngine struct. Rather, when being used more generally across 
    // a program, it would be much more convenient to just treat this "struct simpleEngine" as its own type.

    // Therefore, we can more productively create a new type for our simpleEngine as follows.
    typedef struct simpleEngine SIMPLE_ENGINE;
    // typedef, where the type is "struct simleEngine" and the new alias for that type is SIMPLE_ENGINE;


    // Because we know how declarations, struct declarations too, work, we can use an anonymous declaration to
    // declare a struct type as follows:
    typedef struct {
        char* username[128];
        int age;
        int yearBorn;

    } USER;
    // This might make more sense syntactically if it was laid out like this:
    // typedef struct { char* username[128]; int age; int yearBorn;} USER;
    // This means the same thing, and just might make a little bit more sense when understanding
    // how declarations work and how they're referenced/used.


    // NOTE: TYPEDEFS ARE JUST DECLARATIONS. Therefore, just like any function declaration, their scope is limited
    //       to the scope in which they are defined, unless specified/declared otherwise.

    u3.age = 10;

    return 0;
} 