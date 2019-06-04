---
# Methods
---

## AMRFinder database

The AMRFinder database is generated at NCBI and curation is ongoing. Recent versions are available at https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/AMRFinder/data. For more information on the format and methods used to compile the database see the [AMRFinder database page](AMRFinder-database.md).

## Protein searches (-p)

AMRFinder-prot uses the database of AMR gene sequences, protein profile hidden Markov models
(HMMs), the hierarchical tree of AMR protein families, and a custom
rule-set to generate names and coordinates for AMR genes, along with
descriptions of the evidence used to identify the sequence. Genes are reported
with the following procedure after both HMMER and BLASTP searches are run.

### BLASTP matches

BLASTP is run with the `-evalue 1e-20 -comp_based_stats 0` 
options against the AMR gene database described above.  Exact BLAST matches
over the full length of the reference protein are reported. If there is no
exact match, then the following rules are applied unless overridden by
a curated blast cutoff: Matches with < 90% identity or with < 50% coverage of
the protein are dropped. If the hit is to a fusion protein than at least 90% of
the protein must be covered. Curated blast cutoffs override these defaults.
A BLAST match to a reference protein is removed if it is covered by another
BLAST match which has more identical residues or the same number of identical
residues, but to a longer reference protein. A single match is chosen as the
best of what remains sorting by the following criteria in order (1) if it is
exact; (2) has more identical residues; (3) hits a shorter protein; or (4) the
gene symbol comes first in alphabetical order.

### HMM matches

HMMER version 3.1b2 (http://hmmer.org/) is run using the `--cut_tc -Z 10000`
options with the HMM database described above. HMM matches with full_score
< TC1 or domain_score < TC2 are dropped. All HMM matches to HMMs for parent
nodes of other HMM matches in the hierarchy are removed. The match(es) with the
highest full score are kept. If a BLAST match is not exact it must agree with an HMM match.

## Translated DNA searches (-n)

Translated alignments using BLASTX of the assembly against the AMR protein
database can be used to help identify partial, split, or unannotated AMR
proteins using the `-task tblastn -word_size 3 -evalue 1e-20 -seg no -comp_based_stats 0 now is:
-word_size 3 -evalue 1e-20 -query_gencode GENCODE -seg no -comp_based_stats 0 -max_target_seqs 10000` 
options. The algorithm for selecting hits is as described above for proteins,
but note that HMM searches are not performed against the unannotated
assembly.

## Point mutation identification

Point mutations are identified by blastn alignments that must cover at least 50%
of the reference (or `--coverage_min` if set) at 90% identity (or `--ident_min`
if set). Offsets are calculated relative to the beginning of the reference and
reported in that coordinate system. That is if there are indels within the
query sequence the coordinates of the point mutation will reflect the offset
from the start codon in the reference rather than in the query sequence.

## Combining translated nucleotide and protein hits

When provided with nucleotide, protein, and coordinate information AMRFinder will attempt to remove hits that overlap by > 75%

Protein hits generally take priority over nucleotide hits, except if the protein hit is of lower "quality" than the nucleotide hit where quality is measured by the hit "Method". 

    ALLELE > EXACT > BLAST > INTERNAL_STOP > PARTIAL_CONTIG_END > PARTIAL > HMM


