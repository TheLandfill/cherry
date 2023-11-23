# Cherry

I don't know what to call this, but it's basically everything I need to
implement working finite fields. If anyone can think of a better name, let me
know.

## Setup

It's a header-only library, so the steps are trivial.

1.  Download the repository or just the [include directory](include/).
1.  Include what you need or just `#include "gf.hpp"`.

## Use

There are three main types of objects in this project.

-   Finite Fields
-   Polynomials
-   Matrices

I've worked hard on making sure that all of these objects work as you'd expect.

### Finite Fields

There are actually two kinds of finite fields: GF(p) and GF(p^n). I need them to
be separate so that I can use GF(p) as a base for GF(p^n). You can see how to
use GF1\<p\> below.

```cpp
#include "gf.hpp"

int main() {
    GF1<5> a = 3;
    GF1<5> b = 4;
    std::cout <<
    "Variable Values\n"
    "----------------------------------------\n";
    std::cout << "a\t= " << a << "\n";
    std::cout << "b\t= " << b << "\n\n";
    std::cout <<
    "Arithmetic Operations\n"
    "----------------------------------------\n";
    std::cout << "a + b\t= " << a + b << "\n";
    std::cout << "a - b\t= " << a - b << "\n";
    std::cout << "a b\t= " << a * b << "\n";
    std::cout << "a / b\t= " << a / b << "\n\n";
    std::cout << "Powers of a\n"
    "----------------------------------------\n";
    for (int i = -5; i <= 5; i++) {
        std::cout << "a^" << i << "\t= " << (a^i) << "\n";
    }
    std::cout << "\n"
    "Powers of b\n"
    "----------------------------------------\n";
    for (int i = -5; i <= 5; i++) {
        std::cout << "b^" << i << "\t= " << (b^i) << "\n";
    }
    std::cout << one<GF1<5>>() << "\n";
    std::cout << zero<GF1<5>>() << "\n";
    return 0;
}
```

For n > 2, it's almost the same except you have to provide an irreducible
polynomial whose splitting field is the finite field you want to work in. ***YOU
CANNOT HAVE ELEMENTS WITH DIFFERENT IRREDUCIBLE POLYNOMIALS INTERACT***.

```cpp
#include "gf.hpp"

int main() {
    Polynomial<GF1<5>> irreducible_poly{{1, 4, 3, 1}};
    Polynomial<GF1<5>> poly1{{2, 4, 1}};
    Polynomial<GF1<5>> poly2{{2, 4, 1}};
    GF<5> a{irreducible_poly, poly1};
    GF<5> b{irreducible_poly, poly2};
    std::cout <<
    "Variable Values\n"
    "----------------------------------------\n";
    std::cout << "a\t= " << a << "\n";
    std::cout << "b\t= " << b << "\n\n";
    std::cout <<
    "Arithmetic Operations\n"
    "----------------------------------------\n";
    std::cout << "a + b\t= " << a + b << "\n";
    std::cout << "a - b\t= " << a - b << "\n";
    std::cout << "a b\t= " << a * b << "\n";
    std::cout << "a / b\t= " << a / b << "\n\n";
    std::cout << "Powers of a\n"
    "----------------------------------------\n";
    for (int i = -5; i <= 5; i++) {
        std::cout << "a^" << i << "\t= " << (a^i) << "\n";
    }
    std::cout << "\n"
    "Powers of b\n"
    "----------------------------------------\n";
    for (int i = -5; i <= 5; i++) {
        std::cout << "b^" << i << "\t= " << (b^i) << "\n";
    }
    // You have to specify the irreducible polynomial for GF<p> to work since
    // you at least need to know the order of the field, which means I can't
    // come up with an all-purpose zero function. You need to specify the type
    // and the irreducible polynomial. Here, we're specifying it explicitly.
    std::cout << one<GF<5>>(nullptr, &irreducible_poly) << "\n";
    // Here, we're specifying it implicitly since the compiler can infer the
    // type from the type of a.
    std::cout << zero(&a, &irreducible_poly) << "\n";
    return 0;
}
```

## Supported Features

-   **Finite Fields**
    -   **Arithmetic Operations:** You can add, subtract, multiply, and divide
        by any element of any finite field.
    -   **Exponentiation:** You can raise any element of a finite field to an
        arbitrary integer power.
    -   **Printing:** You can print elements of finite fields out like normal.
        For `GF1<p>`, it's just printed out as an integer. For `GF<p>`, it's
        printed out as a list of coefficients from highest degree to lowest
        degree.
-   **Matrices**
    -   **Arithmetic Operations:** You can do all the standard arithmetic
        operations for matrices with elements in any field.
    -   **Exponentiation:** You can raise any matrix arbitrary integer power.
    -   **Accessing Elements:** I've overloaded the `operator()` to access
        elements.
    -   **Matrix Inverses:** I've implemented the standard RREF algorithm for
        matrices to find the inverse matrix.
    -   **Elementary Row Operations:** Speaking of RREF, I've also implemented
        the elementary row operations.
    -   **Simple Matrices:** I've implemented certain base matrices, including
        the zero matrix, the identity matrix, the circulant matrix basis, etc.
-   **Polynomials**
    -   **Addition and Subtraction:** Wasn't too difficult to implement.
    -   **Accessing the Coefficients:** Same as with a vector.
    -   **Evaluating for Arbitrary Input:** If you can reasonably plug a value
        into a polynomial, you can evaluate the polynomial at that value. For
        example, you can plug a square matrix into a polynomial so long as you
        can multiply the matrix by the coefficients.

## Missing Features

Here's a list of all the features the code does not support:

-   **Finite Fields**
    -   **Factoring:** Currently, I'm factoring polynomials by guessing every
        possible root in the finite field large enough to hold all possible
        solutions. It's also not an explicit function in the code anywhere
        except in one of the tests.
    -   **Field Extensions:** I have no support for field extensions besides
        redoing your work in a larger field.
    -   **Field Reduction:** There's no way to convert an element in the field
        GF(p^n) to GF(p) even when you end up with an element in the field.
    -   **Large Finite Fields:** Under the hood, I'm using things like integers
        to work with finite fields, which means I only have 32 or 64 bits to
        work with. This becomes a big problem when you have fields with too many
        elements to fit into 64 bits.
    -   **Other Representations:** While I've set the code up to work with
        arbitrary representations of GF(p^n), I've chosen to work with the
        matrix representation of finite fields. If you want to implement the
        polynomial representation directly, feel free.
    -   **Specialization for GF(2^n):** Everyone and their mother has already
        implemented this, but it should be easy enough to extend.
    -   **Generating Roots of Unity:** It's missing in general from every field.
-   **Matrices**
    -   **General RREF:** I only implemented this when finding the inverse, but
        I can probably implement something else later.
    -   **Dealing with Poorly Behaved Floating Point Matrices:** Currently, I'm
        checking if certain elements are zero because I've been working with
        finite fields. For floats, I should instead use a better algorithm.
    -   **Determinants:** Not implemented yet as I'm looking into good
        algorithms to calculate the determinant.
    -   **Characteristic Polynomials:** Not implemented yet as I'm looking into
        good algorithms to calcualte the characteristic polynomial.
    -   **Matrix Factorizations:** Probably lowest on my list, tbh. Might be
        fun, though.
-   **Polynomials**
    -   **Factoring:** I'm starting to think that factoring might be a difficult
        sort of problem.
    -   **Root Finding:** Same as factoring. I'd have to implement Newton's
        Method for real/complex numbers and finite field stuff.
    -   **Polynomial Multiplication and Division:** I should be able to
        implement this really quickly. If I haven't in a week or so, feel free
        to tell me.
    -   **Special Polynomials:** If it's a named class of polynomials, I don't
        have it.

It may look like a lot, but getting GF(p^n) with n > 1 to work is by far the
most complicated part of this process.

## Extensions

If you want to extend this code to your own types, you'll need to probably
extend from `Division_Ring` in the Curiously Recurring Template Pattern to make
sure that you have all the right functions. You'll also want to implement the
`zero` and `one` functions for your specific types. See the various code I have
for examples.
