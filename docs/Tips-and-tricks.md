### Contents
* [Use a bash loop to run on a number of files serially](#use-a-bash-loop-to-run-on-a-number-of-files-serially)
* [Combine mutliple AMRFinderPlus output files](#combine-mutliple-amrfinderplus-output-files)
* [Combine the above two tricks to run AMRFinderPlus on a series of files and combine the results](#combine-the-above-two-tricks-to-run-amrfinderplus-on-a-series-of-files-and-combine-the-results)
* [Filter AMRFinderPlus output for a specfic type/subtype/scope/class, etc. using awk.](#filter-amrfinderplus-output-for-a-specfic-typesubtypescopeclass-etc-using-awk)
* [Using Prokka or RAST GFF files with AMRFinderPlus](#using-prokka-or-RAST-gff-files-with-amrfinderplus)
* [Considerations for HPC and maximizing throughput](#considerations-for-hpc-and-maximizing-throughput)

### Use a loop to run on a number of files serially

This assumes that you're using a consistent filename format with .assembly.fa as the extension on your assembly nucleotide FASTA files, and that you want to run AMRFinderPlus serially (one job after the other). See [issue 32](https://github.com/ncbi/amr/issues/32) for another example.

```
# for each of the files ending in .assembly.fa
for assembly in *.assembly.fa
do
    # get the base of the filename
    base=$(basename $assembly .assembly.fa)
    # run AMFinderPlus with output going to the base.amrfinder
    amrfinder -n $assembly --threads 8 --plus > $base.amrfinder
done
```

### Combine mutliple AMRFinderPlus output files

When running AMRFinderPlus on many assemblies it is often useful to combine the output from many runs into one file with an additional column for an assembly identifier. There are a few ways to do this outlined in [issue 25](https://github.com/ncbi/amr/issues/25). 

The following assumes the files you want to combine are named `*.amrfinder`. It will create a file named combined.tsv that contains all of the AMRFinderPlus files combined.

```
# get one copy of the header
head -1 $(ls *.amrfinder | head -1) > combined.tsv
# skip headers and concatinate all files ending in .amrfinder
grep -h -v 'Protein identifier' *.amrfinder >> combined.tsv
```

### Combine the above two tricks to run AMRFinderPlus on a series of files and combine the results

See also Github issue https://github.com/ncbi/amr/issues/40

```
for assembly in *.assembly.fa
do
    base=$(basename $assembly .assembly.fa)
    # note that we use the --name option to add the "base" as the first
    # column of the output
    amrfinder -n $assembly --threads 8 --plus --name=$base > $base.amrfinder
done

head -1 $(ls *.amrfinder | head -1) > combined.tsv
grep -h -v 'Protein identifier' *.amrfinder >> combined.tsv
```

### Filter AMRFinderPlus output for a specfic type/subtype/scope/class, etc. using awk.

There are, of course, many ways to do this. Ex:

```
awk 'NR == 1 || $<column number> == "<value>" { print }' <amrfinder_output>
```

For example to filter for 'core' genes only:

```
awk 'NR == 1 || $i == 'core' { print }' <amrfinder_output>
```

To use the technique described here you need to know what column number you
want to filter on. For example to filter a run for only beta-lactam resistance
genes: For a combined (protein + nucleotide) of AMRFinderPlus the "Class"
column is column 11. (This uses the [AMRFinderPlus test
data](https://raw.githubusercontent.com/ncbi/amr/master/test_both.expected)
included with the source code)

```
# download the test data
curl -O https://raw.githubusercontent.com/ncbi/amr/master/test_both.expected

# filter for beta-lactam resistance elements
awk -F'\t' 'NR == 1 || $11 == "BETA-LACTAM" { print }' test_both.expected
```



### Using Prokka or RAST GFF files with AMRFinderPlus

Using GFF files output from [Prokka](https://github.com/tseemann/prokka) or [RAST](https://rast.nmpdr.org/) will not work by default because their formats are different from what AMRFinderPlus expects. Running the following perl one-liner will convert the Prokka/RAST output into a GFF file that AMRFinderPlus can read (replace <prokka_output.gff> with the GFF file you wish to use and <amrfinder.gff> with the name you wish to use for the AMRFinderPlus-compatible GFF file):

```
perl -pe '/^##FASTA/ && exit; s/(\W)Name=/$1OldName=/i; s/ID=([^;]+)/ID=$1;Name=$1/' <prokka_output.gff>  > <amrfinder.gff>
```

See also the discussion for issues [#24](https://github.com/ncbi/amr/issues/24#issuecomment-615538273) and [#55](https://github.com/ncbi/amr/issues/55).

### Considerations for HPC and maximizing throughput

Groups wanting to do very large numbers of AMRFinderPlus analyses may want to run it on a cluster or to run many jobs in parallel.

In our experience CPU is the primary bottleneck when trying to run many runs of AMRFinderPlus, so to maximize efficiency when running many jobs in parallel we suggest using `--threads 1` on each of the jobs. 

This assumes you have sufficient RAM to run all of the jobs in parallel. RAM requirements depend on the sequence input because BLAST and HMMER RAM requirements will depend on how many hits they have to keep track of while they're running. In our experience RAM is usually not an issue, but tests on your data may be required to get a good idea of maximum RAM use if your environment is memory limited.

AMRFinderPlus reads and writes some fairly large temporary files in `/tmp`, so it is possible disk access to `/tmp` may have a significant effect on performance. You can change the location for the temporary files by setting the `TMPDIR` environment variable.
