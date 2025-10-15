# Contributing to the Probabilistic Library.

The Probabilistic Library is open source and we welcome contributions from everyone.
However, we have some rules for that.

I. It should fit in our primary focus: probabilistic.
New methods or distributions should be usefull for more than one purpose.

II. Code should follow our guidelines (see below).

III. New code should be tested with unit tests and documented (see below).

IV. New code should not lead to new dependencies.
If you think a new dependency is needed, contact us before making your changes.
The current dependencies are given in:
[wiki PL](https://github.com/Deltares/ProbabilisticLibrary/wiki/Tools-and-other-software)

V. Make a pull request for your contribution, or create a issue with your suggestion.

VI. All contributions will get a header with (c) Deltares

# Code guide lines

We follow a part of the guide lines
of [AirSim](https://github.com/microsoft/AirSim/blob/main/docs/coding_guidelines.md)
with a few exceptions :

## Naming Conventions

Avoid using any sort of Hungarian notation on names and "_ptr" on pointers.

| **Code Element** | **Style** | **Comment** |
| --- | --- | --- |
| Namespace | CamelCase | Differentiate from class names ; nested namespaces are joined on one line with '::'. |
| Class name | CamelCase | To differentiate from STL types which ISO recommends (do not use "C" or "T" prefixes) |
| Function name | camelCase | Lower case start is almost universal except for .Net world |
| Parameters/Locals | under\_scored | Vast majority of standards recommends this because \_ is more readable to C++ crowd (although not much to Java/.Net crowd) |
| Member variables | under\_scored | The prefix \_ is heavily discouraged as ISO has rules around reserving \_identifiers |
| Enums and its members | CamelCase | Most except very old standards agree with this one |
| Globals | g\_under\_scored | You shouldn't have these in first place! |
| Constants | under\_scored | Very contentious and we just have to pick one here, unless if is a private constant in class or method, then use naming for Members or Locals |
| File names | Match case of class name in file | In principe one class per file. A small helper struct is allowed in the same file. |

## Bracketing

We are using the Allman style of Bracketing, see: [Indent style](https://en.wikipedia.org/wiki/Indent_style).
Notice that curlies are also required if you have a single statement.

```
int main(int argc, char* argv[])
{
    while (x == y)
    {
        foo();
        bar();
    }
}
```

## Const and References

Religiously review all non-scalar parameters you declare to be candidate for const and references.
If you are coming from languages such as C#/Java/Python,
the most often mistake you would make is to pass parameters by value instead of `const T&;`
Especially most of the strings, vectors and maps you want to 
pass as `const T&;` (if they are readonly) or `T&` (if they are writable). Also add `const` suffix to methods as much as possible.

Resharper can help you with this.

## Overriding
When overriding virtual method, use override suffix.


## Pointers

This is really about memory management.
A simulator has much performance critical code, so we try and avoid overloading the memory manager
with lots of calls to new/delete.
We also want to avoid too much copying of things on the stack, so we pass things by reference when ever possible.
But when the object really needs to live longer than the call stack you often need to allocate that object on
the heap, and so you have a pointer.  Now, if management of the lifetime of that object is going to be tricky we recommend using 
[C++ 11 smart pointers](https://cppstyle.wordpress.com/c11-smart-pointers/). 
But smart pointers do have a cost, so don’t use them blindly everywhere.  For private code 
where performance is paramount, raw pointers can be used.  Raw pointers are also often needed when interfacing with legacy systems
that only accept pointer types, for example, sockets API.  But we try to wrap those legacy interfaces as
much as possible and avoid that style of programming from leaking into the larger code base.  

Religiously check if you can use const everywhere, for example, `const float * const xP`.
Avoid using prefix or suffix to indicate pointer types in variable names,
i.e. use `my_obj` instead of `myobj_ptr` except in cases where it might make sense to differentiate variables better, for example,
`int mynum = 5; int* mynum_ptr = mynum;`

## Indentation

The C++ code base uses four spaces for indentation (not tabs).
In Python code tabs are allowed, but per file there is consistent usage of tabs or spaces.

## Other style settings

- we use ` #pragma once ` in header files to protect against multiple inclusion.
- do not use auto for basic types as int, double and string.
- use auto to avoid a classname left and right of the assignment,
especially in combination with an unique or shared pointer declaration.
- maximum line length is 120 characters; maximum file size is 750 lines.
- when throwing an exception, use the exception class in probLibException.h,
so that we can distinguish between exception from our own library or system libraries.
- when using std::format provide fall back code for compilers that do not support it.
- TODO's are allowed to identify corner cases.
- use 0.0, 1.0 etc if they are doubles
- counters may be i,j,k , but loops are preferable of the form : for( const auto& o : listOfObjects) {}
- in principle no abbreviations in names of methods, members and variables.
- Boy Scout Rule: if you change an existing file, leave the code better than you found it.
If the change is very small, do your changes in the existing style. That makes reviewing easier.
If the change is substantial, and a style change is needed, do the style changes in a separate commit.

# Finishing your work

After your pull request is approved, merge your changes into the master.
That can be done using a merge commit or a squash commit.
The last one keeps the history cleaner, but can give difficulties when another issue is based on this branch.

Note that the release notes are generated using the title of the PR,
so check the title for typos and make sure it is understandable for others.

After the merge the branch must be deleted.

# Documentation
Algorithms are described in the scientific background.

Code should have inline documentation for each class and all the public methods and members.

Example:
```
    /**
     * \brief Performs adaptive importance sampling
     */
    class AdaptiveImportanceSampling : public ReliabilityMethod
    {
    public:
        /**
         * \brief Settings for adaptive importance sampling
         */
        auto Settings = AdaptiveImportanceSamplingSettings();

        /**
         * \brief Executes the adaptive importance sampling
         * \param modelRunner Performs execution of samples
         * \return Design point
         */
        DesignPoint getDesignPoint(const Models::ModelRunner& modelRunner) override;
        ....
    }

```
