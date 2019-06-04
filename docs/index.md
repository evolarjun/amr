# NCBI Antimicrobial Resistance Gene Finder Plus (AMRFinder+)

## Overview for AMRFinder+ 3.0-alpha

This software and the accompanying database are designed to find acquired
antimicrobial resistance genes in protein or nucleotide sequences. With 3.0b
we have added select members of additional classes of genes of interest such
as virulence factors, biocide, heat, acid, and metal resistance genes.

### New in version 3.0b   *alpha*

**NOTE** This is *alpha* software still under active development. Production users are encouraged to use Version 1 in mission-critical applications.

See [AMRFinder 3.0b](AMRFinder-3.0b.md) for details

- Screening for point mutations in Salmonella, E. coli and Camplyobacter added
- New "plus" genes of interest requested by our collaborators
    - virulence factors
    - biocide resistance
    - metal resistance
    - heat resistance
- Combined and integrated results from nucleotide and protein searches
- New Class and Subclass fields for estimated AMR phenotypes where known
- Updated algorithm also handles curated blast-similarity-based rules for some genes where HMMs have not been developed

## Installation

AMRFinder requires BLAST and HMMER. We provide instructions using BioConda to install the prerequisites.

[Installation Instructions](Installing-AMRFinder.md)<BR>
[Compile AMRFinder from source](Compile-AMRFinder-from-source.md)

## Mechanism

AMRFinder can be run in two modes with protein sequence as input or with DNA
sequence as input. When run with protein sequence it uses both BLASTP and HMMER
to search protein sequences for AMR genes along with a hierarchical tree of
gene families to classify and name novel sequences. With nucleotide sequences
it uses BLASTX translated searches and the hierarchical tree of gene families.

[Running AMRFinder](Running-AMRFinder.md)<br>
[Methods](Methods.md)<br>
[AMRFinder database](AMRFinder-database.md)

## Help

Note that this version of AMRFinder is an alpha version. If you have any questions about or experience problems running AMRFinder, please contact pd-help@ncbi.nlm.nih.gov.

[Licenses](Licenses.md)

## Publication

See our our [paper at bioRxiv](https://www.biorxiv.org/content/10.1101/550707v1) for more details.
