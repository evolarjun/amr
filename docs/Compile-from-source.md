* [[Install with bioconda]]
* [[Install with binary]]

# Manual installation summary

If you want to compile AMRFinderPlus yourself you'll need [gcc](https://gcc.gnu.org/) and [GNU make](https://www.gnu.org/software/make/) to compile the software as well as the other prerequisites [BLAST+](https://www.ncbi.nlm.nih.gov/books/NBK279690/), [HMMER](http://hmmer.org/), and [libcurl](https://curl.haxx.se/libcurl/). We have only tested compiling AMRFinderPlus with GNU tools on Linux. Your mileage may vary. 

AMRFinderPlus was developed with g++ (GCC) 4.9.3 and libcurl 7.29.0

To install binaries see [[Installing AMRFinder]].

# Downloading

You can clone the repository using 
```bash
git clone https://github.com/ncbi/amr.git
cd amr
git checkout master
```

Or download the source from the [AMRFinderPlus github site](https://github.com/ncbi/amr).

# Compiling

```bash
make 
```

To adjust the default database directory use something like the following (we are working on a more flexible way to define directories, but this solves the problem in the short term):

```bash
make clean
make DEFAULT_DB_DIR=/usr/local/share/amrfinder/data
```

**NOTE**: Right now when using a `DEFAULT_DB_DIR` AMRFinderPlus will print a WARNING message about $CONDA_PREFIX. Please ignore this warning, it will go away in the next release.

You may also have to adjust flags in the Makefile for libcurl and for different versions of GCC.

# Installation

```bash
make install
```
will copy the AMRFinderPlus executables to `/usr/local/share/amrfinder`. To change the installation location add a INSTALL_DIR option to make e.g.:

```bash
make install INSTALL_DIR=$HOME/amrfinder
```

See [[Test your installation]].
