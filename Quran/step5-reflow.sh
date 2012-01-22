#!/bin/bash

# reflows text, but ensure that start of sura's stars a new paragraph.
# needs to be done after latin step, because fmt doesnt understand unicode.
# TODO: use par wich has unicode but doesnt seem to get it right either.
sed -e 's/\([0-9]*\).1 /\n\n\1.1 /' |
fmt -w 30

