# Installation Summary

AMRFinder requires HMMER, BLAST+, Linux, and perl. We provide bioconda packages, Linux binaries, and source code  if you want to compile it yourself, though we haven't extensively tested compiling AMRFinder on other systems and aren't supporting non-Linux systems at this time.

This new bioconda installation should install all of the prerequisites as well as AMRFinder once you have bioconda installed. It should also run on both Mac and Linux without recompilation. 

# Installing bioconda

The simplest way to install AMRFinder is using the [Bioconda](https://bioconda.github.io/) package. If you already have bioconda installed skip ahead to the Installing AMRFinder section. If you need to install bioconda for the first time. 

### Install Miniconda on linux

```bash
~$ curl -O https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
~$ bash ./Miniconda3-latest-Linux-x86_64.sh 
```
Follow prompts to accept license, choose install path, and allow the new bin directory to be added to .bashrc

### Install Miniconda on MacOS

```bash
~$ curl -O https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOS-x86_64.sh
~$ bash ./Miniconda3-latest-MacOS-x86_64.sh 
# Follow prompts to accept license, choose install path, and allow the new bin directory to be added to .bashrc
```
Follow prompts to accept license, choose install path, and allow the new bin directory to be added to .bashrc

## Set up Bioconda channel

```bash
~$ export PATH=$HOME/miniconda3/bin:$PATH # Change to match installation location, if not default.
~$ conda config --add channels defaults
~$ conda config --add channels bioconda
~$ conda config --add channels conda-forge
```

# Installing AMRFinder

```bash
~$ conda install ncbi-amr
```