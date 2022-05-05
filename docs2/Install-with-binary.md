* [[Install with bioconda]]
* [[Compile from source]]

AMRFinderPlus requires libcurl, HMMER and BLAST+. We only provide binaries for Linux, and the source code is available to compile AMRFinderPlus yourself though we haven't extensively tested compiling AMRFinderPlus on other systems and aren't supporting non-Linux systems at this time.

We have not tested compiling AMRFinder in all circumstances and we recommend [[installing with bioconda|Install with bioconda]] for ease and simplicity. For basic instructions on compiling AMRFinderPlus see [[Compile from source]].

## Prerequisites

AMRFinder requires the executables for the following to be in your path:
* [BLAST](https://www.ncbi.nlm.nih.gov/books/NBK279690/)
* [HMMER](http://hmmer.org/)

AMRFinder also requires shared libraries for [libcurl](https://curl.haxx.se/libcurl/).

We recommend using Bioconda to install the prerequisites and provide instructions for how to do that below, or you could just [[Install everything with bioconda|Install with bioconda]]. Notably you will need shared libraries for libcurl, and the executables for [BLAST](https://www.ncbi.nlm.nih.gov/books/NBK279690/) and [HMMER](http://hmmer.org/) will need to be in your path. Note, it's not a prerequisite, but these instructions use Borne shell syntax (e.g., bash). If you're using another shell you might have to modify them slightly.

### Bioconda

While not strictly a prerequisite Bioconda is how we recommend installing the prerequisites. If you don''t have bioconda and/or the prerequisites already installed you should run the following

```bash
# Download and install miniconda
# Follow prompts to accept license, choose install path, and allow the new bin directory to be added to .bashrc
curl -O https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash ./Miniconda3-latest-Linux-x86_64.sh

export PATH=$HOME/miniconda3/bin:$PATH # Change to match installation location, if not default.

# Configure the bioconda "channel"
conda config --add channels defaults
conda config --add channels bioconda
conda config --add channels conda-forge
```

### Install the prerequisites

With bioconda the three prerequisites can be installed in one command

```bash
~$ conda install -y blast hmmer libcurl
```

Note that you may need to run `source ~/miniconda3/bin/activate` before running `amrfinder` so that the prerequisites are in your path. Alternatively if you run `~/miniconda3/bin/conda init` miniconda will add activation code to your login scripts.

## Installing AMRFinderPlus itself

Download the latest binary release from https://github.com/ncbi/amr/releases/latest and untar it into a directory.

```bash
mkdir amrfinder
cd amrfinder
# Get the URL of the latest binary tarball (linux only)
URL=`curl -s https://api.github.com/repos/ncbi/amr/releases/latest \
    | grep "browser_download_url.*amrfinder_binaries" \
    | cut -d '"' -f 4`
# Download the latest binary tarball
curl -sOL "$URL"

# untar AMRFinderPlus
filename=`basename $URL`
tar xvfz $filename

# Don't forget to download the latest database
./amrfinder -u
```

Note that to run AMRFinderPlus you will need to have the BLAST and HMMER binaries in your path. If you installed the prerequisites with bioconda as recommended above you may need to run the following, log out, and log back in before AMRFinderPlus will work.

```bash
~/miniconda3/bin/conda init
```

### Test your installation

See [[Test your installation]]

### Email

If you are still having trouble installing AMRFinderPlus or have any questions let us know by emailing us at pd-help@ncbi.nlm.nih.gov. 


