import setuptools
 
with open("README.md", "r") as fh:
    long_description = fh.read()
 
setuptools.setup(
    name="ptk",
    version="0.0.1",
    author="Deltares",
    author_email="software.support@deltares.nl",
    description="Package to perform probabilistic analyses",
    long_description=long_description,
    long_description_content_type="text/markdown",
    packages=setuptools.find_packages(),
    package_data={'ptk':['bin/libDeltares.Probabilistic.CWrapper.so']},    
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: AGPL License",
        "Operating System :: OS Independent",
    ],
    python_requires='>=3.7',
)

