# Overview

`amrfinder_customize` allows the addition of gene sequences and protein point mutations to the AMRFinderPlus database in a fairly consistent way. There are several limitations at this time, so if they impact you please let us know and we can work on fixing them.

You can only add protein genes, either to existing "families" or to new families. They will not be treated as "alleles" in our database which have a special handling where a family symbol is returned if the sequence match is not identical. 

Right now we only have protein point mutations that can be added, either to existing genes for an existing "`--organism`" or for new genes and/or organisms. 

# Testing your installation
```
make test_customdb
```

# Running `amrfinder_customize`

This requires three files. One a FASTA file, and a tab-delimited metadata file that provides information about each of the genes, and a mutation metadata file. See below for formats.

Example input files are contained in the github directory `custom.fa`, and `custom.meta`, `custom.pm`.  (add links once in production)

```
amrfinder_customize --database_in data/latest --database_out data/customdb --prot custom.fa --metadata custom.meta
# Adding point mutations
```
This requires a FASTA file with the wildtype sequence and a tab-delimited metadata file for the point mutations. See below for formats.

## File formats

### FASTA file

The defline needs to contain two tab-delimited fields. 
1. The sequence accession (or other identifier, will appear in the AMRFinderPlus report and the point mutation metadata file if this is a wildtype sequence for a point mutation)
2. The gene symbol (this will be printed in the AMRFinderPlus report for genes that you are adding to the database. This should appear in the gene metadata file for genes)

Example: 
```
>aac6p_me_acc   aac(6')-novel
MTNSNDSVTLRLMTEHDLAMLYEWLNRSHIVEWWGGEEARPTLADVQEQYLPSVLAQESVTPYIAMLNGE
PIGYAQSYVALGSGDGWWEEETDPGVRGIDQLLANASQLGKGLGTKLVRALVELLFNDPEVTKIQTDPSP
SNLRAIRCYEKAGFEKQGTVTTPDGPAVYMVQTRQAFERTRSVA
>mexR_acc       mexR
MNYPVNPDLMPALMAVFQHVRTRIQSELDCQRLDLTPPDVHVLKLIDEQRGLNLQDLGRQMCRDKALITRKIRELEGRNLVRRERNPSDQRSFQLFLTDEGLAIHQHAEAIMSRVHDELFAPLTPVEQATLVHLLDQCLAAQPLEDI*
```

### Gene metadata file

This is a tab-delimited file describing each of the genes you want to add to the AMRFinderPlus database with the following fields. Each gene symbol must have an entry in this file to be identified. Note that if you are just adding new gene sequences to an existing AMRFinderPlus gene/family you do not have to put a line in this file, though you should make sure the "gene symbol" in the FASTA file matches a `fam_id` from the `fam.tab` file.

1. `gene_symbol` - The symbol that will be printed in the Gene symbol field of the AMRFinderPlus results
2. `reportable`  - This determines the "Scope" of the added gene. `1` for "Plus" genes and `2` for "Core"
3. `type` - text that will go in the `type` field of the AMRFinderPlus report for this gene
4. `subtype` - text that will go in the `subtype` field of the AMRFinderPlus report for this gene
5. `class` - text that will go in the `class` field of the AMRFinderPlus report for this gene
6. `subclass` - text that will go in the `subclass` field of the AMRFinderPlus report for this gene
7. `protein_name` - text that will go in the `Sequence name` field of the AMRFinderPlus report for this gene

A header line starting with a '#' is also required. If you are only adding point mutations then a file with just the header line should be used. See the examples in the src directory.

Example:
```
#gene_symbol	reportable	type	subtype	class	        subclass	protein_name
aac(6')-novel   2               AMR     AMR     AMINOGLYCOSIDE  AMINOGLYCOSIDE  Aminoglycoside modifying enzyme AAC(6')-novel
```

### Point mutation metadata file

This is a tab-delimited file describing each of the point mutations you want to add to the AMRFinderPlus database. Each wildtype sequence must be in the FASTA file, and the accession / FASTA identifier must match this file. If you are only adding genes then this file may only contain the header line.

1. `protein_identifier` - This is the first field on the defline for the wildtype gene in the FASTA file
2. `gene_symbol` - This is the symbol that will be printed in the `Gene symbol` field of the AMRFinderPlus report when this mutation is identified
3. `organism` - This is the --organism/-O option that the AMRFinderPlus user needs to use to screen for this mutation
4. `mutation` - This is point mutation you are searching for in the format <wildtype_allele><position><mutant_allele> E.g., V126E
5. `type` - text that will go in the `type` field of the AMRFinderPlus report when this point mutation is found
6. `subtype` - text that will go in the `subtype` field of the AMRFinderPlus report when this point mutation is found
7. `class` - text that will go in the `class` field of the AMRFinderPlus report for when this point mutation is found
8. `subclass` - text that will go in the `subclass` field of the AMRFinderPlus report when this point mutation is found
9. `mutated_protein_name` - text that will go in the `Sequence name` field of the AMRFinderPlus report when this point mutation is found

Note that the header is required, see the examples in the src directory.

Example:
```
#protein_identifier	gene_symbol	organism	        mutation	class	        subclass        mutated_protein_name
mexR_acc                mexR            Pseudomonas_aeruginosa  V126E           FLUOROQUINOLONE FLUOROQUINOLONE Pseudomonas aeruginosa multidrug resistance operon repressor MexR
```

