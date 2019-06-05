# New features
* Screen for select point mutations for antibiotic resistance from Salmonella, E. coli, and Campylobacter
    * Protein mutations in: 50S_L22 protein, acrB, acrR, basR, cmeR, cyaA, fabG, fabI, folP, gyrA, gyrB, lon, marR, murA, nfsA, ompF, parC, parE, pmrA, pmrB, ptsI, ramR, rpoB, rpsL, soxR, soxS, tufA, uhpA, uhpT
    * Nucleotide mutations in 16S and 23S rRNA
    * Included point mutations are listed in the reference gene database (https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/)
* New "Plus" genes 
    * virulence factors
    * biocide resistance
    * metal resistance
    * heat resistance
* Combined and integrated results from nucleotide and protein searches

## Known bugs

* The association between GFF lines and FASTA entries is fragile. AMRFinderPlus works with NCBI produced GFF and protein files, but possibly not others. 
* Customers have sometimes had linking problems because of glibc versions and libcurl. Please let us know if you have problems.

# Installation

AMRFinderPlus requires HMMER, BLAST+, Linux, and perl. We provide Linux binaries, and the source code is available on github if you want to compile it yourself, though we haven't extensively tested compiling AMRFinderPlus on other systems and aren't supporting non-Linux systems at this time.

## Prerequisites

We recommend using Bioconda to install the prerequisites and provide instructions for how to do that below, however that's just a suggestion. You may already have all of the prerequisites installed in another way. Notably you will need libcurl and the executables for [BLAST](https://www.ncbi.nlm.nih.gov/books/NBK279690/) and [HMMER](http://hmmer.org/) will need to be in your path. Note, it's not a prerequisite, but these instructions use Borne shell syntax (e.g., bash). If you're using another shell you might have to modify them slightly.

### Bioconda

While not strictly a prerequisite Bioconda is how we recommend installing the prerequisites. If you don''t have bioconda already installed you should run the following

```bash
~$ curl -O https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
~$ bash ./Miniconda3-latest-Linux-x86_64.sh # Follow prompts to accept license, choose install path, and allow the new bin directory to be added to .bashrc
~$ export PATH=$HOME/miniconda3/bin:$PATH # Change to match installation location, if not default.
~$ conda config --add channels defaults
~$ conda config --add channels bioconda
~$ conda config --add channels conda-forge
```

### Install the prerequisites

With bioconda the three prerequisites can be installed in one command

```bash
~$ conda install -y blast hmmer libcurl
```

### Installing AMRFinderPlus itself

```bash
~$ mkdir amrfinder && cd amrfinder
~/amrfinder$ curl -sL https://ftp.ncbi.nlm.nih.gov/pathogen/Technical/AMRFinder_technical/amrfinder_binaries_v3.01b.tar.gz | tar xvz
~/amrfinder$ ./amrfinder -u
~/amrfinder$ ./amrfinder -p test_prot.fa
```

Note that to run AMRFinderPlus you will need to have the BLAST and HMMER binaries in your path. If you installed the prerequisites with bioconda as recommended above you may need to run the following, log out, and log back in before AMRFinderPlus will work.

```bash
~/miniconda3/bin/conda init
```

### Email
If you are still having trouble installing AMRFinderPlus or have any questions let me know by emailing me at aprasad@ncbi.nlm.nih.gov.

*NOTE:* The handling of threading is incomplete, this version will not run on machines with < 4 cores.

# Usage: 

    # print a list of command-line options
    amrfinder --help 

    # Download the latest AMRFinderPlus database
    amrfinder -u
  
    # Protein AMRFinderPlus with "plus" genes
    amrfinder -p <protein.fa> 

    # Translated nucleotide AMRFinderPlus with "plus" genes
    amrfinder -n <assembly.fa>

    # protein AMRFinderPlus using GFF to get genomic coordinates
    amrfinder -p <protein.fa> -g <protein.gff> 

    # search for AMR genes and Campylobacter protein mutations
    amrfinder -p <protein.fa> -O Campylobacter 

    # Search for everything AMRFinderPlus can find:
    # AMR genes, plus genes, protein and nucleotide point mutations and combine results
    amrfinder -p <protein.fa> -O Campylobacter -g <protein.gff> -n <assembly.fa> 
    
The one fragile part of running combined nucleotide and protein analyses is the association between proteins and GFF file lines. AMRFinderPlus works ok with NCBI's public formats for those files, but since there's no standard for how to associate FASTA entries with GFF lines it can be finicky. If you have any problems email me (aprasad@ncbi.nlm.nih.gov) with examples.
