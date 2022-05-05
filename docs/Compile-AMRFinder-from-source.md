# Manual installation summary

If you want to compile AMRFinder yourself you'll need [gcc](https://gcc.gnu.org/) and [GNU make](https://www.gnu.org/software/make/) to compile the software as well as the other prerequisites [BLAST+](https://www.ncbi.nlm.nih.gov/books/NBK279690/), [HMMER](http://hmmer.org/), and [Perl](https://www.perl.org/). We have only tested compiling AMRFinder with GNU tools on Linux, so your mileage may vary. 

AMRFinder was developed with g++ (GCC) 4.9.3.


# Downloading

You can clone the repository using 
```bash
git clone https://github.com/ncbi/amr.git
```

Or download the source from the [AMRFinder github site](https://github.com/ncbi/amr).

# Compiling

```bash
make 
```

# Installation

```bash
make install
```
will copy the AMRFinder executables to `/usr/local/share/amrfinder`. To change the installation location add a INSTALL_DIR option to make e.g.:

```bash
make install INSTALL_DIR=$HOME/amrfinder
```