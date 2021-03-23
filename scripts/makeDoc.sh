#!/bin/bash

# Generate a documentation based on the configuration file
cd ../doc
doxygen config

# Make a single pdf file
cd latex
pdflatex refman.tex
mv refman.pdf ../doc.pdf

# Show the resulted documentation
cd ../html
firefox annotated.html

