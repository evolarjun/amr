# AMRFinderPlus 3.10

__WARNING:__ This version has a different database format from versions 1.0-3.9, and is not compatible with those database versions (Database releases 2018-03-30.1 to 2020-12-17.1). Run `amrfinder -u` after installing to download the most recent compatible database version.

Note that as of database release 2021-09-30.1 we renamed the _fam_id_ and _parent_fam_id_ to _node_id_ and _parent_node_id_ to maintain consistency with the new _node_id_, _parent_node_id_, and _hierarchy_node_ columns of [[ReferenceGeneHierarchy.txt|AMRFinderPlus database#referencegenehierarchytxt]] and [[ReferenceGeneCatalog.txt|AMRFinderPlus database#referencegenecatalogtxt]]. 

## New features

### 3.10.16

- Blast rules for non-reportable proteins can now override reportable HMM hits to suppress results. 

### 3.10.14

- Several improvements that increase speed and improve parallelization. Speedups range from a few percent to more than 70% depending on number of threads and the characteristics of the input sequence.
- Improved error message on missing blast database
- FinderPlus will remove '-' from files passed to HMMER / BLAST because some versions of HMMER choke on protein files with '-' characters

### 3.10.5

- New option to output nucleotide FASTA file with sequence of element plus a 5' flank
- New option to report all equal scoring reference proteins on separate rows.

### 3.10.1

- Allows reversed cutoffs to identify sequences more divergent than others. 

### 3.9.8

- Will now use the environment variable TMPDIR to determine where temporary files go

### 3.9.3

- The format of the database has changed to allow allele-based setting of class, subclass, core and plus.
- Fusion genes are now reported on one line with the two gene symbols and product names separated by a '/'

The current database files are [[described here|AMRFinderPlus database]].

### 3.8.28

 - amrfinder -u option no longer overwrites databases if they already exist.
 New --force_update option will still overwrite the database as the old -u
 option did. (https://github.com/ncbi/amr/issues/16)
 - New --protein_output and --nucleotide_output options will produce FASTA files
 containing the sequence of AMRFinderPlus hits
 - --mutation_all output has been streamlined and some bugs have been fixed
 - New --name option will prepend a "Name" field to every row of the
 AMRFinderPlus report (https://github.com/ncbi/amr/issues/25)
 - Fusion genes are now reported with both element symbols separated by a '/' on
 both lines of the report. E.g., aac(6')-Ie/aph(2'')-If2
 - For HMM-only hits (have HMM in the method column which means they do not have
 BLAST results that meet cutoff) BLAST statistics will now be reported
 if any alignment at that location was made.
 - When in COMBINED (nucleotide + protein) mode and when an "INTERNAL_STOP" is detected
 at a locus that also has a protein, the protein result will be reported with
 Method of INTERNAL_STOP (Previously the nucleotide result was reported. This
 would affect the identity, length, and coverage statistics AMRFinderPlus reports.
 - Improved handling of partial alignments for point-mutation detection, including
 prioritization of translated matches (POINTX) when the protein match identifies fewer
 known SNPs.
 - Incorrect amino acids inferred by protein annotations extended in the 5' (N-terminal)
 direction because correct annotations start with alternative start codons will
 no longer count as mismatches.
 - Small improvements in performance.

## Upgrading

See [[Upgrading]] for instructions on how to update to the latest version of the software and database.

See [[Installing AMRFinder]] for help with new installations.

## Known issues

* The association between GFF lines and FASTA entries is fragile. AMRFinderPlus works with NCBI produced GFF and protein files, but possibly not others. See the --pgap option to use AMRFinderPlus with annotation files created with [PGAP](https://github.com/ncbi/pgap/wiki)
* Customers installing from the binary distribution have sometimes had linking problems because of glibc versions and libcurl. Please let us know if you have problems. Bioconda installations should avoid these issues.

# Usage: 

    # print a list of command-line options
    amrfinder --help 

    # Download the latest AMRFinderPlus database
    amrfinder -u
    
    # Protein AMRFinderPlus
    amrfinder -p <protein.fa> 

    # Translated nucleotide AMRFinderPlus
    amrfinder -n <assembly.fa>

    # protein AMRFinderPlus using GFF to get genomic coordinates
    amrfinder -p <protein.fa> -g <protein.gff> 

    # search for AMR genes and Campylobacter protein mutations
    amrfinder -p <protein.fa> -O Campylobacter 

    # Search for everything AMRFinderPlus can find:
    # AMR genes, plus genes, protein and nucleotide point mutations, skip blacklisted genes and combine results
    amrfinder -p <protein.fa> -g <protein.gff> -n <assembly.fa> -O Campylobacter 

# Help

Please email us at pd-help@ncbi.nlm.nih.gov if you any questions or concerns.

