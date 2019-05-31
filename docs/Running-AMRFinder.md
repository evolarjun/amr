See [testing your installation](Installing-AMRFinder.md#testing-your-installation) for some basic examples and expected output.

# Usage:

`amrfinder (-p <protein_fasta> | -n <nucleotide_fasta) [options]`

### Typical options

The only required arguments are either `-p <protein_fasta>` for proteins or `-n <nucleotide_fasta>` for nucleotides. We also provide an automatic update
mechanism to update the code and database by using `-u`. This will update to
the latest AMR database, as well as any code changes in AMRFinder. Use '--help'
to see the complete set of options and flags.

## Options:

### Commonly used options:

`-p <protein_fasta>` or `--protein <protein_fasta>` Protein FASTA file to search.

`-n <nucleotide_fasta>` or `--nucleotide <nucleotide_fasta>` Nucleotide FASTA file to search.

`-g <gff_file>` or `--gff <gff_file>` GFF file to give sequence coordinates for
proteins listed in `-p <protein_fasta>`. Required for combined searches of
protein and nucleotide. The value of the 'Name=' variable in the 9th field in
the GFF must match the identifier in the protein FASTA file (everything between
the '>' and the first whitespace character on the defline). The first
column of the GFF must be the nucleotide sequence identifier in the
nucleotide_fasta if provided (everything between the '>' and the first
whitespace character on the defline).

`-O <organism>` or `--organism <organism>` Taxon used for screening known
resistance causing point mutations. Currently limited to one of
*Campylobacter*, *Escherichia*, or *Salmonella*. Note that rRNA mutations will
not be screened if only a protein file is provided. To screen known Shigella
mutations use Escherichia as the organism.
 
`-u` or `--update` Download the latest version of the AMRFinder database to the default location (location of the AMRFinder binaries/data). Creates a directory under `data` in the format YYYY-MM-DD.<version> (e.g., `2019-03-06.1`).
    
## Less frequently used options:

`--threads <#>` The number of threads to use for processing. AMRFinder defaults
to 4 on hosts with >= 4 cores. Setting this number higher than the number of
cores on the running host may cause `blastp` to fail. Using more than 4 threads
may speed up searches with nucleotide sequences, but will have little effect if
only protein is used as the input.

`-o <output_file>` Print AMRFinder output to a file instead of standard out.

`-q` suppress the printing of status messages to standard error while AMRFinder
is running. Error messages will still be printed.

`-d <database_dir>` or `--database <database_dir>` Use an alternate database
directory. This can be useful if you want to run an analysis with a database
version that is not the latest. This should point to the directory containing
the full AMRFinder database files. It is possible to create your own custom
databases, but it is not a trivial exercise. See [AMRFinder
database](AMRFinder-database.md) for details on the format.

`-i <0-1>` or `--ident_min <0-1>` Minimum identity for a blast-based hit hit
(Methods BLAST or PARTIAL). -1 means use a curated threshold if it exists and
0.9 otherwise. Setting this value to something other than -1 will override any
curated similarity cutoffs.

`-c <0-1>` or `--coverage_min <0-1>` Minimum proportion of reference gene covered for a BLAST-based hit (Methods BLAST or PARTIAL). 

`--point_mut_all <point_mut_report>` Report genotypes at all locations screened
for point mutations. This file allows you to distinguish between called point
mutations that were the sensitive variant and the point mutations that could
not be called because the sequence was not found.

`-t <1-33>` or `--translation_table <1-33>` Number from 1 to 33 to represent
the translation table used for BLASTX. Default is 11. See [Translation table
description](https://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi) for
a description of the available tables.

`--blast_bin <directory>` Directory to search for 3rd party binaries (blast and
hmmer) if not in the path.

`--parm <parameter string>` Pass additional parameters to amr_report. This is mostly used for internal purposes.

`--debug` Perform some additional integrity checks. May be useful for debugging, but not intended for public use.

## Examples

    # print a list of command-line options
    amrfinder --help
    
    # Download the latest AMRFinder plus database
    amrfinder -u
    
    # Protein AMRFinder with no genomic coordinates
    amrfinder -p test_prot.fa
    
    # Translated nucleotide AMRFinder (will not use HMMs)
    amrfinder -n test_dna.fa
    
    # Protein AMRFinder using GFF to get genomic coordinates
    amrfinder -p test_prot.fa -g test_prot.gff
    
    # Protein AMRFinder with Campylobacter protein point mutations
    amrfinder -p test_prot.fa -O Campylobacter
    
    # Full AMRFinder+ search combining results
    amrfinder -p test_prot.fa -g test_prot.gff -n test_dna.fa -O Campylobacter


## Input file formats

`-p <protein_fasta>` and `-n <nucleotide_fasta>`: FASTA files are in standard
format. The identifiers reported in the output are the first non-whitespace
characters on the defline.

`-g <gff_file>` GFF files are used to get sequence coordinates for AMRFinder
hits from protein sequence. The identifier from the identifier from the FASTA
file is matched up with the 'Name=' attribute from field 9 in the GFF file. See
test_prot.gff for a simple example. (e.g., `amrfinder -p test_prot.fa -g 
test_prot.gff` should result in the sample output shown below)

`-p <protein_fasta>` and `-n <nucleotide_fasta>`

FASTA files are in standard format. The identifiers reported in the output are
the first non-whitespace characters on the defline. Example:
[`test_prot.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.fa)
or
[`test_dna.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_dna.fa).

`-g <gff_file>`

GFF files are used to get sequence coordinates for AMRFinder hits from protein
sequence. The identifier from the identifier from the FASTA file is matched up
with the 'Name=' attribute from field 9 in the GFF file. See [`test_prot.gff`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.gff) for
a simple example. 

## Output format

AMRFinder output is in tab-delimited format (.tsv). The output format depends
on the options `-p`, `-n`, and `-g`. Protein searches with gff files (`-p <file.fa> -g <file.gff>` and translated dna searches (`-n <file.fa>`) will  
include the `Contig id`, `start`, and `stop` columns. 

### Sample AMRFinder report:

`amrfinder -p `[`test_prot.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.fa)` -g `[`test_prot.gff`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.gff)` -n `[`test_dna.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_dna.fa)` -O Campylobacter`

Should
result in the sample output shown below and in [`test_prot.expected`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.expected).

    Protein identifier Contig id Start Stop Strand Gene symbol   Sequence name                                       Element type Element subtype Class          Subclass       Method              Target length Reference sequence length % Coverage of reference sequence % Identity to reference sequence Alignment length Accession of closest sequence Name of closest sequence                                            HMM id     HMM description                                    
    NA                 Contig5     260 2021      - 23S_A2075G    Campylobacter macrolide resistant 23S               AMR          POINT           MACROLIDE      MACROLIDE      POINTN                       2021                      2912                            60.51                            99.83             1762 NC_022347.1:1040292-1037381   23S                                                                 NA         NA                                                 
    blaTEM-156         contig1     101  961      + blaTEM-156    class A beta-lactamase TEM-156                      AMR          AMR             BETA-LACTAM    BETA-LACTAM    ALLELEP                       286                       286                           100.00                           100.00              286 WP_061158039.1                class A beta-lactamase TEM-156                                      NF000531.2 TEM family class A beta-lactamase                  
    NA                 contig10    486 1307      + blaOXA        class D beta-lactamase                              AMR          AMR             BETA-LACTAM    BETA-LACTAM    INTERNAL_STOP                 274                       274                           100.00                            99.64              274 WP_000722315.1                oxacillin-hydrolyzing class D beta-lactamase OXA-9                  NF000270.1 class D beta-lactamase                             
    blaPDC-114_blast   contig2       1 1191      + blaPDC        PDC family class C beta-lactamase                   AMR          AMR             BETA-LACTAM    CEPHALOSPORIN  BLASTP                        397                       397                           100.00                            99.75              397 WP_061189306.1                class C beta-lactamase PDC-114                                      NF000422.6 PDC family class C beta-lactamase                  
    blaOXA-436_partial contig3     101  802      + blaOXA        OXA-48 family class D beta-lactamase                AMR          AMR             BETA-LACTAM    BETA-LACTAM    PARTIALP                      233                       265                            87.92                           100.00              233 WP_058842180.1                OXA-48 family carbapenem-hydrolyzing class D beta-lactamase OXA-436 NF000387.2 OXA-48 family class D beta-lactamase               
    vanG               contig4     101 1147      + vanG          D-alanine--D-serine ligase VanG                     AMR          AMR             GLYCOPEPTIDE   VANCOMYCIN     EXACTP                        349                       349                           100.00                           100.00              349 WP_063856695.1                D-alanine--D-serine ligase VanG                                     NF000091.3 D-alanine--D-serine ligase VanG                    
    NA                 contig6    2680 3102      + 50S_L22_A103V Campylobacter macrolide resistant 50S L22           AMR          POINT           MACROLIDE      MACROLIDE      POINTX                        141                       141                           100.00                            97.16              141 WP_002851214.1                50S L22                                                             NA         NA                                                 
    gyrA               contig6      31 2616      + gyrA_T86I     Campylobacter quinolone resistant GyrA              AMR          POINT           QUINOLONE      QUINOLONE      POINTP                        862                       863                            99.88                            99.54              862 WP_002857904.1                gyrA                                                                NA         NA                                                 
    50S_L22            contig7     100  525      + 50S_L22_A103V Campylobacter macrolide resistant 50S L22           AMR          POINT           MACROLIDE      MACROLIDE      POINTP                        141                       141                           100.00                            97.16              141 WP_002851214.1                50S L22                                                             NA         NA                                                 
    NA                 contig8     101  700      + blaTEM        TEM family class A beta-lactamase                   AMR          AMR             BETA-LACTAM    BETA-LACTAM    PARTIAL_CONTIG_ENDX           200                       286                            69.93                           100.00              200 WP_061158039.1                class A beta-lactamase TEM-156                                      NF000531.2 TEM family class A beta-lactamase                  
    NA                 contig9       1  675      - aph(3'')-Ib   aminoglycoside O-phosphotransferase APH(3'')-Ib     AMR          AMR             AMINOGLYCOSIDE STREPTOMYCIN   PARTIAL_CONTIG_ENDX           225                       275                            81.82                           100.00              225 WP_109545041.1                aminoglycoside O-phosphotransferase APH(3'')-Ib                     NF032895.1 aminoglycoside O-phosphotransferase APH(3'')-Ib    
    NA                 contig9     715 1377      - sul2          sulfonamide-resistant dihydropteroate synthase Sul2 AMR          AMR             SULFONAMIDE    SULFONAMIDE    PARTIAL_CONTIG_ENDX           221                       271                            81.55                           100.00              221 WP_001043265.1                sulfonamide-resistant dihydropteroate synthase Sul2                 NF000295.1 sulfonamide-resistant dihydropteroate synthase Sul2
    nimIJ_hmm          contigX       1  501      + nimIJ         NimIJ family nitroimidazole resistance protein      AMR          AMR             NITROIMIDAZOLE NITROIMIDAZOLE HMM                           166                        NA                               NA                               NA               NA NA                            NA                                                                  NF000262.1 NimIJ family nitroimidazole resistance protein     

Fields:

- Protein Identifier - This is from the FASTA defline for the protein or DNA sequence.
- Contig id - (optional) Contig name.
- Start - (optional) 1-based coordinate of first nucleotide coding for protein in DNA sequence on contig.
- Stop - (optional) 1-based corrdinate of last nucleotide coding for protein in DNA sequence on contig.
- Gene symbol - Gene or gene-family symbol for protein or nucleotide hit. For point mutations it is a combination of the gene symbol and the SNP definition separated by "\_"
- Sequence name - Full-text name for the protein or RNA.
- Element type - AMRFinder+ genes are placed into functional categories based on predominant function AMR, STRESS, or VIRULENCE.
- Element subtype - Further elaboration of functional category into (ANTIGEN, BIOCIDE, HEAT, METAL, PORIN) if more specific category is available, otherwise he element is repeated.
- Class - For AMR genes this is the class of drugs that this gene is known to contribute to resistance of. 
- Subclass - If more specificity about drugs within the drug class is known it is elaborated here. 
- Method - Type of hit found by AMRFinder. A suffix of 'P' or 'X' is appended to "Methods" that could be found by protein or nucleotide.
  - ALLELE - 100% sequence match over 100% of length to a protein named at the allele level in the AMRFinder database.
  - EXACT - 100% sequence match over 100% of length to a protein in the database that is not a named allele.
  - BLAST - BLAST alignment is > 90% of length and > 90% identity to a protein in the AMRFinder database.
  - PARTIAL - BLAST alignment is > 50% of length, but < 90% of length and > 90% identity to the reference, and does not end at a contig boundary.
  - PARTIAL_CONTIG_END - BLAST alignment is > 50% of length, but < 90% of length and > 90% identity to the reference, and the break occurrs at a contig boundary indicating that this gene is more likely to have been split by an assembly issue.
  - HMM - HMM was hit above the cutoff, but there was not a BLAST hit that met standards for BLAST or PARTIAL. This does not have a suffix because only protein sequences are searched by HMM. 
  - INTERNAL_STOP - Translated blast reveals a stop codon that occurred before the end of the protein. This can only be assessed if the `-n <nucleotide_fasta>` option is used.
  - POINT - Point mutation identified by blast.  
- Target length - The length of the query protein or gene. The length will be in amino-acids if the reference sequence is a protein, but nucleotide if the reference sequence is nucleotide.
- Reference sequence length - The length of the Reference protein or nucleotide in the database (NA if HMM-only hit).
- % Coverage of reference sequence - % of reference covered by blast hit (NA if HMM-only hit).
- % Identity to reference sequence - % amino-acid identity to reference protein or nucleotide identity for nucleotide reference. (NA if HMM-only hit).
- Alignment length - Length of BLAST alignment in amino-acids or nucleotides if nucleotide reference (NA if HMM-only hit).
- Accession of closest protein - RefSeq accession for reference hit by BLAST. Note that only one reference will be chosen if the blast hit is equidistant from multiple references (NA if HMM-only hit).
- Name of closest protein - Full name assigned to the closest reference hit (NA if HMM-only hit).
- HMM id - Accession for the HMM, NA if none.
- HMM description - The family name associated with the HMM, NA if none.

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
