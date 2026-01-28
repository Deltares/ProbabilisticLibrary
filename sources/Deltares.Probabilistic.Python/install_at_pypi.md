### How to get the probabilistic library Python wrapper in PyPI ?

-   Use an account at PyPI that has maintaince access to the project probabilistic library.
Create an API token for your account.
-	Make sure the correct version number is in setup.py and __init__ files.
-   Each upload has an unique version number. Use rc (release candidate) for pre-releases.
-	Get the wheel and tar.gz from TeamCity config `Python create wheel`.
-	Put these two files in a `dist` folder.
-   Install `twine` if you didn't do this before:
-	`pip install --upgrade twine`
-   the next and final step will ask for your API token:
-	`twine upload dist/*`
