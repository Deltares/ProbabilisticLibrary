rem prerequisites
rem install https://nodejs.org/en 
rem conda install python=3.11
rem conda install poetry=1.3.2
rem conda install sphinx=8.2.3
rem conda install sphinx-autosummary-accessors=2023.4
rem conda install sphinx-design=0.6.1
rem conda install myst-parser
rem conda install nbsphinx
rem conda install sphinx_autodoc_typehints=3.5.2
rem npm install -g @mermaid-js/mermaid-cli

activate prob-lib-dev
pause

sphinx-build -b html ../../docs ../../docs/build/html

pause