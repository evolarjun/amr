#!/bin/sh

cp class-subclass.md class-subclass.md.bak
curl -O https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/AMRFinderPlus/database/latest/fam.tab

echo "| Class | Subclass |" > class-subclass.md
echo "| ----- | -------- |" >> class-subclass.md
cut -f 16,17 fam.tab | sort -u | egrep -v '^\s*$|^class' | perl -pe 's/^/| /; s/\t/ | /; s/$/ |/; ' >> class-subclass.md

# clean up after myself
rm fam.tab
