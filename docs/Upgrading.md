Database Upgrades
=================

This should be as easy as running:
    
    amrfinder --update

Some users have reported errors with the automatic updating. You can automatically update to a directory different from the default by using:

    amrfinder_update -d <database_directory>

To use that directory use the `-d` option: 

    amrfinder -d <database_directory>/latest  ...

Software Upgrades
=================

Instructions for upgrading AMRFinderPlus depend on how it was installed in the first place. A summary and new features for each release can be seen in the [list of releases](https://github.com/ncbi/amr/releases).

Bioconda installations
----------------------

For Bioconda installations upgrading is simple:

If necessary

    source ~/miniconda3/bin/activate

Then

    conda update -c bioconda -c conda-forge ncbi-amrfinderplus

Binary installations
--------------------

Basically just re-install, though you shouldn't have to install any prerequisites since presumably they're already installed so you just need to follow [[the instructions to download and untar the binary|Install-with-binary#installing-amrfinderplus-itself]]

Source installation
-------------------

You should update your sources to the latest release version of the software (e.g., `git pull`)
or download the latest sources from the [latest release](https://github.com/ncbi/amr/releases/latest). For example:
    
    git pull
    make clean
    make

See [[Compile from source]] for more details.

