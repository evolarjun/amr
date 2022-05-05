## Note regarding Genotype vs. Phenotype

Users of AMRFinderPlus or its supporting data files are cautioned that presence
of a gene encoding an antimicrobial resistance (AMR) protein or resistance causing mutation does
not necessarily indicate that the isolate carrying the gene is resistant to the
corresponding antibiotic. AMR genes must be expressed to confer resistance.
Many AMR proteins reduce antibiotic susceptibility somewhat, but not
sufficiently to cross clinical breakpoints. Meanwhile, an isolate may gain or
lose resistance to an antibiotic by mutational processes, such as the loss
of a porin required to allow the antibiotic into the cell. For some families of
AMR proteins, especially those borne by plasmids, correlations of genotype to
phenotype are much more easily deciphered, but users are cautioned against
over-interpretation. 

## Summary

See the [[Running AMRFinderPlus page|Running-AMRFinderPlus#output-format]] for
details on what the output fields are.

There are several columns that can help inform your understanding of function.
See [[An example report|Running-AMRFinderPlus#sample-amrfinder-report]] for an
example of the possible values for the different columns described below. To
briefly summarize, the "Method" column combined with the % identity  indicates
how a hit was identified, and whether the protein is a partial, an exact match
to a known sequence, etc. This should give an indication of how confident you
should be that this is a full-length functional gene. The "core" vs. "plus"
column should help you to determine the level of curation that went into the
inclusion of that gene in the database. The Sequence name, Type, Subtype,
Class, and Subclass provides an estimation of the category and
function for the gene.

## The "Scope" column and the `--plus` option

AMRFinderPlus splits the database into two subsets. By default it only returns genes in the 'core' subset.

-    "Core": this subset includes highly curated, AMR-specific genes and
     proteins from the Bacterial Antimicrobial Resistance Reference Gene
     Database (BioProject PRJNA313047), plus point mutations. The sources of
     input for this curated database include: 1) allele assignments, 2)
     exchanges with other external curated resources, 3) reports of novel
     antimicrobial resistance proteins in the literature.
-    "Plus": this subset includes genes related to biocide and stress
     resistance, general efflux, virulence, or antigenicity, or AMR genes whose
     presence/absence are unlikely to affect phenotype and/or is highly
     uncertain. These genes are only shown if the --plus option is used.

## The "Method" column

Important information about how likely the hit is to be functional and/or how
novel a hit is can be found in the "method" column. A description of possible
values is listed on the [[Running AMRFinderPlus page|Running-AMRFinderPlus#sample-amrfinder-report]]. 
Note that for BLAST-based "Methods" there is a suffix of either 'X' or 'P'
that indicates whether it was found using translated genomic sequence or
protein sequence. 

### X, P, or N suffix

The methods that can apply to either nucleotide or protein sequence have
a suffix appended to designate which type of search was used for that line in
the report. The suffix indicates whether the line was identified in a protein
(_P_) or nucleotide translated (_X_) input file. Nucleotide BLAST (BLASTN) is used to identify nucleotide point mutations and POINTN is used as the Method for those hits.  

Note as of version 3.10.11 BLASTX / POINTX / EXACTX results may actually come from `tblastn`. Tblastn is used instead of blastx to increase speed in some cases.

### EXACTX, EXACTP, ALLELEX, and ALLELEP

These mean that we have an amino-acid sequence identical protein in the
AMRFinderPlus database. The ALLELE method is reserved for genes that have an
allele numbering scheme, so the "Gene symbol" for an ALLELE hit is really an
allele symbol specific for that exact sequence. EXACT matches have an identical
amino-acid sequence in the database, but the family does not have alleles so
the same gene symbol may be shared with other closely related sequences. Note
that if you run a combined search (including `-p`, `-n`, and `-g` options) and
you get an ALLELEX or EXACTX result that means that the annotation did not
contain the protein. 

### BLASTX and BLASTP

"BLAST" hits are hits that are < 100% identical to a database protein, but at
coverage > 90%. The percent identity cutoff is, by default, 90%, but may be
higher or lower if it was manually curated. Manual curation of BLAST parameters
usually occurrs for 'plus' genes where no HMM and cutoff have been curated. 

### PARTIALX, PARTIALP, PARTIAL_CONTIG_ENDX, and PARTIAL_CONTIG_ENDP

Hits < 90% of the length of the database protein are called either "PARTIAL" if
the hit is internal to a contig or "PARTIAL_CONTIG_END" if the gene could have
been broken by a contig boundary. "PARTIAL_CONTIG_END" require that the protein
alignment ends within two nucleotides of the end of the contig with an
orientation such that the end of the protein would extend off the end of the
contig. Because assemblers sometimes split genes over multiple contigs, genes
that are PARTIAL_CONTIG_END are often full-length in reality. Note that
searches using only protein FASTA files (`-p <protein_fasta`) do use
contig location and so all < 90% coverage hits will just be called PARTIAL. 

Note that by default AMRFinderPlus does not identify gene fragments that cover 
< 50% of the reference, so alignments from genes marked PARTIAL cover between 50% 
and 90% of the reference.  The default minimum coverage can be changed by using 
the `--coverage_min` option (see [[Usage (syntax/options)|Running AMRFinderPlus#usage]] 
for more information).

### INTERNAL_STOP

These are genes that when translated from genomic sequence have a stop codon
before the end of the database protein. These are less likely to be functional,
and can only be assessed if the `-n <nucleotide_fasta>` option is used.

### HMM

HMM-only hits happen when the criteria for a BLAST hit is not met and an HMM
match is above the curated cutoff for an HMM that has been created for that
gene or gene family. These will usually be distant relatives of known gene
families and may be candidates for a new gene family. Occasionally, partial
proteins that have diverged enough from known database proteins to not
meet the BLAST cutoffs will show up as HMM-only hits.

### POINTX, POINTP, and POINTN

Point mutation detection is only enabled when an `--organism` option is included for a particular taxonomic group. AMRFinderPlus does not report all point mutations in those genes, only ones that have been found in papers describing their functional relevance. Thus the absence of a POINTP, POINTX, or POINTN result does not mean that there are no functional point mutations, only that AMRFinderPlus was unable to find a gene with known point mutations. This could be either because the gene does not exist in the assembly, or the functional point mutation was not one included in the database. See [The Pathogen Detection Reference Gene Browser](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/subtype:POINT) for a list of known point mutations. The `--mutation_all` option will create a file with all genotype calls made by AMRFinderPlus. With that output you can assess whether or not a specific gene was identified in the query sequence. 

## Element Type and Subtype

These fields describe the classification of the AMRFinderPlus gene. "Element type" is split into 3 categories, AMR, STRESS, or VIRULENCE. "Element subtype" is a duplicate of "Element type" unless a more specific category has been defined. 

| Element type  | Element subtype   | Description | 
| ------------  | ---------------   | ----------- |
| AMR           | AMR               | Antimicrobial resistance gene |
| AMR           | AMR-SUSCEPTIBLE   | Not used in AMRFinderPlus output, but in the Reference Gene Catalog (see [note](#a-note-about-subtype-amr-susceptible-and-streptococcus-pneumoniae) below) |
| AMR           | POINT             | Known point mutation associated with antimicrobial resistance |
| VIRULENCE     | VIRULENCE         | Virulence gene |
| VIRULENCE     | ANTIGEN           | Gene codes for a known antigen (these are often used for typing) |
| STRESS        | ACID              | Acid resistance gene |
| STRESS        | BIOCIDE           | Biocide resistance gene |
| STRESS        | HEAT              | Heat resistance gene |
| STRESS        | METAL             | Metal resistance gene | 

#### A note about subtype AMR-SUSCEPTIBLE and _Streptococcus pneumoniae_

This is used for Pencillin-resistant Streptococcus pneumoniae pbp alleles,
which are defined as those alleles which are below a protein identity threshold
to a set of pbp1a, pbp2b, or pbp2x reference alleles: the thresholds are 99%,
99%, and 98% for pbp1a, pbp2b, and pbp2x respectively. The susceptible
reference alleles can be found in NCBI's [Reference Gene
Catalog](https://www.ncbi.nlm.nih.gov/pathogens/refgene/) where they have
subtype AMR-SUSCEPTIBLE to indicate that the reference is the susceptible
version. These alleles may also confer resistance to other beta-lactam
antibiotics. If you are using AMRFinderPlus with Streptococcus pneumoniae pbp
genes you may need to analyze further as necessary. Subtype AMR-SUSCEPTIBLE
will not appear in AMRFinderPlus output because only putatively resistant forms
will be identified.

## Class and Subclass

These are classifications of the effect of the gene on resistance to various
stresses. Be aware that genotype and phenotype may not correspond (see also [note regarding genotype and phenotype](https://github.com/ncbi/amr/wiki/Interpreting-results#note-regarding-genotype-vs-phenotype)). The class and subclass fields are also used to add typing information in the cases
of stx and intimins. These fields are blank for many "plus" genes.

For a list of all possible combinations of class and subclass see
[[class-subclass]].  For AMR genes "Class" can be thought of as drug class, and
'Subclass' contains a more specific drug designations where known. While most subclass designations are self-explanatory, a few others have particular meanings. Specifically, "CEPHALOSPORIN" is equivalent to the Lahey 2be definition; "CARBAPENEM" means the protein has carbapenemase activity, but it might or might not confer resistance to other beta-lactams; "QUARTERNARY AMMONIUM" are quarternary ammonium compounds. Where the phenotypic information is incomplete, contradictory, or unclear, the "Class" value is used for the "Subclass" value.

For some
antigen and virulence genes that are often referred to by type, specific type
information is included here as follows:

For eae (Intimin) genes the "Class" is INTIMIN, while the subclass contains the
intimin type (ALPHA, BETA, EPSILON, GAMMA, IOTA, LAMBDA, or RHO).

For stx (Siga toxin) genes the possibilities for "Class" and "Subclass" provide
typing information as follows:

| Class | Subclass |
| ----- | -------- |
| STX1  |  STX1A   |
| STX1  |  STX1B   |
| STX1  |  STX1C   |
| STX1  |  STX1D   |
| STX2  |  STX2A   |
| STX2  |  STX2B   |
| STX2  |  STX2C   |
| STX2  |  STX2D   |
| STX2  |  STX2E   |
| STX2  |  STX2F   |
| STX2  |  STX2G   |

## Known Issues

### Fusion genes

Handling of fusion genes is still under active development. Currently they are
reported as two lines, one for each portion of the fusion. Gene symbol, Protein
name, Name of closest protein, HMM id, and HMM description are reported with respect to
the individual elements of the fusion. This behavior is subject to change.

### Frame shifted proteins

AMRFinderPlus has no explicit detection of frame shifts. Frame shift mutations in nucleotide inputs will usually result in genes detected with the "method" INTERNAL_STOP or PARTIAL, but that will depend on the nature of the frame shift and the sequence following it.

### Input file formats

File format checking is rudimentary. Software behavior with incorrect input files is not defined, and error messages may be cryptic. Email us if you have questions or issues and we'll be happy to help and use your input to improve our error checking and messages.

If you find bugs or have other questions/comments please email
us at pd-help@ncbi.nlm.nih.gov.
