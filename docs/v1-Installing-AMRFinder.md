# Installation Summary

AMRFinder requires HMMER, BLAST+, Linux, and perl. We provide Linux binaries, and the source code is available if you want to [[compile it yourself|v1 Compile AMRFinder from source]], though we haven't extensively tested compiling AMRFinder on other systems and aren't supporting non-Linux systems at this time.

## New! [[Install AMRFinder via Bioconda|v1 BioConda installation]]

# Prerequisites

We recommend using Bioconda to install the prerequisites and provide instructions for how to do that below. The executables for [BLAST](https://www.ncbi.nlm.nih.gov/books/NBK279690/), [HMMER](http://hmmer.org/), and [Perl](https://www.perl.org/) will need to be in your path. See the [Troubleshooting](#Troubleshooting) section below to test them.  Note, it's not a prerequisite, but these instructions use Borne shell style syntax (e.g., that from bash). If you're using another shell syntax you might have to modify them slightly.

## Bioconda

While not strictly a prerequisite Bioconda is how we recommend installing the prerequisites. If you don''t have bioconda already installed you should run the following

```bash
~$ curl -O https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
~$ bash ./Miniconda3-latest-Linux-x86_64.sh # Follow prompts to accept license, choose install path, and allow the new bin directory to be added to .bashrc
~$ export PATH=$HOME/miniconda3/bin:$PATH # Change to match installation location, if not default.
~$ conda config --add channels defaults
~$ conda config --add channels bioconda
~$ conda config --add channels conda-forge
```

## Install the prerequisites

With bioconda the three prerequisites can be installed in one command

```bash
~$ conda install -y blast hmmer perl
```

# Installing AMRFinder

```bash
~$ mkdir amrfinder && cd amrfinder
~/amrfinder$ curl -sL https://github.com/ncbi/amr/releases/download/amrfinder_v1.04/amrfinder_binaries_v1.04.tar.gz | tar xvz
~/amrfinder$ ./amrfinder.pl -U
~/amrfinder$ ./amrfinder.pl -p test_prot.fa
```

# Testing your installation

```bash
~/amrfinder$ ./amrfinder.pl -p test_prot.fa
```

You should see something like [test_prot.expected](https://github.com/ncbi/amr/blob/master/test_prot.expected)

    Target identifier	Gene symbol	Protein name	Method	Target length	Reference protein length	% Coverage of reference protein	% Identity to reference protein	Alignment length	Accession of closest protein	Name of closest protein	HMM id	HMM description
    blaOXA-436_partial	blaOXA	OXA-48 family class D beta-lactamase	PARTIAL	233	265	87.92	100.00	233	WP_058842180.1	OXA-48 family carbapenem-hydrolyzing class D beta-lactamase OXA-436	NF000387.2	OXA-48 family class D beta-lactamase
    blaPDC-114_blast	blaPDC	PDC family class C beta-lactamase	BLAST	397	397	100.00	99.75	397	WP_061189306.1	class C beta-lactamase PDC-114	NF000422.2	PDC family class C beta-lactamase
    blaTEM-156	blaTEM-156	class A beta-lactamase TEM-156	ALLELE	286	286	100.00	100.00	286	WP_061158039.1	class A beta-lactamase TEM-156	NF000531.2	TEM family class A beta-lactamase
    nimIJ_hmm	nimIJ	NimIJ family nitroimidazole resistance protein	HMM	166	NA	NA	NA	NA	NA	NA	NF000262.1	NimIJ family nitroimidazole resistance protein
    vanG	vanG	D-alanine--D-serine ligase VanG	EXACT	349	349	100.00	100.00	349	WP_063856695.1	D-alanine--D-serine ligase VanG	NF000091.3	D-alanine--D-serine ligase VanG

```shell
~/amrfinder$ ./amrfinder -n test_dna.fa
```

You should see something like [test_dna.expected](https://github.com/ncbi/amr/blob/master/test_dna.expected)


    Target identifier	Contig id	Start	Stop	Strand	Gene symbol	Protein name	Method	Target length	Reference protein length	% Coverage of reference protein	% Identity to reference protein	Alignment length	Accession of closest protein	Name of closest protein	HMM id	HMM description
    contig1	contig1	101	958	+	blaTEM-156	class A beta-lactamase TEM-156	ALLELE	286	286	100.00	100.00	286	WP_061158039.1	class A beta-lactamase TEM-156	NF000531.2	TEM family class A beta-lactamase
    contig2	contig2	1	1191	+	blaPDC	PDC family class C beta-lactamase	BLAST	397	397	100.00	99.75	397	WP_061189306.1	class C beta-lactamase PDC-114	NF000422.2	PDC family class C beta-lactamase
    contig3	contig3	101	802	+	blaOXA	OXA-48 family class D beta-lactamase	PARTIAL	234	265	88.30	100.00	234	WP_058842180.1	OXA-48 family carbapenem-hydrolyzing class D beta-lactamase OXA-436	NF000387.2	OXA-48 family class D beta-lactamase
    contig4	contig4	101	1147	+	vanG	D-alanine--D-serine ligase VanG	EXACT	349	349	100.00	100.00	349	WP_063856695.1	D-alanine--D-serine ligase VanG	NF000091.3	D-alanine--D-serine ligase VanG

# Troubleshooting

To run AMRFinder all of the prerequisites must be installed and in your PATH. We also recommend placing the AMRFinder executables in your path (e.g., `export PATH=~/amrfinder`)

## Testing the prerequisites

### [BLAST+](https://www.ncbi.nlm.nih.gov/books/NBK279690/)
To make sure blast is in your path try running:
```
blastp -version
```

### [HMMER](http://hmmer.org/)
To make sure HMMER is working and in your path try running:
```
hmmsearch -h
```

### [Perl](https://www.perl.org/)
To make sure perl is installed and in your path try running:
```
perl -v
```
AMRFinder also requires the Net::FTP module to download the AMRFinder database, to confirm it is installed run:
```
perldoc -lm Net::FTP
```

## Email
If you are still having trouble installing AMRFinder let us know by emailing us at pd-help@ncbi.nlm.nih.gov, and we'll do what we can to help.
