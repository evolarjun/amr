# Introduction

The AMRFinder database files are used by the AMRFinder process as part of the Pathogen Detection Pipeline (https://www.ncbi.nlm.nih.gov/pathogens) as well as the command-line version of AMRFinder. The current version focuses on acquired or intrinsic AMR gene products. Currently it does not include tools for analyzing adaptive resistance mutations such as point mutations in ribosomal RNA genes, or promoter-affecting mutations.

Note that this database is compiled as part of the National Database of Antibiotic Resistant Organisms (NDARO) and more user-friendly access to the data is available at https://www.ncbi.nlm.nih.gov/pathogens/antimicrobial-resistance/. The rest of this document describes the format and structure of the database as it used by AMRFinder. 

## Genotype vs. Phenotype

Users of AMRFinder or its supporting data files are cautioned that presence of a gene encoding an antimicrobial resistance (AMR) protein does not necessarily indicate that the isolate carrying the gene is resistant to the corresponding antibiotic.  AMR genes must be expressed to confer resistance. An enzyme that acts on a class of antibiotic, such as the cephalosporins, may confer resistance to some but not others. Many AMR proteins reduce antibiotic susceptibility somewhat, but not sufficiently to change the isolate from "sensitive" to "intermediate" or "resistant."   Meanwhile, an isolate may gain resistance to an antibiotic by mutational processes, such as the loss of porin required to allow the antibiotic into the cell.  For some families of AMR proteins, especially those borne on plasmids, correlations of genotype to phenotype are much more easily deciphered, but users are cautioned against over-interpretation.

## Availability

The AMRFinder database is publicly available at https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/AMRFinder/data. Files on the FTP site are in the structure:

         |- data
              |- latest
              |- YYYY-MM-DD.#
                   |- fam.tab
                   |- AMRProt
                   |- AMR.LIB
                   |- changes.txt
              |- YYYY-MM-DD.#
                ...

Where [data/latest](https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/Data/latest/) is a link to the most recent version of the AMRFinder database.

# Files

Within each database directory there are the following files

* `fam.tab` - Tab-delimited file used internally by AMRFinder to define the hierarchical structure behind the AMRFinder database.
* `AMRProt` - A FASTA file of curated AMR protiens with a custom formatted defline containing metadata used by AMRFinder for each sequence.
* `AMR.LIB` - A file of curated AMR HMMs with trusted cutoffs
* `changes.txt` - A human-readable log listing the updates/changes with each release of the database

# File formats

## `AMRProt`
FASTA formatted file containing curated AMR Protein sequences along with thename of the gene/allele that we have assigned them. This FASTA file has a custom formatted defline with additional metadata to aid in gene naming. Fusion proteins have two FASTA entries, one describing the activity of each component.

Fields in the defline are separated by '|' characters and are as follows:
1. Protein GI
2. RefSeq protein accession
3. GenBank nucleotide accession <WE NEED TO REPLACE THIS WITH THE NG ACCESSION>
4. Fusion gene part number, 1 if not a fusion gene
5. Total number of fusion parts, 1 if not a fusion gene
6. Internal family identifier - see fam.tab
7. Internal family class, which is the parent internal family identifier if the protein is an allele, or the internal family identifier otherwise
8. Resistance mechanism type
9. Protein name

## `AMR.LIB`
HMM library in HMMER3 ASCII text format. Each model in the AMR.LIB has a format similar to the HMMs of Pfam (see http://pfam.xfam.org ).

For a general description of the file format see the HMMER User's guide at:
http://hmmer.org/documentation.html for a general description of the format.

Some notes on how we use the descriptive fields in the AMR.LIB file:

* `DESC`	The name applied to sequences hit by this HMM if no more specific HMM
	or BLAST hit applies.  See the section on fam.tab for details about
	the heirarchical structure behind some of our HMMs.

* `NAME`	Contains an internal node identifier that matches a FAM.id in the
	fam.tab file below.

Notes on the cutoffs included in the file (`TC`, `GA`, `NC`):
The `TC` field is the only one of the cutoffs used by and universally curated in	 our system. HMMs were not used to generate libraries, so we set `GC`=`TC` for convenience. `NC` was set by manual examination of results for some HMMs, but otherwise `NC`=`TC`.

## `fam.tab`

Tab-delimited file used internally by AMRFinder to define the heirarchical gene/protein family structure behind the AMRFinder database.

Fields are separated by tab characters and contain as follows:

i. **FAM.id** - the ID of the family described by this line

ii. **parent FAM.id** - Except for the root all contents of this field should have
another line describing the parent FAM.id. The root has an empty value.

iii. **Gene symbol** - The gene symbol to be reported for hits at this level.

iv. **HMM identifier** - The internal identifier (NAME) used to identify the HMM (if
any) that is used at this level in the heirarchy.

v. **HMM trusted cutoff 1** - Trusted cutoff for full_score

vi. **HMM trusted cutoff 2** - Trusted cutoff for domain_score

vii. **Reportable 0/1** - Whether a hit at this level will be reported as an AMRFinder hit

viii. **Family name** - The gene name to be reported for hits at this level

# Methods

## Sources of AMR proteins and HMMs

To build the AMR HMM collection, NCBI first assembled a comprehensive collection of acquired (and intrinsic) anti-microbial resistance proteins.  Sources, published or collaborative, include

*    the Lahey Clinic compilation of beta-lactamase sequences (http://www.lahey.org/studies/ and personal communications from Dr. George Jacoby and Karen Bush)
*    the Pasteur Institute collection of beta-lactamase sequences
*    ResFinder
*    Comprehensive Antibiotic Resistance Database (CARD)
*    the RAC and Integrall collections of AMR proteins found in integrons
*    the Center for Veterinary Medicine
*    Marilyn Roberts personal communications
*    "Oxford" - Derrick Crook personal communications

In addition, NCBI continually mines the literature for new reports of AMR proteins.  The ResFams collection of AMR HMMs aided provided important help early in our efforts to develop the AMR protein hierarchy and the AMRFinder tool, but all models were rebuilt with new seed alignment sequences, new alignments, new cutoff scores, and new biocuration. Development continued until all reportable proteins were covered by at least one AMR HMM, and classification by HMM was sufficiently specific.

NCBI is also responsible for the assignment of new beta lactamase alleles for certain families. Once new alleles are released, then they are immediately incorporated into AMRFinder. See this page for more information:

https://www.ncbi.nlm.nih.gov/pathogens/submit-beta-lactamase/

## Types of AMR proteins covered.

This collection covers AMR resistance proteins only if they confer resistance in bacteria. Alleles for AMR proteins are included in the data set only if they are naturally-occurring. The antibiotic affected by the AMR protein does not need to be used clinically as an antibiotic in human patients.  Our collection includes proteins that contribute to resistance quaternary ammonium compounds (which are not antibiotics, strictly speaking) and to antibiotics whose use is restricted to agricultural or veterinary applications, e.q. olaquindox.  Source databases we drew from (e.g. CARD) contained a large number of intrinsic proteins that contribute weakly to resistance (loss-of-function mutants show increased susceptibility), and that may contribute more strongly after mutational events increase their expression. We avoided including such proteins in the release, for now, since flagging such proteins makes the reports on AMR proteins identified far more difficult to read and understand.

At present, we provide no analysis of adaptive resistance, in which mutational processes rather than gene acquisition or intrinsic function are responsible of the resistance that may be observed. Thus, the files provided will not help much in finding sources of resistance in Mycobacterium tuberculosis, where nearly all the recent increase in antimicrobial resistance is attributable to mutational changes. We expect analysis of adaptive resistance to become a feature in future versions of AMRFinder.

As we collected AMR proteins from various sources, we examined them in multiple sequence alignments to determine whether we judged any to have structural problems, including truncations, frameshifts, or incorrect start sites. For aminoglycoside-modifying enzymes found in integrons, in particular, the most appropriate start site to choose often is unclear. We chose start sites such that regions of homology across a family were preserved, but regions clearly derived from the sites at which the genes integrated were removed. Note that this is separate from the alignment trimming step below.

## HMMs.

All HMMs used by AMRFinder can be found in the library AMR.LIB. This library is a special subset from a larger collection of protein profile HMMs being constructed at NCBI, and referred to on the whole as NCBIfams.  The AMR library consists of a subset of models designed for detecting and classifying candidate AMR proteins.  Each model was built from a manually reviewed and trimmed multiple sequence alignment, called the seed alignment. Regions of sequence that appeared extraneous, as from incorrect prediction of an upstream start site, were removed by trimming. Sequences that appeared non-representative, such as those with frameshift mutations or truncations, were removed. Models were built using the HMMER3 package (http://hmmer.org; PMID:22039361). For some HMMs, the seed alignment may consist of a single sequence.

Three types of cutoffs are provided, for compatibility: trusted_cutoff (TC), gathering threshold (GA), and noise_cutoff (NC).  In this library, TC and GA are always set to be identical. If NC is set to be the same as TC, this indicates that the noise cutoff has not yet been manually reviewed. Because all three types of cutoff are provided, searches may be performed using any of the HMMER package switches, --cut_tc, --cut_ga, or --cut_nc. AMRFinder uses --cut_tc.

## Gene/Protein Hierarchy

AMRFinder treats all families and alleles of AMR proteins as nodes in hierarchical tree.  Below is an example of the parent-child relationships for a beta-lactamase allele and the broader families that contain it.

    bla - beta-lactamase
      bla-A - class A beta-lactamase
        bla-A_carba - carbapenem-hydrolyzing class A beta-lactamase
          blaKPC - KPC family carbapenem-hydrolyzing class A beta-lactamase
            blaKPC-2 - carbapenem-hydrolyzing class A beta-lactamase KPC-2 (allele)

Evidence used to annotate proteins relies on HMM and BLAST.  AMRFinder will assign the most specific name it can find that is justified by the evidence.  In the example shown above, the bottom level represents an allele. Assigning an AMR protein to a specific allele requires a 100% identity BLAST match.  Each higher level node may have an associated HMM, with defined cutoff scores that make assignment to that family deterministic.  Not every (non-allele) node has its own HMM, but every AMR protein is covered by at least one HMM somewhere above it in the hierarchy.

Note that some nodes in the hierarchy group together child families that are not necessarily related by homology.  Such nodes will always lack an HMM.    For example, the family bla ("beta-lactamase") contains child families bla-A ("class A beta-lactamase") and "metallo-beta-lactamase"), which are similar in function but lack any sequence homology to each other.

