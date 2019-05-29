# Installation

AMRFinder requires HMMER, BLAST+, Linux, and perl. We provide Linux binaries, and the source code is available to compile AMRFinder yourself though we haven't extensively tested compiling AMRFinder on other systems and aren't supporting non-Linux systems at this time.

For basic instructions on compiling AMRFinder see [Compile AMRFinder from source](Compile-AMRFinder-from-source.md).

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

### Installing AMRFinder itself

```bash
~$ mkdir amrfinder && cd amrfinder
~/amrfinder$ curl -sL https://ftp.ncbi.nlm.nih.gov/pathogen/Technical/AMRFinder_technical/amrfinder_binaries_v3.01b.tar.gz | tar xvz
~/amrfinder$ ./amrfinder -u
~/amrfinder$ ./amrfinder -p test_prot.fa
```

Note that to run AMRFinder you will need to have the BLAST and HMMER binaries in your path. If you installed the prerequisites with bioconda as recommended above you may need to run the following, log out, and log back in before AMRFinder will work.

```bash
~/miniconda3/bin/conda init
```

### Test your installation


```bash
./amrfinder -p test_prot.fa -g test_prot.gff > test_prot.got
diff test_prot.expected test_prot.got
```

There should be no differences in output


### Email

If you are still having trouble installing AMRFinder or have any questions let me know by emailing us at pd-help@ncbi.nlm.nih.gov. 

*NOTE:* The handling of threading is incomplete, this version will not run on machines with < 4 cores.

# Usage: 

    # print a list of command-line options
    amrfinder --help 

    # Download the latest AMRFinder plus database
    amrfinder -u
  
    # Protein AMRFinder with "plus" genes
    amrfinder -p <protein.fa> 

    # Translated nucleotide AMRFinder with "plus" genes
    amrfinder -n <assembly.fa>

    # protein AMRFinder using GFF to get genomic coordinates
    amrfinder -p <protein.fa> -g <protein.gff> 

    # search for AMR genes and Campylobacter protein mutations
    amrfinder -p <protein.fa> -O Campylobacter 

    # Search for everything AMRFinder can find:
    # AMR genes, plus genes, protein and nucleotide point mutations and combine results
    amrfinder -p <protein.fa> -O Campylobacter -g <protein.gff> -n <assembly.fa> 
    
The one fragile part of running combined nucleotide and protein analyses is the association between proteins and GFF file lines. AMRFinder works ok with NCBI's public formats for those files, but since there's no standard for how to associate FASTA entries with GFF lines it can be finicky. If you have any problems email us at pd-help@ncbi.nlm.nih.gov with examples.
