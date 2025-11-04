# 🤝 Contributing to the Probabilistic Library.

The **Probabilistic Library** is open source, and we’re excited to welcome contributions from anyone interested!
To keep things consistent and high quality, please take a moment to read through our guidelines below.

🧭 I. Scope
We focus on **probabilistic**, **sensitivity**, and **uncertainty analysis**.
Contributions should fit within this scope.
Deltares will review and decide whether a contribution is included in the main branch.

💻 II. Code Style
Please make sure your code follows our coding guidelines (see below).
Consistent style helps keep the library easy to read and maintain.

🧪 III. Testing and Documentation
All new code should include **unit tests** and **clear documentation**.
If you’re adding something new, show how it works and how others can use it.

©️ IV. Copyright
Every contribution will include a header with **(c) Deltares**.

📦 V. Dependencies
New code should not lead to new dependencies.
Try to avoid adding new dependencies.
If you believe one is truly needed, please discuss it with us before starting your work.
You can find our current dependencies here: [wiki PL](https://github.com/Deltares/ProbabilisticLibrary/wiki/Tools-and-other-software).

Quick summary:
- **C++** code follows the **C++-20** standard
- **Python** code targets **Python 3.11**.

✅ VI. Completeness
New code should be **ready to use** — no TODOs, please!
That means including any required **Python wrappers** and, for new distributions, implementing **all necessary methods**.

🧹 VII. **No** Boy Scout Rule
When you’re fixing or improving something, focus **only on the changes related to your issue**.
This keeps reviews and merges clean and simple.
If you notice something else that needs cleanup, feel free to **open a new issue** for it.

🚀 VIII. Submitting Your Work

You can contribute in two ways:
- Make a pull request with your changes.
- Open an issue if you want to discuss an idea first.

We’re happy to help guide you through the process — whether it’s your first contribution or your fiftieth!


# Code guidelines

For C++ we follow a part of the guidelines of [AirSim](https://github.com/microsoft/AirSim/blob/main/docs/coding_guidelines.md) with a few exceptions.

For Python we will follow [PEP8 - Style guide for Python Code](https://peps.python.org/pep-0008/).

## Naming Conventions for C++

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
Notice that curlies are also required if you have a single statement, except for return statements.

```
int main(int argc, char* argv[])
{
    ...

    if (x == 0.0) return -1

    while (x == y)
    {
        foo();
        bar();
    }
    
}
```

## Const and References

Review all non-scalar parameters you declare to be candidate for const and references.
Especially most of the strings, vectors and maps you want to 
pass as `const T&;` (if they are readonly) or `T&` (if they are writable). Also add `const` suffix to methods as much as possible.

Resharper can help you with this.

## Overriding
When overriding a virtual method, use override suffix.

## Pointers

The Probabilistic Library has much performance critical code, so we try and avoid overloading the memory manager
with lots of calls to new/delete.
We also want to avoid too much copying of things on the stack, so we pass things by reference when ever possible.
But when the object really needs to live longer than the call stack you often need to allocate that object on
the heap, and so you have a pointer. In that case we use unique pointers or shared pointers, both implementations of smart pointers.
But smart pointers do have a cost, so use them only where needed.

## Indentation

Both Python and C++ code base uses four spaces for indentation (not tabs).

## Other style settings, both Python and C++

- the maximum line length is 120 characters; maximum file size is 750 lines.
Long lines are inconvenient when doing a side-by-side diff.
- use 0.0, 1.0 etc if they are floats/doubles; use 0, 1 etc if they are integers.
- in principle no abbreviations in names of methods, members and variables.

## Other style settings, C++ specific

- we use ` #pragma once ` in header files to protect against multiple inclusion.
- do not use ` auto ` for basic types as int, double and string.
- use ` auto ` to avoid a classname left and right of the assignment,
especially in combination with an unique or shared pointer declaration.
- when throwing an exception, use the exception class in Deltares::Probabilistic::Utils,
so that we can distinguish between exceptions from our own library or the system libraries.
- when using std::format provide fall back code for compilers that do not support it.
Besides this, all features of C++20 are allowed. We do not use the Boost library.
- counters may be i,j,k , but loops are preferable of the form : for( const auto& o : listOfObjects) {}

## Finishing your work

Once your pull request (PR) has been approved — great job! 🎉
You can now **merge your changes** into the ` master ` branch.

You can use either:

- a **merge commit**, or
- a **squash commit** (this keeps the history cleaner, but can cause issues if another branch depends on your work).

Before merging, double-check your **PR title**:

- The title is used to automatically generate the release notes.
Please make sure it’s free of typos and clearly describes your change.

After merging, don’t forget to **delete your branch** — keeping things tidy helps everyone!

# Documentation
We aim to make both our code and our scientific background easy to understand and extend.

- **Scientific background**:
Algorithms are explained in the *Scientific Background* document, which follows the Deltares corporate identity.
While generating this document can be complex, the main body is written in plain LaTeX, so external contributors can update it if needed.
- **Python examples**:
Python functionality should be demonstrated with **examples in a Jupyter notebook**.
This helps users understand how to apply your code in real-world scenarios.
- **Inline documentation**:
Each **class**, and all **public methods and members**, should have clear inline documentation.
A few well-written comments can save future contributors (including you!) a lot of time.

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
