#!/bin/bash
if [ ! -f "./logo.svg" ]; then
    echo "logo.svg file not found"
else
    convert -density 384 -background transparent logo.svg -define icon:auto-resize -colors 256 logo.ico
fi