* [[Install with binary]]
* [[Compile from source]]

AMRFinderPlus supports installation to Linux and MacOS via bioconda

There are two steps.

## 1. Install conda if you don't already have it

If you don't already have bioconda installed, otherwise [[skip ahead|Install with bioconda#Install AMRFinder with bioconda]].

### [See the official install miniconda instructions](https://docs.conda.io/en/latest/miniconda.html) for Linux
OR 
```bash
curl -O https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh
bash ./Miniconda3-latest-Linux-x86_64.sh 
```
Follow prompts to accept license, choose install path, and allow the new bin directory to be added to .bashrc

### [See Install miniconda instructions](https://docs.conda.io/en/latest/miniconda.html) for MacOS
OR
```bash
curl -O https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh
bash ./Miniconda3-latest-MacOSX-x86_64.sh 
```
Follow prompts to accept license, choose install path, and allow the new bin directory to be added to .bashrc

We recommend you set up bioconda as the default channel, but it is not required:

```bash
export PATH=$HOME/miniconda3/bin:$PATH # Change to match installation location, if not default.
conda config --add channels defaults
conda config --add channels bioconda
conda config --add channels conda-forge
```

# 2. Install AMRFinder with bioconda

Make sure the conda environment you just created is activated
```bash
source ~/miniconda3/bin/activate
```

Install AMRFinder and all of the prerequisites. 

```bash
conda install -y -c bioconda -c conda-forge ncbi-amrfinderplus
```

# What next?

Note that conda will have to be activated for AMRFinderPlus and its prerequisites to be in your path. To make that happen automatically on login run the following to add activation commands to your login scripts:

```bash
~/miniconda3/bin/conda init
```

Otherwise to activate conda on a case-by-case basis use:
```bash
source ~/miniconda3/bin/activate
```

## Testing your installation

See [[Test your installation]] for testing instructions

## To update your installation

```bash
conda update -y -c bioconda ncbi-amrfinderplus
```

## Email

If you are still having trouble installing AMRFinderPlus or have any questions let us know by emailing us at pd-help@ncbi.nlm.nih.gov and we'll do what we can to help.

