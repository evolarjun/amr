# NCBI Antimicrobial Resistance Gene Finder (AMRFinder)

We have a newer version [[AMRFinderPlus v3.0|Home]] that has slightly changed options and significant new features. We are supporting AMRFinder version 1 until AMRFinderPlus is fully tested. If you have issues or concerns please let us know by emailing us at pd-help@ncbi.nlm.nih.gov.

## Overview

This software and the accompanying database are designed to find acquired
antimicrobial resistance genes in protein or nucleotide sequences.

## Installation

AMRFinder requires BLAST, HMMER, and perl. We provide instructions using BioConda to install the prerequisites (and eventually AMRFinder).

[[Standard installation instructions|v1 Installing AMRFinder]]  
[[Bioconda installation instructions|v1 BioConda installation]]  
[[Compile AMRFinder from source|v1 Compile AMRFinder from source]]

## Mechanism

AMRFinder can be run in two modes with protein sequence as input or with DNA
sequence as input. When run with protein sequence it uses both BLASTP and HMMER
to search protein sequences for AMR genes along with a hierarchical tree of
gene families to classify and name novel sequences. With nucleotide sequences
it uses BLASTX translated searches and the hierarchical tree of gene families.

[[Running AMRFinder|v1 Running AMRFinder]]  
[[Methods|v1 Methods]]  
[[AMRFinder database|v1 AMRFinder database]]  

[Paper at bioRxiv](https://www.biorxiv.org/content/10.1101/550707v1): Using the NCBI AMRFinder Tool to Determine Antimicrobial Resistance Genotype-Phenotype Correlations Within a Collection of NARMS Isolates 

## Citation

Feldgarden, Michael, Vyacheslav Brover, Daniel H. Haft, Arjun B. Prasad, Douglas J. Slotta, Igor Tolstoy, Gregory H. Tyson, et al. “Using the NCBI AMRFinder Tool to Determine Antimicrobial Resistance Genotype-Phenotype Correlations Within a Collection of NARMS Isolates.” BioRxiv, February 15, 2019, 550707. https://doi.org/10.1101/550707.

## Help

[Subscribe to the amrfinder-announce mailing list](http://www.ncbi.nlm.nih.gov/mailman/listinfo/amrfinder-announce) to get updates when we release new versions of
the software or database. This is a low-traffic list.

Note that this version of AMRFinder is an alpha version. If you have any questions about or experience problems running AMRFinder, please contact pd-help@ncbi.nlm.nih.gov.

[[Licenses]]
