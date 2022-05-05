## Prerequisites

AMRFinder requires [BLAST+](https://www.ncbi.nlm.nih.gov/books/NBK279690/) and [HMMER](http://hmmer.org/) to run as well as [libcurl](https://curl.haxx.se/libcurl/). 

You may already have all of the prerequisites installed in another way. You will need libcurl. The executables for [BLAST](https://www.ncbi.nlm.nih.gov/books/NBK279690/) and [HMMER](http://hmmer.org/) will need to be in your path. Note, it's not a prerequisite, but these instructions use Borne shell syntax (e.g., bash). If you're using another shell you might have to modify them slightly.

### Bioconda

While not strictly a prerequisite Bioconda is how we recommend installing the prerequisites and indeed [[AMRFinderPlus|Install with bioconda]]. If you don''t have bioconda already installed you should run the following

```bash
~$ curl -O https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
~$ bash ./Miniconda3-latest-Linux-x86_64.sh # Follow prompts to accept license, choose install path, and allow the new bin directory to be added to .bashrc
~$ export PATH=$HOME/miniconda3/bin:$PATH # Change to match installation location, if not default.
~$ conda config --add channels defaults
~$ conda config --add channels bioconda
~$ conda config --add channels conda-forge
```

With bioconda the three prerequisites can be installed in one command

```bash
~$ conda install -y blast hmmer libcurl
```

[[Install AMRFinder|Installing AMRFinder]]
