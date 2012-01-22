#!/bin/bash

# Transcodes arabic symbols to ascii, so that we can send to braille printer.


sed -e 's/ب/b/g' -e 's/َ/,/g' -e 's/ِ/e/g' -e 's/س/s/g' -e 's/م/m/g' \
-e 's/ا/a/g' -e 's/ل/l/g' -e 's/ه/h/g' \
-e 's/ر/r/g' -e 's/ح/</g' -e 's/ن/n/g' -e 's/ي/i/g' \
-e 's/د/d/g' -e 's/ُ/u/g' -e 's/ع/(/g' \
-e 's/ك/k/g' -e 's/و/w/g' -e 's/إ/`/g' \
-e 's/ص/[/g' -e 's/ط/)/g' -e 's/ت/t/g' -e 's/ق/q/g' \
-e 's/ذ/{/g' -e 's+أ+/+g' -e 's/غ/>/g' \
-e 's/ض/D/g' -e 's/ذ/T/g' \
-e 's/ؤ/?/g' -e 's/ة/\\/g' -e 's/ز/z/g' \
-e 's/آ/|/g' -e 's/خ/x/g' \
-e 's/ئ/y/g' -e 's/ف/f/g' \
-e 's/ء/./g' -e 's/ظ/=/g' -e 's/ش/%/g' \
-e 's/ث/#/g' -e 's/ج/j/g' -e 's/ى/o/g'
