See [[Test your installation]] for some basic examples of expected input and expected output.

# Usage:

`amrfinder (-p <protein_fasta> | -n <nucleotide_fasta) [options]`

The only required arguments are either `-p <protein_fasta>` for proteins or `-n
<nucleotide_fasta>` for nucleotides. We also provide an automatic update
mechanism to update the code and database by using `-u`. This will update to
the latest AMR database, as well as any code changes in AMRFinderPlus. Use
'`--help`' to see the complete set of options and flags.

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
nucleotide\_fasta if provided (everything between the '>' and the first
whitespace character on the defline). To interpret the output of
[external PGAP](https://github.com/ncbi/pgap) annotations use the `--pgap`
option.

`-O <organism>` or `--organism <organism>` Taxon used for screening known
resistance causing point mutations and blacklisting of common, non-informative
genes. `amrfinder -l` will list the possible values for this option. Note that
rRNA mutations will not be screened if only a protein file is provided. To
screen known Shigella mutations use Escherichia as the organism. See [Organism
option](#--organism-option) below for more details.
 
`-u` or `--update` Download the latest version of the AMRFinderPlus database to
the default location (location of the AMRFinderPlus binaries/data). Creates a
directory under `data` in the format `YYYY-MM-DD.<version>` (e.g.,
`2019-03-06.1`). Will not overwrite an existing directory. Use `--force_update`
to overwrite the existing directory with a new download.

`-U` or `--force_update` Download the latest version of the AMRFinderPlus
database to the default location. Creates a directory under `data` in the
format `YYYY-MM-DD.<version>` (e.g., `2019-03-06.1`), and overwrites an
existing directory.

`-l` or `--list_organisms` Print the list of all possible taxonomy groups used
with the `-O/--organism` option.

<a name="option--plus">
`--plus` Provide results from "Plus" genes such as virulence factors,
stress-response genes, etc. See [[AMRFinderPlus
database|AMRFinderPlus-database#types-of-proteins-covered]] for details.

## Less frequently used options:

`--blast_bin <directory>` Directory to search for 3rd party binaries (blast and
hmmer) if not in the path.

`-d <database_dir>` or `--database <database_dir>` Use an alternate database
directory. This can be useful if you want to run an analysis with a database
version that is not the latest. This should point to the directory containing
the full AMRFinderPlus database files. It is possible to create your own custom
databases, but it is not a trivial exercise. See [[AMRFinderPlus database]] for
details on the format.

`--threads <#>` The number of threads to use for processing. AMRFinderPlus defaults
to 4 on hosts with >= 4 cores. Setting this number higher than the number of
cores on the running host may cause `blastp` to fail. Using more than 4 threads
may speed up searches with nucleotide sequences, but will have little effect if
only protein is used as the input.

`--mutation_all <point_mut_report>` Report genotypes at all locations screened
for point mutations. This file allows you to distinguish between called point
mutations that were the sensitive variant and the point mutations that could
not be called because the sequence was not found. This file will contain all
detected variants from the reference sequence, so it could be used as an
initial screen for novel variants. Note "Gene symbols" for mutations not in the
database (identifiable by [UNKNOWN] in the Sequence name field) have offsets
that are relative to the start of the sequence indicated in the field
"Accession of closest sequence" while "Gene symbols" from known point-mutation
sites have gene symbols that match the
[Pathogen Detection Reference Gene Catalog](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/) 
standardized nomenclature for point mutations.

`--name <identifier>` Prepend a column containing an identifier for this run of 
AMRFinderPlus. For example this can be used to add a sample name column to the 
AMRFinderPlus results.

`--nucleotide_output <nucleotide.fasta>` Print a FASTA file of just the regions
of the `--nucleotide <input.fa>` that were identified by AMRFinderPlus. This
will include the entire region that aligns to the references for point
mutations. 

`--nucleotide_flank5_output <nucleotide.fasta>` FASTA file of the regions of
the `--nucleotide <input.fa>` that were identified by AMRFinderPlus plus
`--nucleotide_flank5_size` additional nucleotides in the 5' direction, this
will include the entire region that aligns to the references for point
mutations plus the additional flank. 

`--nucleotide_flank5_size <num_bases>` The number of additional bases in the 5'
direction to add to the element sequence in the `--nucleotide_flank5_output`
direction.

`-o <output_file>` Print AMRFinderPlus output to a file instead of standard out.

`--protein_output <protein.fasta>` Print FASTA file of Proteins identified by
AMRFinderPlus in the `--protein <input.fa>` file. Only selects from the
proteins provided on input, AMRFinderPlus will not do a translation of hits
identified from nucleotide sequence. 

`-q` suppress the printing of status messages to standard error while AMRFinderPlus
is running. Error messages will still be printed.

`--report_all_equal` Report all equally scoring BLAST and HMM matches. This
will report multiple lines for a single element if there are multiple reference
proteins that have the same score. On those lines the fields _Accession of
closest sequence_ and _Name of closest sequence_ will be different showing each
of the database proteins that are equally close to the query sequence.

`-i <0-1>` or `--ident_min <0-1>` Minimum identity for a blast-based hit hit
(Methods BLAST or PARTIAL). -1 means use a curated threshold if it exists and
0.9 otherwise. Setting this value to something other than -1 will override any
curated similarity cutoffs.

`-c <0-1>` or `--coverage_min <0-1>` Minimum proportion of reference gene
covered for a BLAST-based hit (Methods BLAST or PARTIAL). 

`-t <1-33>` or `--translation_table <1-33>` Number from 1 to 33 to represent
the translation table used for BLASTX. Default is 11. See [Translation table
description](https://www.ncbi.nlm.nih.gov/Taxonomy/Utils/wprintgc.cgi) for
a description of the available tables.

`--pgap` Alters the GFF and FASTA file handling to correctly interpret the
output of the pgap pipeline. Note that you should use the `annot.fna` file in
the pgap output directory as the argument to the `-n/--nucleotide` option.

`--gpipe_org` Use Pathogen Detection taxgroup names as arguments to the --organism option

`--parm <parameter string>` Pass additional parameters to amr_report. This is
mostly used for development and debugging.

`--debug` Perform some additional integrity checks. May be useful for
debugging, but not intended for public use.

## `--organism` option

The `-O/--organism` option is used to get organism-specific results. For those
organisms which have been curated, using `--organism` will get optimized
organism-specific results. AMRFinderPlus uses the `--organism` for two
purposes:

1. To screen for point mutations
2. To filter out genes that are nearly universal in a group and uninformative
3. To identify divergent Salmonella pbp proteins that are usually Penicillin resistant

We currently curate a limited set of organisms for point mutations and/or
blacklisting of some [[plus genes|AMRFinderPlus-database#plus-proteins]] that
are not likely to be informative in those species. Use `amrfinder -l` to list
the organism options that can be used in the current database. Use the
[Reference Gene
Catalog](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/) to identify
the [point
mutations](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/whitelisted_taxa:*)
and [blacklisted
genes](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/blacklisted_taxa:*)
that are affected by this option.



| Organism option                 | Point mutations   | Blacklisted `--plus` genes | Notes         |
| :-------------------------------| :---------------: | :------------------------: | :------------ |
| Acinetobacter_baumannii         | X                 |                            | Use for the _A. baumannii-calcoaceticus_ species complex              |
| Campylobacter                   | X                 |                            | Use for _C. coli_ and _C. jejuni_ | 
| Enterococcus_faecalis           | X                 |                            |               |
| Enterococcus_faecium            | X                 |                            | Use for _E. hirae_              |
| Escherichia                     | X                 | X                          | Use for _Shigella_ and _Escherichia_ |
| Klebsiella                      | X                 | X                          | Use for _K. pneumoniae_ species complex and _K. aerogenes_ and not _K. oxytoca_              |
| Neisseria                       | X                 |                            | Point mutations have been vetted only for _N. gonorrhea_ and _N. meningitidis_ |
| Pseudomonas_aeruginosa          | X                 |                            |               |
| Salmonella                      | X                 | X                          |               |
| Staphylococcus_aureus           | X                 |                            |               |
| Staphylococcus_pseudintermedius | X                 | X                          |               |
| Streptococcus_agalactiae        | X                 |                            |               |
| Streptococcus_pneumoniae        | X                 |                            | Use for _S. pneumoniae_ and _S. mitis_              |
| Streptococcus_pyogenes          | X                 |                            |               |
| Vibrio_cholerae                 |                   | X                          |               |

Note that variant detection for Streptococcus\_pneumoniae uses a new mechanism
identifying divergent alleles. See [[Interpreting Results|Interpreting results#a-note-about-subtype-amr-susceptible-and-streptococcus-pneumoniae]] for more
information.

# Temporary files

AMRFinderPlus creates a fair number of temporary files in `/tmp` by default. If the environment variable `TMPDIR` is set AMRFinderPlus will instead put the temporary files in the directory pointed to by `$TMPDIR`.

# Examples

These examples use the test files [test_prot.gff](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.gff), [test_prot.fa](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.fa), and [test_dna.fa](https://raw.githubusercontent.com/ncbi/amr/master/test_dna.fa) if you want to try them for yourself.

    # print a list of command-line options
    amrfinder --help
    
    # Download the latest AMRFinder plus database
    amrfinder -u
    
    # Protein AMRFinder with no genomic coordinates
    amrfinder -p test_prot.fa
    
    # Translated nucleotide AMRFinder (will not use HMMs)
    amrfinder -n test_dna.fa
    
    # Protein AMRFinder using GFF to get genomic coordinates and 'plus' genes
    amrfinder -p test_prot.fa -g test_prot.gff --plus
    
    # Protein AMRFinder with Escherichia protein point mutations
    amrfinder -p test_prot.fa -O Escherichia
    
    # Full AMRFinderPlus search combining results
    amrfinder -p test_prot.fa -g test_prot.gff -n test_dna.fa -O Escherichia --plus


# Input file formats

#### `-g <gff_file>` 

GFF files are used to get sequence coordinates for AMRFinder hits from protein
sequence. __The value of the 'Name=' variable in the 9th field in the GFF must
match the identifier in the protein FASTA file__ (everything between the '>'
and the first whitespace character on the defline). The first column of the GFF
must be the nucleotide sequence identifier in the nucleotide_fasta if provided
(everything between the '>' and the first whitespace character on the defline).
See
[test_prot.gff](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.gff),
[test_prot.fa](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.fa),
and
[test_dna.fa](https://raw.githubusercontent.com/ncbi/amr/master/test_dna.fa)
for a simple example. See [[Test your installation]] for how to run the
examples. See [Using PROKKA or RAST GFF files with AMRFinderPlus](https://github.com/ncbi/amr/wiki/Tips-and-tricks#using-prokka-or-RAST-gff-files-with-amrfinderplus) on the [[Tips and Tricks]] page to see commands to convert those GFF files for AMRFinderPlus use.

Simple example below (These were taken from [test_prot.gff](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.gff). See those files to see how the identifiers line up):

```
contig09	.	gene	1	675	.	-	.	Name=aph3pp-Ib_partial_5p_neg
contig09	.	gene	715	1377	.	-	.	Name=sul2_partial_3p_neg
contig11	.	gene	113	547	.	+	.	Name=blaTEM-internal_stop
```
Matching deflines from assembly:
```
>contig09 >case4_case6_sul2_aph3pp-Ib Providencia rettgeri strain Pret_2032, whole genome shotgun sequence  2160922-2162737  150-1527 (reverse comp'd)
>contig11 blaTEM divergent, with internal stop codon
```
Matching protien deflines:
```
>aph3pp-Ib_partial_5p_neg  NZ_QKNQ01000001.1 Providencia rettgeri strain Pret_2032, whole genome shotgun sequence  2160922-2162737  150-1527  704-137
>sul2_partial_3p_neg   NZ_QKNQ01000001.1 Providencia rettgeri strain Pret_2032, whole genome shotgun sequence  2160922-2162737  150-1377  2-667
>blaTEM-internal_stop
```


Many annotation pipelines will produce annotation files that AMRFinderPlus will have trouble reading, but it it usually a simple matter to convert them to an appropriate format. If you are having trouble email us at pd-help@ncbi.nlm.nih.gov (or open a [github issue](https://github.com/ncbi/amr/issues))  with examples of the FASTA and GFF files you are trying to use and we should be able to help. For Prokka or RAST generated GFFs see [Using Prokka or RAST GFF files with AMRFinderPlus](Tips-and-tricks#using-prokka-or-rastgff-files-with-amrfinderplus) on our [[Tips and Tricks]] page.

#### `-p <protein_fasta>` and `-n <nucleotide_fasta>`
FASTA files are in a fairly standard format: Lines beginning with '>' are considered deflines, and sequence identifiers are
the first non-whitespace characters on the defline. Sequence identifiers are what is reported AMRFinder output Example FASTA files:
[`test_prot.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.fa)
and
[`test_dna.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_dna.fa). The sequence identifiers must match  the GFF file to use combined searches or add genomic coordinates to protein searches (see above).

# Output format

AMRFinder output is in tab-delimited format (.tsv). The output format depends
on the options `-p`, `-n`, and `-g`. Protein searches with gff files (`-p <file.fa> -g <file.gff>` and translated dna searches (`-n <file.fa>`) will  
include the `Contig id`, `start`, and `stop` columns. 

### Sample AMRFinder report:

`amrfinder -p ` [`test_prot.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.fa) ` -g ` [`test_prot.gff`](https://raw.githubusercontent.com/ncbi/amr/master/test_prot.gff) ` -n ` [`test_dna.fa`](https://raw.githubusercontent.com/ncbi/amr/master/test_dna.fa)` -O Campylobacter`

Should
result in the sample output shown below and in [`test_both.expected`](https://raw.githubusercontent.com/ncbi/amr/master/test_both.expected).

```
Protein identifier       Contig id Start Stop Strand Gene symbol   Sequence name                                       Scope Element type Element subtype Class               Subclass            Method              Target length Reference sequence length % Coverage of reference sequence % Identity to reference sequence Alignment length Accession of closest sequence Name of closest sequence                                            HMM id     HMM description
NA                       Contig5     260 2021      - 23S_A2075G    Campylobacter macrolide resistant 23S               core  AMR          POINT           MACROLIDE           MACROLIDE           POINTN                       2021                      2912                            60.51                            99.83             1762 NC_022347.1:1040292-1037381   23S                                                                 NA         NA
blaTEM-156               contig1     101  961      + blaTEM-156    class A beta-lactamase TEM-156                      core  AMR          AMR             BETA-LACTAM         BETA-LACTAM         ALLELEP                       286                       286                           100.00                           100.00              286 WP_061158039.1                class A beta-lactamase TEM-156                                      NF000531.2 TEM family class A beta-lactamase
NA                       contig10    486 1307      + blaOXA        class D beta-lactamase                              core  AMR          AMR             BETA-LACTAM         BETA-LACTAM         INTERNAL_STOP                 274                       274                           100.00                            99.64              274 WP_000722315.1                oxacillin-hydrolyzing class D beta-lactamase OXA-9                  NF000270.1 class D beta-lactamase
blaTEM-internal_stop     contig11    113  547      + blaTEM        TEM family class A beta-lactamase                   core  AMR          AMR             BETA-LACTAM         BETA-LACTAM         PARTIALP                      144                       286                            50.35                            97.22              144 WP_000027057.1                class A broad-spectrum beta-lactamase TEM-1                         NF000531.2 TEM family class A beta-lactamase
qacR-curated_blast       contig12     71  637      + qacR          multidrug-binding transcriptional regulator QacR    plus  STRESS       BIOCIDE         QUATERNARY AMMONIUM QUATERNARY AMMONIUM BLASTP                        188                       188                           100.00                            99.47              188 ADK23698.1                    multidrug-binding transcriptional regulator QacR                    NA         NA
blaPDC-114_blast         contig2       1 1191      + blaPDC        PDC family class C beta-lactamase                   core  AMR          AMR             BETA-LACTAM         CEPHALOSPORIN       BLASTP                        397                       397                           100.00                            99.75              397 WP_061189306.1                class C beta-lactamase PDC-114                                      NF000422.6 PDC family class C beta-lactamase
blaOXA-436_partial       contig3     101  802      + blaOXA        OXA-48 family class D beta-lactamase                core  AMR          AMR             BETA-LACTAM         BETA-LACTAM         PARTIALP                      233                       265                            87.92                           100.00              233 WP_058842180.1                OXA-48 family carbapenem-hydrolyzing class D beta-lactamase OXA-436 NF000387.2 OXA-48 family class D beta-lactamase
vanG                     contig4     101 1147      + vanG          D-alanine--D-serine ligase VanG                     core  AMR          AMR             GLYCOPEPTIDE        VANCOMYCIN          EXACTP                        349                       349                           100.00                           100.00              349 WP_063856695.1                D-alanine--D-serine ligase VanG                                     NF000091.3 D-alanine--D-serine ligase VanG
NA                       contig6    2680 3102      + 50S_L22_A103V Campylobacter macrolide resistant 50S L22           core  AMR          POINT           MACROLIDE           MACROLIDE           POINTX                        141                       141                           100.00                            97.16              141 WP_002851214.1                50S L22                                                             NA         NA
gyrA                     contig6      31 2616      + gyrA_T86I     Campylobacter quinolone resistant GyrA              core  AMR          POINT           QUINOLONE           QUINOLONE           POINTP                        862                       863                            99.88                            99.54              862 WP_002857904.1                gyrA                                                                NA         NA
50S_L22                  contig7     101  526      + 50S_L22_A103V Campylobacter macrolide resistant 50S L22           core  AMR          POINT           MACROLIDE           MACROLIDE           POINTP                        141                       141                           100.00                            97.16              141 WP_002851214.1                50S L22                                                             NA         NA
NA                       contig8     101  700      + blaTEM        TEM family class A beta-lactamase                   core  AMR          AMR             BETA-LACTAM         BETA-LACTAM         PARTIAL_CONTIG_ENDX           200                       286                            69.93                           100.00              200 WP_061158039.1                class A beta-lactamase TEM-156                                      NF000531.2 TEM family class A beta-lactamase
aph3pp-Ib_partial_5p_neg contig9       1  675      - aph(3'')-Ib   aminoglycoside O-phosphotransferase APH(3'')-Ib     core  AMR          AMR             AMINOGLYCOSIDE      STREPTOMYCIN        PARTIAL_CONTIG_ENDP           225                       275                            81.82                            99.56              225 WP_109545041.1                aminoglycoside O-phosphotransferase APH(3'')-Ib                     NF032895.1 aminoglycoside O-phosphotransferase APH(3'')-Ib
sul2_partial_3p_neg      contig9     715 1377      - sul2          sulfonamide-resistant dihydropteroate synthase Sul2 core  AMR          AMR             SULFONAMIDE         SULFONAMIDE         PARTIAL_CONTIG_ENDP           221                       271                            81.55                           100.00              221 WP_001043265.1                sulfonamide-resistant dihydropteroate synthase Sul2                 NF000295.1 sulfonamide-resistant dihydropteroate synthase Sul2
nimIJ_hmm                contigX       1  501      + nimIJ         NimIJ family nitroimidazole resistance protein      core  AMR          AMR             NITROIMIDAZOLE      NITROIMIDAZOLE      HMM                           166                        NA                               NA                               NA               NA NA                            NA                                                                  NF000262.1 NimIJ family nitroimidazole resistance protein
```

### Fields:

- Protein Identifier - This is from the FASTA defline for the protein or DNA sequence.
- Contig id - (optional) Contig name.
- Start - (optional) 1-based coordinate of first nucleotide coding for protein in DNA sequence on contig.
- Stop - (optional) 1-based coordinate of last nucleotide coding for protein in DNA sequence on contig.  Note that for protein hits (where the Method is HMM or ends in P) the coordinates are taken from the GFF, which means that for circular contigs when the protein spans the contig break the stop coordinate may be larger than the contig size (see the [GFF3 standard](https://github.com/The-Sequence-Ontology/Specifications/blob/master/gff3.md) for details)
- Gene symbol - Gene or gene-family symbol for protein or nucleotide hit. For point mutations it is a combination of the gene symbol and the SNP definition separated by "\_"
- Sequence name - Full-text name for the protein, RNA, or point mutation.
- Scope - The AMRFinderPlus database is split into 'core' AMR proteins that are expected to have an effect on resistance and 'plus' proteins of interest added with less stringent inclusion criteria. These may or may not be expected to have an effect on phenotype. 
- Element type - AMRFinder+ genes are placed into functional categories based on predominant function AMR, STRESS, or VIRULENCE.
- Element subtype - Further elaboration of functional category into (ANTIGEN, BIOCIDE, HEAT, METAL, PORIN) if more specific category is available, otherwise he element is repeated.
- Class - For AMR genes this is the class of drugs that this gene is known to contribute to resistance of. 
- Subclass - If more specificity about drugs within the drug class is known it is elaborated here. 
- Method - Type of hit found by AMRFinder. A suffix of 'P' or 'X' is appended to "Methods" that could be found by protein or nucleotide.
  - ALLELE - 100% sequence match over 100% of length to a protein named at the allele level in the AMRFinderPlus database.
  - EXACT - 100% sequence match over 100% of length to a protein in the database that is not a named allele.
  - BLAST - BLAST alignment is > 90% of length and > 90% identity to a protein in the AMRFinderPlus database.
  - PARTIAL - BLAST alignment is > 50% of length, but < 90% of length and > 90% identity to the reference, and does not end at a contig boundary.
  - PARTIAL_CONTIG_END - BLAST alignment is > 50% of length, but < 90% of length and > 90% identity to the reference, and the break occurrs at a contig boundary indicating that this gene is more likely to have been split by an assembly issue.
  - HMM - HMM was hit above the cutoff, but there was not a BLAST hit that met standards for BLAST or PARTIAL. This does not have a suffix because only protein sequences are searched by HMM. 
  - INTERNAL_STOP - Translated blast reveals a stop codon that occurred before the end of the protein. This can only be assessed if the `-n <nucleotide_fasta>` option is used.
  - POINT - Point mutation identified by blast.  
- Target length - The length of the query protein or gene. The length will be in amino-acids if the reference sequence is a protein, but nucleotide if the reference sequence is nucleotide.
- Reference sequence length - The length of the Reference protein or nucleotide in the database if a blast alignment was detected, otherwise NA.
- % Coverage of reference sequence - % of reference covered by blast hit if a blast alignment was detected, otherwise NA.
- % Identity to reference sequence - % amino-acid identity to reference protein or nucleotide identity for nucleotide reference if a blast alignment was detected, otherwise NA.
- Alignment length - Length of BLAST alignment in amino-acids or nucleotides if nucleotide reference if a blast alignment was detected, otherwise NA.
- Accession of closest protein - RefSeq accession for reference hit by BLAST if a blast alignment was detected otherwise NA. Note that only one reference will be chosen if the blast hit is equidistant from multiple references. For point mutations the reference is the sensitive "wild-type" allele, and the element symbol describes the specific mutation. Check the [Reference Gene Catalog](https://www.ncbi.nlm.nih.gov/pathogens/refgene/) for more information on specific mutations or reference genes.
- Name of closest protein - Full name assigned to the closest reference hit if a blast alignment was detected, otherwise NA.
- HMM id - Accession for the HMM, NA if none.
- HMM description - The family name associated with the HMM, NA if none.

## Common errors and what they mean

### Protein id "\<protein id\>" is not in the .gff-file

To automatically combine overlapping results from protein and nucleotide searches the coordinates of the protein in the assembly contigs must be indicated by the GFF file. This requires a GFF file where the value of the 'Name=' variable of the 9th field in the GFF must match the identifier in the protein FASTA file (everything between the '>' and the first whitespace character on the defline). See the [section on GFF file format](Running-AMRFinderPlus#-g-gff_file) for details of how AMRFinderPlus associates FASTA file entries with GFF file entries.

## Known Issues

#### Circular contigs
AMRFinderPlus does not have the concept of circular contigs, so genes that cross the break in circular contigs may be detected only as fragments. By default AMRFinderPlus has a length cutoff of 50% of the full gene length, so one side or the other should be detected at least as a partial_contig_end or blast hit. Depending on the annotation system proteins may be annotated crossing the contig boundary in circular contigs (NCBI's PGAP does this). These full-length proteins will be analyzed by AMRFinderPlus. Note that the stop coordinate in this case will extend past the contig boundary as described by the [GFF specification](https://github.com/The-Sequence-Ontology/Specifications/blob/master/gff3.md).

#### GFF file formats are not all standard
GFF files are used to identify coordinates for protein sequences, but the association between the identifiers in the GFF and FASTA files is not part of the standard. See the [section on GFF file format](Running-AMRFinderPlus#-g-gff_file) for details of how AMRFinderPlus associates FASTA file entries with GFF file entries. If you have issues getting your GFF files to work please file an issue or email us at pd-help@ncbi.nlm.nih.gov including the files you are trying to use. 

#### Prokka GFF files incompatible
Using GFF files output from [Prokka](https://github.com/tseemann/prokka) will not work by default. See [Using Prokka GFF files with AMRFinderPlus](Tips-and-tricks#using-prokka-gff-files-with-amrfinderplus) for a Perl one-liner to convert Prokka GFFs to GFFs compatible with AMRFinderPlus.

If you find bugs or have other questions/comments please email
us at pd-help@ncbi.nlm.nih.gov or Create a [GitHub issue](https://github.com/ncbi/amr/issues).
