#include <stdio.h>

int add_one(int a) {
    return a + 1;
}

int main() {

    int (*adder_fxn_ptr)(int) = add_one;
    adder_fxn_ptr = NULL;

    // "adder_fxn_ptr" is the name of the function pointer.
    // int (*adder_fxn_ptr)(int) is the DECLARATION for this function pointer.
    // That is, we are declaring what TYPE of FUNCTION's address adder_fxn_ptr will maintain!

    // NOW, when creating a type definition for a function pointer type, it works the same way as when we create 
    // a type definition for any other type. For example, a regular variable definition:
    int age;
    // We define an integer variable named age. Now, if we want to define a new type that is an integer for greater
    // clarity, we would do:
    typedef int agevar;
    // Notice: by adding the typedef, what would have previously been the name of our variable now becomes the name
    // of the type definition we just created. That's because typedef looks at the declaration (type) following it
    // and then the name right afterwards. This works the same way when creating type definitions for struct types.
    struct person_struct {
        char name[10];
        int age;
    };
    typedef struct person_struct my_struct_type;

    // NOW, for FUNCTION POINTERS, the same logic applies. The reason I'm clarifying this is because it can be a little
    // unclear what's happening syntactically, when in reality the same rules apply. That is, when definining an actual
    // function pointer with its entire type declaration in-line, it looks like this:
    double (*my_func_ptr)(long);
    my_func_ptr = NULL;
    // THEN, if we want to create a type definition for this "TYPE OF FUNCTION POINTER," we simply do so by providing
    // the declaration, and then the name of the type declaration (the alias for our new type) in the same spot. Ex:
    typedef double (*some_type_of_func)(long);
    // Now, you can more easily create definitions for function pointers of this type without having to write out the
    // cumbersome old declaration with the nasty syntax. For instance:
    some_type_of_func my_func_ptr2 = NULL;

    /**
     * STRUCTURE PADDING AND ALIGNMENT
     */
    struct user {
        char first_initial;
        char last_initial;
        double balance;
        int age;
    };
    // Define using named struct type, rather than using an anonymous struct type declaration each time.
    struct user u1 = {'N', 'L', 30.25, 21};
    struct user u2 = {'K', 'L', 400.17, 23};

    return 0;
}