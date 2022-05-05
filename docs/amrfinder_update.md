`amrfinder_update` will download the latest version of the AMRFinderPlus database and by default make a directory format under the amrfinder directory with a directory for the database and a link from 'latest' to that directory.

### Usage: 

amrfinder_update [options] --database=<database_directory>

### Options:

`-d <directory>` or `--database <directory>` The root directory for the
databases. Databases will be created in subdirectories named by the version
under that directory.

`--force_update` Force overwriting an existing database directory. This is
useful in case of corruption or some other error.

### Output:

You should end up with a directory structure something like the following:

      amr
        |-data
            |-latest -> 2018-11-07.1
            |-2018-11-07.1
            |-2018-09-14.1

Where `data` is the directory indicated with the `-d` or `--database` option.
