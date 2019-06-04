# AMRFinder+ 3.0b

## New features
* Screen for select point mutations for antibiotic resistance from Salmonella, E. coli, and Campylobacter
    * Protein mutations in: 50S_L22 protein, acrB, acrR, basR, cmeR, cyaA, fabG, fabI, folP, gyrA, gyrB, lon, marR, murA, nfsA, ompF, parC, parE, pmrA, pmrB, ptsI, ramR, rpoB, rpsL, soxR, soxS, tufA, uhpA, uhpT
    * Nucleotide mutations in 16S and 23S rRNA
    * Included point mutations are listed in the reference gene database (https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/)
* New "Plus" genes 
    * virulence factors
    * biocide resistance
    * metal resistance
    * heat resistance
* Combined and integrated results from nucleotide and protein searches

## Known bugs

* Right now AMRFinder will only run on machines with 4 or more cores. This will be fixed in the release version.
* The association between GFF lines and FASTA entries is fragile. AMRFinder works with NCBI produced GFF and protein files, but possibly not others. 
* Customers have sometimes had linking problems because of glibc versions and libcurl. Please let us know if you have problems.

*NOTE:* The handling of threading is incomplete, this version will not run on machines with < 4 cores.

# Usage: 

    # print a list of command-line options
    amrfinder --help 

    # Download the latest AMRFinder plus database
    amrfinder -u
  
    # Protein AMRFinder with "plus" genes
    amrfinder -p <protein.fa> 

    # Translated nucleotide AMRFinder with "plus" genes
    amrfinder -n <assembly.fa>

    # protein AMRFinder using GFF to get genomic coordinates
    amrfinder -p <protein.fa> -g <protein.gff> 

    # search for AMR genes and Campylobacter protein mutations
    amrfinder -p <protein.fa> -O Campylobacter 

    # Search for everything AMRFinder can find:
    # AMR genes, plus genes, protein and nucleotide point mutations and combine results
    amrfinder -p <protein.fa> -O Campylobacter -g <protein.gff> -n <assembly.fa> 
    
The one fragile part of running combined nucleotide and protein analyses is the association between proteins and GFF file lines. AMRFinder works ok with NCBI's public formats for those files, but since there's no standard for how to associate FASTA entries with GFF lines it can be finicky. If you have any problems email me (aprasad@ncbi.nlm.nih.gov) with examples.