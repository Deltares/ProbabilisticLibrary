# Contributing to the Probabilistic Library.

The Probabilistic Library is open source and we welcome contributions from everyone.
However, we have some rules for that.

I. It should fit in our primary focus: probabilistic, sensitivity and uncertainty analysis.
It is up to Deltares to decide if it should be part of the main line.

II. Code should follow our guidelines (see below).

III. New code should be tested with unit tests and documented (see below).

IV. All contributions will get a header with (c) Deltares

V. New code should not lead to new dependencies.
If you think a new dependency is needed, contact us before making your changes.
The current dependencies are given in:
[wiki PL](https://github.com/Deltares/ProbabilisticLibrary/wiki/Tools-and-other-software).
In short : C++ code follows the C++-20 standard; the python version is 3.11.

VI. New code should be complete, no TODO's.
That means: include a python wrapper, and if you add an new distribution,
all methods for distributions must be implemented.

VII. **No** Boy Scout Rule: if you change an existing file, make only the necessary changes for your issue.
This makes reviewing and merging easier.
If you think the file does not comply to our guidelines, make an issue for that.

VIII. Make a pull request for your contribution, or create a issue with your suggestion.


# Code guidelines

For C++ we follow a part of the guidelines
of [AirSim](https://github.com/microsoft/AirSim/blob/main/docs/coding_guidelines.md)
with a few exceptions.

For Python will follow [PEP8 - Style guide for Python Code](https://peps.python.org/pep-0008/).

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

Don't do this:
```
        auto distribution = std::make_shared<LogNormalDistribution>();
        auto properties = std::make_shared<StochastProperties>();
        distribution->initialize(properties, {1.0, 2.0});
        ...
        void LogNormalDistribution::initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values)
        {
            stochast->Shift = values[2];
            setMeanAndDeviation(stochast, values[0], values[1]);
        }

        void LogNormalDistribution::setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation)
        {
                ...
                double p = deviation / (mean - stochast->Shift);
                stochast->Scale = sqrt(log(1 + p * p));
                stochast->Location = log(mean - stochast->Shift) - 0.5 * stochast->Scale * stochast->Scale;
        }

```
But do this:
```
        auto distribution1 = std::make_shared<LogNormalDistribution>();
        auto properties1 = std::make_shared<StochastProperties>();
        distribution1->initialize(*properties1, {1.0, 2.0});
        ...
        // will not work in situation above
        auto distribution2 = std::make_unique<LogNormalDistribution>();
        auto properties2 = std::make_unique<StochastProperties>();
        distribution2->initialize(*properties2, {1.0, 2.0});
        ...
        // prefered situation; will not work in situation above
        auto distribution3 = LogNormalDistribution();
        auto properties3 = StochastProperties();
        distribution3.initialize(properties3, {1.0, 2.0});
        ...
        void LogNormalDistribution::initialize(StochastProperties& stochast, const std::vector<double>& values)
        {
            stochast.Shift = values[2];
            setMeanAndDeviation(stochast, values[0], values[1]);
        }

        void LogNormalDistribution::setMeanAndDeviation(StochastProperties& stochast,
            const double mean, const double deviation)
        {
                ...
                double p = deviation / (mean - stochast.Shift);
                stochast.Scale = sqrt(log(1.0 + p * p));
                stochast.Location = log(mean - stochast.Shift) - 0.5 * stochast.Scale * stochast.Scale;
        }
```
Now it is clear that the stochast is updated by the initialize, but the values remain unchanged.
And the caller of this initialize is not forced to use a shared pointer for the stochast, but still has that possibility.
When at a deep level (in this case setMeanAndDeviation) one of the arguments is in a shared pointer, also a level higher should use a shared pointer.

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

After your pull request (PR) is approved, merge your changes into the master.
That can be done using a merge commit or a squash commit.
The last one keeps the history cleaner, but can give difficulties when another issue is based on this branch.

Note that the release notes are generated using the title of the PR,
so check the title for typos and make sure it is understandable for others.

After the merge the branch must be deleted.

# Documentation
Algorithms are described in the scientific background.
This document uses the Deltares corporate identity, and may be hard to generate.
The body of the manual is plain latex, so that can be updated by externals.

Python code is documented with examples in a notebook.

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
