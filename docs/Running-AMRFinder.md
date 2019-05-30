See [testing your installation](Installing-AMRFinder.md#testing-your-installation) for some basic examples and expected output.

## Usage:

`amrfinder (-p <protein_fasta> | -n <nucleotide_fasta) [options]`

### Typical options

The only required arguments are either
`-p <protein_fasta>` for proteins or `-n <nucleotide_fasta>` for nucleotides.
We also provide an automatic update mechanism to update the code and database
by using `-u`. This will update to the latest AMR database, as well as any code
changes in AMRFinder. Use '--help' to see the complete set of options and
flags.

## Options:

### Commonly used options:

`-p <protein_fasta>` or `--protein <protein_fasta>` Protein FASTA file to search.

`-n <nucleotide_fasta>` or `--nucleotide <nucleotide_fasta>` Nucleotide FASTA file to search.

`-g <gff_file>` or `--gff <gff_file>` GFF file to give sequence coordinates for proteins listed in `-p <protein_fasta>`. Required for combined searches of protein and nucleotide. The value of the 'Name=' variable in the 9th field in the GFF must match the identifier in the protein FASTA file (everything between the '>' and the first whitespace character on the defline). The first column of the GFF must be the nucleotide sequence identifier in the nucleotide_fasta if provided (everything between the '>' and the first whitespace character on the defline).

`-O <organism>` or `--organism <organism>` Taxon used for screening known resistance causing point mutations. Currently limited to one of *Campylobacter*, *Escherichia*, or *Salmonella*. Note that rRNA mutations will not be screened if only a protein file is provided. To screen known Shigella mutations use Escherichia as the organism.
 
`-u` or `--update` Download the latest version of the AMRFinder database to the default location (location of the AMRFinder binaries/data). Creates a directory under `data` in the format YYYY-MM-DD.<version> (e.g., `2019-03-06.1`).
    
## Less frequently used options:

`--threads <#>` The number of threads to use for processing. AMRFinder defaults to 4 on hosts with >= 4 cores. Setting this number higher than the number of cores on the running host may cause `blastp` to fail. Using more than 4 threads may speed up searches with nucleotide sequences, but will have little effect if only protein is used as the input.

`-o <output_file>` Print AMRFinder output to a file instead of standard out.

`-q` suppress the printing of status messages to standard error while AMRFinder is running. Error messages will still be printed.

`-d <database_dir>` or `--database <database_dir>` Use an alternate database directory. This can be useful if you want to run an analysis with a database version that is not the latest. This should point to the directory containing the full AMRFinder database files. It is possible to create your own custom databases, but it is not a trivial exercise. See [AMRFinder database](AMRFinder-database.md) for details on the format.

`-i <0-1>` or `--ident_min <0-1>` Minimum identity for a blast-based hit hit (Methods BLAST or PARTIAL). -1 means use a curated threshold if it exists and 0.9 otherwise. Setting this value to something other than -1 will override any curated similarity cutoffs.

`-c <0-1>` or `--coverage_min <0-1>` Minimum proportion of reference gene covered for a BLAST-based hit (Methods BLAST or PARTIAL). 

`--point_mut_all <point_mut_report>` Report genotypes at all locations screened for point mutations. This file allows you to distinguish between called point mutations that were the sensitive variant and the point mutations that could not be called because the sequence was not found.

`-t <1-33>` or `--translation_table <1-33>` Number from 1 to 33 to represent the translation table used for BLASTX. Default is 11. See [Translation table description](https://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi) for a description of the available tables.

`--blast_bin <directory>` Directory to search for 3rd party binaries (blast and hmmer) if not in the path.

`--parm <parameter string>` Pass additional parameters to amr_report. This is mostly used for internal purposes.

`--debug` Perform some additional integrity checks. May be useful for debugging, but not intended for public use.


## Input file formats

`-p <protein_fasta>` and `-n <nucleotide_fasta>`:
FASTA files are in standard format. The identifiers reported in the output are
the first non-whitespace characters on the defline.

`-g <gff_file>`
GFF files are used to get sequence coordinates for AMRFinder hits from protein
sequence. The identifier from the identifier from the FASTA file is matched up
with the 'Name=' attribute from field 9 in the GFF file. See test_prot.gff for
a simple example. (e.g., `amrfinder -p test_prot.fa -g test_prot.gff` should
result in the sample output shown below)

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
<file.fa> -g <file.gff>` and translated dna searches (`-n <file.fa>`) will  
include the `Contig id`, `start`, and `stop` columns. 

A sample AMRFinder report:
=======
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

...

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
