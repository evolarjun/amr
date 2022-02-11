Make sure `amrfinder` and the dependencies blast and HMMER are executable and in your path. If you installed via bioconda you may need to run:
```bash
source ~/miniconda3/bin/activate
```
Make sure you have the latest AMRFinder database files:
```bash
amrfinder -u
```

The [[bioconda installation|Install with bioconda]] doesn't yet include the test files, so you can use the following commands to download them:

```bash
curl -O https://raw.githubusercontent.com/ncbi/amr/master/test_dna.fa \
     -O https://raw.githubusercontent.com/ncbi/amr/master/test_prot.fa \
     -O https://raw.githubusercontent.com/ncbi/amr/master/test_prot.gff \
     -O https://raw.githubusercontent.com/ncbi/amr/master/test_both.expected \
     -O https://raw.githubusercontent.com/ncbi/amr/master/test_dna.expected \
     -O https://raw.githubusercontent.com/ncbi/amr/master/test_prot.expected 

```


Protein only search:
```bash
amrfinder --plus -p test_prot.fa -g test_prot.gff -O Escherichia > test_prot.got
diff test_prot.expected test_prot.got

```

Nucleotide only search:
```bash
amrfinder --plus -n test_dna.fa -O Escherichia > test_dna.got
diff test_dna.expected test_dna.got

```

Full combined search:
```bash
amrfinder --plus -n test_dna.fa -p test_prot.fa -g test_prot.gff -O Escherichia > test_both.got
diff test_both.expected test_both.got

```

There should be no differences in output.

## Troubleshooting

### Make sure the dependencies are executable

The most common issues are that the AMRFinderPlus can't find blast or HMMER binaries. Make sure you can run the following:

```bash
blastp -help
```
If that does not print out a long help message from blastp then AMRFinder may not know where to find your blast executables. If you installed blast via bioconda then running `source ~/miniconda3/bin/activate` may fix this issue.

```bash
hmmsearch -h
```
If that command doesn't print a help message from HMMER then AMRFinder may not know where to find the HMMER executables.  If you installed HMMER via bioconda then running `source ~/miniconda3/bin/activate` may fix this issue.

### Download the latest database

```bash
amrfinder -u
```
### Try updating to the latest version

```bash
conda update -c bioconda -y ncbi-amrfinderplus
```

## Email

If you are still having trouble installing AMRFinderPlus or have any questions let us know by emailing us at pd-help@ncbi.nlm.nih.gov. 
