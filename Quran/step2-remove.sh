#!/bin/bash
# This step removes  shadda, sokon, and the three types of tanween.
sed -e 's/ْ//g' -e 's/ً//g' -e 's/ٌ//g' -e 's/ٍ//g' -e 's/ّ//g'
-e 's/!//g' -e 's/،//g' # we dont need esclames or arabic commas.
