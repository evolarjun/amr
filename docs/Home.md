# NCBI Antimicrobial Resistance Gene Finder Plus (AMRFinderPlus)

## Overview for AMRFinderPlus 3.10

This software and the accompanying database identify acquired
antimicrobial resistance genes in bacterial protein and/or assembled nucleotide
sequences as well as known resistance-associated point mutations for several taxa. With AMRFinderPlus
we added select members of additional classes of genes such as virulence
factors, biocide, heat, acid, and metal resistance genes.

By default the results returned by AMRFinderPlus will be similar to those returned by the original AMRFinder and include antimicrobial resistance genes of known function.  Use the [--plus option](Running-AMRFinderPlus#option--plus) to include results that include additional classes of genes like virulence factors, biocide, heat, acid, and metal resistance genes.

Run amrfinder -u to get the most
recent release version of the database before using. See [[New in AMRFinderPlus]] and the [list of releases](https://github.com/ncbi/amr/releases) for more information on what's new.

<!--

### Why use AMRFinderPlus?

- blaKPC-2 is not the same as blaKPC-52. AMRFinderPlus answers the question "What is this gene?", not what it looks like it might be. Heirarchical database and parameters are designed to deliver accurate names and symbols. 
- Our database is lovingly curated by senior microbiologists with over 50 years of collective annotation experience. 
- The "core" database is curated to relevant proteins, we don't include everything anyone ever said is an AMR gene. We remove genes (like mcr-9) when evidence accumulates that they are not AMR genes in the real world.
- Results and names are based on _protein_ sequence, not _nucleotide_ sequence because function is dependent on protein sequence.

### Why not use AMRFinderPlus?
- It only supports contigs, not reads.
- The "core" database is curated to relevant proteins, we don't include everything anyone ever said is an AMR gene. We remove genes (like mcr-9) when evidence accumulates that they are not AMR genes in the real world.
- It is usually slower than tools that just run one of blastn, blastp, or HMMER.

Other options: [ABRicate](https://github.com/tseemann/abricate), [Ariba](https://github.com/sanger-pathogens/ariba), [Resfinder](https://cge.cbs.dtu.dk/services/ResFinder/), [RGI](https://card.mcmaster.ca/analyze/rgi), [SRST2](https://github.com/katholt/srst2), etc.
-->

# Installation

AMRFinder requires BLAST and HMMER. We provide instructions using BioConda to
install the prerequisites.

[[Installation Instructions|Installing-AMRFinder]]

# Running AMRFinderPlus

[[Examples|Running AMRFinderPlus#examples]]<br>
[[Usage (syntax/options)|Running AMRFinderPlus#usage]]<br>
[[Tips and tricks]]

# Methods

AMRFinder can be run in multiple modes with protein sequence as input and/or
with DNA sequence as input. To get maximum information it should be run with
both protein and nucleotide. When run with protein sequence it uses both BLASTP
and HMMER to search protein sequences for AMR genes along with a hierarchical
tree of gene families to classify and name novel sequences. With nucleotide
sequences it uses BLASTX translated searches and the hierarchical tree of gene
families. Adding the `--organism` option enables screening for point mutations
in select organisms and suppresses the reporting of some that are extremely
common in those organisms.

[[Running AMRFinderPlus]]<br>
[[Interpreting results]]<br>
[[Methods|Methods]]<br>
[[AMRFinderPlus database]]


## A note about ABRicate

Many people use [ABRicate](https://github.com/tseemann/abricate) with the
default "ncbi" database. It has come to our attention that at least some of those
users think they are getting the same results they would get from
AMRFinderPlus and that NCBI recommends. ABRicate uses a subset of the AMRFinderPlus database and
different methods to do AMR gene detection. While we love Torsten and
ABRicate, the results are not the same as those you would get by running
AMRFinderPlus. To identify AMR genes from assembled sequence we recommend using
AMRFinderPlus to get the full value out of the NCBI compiled database,
including correct gene symbols, named allele vs. novel allele
determination, protein-based search/naming, curated cutoffs, HMM
searches, etc.

# Related Resources

- [Pathogen Detection AMR resources](https://www.ncbi.nlm.nih.gov/pathogens/antimicrobial-resistance/resources/) - 
The Pathogen Detection team at NCBI has a page describing the [AMR
Resources](https://www.ncbi.nlm.nih.gov/pathogens/antimicrobial-resistance/resources/) we provide.
- [Pathogen Detection Reference Gene Catalog](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/) -
Proteins used by AMRFinderPlus in table form with query, filtering, and additional metadata. This information is also included in 
a tab-delimited file [Reference Gene Catalog on FTP](https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/Data/latest).
- [Pathogen Detection Reference HMM Catalog](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/hmm/) - HMMs used by AMRFinderPlus in table form with query, filtering, and additional metadata
- [Pathogen Detection Isolates Browser](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/search/) - NCBI runs AMRFinderPlus on all microbial genome assemblies in the Pathogen Detection system (over 600,000 isolate assemblies).  Gene symbols from AMRFinderPlus runs, along with other metadata for the isolates are visible in the [Isolates Browser](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/search/)
- [MicroBIGG-E](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/microbigge/) - Detailed AMRFinderPlus results for individual elements, links to sequence, and additional metadata are included in the [Pathogen Detection Microbial Browser for Identification of Genetic and Genomic Elements (MicroBIGG-E)](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/microbigge/)

# amrfinder-announce mailing list

[Subscribe to the amrfinder-announce mailing list](http://www.ncbi.nlm.nih.gov/mailman/listinfo/amrfinder-announce) to get updates when we release new versions of
the software or database. This is a low-traffic list.

# Help

If you have any
questions about or experience problems running AMRFinderPlus, please contact
pd-help@ncbi.nlm.nih.gov.

See [[Version 1 documentation|v1 Home]] for the old AMRFinder version 1 software

[[License / Public Domain Notice|Licenses]]

# Publication

See our our [AMRFinderPlus paper](https://www.ncbi.nlm.nih.gov/pmc/articles/PMC8208984/) for more details.

### Citation

#### AMRFinderPlus
Feldgarden M, Brover V, Gonzalez-Escalona N, Frye JG, Haendiges J, Haft DH, Hoffmann M, Pettengill JB, Prasad AB, Tillman GE, Tyson GH, Klimke W. AMRFinderPlus and the Reference Gene Catalog facilitate examination of the genomic links among antimicrobial resistance, stress response, and virulence. Sci Rep. 2021 Jun 16;11(1):12728. doi: 10.1038/s41598-021-91456-0. PMID: 34135355; PMCID: PMC8208984.

#### AMRFinder
Feldgarden M, Brover V, Haft DH, Prasad AB, Slotta DJ, Tolstoy I, Tyson GH, Zhao S, Hsu CH, McDermott PF, Tadesse DA, Morales C, Simmons M, Tillman G, Wasilenko J, Folster JP, Klimke W. Validating the AMRFinder Tool and Resistance Gene Database by Using Antimicrobial Resistance Genotype-Phenotype Correlations in a Collection of Isolates. Antimicrob Agents Chemother. 2019 Oct 22;63(11):e00483-19. doi: 10.1128/AAC.00483-19. Erratum in: Antimicrob Agents Chemother. 2020 Mar 24;64(4): PMID: 31427293; PMCID: PMC6811410.
