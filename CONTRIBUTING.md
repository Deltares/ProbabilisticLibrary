# 🤝 Contributing to the Probabilistic Library.

The **Probabilistic Library** is open source, and we’re excited to welcome contributions from anyone interested!
To keep things consistent and high quality, please take a moment to read through our guidelines below.

🧭 I. Scope  
We focus on **reliability**, **sensitivity**, **statistical analysis** and **uncertainty analysis**.
Contributions should fit within this scope.
Deltares will review and decide whether a contribution is included in the main branch.

💻 II. Code Style  
Please make sure your code follows our coding guidelines (see [below](#code-guidelines)).
Consistent style helps keep the library easy to read and maintain.

🧪 III. Testing and Documentation  
All new code should include **unit tests** and **clear documentation**.
If you’re adding something new, show how it works and how others can use it.

©️ IV. Copyright  
Every contribution will include a header with **(c) Deltares**.

📦 V. Dependencies  
New code should not lead to new dependencies.
New dependencies are not allowed.
If you believe one is truly needed, please discuss it with us before starting your work.
You can find our current dependencies here: [wiki PL](https://github.com/Deltares/ProbabilisticLibrary/wiki/Tools-and-other-software).

Quick summary:
- **C++** code follows the **C++20** standard
- **Python** code targets **Python 3.11**.

✅ VI. Completeness  
New code should be **ready to use** — no TODOs, please!
That means including any required **Python wrappers** and,
always implement all needed and less needed functionality.
E.g. for new distributions, implementing **all necessary methods**.

🧹 VII. **No** Boy Scout Rule  
When you’re fixing or improving something, focus **only on the changes related to your issue**.
This keeps reviews and merges clean and simple.
If you notice something else that needs cleanup, feel free to **open a new issue** for it.

🚀 VIII. Submitting Your Work  
You can contribute in two ways:
- Make a pull request with your changes. This pull request will be reviewed by Deltares.
- Open an issue if you want to discuss an idea first.

We’re happy to help guide you through the process — whether it’s your first contribution or your fiftieth!

🏁 IV. Finishing your work  
Once your pull request (PR) has been approved — great job! 🎉
You can now **merge your changes** into the ` master ` branch.

You can use either:

- a **merge commit**, or
- a **squash commit** (this keeps the history cleaner, but can cause issues if another branch depends on your work).

Before merging, double-check your **PR title**:

- The title is used to automatically generate the release notes.
Please make sure it’s free of typos and clearly describes your change.

After merging, don’t forget to **delete your branch** — keeping things tidy helps everyone!


# Code guidelines

For C++ we follow a part of the guidelines of [AirSim](https://github.com/microsoft/AirSim/blob/main/docs/coding_guidelines.md) with a few exceptions.

For Python we will follow [PEP8 - Style guide for Python Code](https://peps.python.org/pep-0008/).

## Naming Conventions for C++

Avoid using any sort of Hungarian notation on names and "_ptr" on pointers.

Do not use abbreviations, only for commonly known things. When using abbreviations, spell them as nouns: only the first character can be upper case, e.g. pdf, calculatePdf, but not PDF.

| **Code Element** | **Style** | **Comment** |
| --- | --- | --- |
| Namespace | PascalCase | The namespace may not be used as a class name ; nested namespaces are joined on one line with '::' |
| Class name | PascalCase | To differentiate from STL types which ISO recommends (do not use "C" or "T" prefixes) |
| Function name | camelCase | Lower case start is almost universal except for .Net world |
| Parameters/Locals | snake\_case | - |
| Member variables | snake\_case | - |
| Enums and its members | PascalCase | Most except very old standards agree with this one |
| Globals | g\_snake\_case | You shouldn't use globals in the first place! |
| Constants | snake\_case | - |
| File names | Match case of class name in file | In principle one class per file. A small helper struct, class or enum is allowed in the same file. |

Function names are a verb (optionally followed by noun). For example: run, doWork, calculateReliability. All other names are noun (optionally preceded with adjective). For example: worker, reliabilityCalculator, reliability.

The naming convention for Google test cases is: TEST(CategoryName, testMethodName).


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

## Const and References (C++ only)

Review all non-scalar parameters you declare to be candidate for const and references.
Especially most of the strings, vectors and maps you want to 
pass as `const T&;` (if they are readonly) or `T&` (if they are writable). Also add `const` suffix to methods, or make them `static`, as much as possible.

Resharper can help you with this.

## Overriding
When overriding a virtual method, use override suffix.

## Memory Management Guidelines (C++ only)

*motivation*

The Probabilistic Library contains a significant amount of performance-critical code. To maintain efficiency, we aim to minimize overhead caused by frequent dynamic memory operations. In particular, avoid excessive calls to `new` and `delete`, as these can overload the memory manager and degrade performance.

To reduce unnecessary copying, prefer passing objects by reference whenever possible. This practice helps minimize stack usage and improves execution speed.

In cases where an object must outlive the current call stack, allocate it on the heap. Such allocations require the use of pointers. For these situations, use `std::unique_ptr` or `std::shared_ptr`, both of which are smart pointer implementations that provide safer and more maintainable memory management.

Be aware that smart pointers introduce a small amount of overhead. Use them judiciously—only when the object’s lifetime or ownership semantics justify their cost.

*rules*

1. Prefer to use locally instantiated objects and pass them by reference in functions  
2. If the object can live longer than the callstack (starting at creation of the object, can be a class where it is owned), use smart pointers  
3. If the object is passed outside the C++ scope, new/delete is allowed if no other option applies  
4. Factories should return an object  

## Indentation

Both Python and C++ code base use four spaces for indentation (not tabs).

## Other style settings, both Python and C++

- The maximum line length is 120 characters; maximum file size is 750 lines.
Long lines are inconvenient when doing a side-by-side diff.
- Use 0.0, 1.0 etc. if they are floats/doubles. Use 0, 1 etc. if they are integers.

## Other style settings, C++ specific

*header files*
- We use ` #pragma once ` in header files to protect against multiple inclusion.
- Code in header files only if very short (at most 3 lines) and trivial.
- `using namespace` is not allowed in header files.

*sources files*
- Do not use ` auto ` for basic types as int, double and string.
- Use ` auto ` to avoid a classname left and right of the assignment,
especially in combination with an unique or shared pointer declaration.
- When throwing an exception, use the exception class in Deltares::Probabilistic::Utils,
so that we can distinguish between exceptions from our own library or the system libraries.
- All features of C++20 are allowed. We do not use the Boost library.
- We prefer new style C++, e.g. casting with std::static_cast.
- We prefer structs over tuples.
- Counters may be i,j,k , but loops are preferable of the form : for( const auto& o : listOfObjects) {}.
- The code complexity (measured as McCabe complexity) must be < 20. This is automatically checked for all Pull Requests.

# Documentation
We aim to make both our code and our scientific background easy to understand and extend.

- **Scientific background**:
Algorithms are explained in the [Scientific Background](https://github.com/Deltares/ProbabilisticLibrary/tree/master/doc) document, which follows the Deltares corporate identity.
While generating this document can be complex, the main body is written in plain LaTeX, so external contributors can update it if needed.
- **Python examples**:
Python functionality should be demonstrated with examples in [Jupyter notebooks](https://github.com/Deltares/ProbabilisticLibrary/tree/master/sources/Deltares.Probabilistic.PWrapper.Notebooks).
This helps users understand how to apply your code in real-world scenarios.
- **Inline documentation**:
Each **class**, and all **public methods and members**, should have clear inline documentation. Also all protected and private methods which need explanation.
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
