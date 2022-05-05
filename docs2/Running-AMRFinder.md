See [[testing your installation|Installing-AMRFinder#testing-your-installation]] for some basic examples and expected output.

## Typical options

The only required arguments are either
`-p <protein_fasta>` for proteins or `-n <nucleotide_fasta>` for nucleotides.
We also provide an automatic update mechanism to update the code and database
by using `-u`. This will update to the latest AMR database, as well as any code
changes in AMRFinder. Use '--help' to see the complete set of options and
flags.

## Input file formats

`-p <protein_fasta>` and `-n <nucleotide_fasta>`

FASTA files are in standard format. The identifiers reported in the output are
the first non-whitespace characters on the defline. Example: [`test_prot.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.fa) or [`test_dna.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_dna.fa).

`-g <gff_file>`

GFF files are used to get sequence coordinates for AMRFinder hits from protein
sequence. The identifier from the identifier from the FASTA file is matched up
with the 'Name=' attribute from field 9 in the GFF file. See [`test_prot.gff`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.gff) for
a simple example. 


## Output format

AMRFinder output is in tab-delimited format (.tsv). The output format depends
on the options `-p`, `-n`, and `-g`. Protein searches with gff files (`-p
<file.fa> -g <file.gff>` and translated dna searches (`-n <file.fa>`) will also 
include contig, start, and stop columns. 

For example:

`amrfinder -p `[`test_prot.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.fa)` -g `[`test_prot.gff`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.gff) 

Should
result in the sample output shown below and in [`test_prot.expected`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.expected).


    Target identifier  Contig id Start Stop Strand Gene symbol Protein name                                   Method  Target length Reference protein length % Coverage of reference protein % Identity to reference protein Alignment length Accession of closest protein Name of closest protein                                             HMM id     HMM description
    blaOXA-436_partial contig1    4001 4699      + blaOXA      OXA-48 family class D beta-lactamase           PARTIAL           233                      265                           87.92                          100.00              233 WP_058842180.1               OXA-48 family carbapenem-hydrolyzing class D beta-lactamase OXA-436 NF000387.2 OXA-48 family class D beta-lactamase
    blaPDC-114_blast   contig1    2001 3191      + blaPDC      PDC family class C beta-lactamase              BLAST             397                      397                          100.00                           99.75              397 WP_061189306.1               class C beta-lactamase PDC-114                                      NF000422.2 PDC family class C beta-lactamase
    blaTEM-156         contig1       1  858      + blaTEM-156  class A beta-lactamase TEM-156                 ALLELE            286                      286                          100.00                          100.00              286 WP_061158039.1               class A beta-lactamase TEM-156                                      NF000531.2 TEM family class A beta-lactamase
    nimIJ_hmm          contig1    1001 1495      + nimIJ       NimIJ family nitroimidazole resistance protein HMM               165                       NA                              NA                              NA               NA NA                           NA                                                                  NF000262.1 NimIJ family nitroimidazole resistance protein
    vanG               contig1    5001 6047      + vanG        D-alanine--D-serine ligase VanG                EXACT             349                      349                          100.00                          100.00              349 WP_063856695.1               D-alanine--D-serine ligase VanG                                     NF000091.3 D-alanine--D-serine ligase VanG


Fields:

- Target Identifier - This is from the FASTA defline for the protein or DNA sequence
- Contig id - (optional) Contig name
- Start - (optional) 1-based coordinate of first nucleotide coding for protein in DNA sequence on contig
- Stop - (optional) 1-based corrdinate of last nucleotide coding for protein in DNA sequence on contig
- Gene symbol - Gene or gene-family symbol for protein hit
- Protein name - Full-text name for the protein
- Method - Type of hit found by AMRFinder one of five options
  - ALLELE - 100% sequence match over 100% of length to a protein named at the allele level in the AMRFinder database
  - EXACT - 100% sequence match over 100% of length to a protein in the database that is not a named allele
  - BLAST - BLAST alignment is > 90% of length and > 90% identity to a protein in the AMRFinder database
  - PARTIAL - BLAST alignment is > 50% of length, but < 90% of length and > 90% identity
  - HMM - HMM was hit above the cutoff, but there was not a BLAST hit that met standards for BLAST or PARTIAL
- Target length - The length of the query protein. The length of the blast hit for translated-DNA searches
- Reference protein length - The length of the AMR Protein in the database (NA if HMM-only hit)
- % Coverage of reference protein - % covered by blast hit (NA if HMM-only hit)
- % Identity to reference protein - % amino-acid identity to reference protein (NA if HMM-only hit)
- Alignment length - Length of BLAST alignment in amino-acids (NA if HMM-only hit)
- Accession of closest protein - RefSeq accession for protin hit by BLAST (NA if HMM-only hit)
- Name of closest protein - Full name assigned to the AMRFinder database protein (NA if HMM-only hit)
- HMM id - Accession for the HMM
- HMM description - The family name associated with the HMM

## Common errors and what they mean

### Protein id "\<protein id\>" is not in the .gff-file

To automatically combine overlapping results from protein and nucleotide searches the coordinates of the protein in the assembly contigs must be indicated by the GFF file. This requires a GFF file where the value of the 'Name=' variable of the 9th field in the GFF must match the identifier in the protein FASTA file (everything between the '>' and the first whitespace character on the defline).

## Genotype vs. Phenotype

Users of AMRFinder or its supporting data files are cautioned that presence of a gene encoding an antimicrobial resistance (AMR) protein does not necessarily indicate that the isolate carrying the gene is resistant to the corresponding antibiotic. AMR genes must be expressed to confer resistance. An enzyme that acts on a class of antibiotic, such as the cephalosporins, may confer resistance to some but not others. Many AMR proteins reduce antibiotic susceptibility somewhat, but not sufficiently to change the isolate from "sensitive" to "intermediate" or "resistant." Meanwhile, an isolate may gain resistance to an antibiotic by mutational processes, such as the loss of porin required to allow the antibiotic into the cell. For some families of AMR proteins, especially those borne on plasmids, correlations of genotype to phenotype are much more easily deciphered, but users are cautioned against over-interpretation.

## Known Issues

Handling of fusion genes is still under active development. Currently they are
reported as two lines, one for each portion of the fusion. Gene symbol, Protein
name, Name of closest protein, HMM id, and HMM description are with respect to
the individual elements of the fusion. This behavior is subject to change.

File format checking is rudimentary. Software behavior with incorrect input files is not defined, and error messages may be cryptic. Email us if you have questions or issues and we'll be happy to help.

If you find bugs not listed here or have other questions/comments please email
us at pd-help@ncbi.nlm.nih.gov.