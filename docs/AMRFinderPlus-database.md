# Introduction

This database derives from the [Pathogen Detection Reference Gene
Catalog](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/) and is used
by the [Pathogen Detection](https://ncbi.nlm.nih.gov/pathogens/) isolate
analysis system to provide results to the [Isolates
browser](https://www.ncbi.nlm.nih.gov/pathogens/isolates) as well as the
command-line version of [[AMRFinderPlus|home]]. The 'core' subset version focuses
on acquired or intrinsic AMR gene products including point mutations in a limited set of taxa.

The 'plus' subset include a less-selective set of genes of interest including
genes involved in virulence, biocide, heat, metal, and acid
resistance. 

The most recent database release can be found in <https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/AMRFinderPlus/database/latest> and a log of changes with each release is available in the [changes.txt](https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/AMRFinderPlus/database/latest/changes.txt) file. Note that this database is compiled as part of the National Database of
Antibiotic Resistant Organisms (NDARO) and more user-friendly access to the
data is available at
https://www.ncbi.nlm.nih.gov/pathogens/antimicrobial-resistance/. The rest of
this document describes the format and structure of the database as it used by
AMRFinderPlus. 

## Genotype vs. Phenotype

Users of AMRFinderPlus or its supporting data files are cautioned that presence of
a gene encoding an antimicrobial resistance (AMR) protein does not necessarily
indicate that the isolate carrying the gene is resistant to the corresponding
antibiotic. AMR genes must be expressed to confer resistance. An enzyme that
acts on a class of antibiotic, such as the cephalosporins, may confer
resistance to some but not others. Many AMR proteins reduce antibiotic
susceptibility somewhat, but not sufficiently to change the isolate from
"sensitive" to "intermediate" or "resistant." Meanwhile, an isolate may gain
resistance to an antibiotic by mutational processes, such as the loss of porin
required to allow the antibiotic into the cell. For some families of AMR
proteins, especially those borne on plasmids, correlations of genotype to
phenotype are much more easily deciphered, but users are cautioned against
over-interpretation.

## Availability

The AMRFinderPlus database is publicly available at https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/
 Files on the FTP site are in the structure:

    |- /AMRFinderPlus
         |- data  # depricated
              |- latest
              |- YYYY-MM-DD.#
                   |- fam.tab
                   |- AMRProt
                   |- AMR.LIB
                   |- changes.txt
                   |- ReferenceGeneCatalog.txt
                   ...
              |- YYYY-MM-DD.#
                ...
         |- database 
             |- <database format version> e.g., 3.10
                   |- YYYY-MM-DD.#
                        |- fam.tab
                        |- AMRProt
                        |- AMRProt-mutation.tab
                        |- AMRProt-suppress
                        |- AMRProt-susceptible.tab
                        |- AMR.LIB
                        |- AMR_CDS
                        |- AMR_DNA-<organism>
                        |- AMR_DNA-<organism>.tab
                        |- changes.txt
                        |- min_software_version.txt
                        |- version.txt
                        |- changes.txt
                        |- taxgroup.tab
                        |- ReferenceGeneCatalog.txt
                        |- ReferenceGeneHierarchy.txt
                        |- amr_targets.fa
                   |- YYYY-MM-DD.#
                   ...

Note the `AMRFinderPlus/data` directory contains archived database versions prior to 2019-10-30.1 that are not compatible with more recent releases of the software.

Where [AMRFinderPlus/database/latest](https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/AMRFinderPlus/database/latest/) is a link to the most recent version of the AMRFinderPlus database.

For easier analysis with other tools we provide the reference genes used by
AMRFinderPlus along with with additional metadata in the [Reference Gene
Browser](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/) and as a
[tab-delimited
file](https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/AMRFinderPlus/data/latest/ReferenceGeneCatalog.txt).
The HMMs along with seed alignments are published on [our ftp site](https://ftp.ncbi.nlm.nih.gov/hmm/NCBIfam-AMRFinder/latest) as well.

# Files

Within each database directory there are the following files

* `ReferenceGeneCatalog.txt` - This is the table we recommend you use if you
  want data from the AMRFinderPlus database. This is the data we use for the
  [Reference Gene Catalog](https://www.ncbi.nlm.nih.gov/pathogens/refgene/),
  and should be the most convenient source of the database.
* `ReferenceGeneHierarchy.txt` - This is the data behind the [Reference Gene
  Hierarchy](https://www.ncbi.nlm.nih.gov/pathogens/genehierarchy/), the
  arrangement of genes, families, and upstream nodes that our curators use to
  organize and relate the genes and HMMs in the Pathogen Detection Reference
  Gene Catalog and Pathogen Detection Reference HMM Catalog. This hierarchy
  drives the gene idenfication and naming algorithm of AMRFinderPlus.
* `fam.tab` - Tab-delimited file used internally by AMRFinderPlus to define the
  hierarchical structure behind the AMRFinderPlus database.
* `AMRProt` - Curated AMR proteins in FASTA format with a custom formatted
  defline containing metadata used by AMRFinderPlus for each sequence.
* `AMR.LIB` - A file of curated AMR HMMs with trusted cutoffs.
* `AMR_CDS` - Coding sequences for each protein in `AMRProt`. This file is not
  directly used by AMRFinderPlus, but is included because it is helpful in
  other analyses.
* `AMR_DNA-*` - Curated resistance causing nucleotide sequences for point
  mutation assessment
* `AMR_DNA-*.tab` - Tab-delimited file of nucleotide point-mutation information
* `changes.txt` - A human-readable log listing the updates/changes with each
  release of the database
* `min_software_version.txt` - To enable software to detect compatible database
  this is the database format version
* `taxgroup.tab` - For NCBI Pathogen Detection taxonomic group -> organism
  mapping (used with `--gpipe_org` option)
* `version.txt` - The database version
* `amr_targets.fa` - The set of targets with reduced sequence redundancy used
  for the Pathogen Detection guided assembler prcess (SAUTE). These are derived
  from 'core' genes plus stx only. 
* `AMRProt-mutation.tab` - Tab-delimited file of protein point mutation
  information
* `AMRProt-suppress` - Tab-delimited file of `-O/--organism` argument and
  protein id to suppress
* `AMRProt-susceptible.tab` - Tab-delimited file of proteins that are only
  reported when forms are more divergent

# File formats

## `ReferenceGeneCatalog.txt`

This tab-delimited file is the canonical source of the Pathogen Detection
Reference Gene Database. This data is also visible in the [Reference Gene
Catalog](https://www.ncbi.nlm.nih.gov/pathogens/isolates#/refgene/) (described
[here](https://www.ncbi.nlm.nih.gov/pathogens/pathogens_help/#reference-gene-catalog)).
While this file is not used directly by the AMRFinderPlus software, it should
be the place to go if you are interested in the data behind AMRFinderPlus. The
other files described here were designed for the use of the AMRFinderPlus
software specifically and may change substantially in the future to enable new
features of the software. 

In this file each row is a protein or point-mutation, and each
column is metadata about that protein or point-mutation.

Tab-delimited fields are as follows:

1. __allele__: If the protein has named alleles (e.g., blaTEM-1) then this
   field will have the allele symbol for this sequence. If this row is a point
   mutation then this will contain a point-mutation symbol (e.g., 23S_C2627A)
   representing a specific point mutation. For point mutations the associated
   accessions in the other columns will be for the reference gene that does not
   contain this mutation.
2. __gene_family__: The gene symbol for this gene family. This is a short
   abbreviation that is used to refer to this gene. Multiple sequences may
   share the same symbol, and this symbol will represent multiple very similar
   gene sequences.
3. __whitelisted_taxa__: This field is only used for point mutations, and
   describes the taxon for which this point mutation is relevant. This
   corresponds to the `--organism` option to AMRFinderPlus. See also
   __blacklisted_taxa__ which is used to exclude universal or nearly universal
   genes.
4. __product_name__: This is the name of the protein produced by the gene.
5. __scope__: The data subset to which this row belongs. It will either be
   'core' (highly curated, AMR-specific genes and point mutations) or 'plus'
   (genes related to biocide and stress resistance, general efflux, virulence,
   or antigenicity).
6. __type__: Classification of the type of protein/point mutation. This will be
   AMR, STRESS, or VIRULENCE.
7. __subtype__: A finer classification for the protein/point mutation if
   relevant. One of AMR, POINT, ACID, BIOCIDE, ANTIGEN, HEAT, METAL, or
   VIRULENCE.
8. __class__: "Class" provides a broad definition of the phenotype affected by
   the gene or allele, and includes phenotypes such as antimicrobial and stress
   resistance, virulence, and antigenicity. See [[class subclass]] for an
   enumeration of all classes and subclasses.
9. __subclass__: Where it is known, "Subclass" provides a more specific
   definition of the particular antibiotics or classes that are affected by the
   gene or point mutation (e.g., that are resisted by the gene/allele). While
   most subclass designations are self-explanatory, a few others have
   particular meanings. Specifically, "CEPHALOSPORIN" is equivalent to the
   Lahey 2be definition; "CARBAPENEM" means the protein has carbapenemase
   activity, but it might or might not confer resistance to other beta-lactams;
   "QUARTERNARY AMMONIUM" are quaternary ammonium compounds. In addition, stx
   subtypes (e.g., STX2E) and intimin subtypes (e.g., ALPHA) are defined for
   Shiga toxin proteins (class of STX1 or STX2) and intimins (class of INTIMIN)
   respectively. Where the phenotypic information is incomplete, contradictory,
   or unclear, the "Class" value is used for the "Subclass" value. See [[class
   subclass]] for an enumeration of all classes and subclasses.
10. __refseq_protein_accession__: Accession of the RefSeq protein sequence
    record that defines this entry. In the case of point mutations this will be
    the accession of the reference sequence which is used to identify the
    mutation, but is usually a sensitive wild-type allele. In the case of rows
    identifying proteins, this accession points to the curated canonical
    sequence for the entry in our database. 
11. __refseq_nucleotide_accession__: Accession of the RefSeq nucleotide
    sequence that contains the gene coding for the RefSeq protein accession
    (column 10).  Where possible this record contains a small amount of
    flanking sequence in addition to the coding sequence. Note that this
    accession may not point to the most common coding sequence for this
    protein.
12. __curated_refseq_start__: During curation all sequences and annotations are
    checked by NCBI curators. On occasion the start site indicated on the
    GenBank record is missing or deemed incorrect and is set by NCBI curators.
    In that case this field will be "Yes" and the RefSeq accessions will point
    to an annotation that is different from that on the GenBank record. In
    other words the GenBank accession points to a translated protein sequence
    that is not identical to that included in the database.
13. __genbank_protein_accession__: The GenBank accession of the protein used to
    create the RefSeq record. Note that where curated_refseq_start == "Yes"
    this will not be identical to the curated RefSeq protein sequence. 
14. __genbank_nucleotide_accession__: The GenBank accession of the nucleotide
    sequence that codes for this protein. This may extend far beyond the coding
    sequence and include other genes.
15. __genbank_strand_orientation__: The orientation of the annotation used to
    create the RefSeq sequence (+/-). If this is "-" then the
    refseq_nucleotide_accession will point to a sequence that is the reverse
    complement of the genbank_nucleotide_accession.
16. __genbank_cds_start__: The coordinate of the start of the coding sequence
    on the GenBank nucleotide record used to create the RefSeq record.
17. __genbank_cds_stop__: The coordinate of the end of the coding sequence on
    the GenBank nucleotide record used to create the RefSeq record.
18. __pubmed_reference__: PubMed reference associated with this entry.
19. __blacklisted_taxa__: This protein is common in these taxonomic groups, and
    does not contribute to acquired phenotype. When AMRFinderPlus is run with
    an `--organism` option the presence of this protein will not be reported if
    the option value matches one of these groups. See also __whitelisted_taxa__
    which is used to identify an organism for point-mutation detection.
20. __synonyms__: Other symbols used to refer to this element/gene.
21. __hierarchy_node__: For protein sequences, this is the node in the AMR
    hierarchy to which this protein belongs. The hierarchy itself is described
    in the ReferenceGeneHierarchy.txt file (and fam.tab). This is not a gene symbol,
   but a helpful shorthand to keep track of the contents of the node. These are
   created for the utility and convenience of curators and we make no
   guarantees that these will persist in content or meaning.
21. __db_version__: The database version of this entry. 

## `ReferenceGeneHierarchy.txt`

This is the data behind the [Reference Gene
Hierarchy](https://www.ncbi.nlm.nih.gov/pathogens/genehierarchy/). 
The
Reference Gene Hierarchy is the arrangement of genes,
families, and upstream nodes that our curators use to organize and relate the
genes and HMMs in the Pathogen Detection Reference Gene Catalog, Pathogen
Detection Reference HMM Catalog. This hierarchy drives the gene idenfication
and naming algorithm of AMRFinderPlus.

Tab-delimited fields are as follows:

1. __node_id__: Unique identifier for this node. This is not a gene symbol,
   but a helpful shorthand to keep track of the contents of the node. These are
   created for the utility and convenience of curators and we make no
   guarantees that these will persist in content or meaning.
2. __parent_node_id__: Identifier for the parent node.
3. __symbol__: The gene symbol that will be reported by AMRFinderPlus for a
   sequence matching at this node and not one more specific.
4. __num_prots__: The number of proteins assigned to this node or below.
5. __hmm_acc__: Accession for the HMM assigned to this node (if any)
6. __prot_acc__: For allelic nodes the accession of the protein at that
   allele. Multiple proteins can exist at nodes that are non-alleleic, and
   those can be seen by going to that node in the ReferenceGeneCatalog.txt
7. __scope__: The data subset to which this row belongs. It will either be
   'core' (highly curated, AMR-specific genes and point mutations) or 'plus'
   (genes related to biocide and stress resistance, general efflux, virulence,
   or antigenicity).
8. __type__: Classification of the type of protein/point mutation. This will be
   AMR, STRESS, or VIRULENCE.
9. __subtype__: A finer classification for the protein/point mutation if
   relevant. One of AMR, POINT, ACID, BIOCIDE, ANTIGEN, HEAT, METAL, or
   VIRULENCE.
10. __class__: "Class" provides a broad definition of the phenotype affected by
   the gene or allele, and includes phenotypes such as antimicrobial and stress
   resistance, virulence, and antigenicity. See [[class subclass]] for an
   enumeration of all classes and subclasses.
11. __subclass__: Where it is known, "Subclass" provides a more specific
    definition of the particular antibiotics or classes that are affected by
    the gene or point mutation (e.g., that are resisted by the gene/allele).
    While most subclass designations are self-explanatory, a few others have
    particular meanings. Specifically, "CEPHALOSPORIN" is equivalent to the
    Lahey 2be definition; "CARBAPENEM" means the protein has carbapenemase
    activity, but it might or might not confer resistance to other
    beta-lactams; "QUARTERNARY AMMONIUM" are quaternary ammonium compounds. In
    addition, stx subtypes (e.g., STX2E) and intimin subtypes (e.g., ALPHA) are
    defined for Shiga toxin proteins (class of STX1 or STX2) and intimins
    (class of INTIMIN) respectively. Where the phenotypic information is
    incomplete, contradictory, or unclear, the "Class" value is used for the
    "Subclass" value. See [[class subclass]] for an enumeration of all classes
    and subclasses.
12. __name__: Name for proteins at this node. For reportable nodes this will be reported by AMRFinderPlus if no more specific node is hit above cutoffs.
13. __synonyms__: Other symbols used for this node is identified in the literature.
14. __db_version__: The database version of this entry. 

## `AMRProt`
FASTA formatted file containing curated Protein sequences along with the name of the gene/allele that we have assigned them. This FASTA file has a custom formatted defline with additional metadata used by AMRFinderPlus to name elements. Fusion proteins have two FASTA entries, one describing the activity of each component.

Fields in the defline are separated by '|' characters and are as follows:
1. Protein GI (0 if GI does not exist)
2. Protein accession-version
3. Fusion gene part number (1 or 2 for a fusion gene, 1 if not a fusion gene)
4. Total number of fusion parts (2 for a fusion gene, 1 if not a fusion gene)
5. node\_id (see [fam.tab](#famtab)) or allele symbol for named alleles
6. The parent\_node\_id if the protein is an allele, otherwise the same node\_id as
   in field 5
7. Resistance mechanism type
8. Reportability level (0 - do not report, 1 - plus, 2 - core)
9. Subclass
10. Class
11. Protein name

## `AMR_CDS`
FASTA formatted file containing coding sequences for curated Protein sequences along with the name of the gene/allele that they code for. This FASTA file has a custom formatted defline with additional metadata. Note that there is only one coding sequence included per protein in the database, and the coding sequence included here may not be a common one. This file is currently not used by AMRFinderPlus, but is included because it is often requested. Also note that these sequences include the stop codon. 

Fields in the defline are separated by '|' characters and are as follows:
1. Protein GI that this sequence codes for
2. Protein accession
3. DNA accession
4. Fusion gene part number (1 or 2 for a fusion gene, 1 if not a fusion gene)
4. Total number of fusion parts (2 for a fusion gene, 1 if not a fusion gene)
5. node\_id (see [fam.tab](#famtab)) or allele symbol for named alleles
6. The parent node\_id if the protein is an allele, otherwise the same node\_id as
   in field 5
7. Resistance mechanism type
8. Protein name

## `AMR.LIB`
HMM library in HMMER3 ASCII text format. Each model in the AMR.LIB has a format similar to the HMMs of Pfam (see http://pfam.xfam.org ).

For a general description of the file format see the HMMER User's guide at:
http://hmmer.org/documentation.html.

Some notes on how we use the descriptive fields in the AMR.LIB file:

* `DESC`	The name applied to sequences hit by this HMM if no more specific HMM
	or BLAST hit applies.  See the section on fam.tab for details about
	the hierarchical structure behind some of our HMMs.

* `NAME`	Contains an internal node identifier that matches a node\_id in the
	[fam.tab](#famtab) file.

* `TC` The cutoff used by AMRFinderPlus to determine what constitutes a "match" for this HMM.

Cutoffs included in the file (`TC`, `GA`, `NC`): The `TC` field is the only one
of the cutoffs used by and universally curated in our system. HMMs were not
used to generate libraries, so we set `GC`=`TC` for convenience. `NC` was set
by manual examination of results for some HMMs, but otherwise `NC`=`TC`.

## `fam.tab`

Tab-delimited file used internally by AMRFinderPlus to define the hierarchical
gene/protein family structure behind the AMRFinderPlus database.

Fields are separated by tab characters and columns are as follows:

1. __node_id__: The ID of the family described by this line. This is an
   internal ID used by AMRFinderPlus, and exposed in ReferenceGeneHierarchy.txt
   and the Reference Gene Catalog. Note that it is not a gene symbol or
   accession and is subject to change.
2. __parent_node_id__: Except for the root for all rows this field should have
   another line for for the parent where the node\_id matches this field. The
   root has an empty value.
3. __Gene symbol__: The symbol to be reported for hits at this level
4. __HMM identifier__: The internal identifier (NAME) used to identify the HMM
   (if any) that is used at this level in the hierarchy
5. __HMM trusted cutoff 1__: Trusted cutoff for full_score (minmum match for
   whole HMM, both cutoffs must be met for a hit)
6. __HMM trusted cutoff 2__: Trusted cutoff for domain_score (minimum match for
   a region, both cutoffs must be met for a hit)
7. __Blast rule cutoff: Complete target identity__: This is overridden by the
   `--ident_min` option. "0.00" means default behavior (90%, though there will
   usually be a curated HMM in those cases). Note that this cutoff is used when
   \>= 90% of the reference protein is covered by the hit
8. __Blast rule cutoff: Complete target coverage__
9. __Blast rule cutoff: Complete reference coverage__
10. __Blast rule cutoff: Partial identity__ This is overridden by the
`--ident_min` option. "0.00" means default behavior (90%). Note that this rule
is used when <90% of the reference protein is covered by the hit
11. __Blast rule cutoff: Partial target coverage__
12. __Blast rule cutoff:  Partial reference coverage__
13. __Reportable 0/1/2__: Whether a hit at this level will be reported as an
AMRFinderPlus hit (0 = do not report, 1 = 'plus' gene/family, 2 = AMRFinder
'core' gene/family
14. __Family name__: The gene name to be reported for hits at this level

Note that the only Blast rule cutoffs used by AMRFinderPlus are fields 7
(Complete target identity) and 10 (Partial identity).

## `AMRProt-mutation.tab`

This contains point mutation information. The sequence of the reference allele is in AMRProt.

Fields are separated by tab characters and columns are as follows:

1. __taxgroup__: The "--organism" option that is associated with this mutation
2. __accession_version__: The accession of the reference protein
4. __mutation_position__: The offset position in the reference protein sequence
   for this mutation. This may differ from the position listed in the mutation
   symbol where the reference sequence doesn't match the traditional numbering
   system.
3. __mutation_symbol__: The "gene symbol" that will be printed when this
   mutation is found
4. __class__: The functional "class" for this mutation (see [[Class and
   subclass|Interpreting-results#class-and-subclass]]) for an explanation
5. __subclass__: The functional "subclass" for this mutation (see [[Class and
   subclass|Interpreting-results#class-and-subclass]]) for an explanation
6. __mutated_protein_name__: The contents of the "Sequence name" column when
   this mutation is found

## `AMR_DNA-<organism>.tab`

This file contains DNA point-mutation information similar to `AMRProt-mutation.tab`

Fields other than the first are separated by tab characters and columns are as follows:

1. __accession_version@gene_name:start-stop__: This is a compound '`:`'
   separated field that indicates the reference sequence location within the
   reference accession 
   - __accession_version__: Accession number of the reference sequence 
   - __gene_name__: The gene name of the reference sequence ('\_' is replaced
     by ' ') 
   - __start-stop__: The start and stop coordinates of the reference sequence
     on the accession provided
2. __mutation_position__: The position of the mutation in the reference
sequence. This may differ from the position listed in the mutation symbol where
the reference sequence doesn't match the traditional numbering system.
3. __mutation_symbol__: The "gene symbol" that will be printed when this
mutation is found
4. __class__: The functional "class" for this mutation (see [[Class and
subclass|Interpreting-results#class-and-subclass]] for an explanation)
5. __subclass__: The functional "subclass" for this mutation (see [[Class and
subclass|Interpreting-results#class-and-subclass]]) for an explanation
6. __mutated_gene_name__: The contents of the "Sequence name" column when this
mutation is found

## `AMRProt-susceptible.tab`

This file lists proteins that are reported only when a protein in the query
sequence aligns, but is more divergent than a cutoff. These proteins are only
reported for a single `--organism`. Used for Streptococcus pneumoniae pbp
variants

1. __taxgroup__: `--organism` to report these genes
2. __gene_symbol__: Gene symbol to report
3. __accession_version__: Accession of the reference protein
4. __resistance_cutoff__: Only report genes more divergent then this cutoff
5. __class__: The functional "class" for this gene when reported (see [[Class
   and subclass|Interpreting-results#class-and-subclass]] for an explanation)
6. __subclass__: The functional "subclass" for this gene when reported (see
   [[Class and subclass|Interpreting-results#class-and-subclass]] for an
   explanation)
7. __resistance_protein_name__: Element name for this protein when reported

## `AMRProt-suppress`

This file lists organisms and genes to not report because for a given
`--organism` because they do not provide additional information for that
organism (e.g., they are nearly universal or don't confer a phenotype in that
organism)

1. __taxgroup__: `--organism` to report these genes
2. __protein_accession__: Accession of the reference protein
3. __protein_gi__: gi of the reference protein

## `taxgroup.tab`

This file contains a mapping of "taxgroups" used by NCBI Pathogen Detection
(see https://www.ncbi.nlm.nih.gov/pathogens) to `--organism` option values, and
is used when the `--gpipe_org` option is included on the commandline. This is
primarily intended for internal use.

Fields are separated by tab characters and columns are as follows:

1. __taxgroup__: The "--organism" option value
2. __gpipe_taxgroup__: The NCBI Pathogen Detection taxonomic group
3. __number_of_nucl_ref_genes__: A count of the number of nucleotide references included for that taxgroup

# Database curation methods

## AMR Proteins

### Sources of AMR proteins and HMMs

Current curation of new genes, alleles, and point mutations is primarily from:

*   Literature review by curators
*   Submissions to NCBI for allele assignment
*   Collaboration with Gen-FS working groups
*   Comprehensive Antibiotic Resistance Database (CARD) collaboration
*   Direct communication / consultation with domain experts

NCBI also maintains a collaboration with [CARD](https://card.mcmaster.ca/) to
resolve issues and communicate updates and new genes.

NCBI continually mines the literature for new reports of AMR proteins.  The
ResFams collection of AMR HMMs provided important help early in our efforts to
develop the AMR protein hierarchy and the AMRFinderPlus tool, but all models
were rebuilt with new seed alignment sequences, new alignments, new cutoff
scores, and new biocuration. Development continued until all reportable AMR
proteins were covered by at least one AMR HMM, and classification by HMM was
sufficiently specific.

NCBI is also responsible for the assignment of new beta lactamase alleles for
certain families. Once new alleles are released, then they are immediately
incorporated into AMRFinderPlus. See this page for more information:

https://www.ncbi.nlm.nih.gov/pathogens/submit-beta-lactamase/

"Plus" proteins were covered based on requests from our collaborators at public
health agencies, and most are identified using curated blast cutoffs. Many of
the "plus" proteins come from classes of genes that are less intensely studied,
and so you may be less confident of their impact on phenotype.

To build the AMRFinderPlus HMM and Protein collection, NCBI first assembled a comprehensive
collection of acquired (and intrinsic) anti-microbial resistance proteins.
Sources, published or collaborative, including

*    the Lahey Clinic compilation of beta-lactamase sequences
     (http://www.lahey.org/studies/ and personal communications from Dr. George
     Jacoby and Karen Bush)
*    the Pasteur Institute collection of beta-lactamase sequences
*    ResFinder
*    Comprehensive Antibiotic Resistance Database (CARD)
*    the RAC and Integrall collections of AMR proteins found in integrons
*    the Center for Veterinary Medicine
*    Marilyn Roberts personal communications
*    "Oxford" - Derrick Crook personal communications

Inputs from all of the above resources were then manually curated, and
assembled into a hierarchical database with manually created and curated HMMs,
blast rules, gene names, and gene symbols.

### Types of proteins covered

#### Core AMR collection

This collection covers AMR resistance proteins only if they confer resistance
in bacteria. Alleles for AMR proteins are included in the data set only if they
are naturally-occurring. The antibiotic affected by the AMR protein does not
need to be used clinically as an antibiotic in human patients. Our collection
includes proteins that contribute to resistance quaternary ammonium compounds
(which are not antibiotics, strictly speaking) and to antibiotics whose use is
restricted to agricultural or veterinary applications, e.g. olaquindox. Source
databases we drew from (e.g. CARD) contained a large number of intrinsic
proteins that contribute weakly to resistance (loss-of-function mutants show
increased susceptibility), and that may contribute more strongly after
mutational events increase their expression. We avoided including most such
proteins in the release, since flagging such proteins makes the reports on AMR
proteins identified far more difficult to read and understand.

##### Point mutations

As part of the 'core' database, known resistance-associated point
mutations are included for some organisms, but others remain
not-covered. See [[--organism option docuemntation|Running-AMRFinderPlus#--organism-option]]
for more which organism groups are covered. Files provided will not help much
in finding sources of resistance in _Mycobacterium tuberculosis_, where nearly
all the recent increase in antimicrobial resistance is attributable to
mutational changes that are not included in the database. We are actively
curating and increasing coverage of point-mutations.    

#### "Plus" proteins

At the request of our collaborators we have added an expanded set of genes that
are of interest in pathogens. This set includes stress response (biocide,
metal, and heat resistance), virulence factors, some antigens, and porins. These
"plus" proteins have been primarily been added to the database with curated
blast cutoffs, and are generally identified by blast searches. Some of these
may not be acquired genes or mutations, but may be intrinsic in some organisms.

### Protein sequence curation

As we collected AMR proteins from various sources, we examined them in multiple
sequence alignments to determine whether we judged any to have structural
problems, including truncations, frameshifts, or incorrect start sites. For
aminoglycoside-modifying enzymes found in integrons, in particular, the most
appropriate start site to choose often is unclear. We chose start sites such
that regions of homology across a family were preserved, but regions clearly
derived from the sites at which the genes integrated were removed. Note that
this is separate from the alignment trimming step below.


### HMMs

All HMMs used by AMRFinderPlus can be found in the library
[AMR.LIB](https://ftp.ncbi.nlm.nih.gov/pathogen/Antimicrobial_resistance/AMRFinderPlus/data/latest/AMR.LIB)
and with seed alignments and some additional data on our [NCBIfam ftp
site](https://ftp.ncbi.nlm.nih.gov/hmm/NCBIfam-AMRFinder/latest). This library
is a special subset from a larger collection of protein profile HMMs being
constructed at NCBI, and referred to on the whole as
[NCBIfams](https://www.ncbi.nlm.nih.gov/Structure/cdd/cdd_help.shtml#CDSource_NCBIfams).
NCBIfams is a collection of protein family hidden Markov models (HMMs) for
improving bacterial genome annotation. A paper by [Haft et al.
2018](https://www.ncbi.nlm.nih.gov/pubmed/29112715) provides additional
information about NCBIfams, which is part of NCBI's Reference Sequence
([RefSeq](https://www.ncbi.nlm.nih.gov/refseq/)) project.

The AMRFinderPlus HMM library consists of a subset of models designed for detecting
and classifying candidate AMR proteins. Each model was built from a manually
reviewed and trimmed multiple sequence alignment, called the seed alignment.
Regions of sequence that appeared extraneous, as from incorrect prediction of
an upstream start site, were removed by trimming. Sequences that appeared
non-representative, such as those with frameshift mutations or truncations,
were removed.  Models were built using the HMMER3 package (http://hmmer.org;
[Eddy, 2011](https://www.ncbi.nlm.nih.gov/pubmed/22039361)). For some HMMs, the
seed alignment may consist of a single sequence.

Three types of cutoffs are provided, for compatibility: trusted_cutoff (TC),
gathering threshold (GA), and noise_cutoff (NC).  In this library, TC and
GA are always set to be identical. If NC is set to be the same as TC,
this indicates that the noise cutoff has not yet been manually reviewed.
Because all three types of cutoff are provided, searches may be performed
using any of the HMMER package switches, `--cut_tc`, `--cut_ga`, or `--cut_nc`.
AMRFinderPlus uses `--cut_tc`.

## Gene/Protein Hierarchy

AMRFinderPlus treats all families and alleles of AMR proteins as nodes in
hierarchical tree.  Below is an example of the parent-child relationships for
a beta-lactamase allele and the broader families that contain it.

    bla - beta-lactamase
      bla-A - class A beta-lactamase
        bla-A_carba - carbapenem-hydrolyzing class A beta-lactamase
          blaKPC - KPC family carbapenem-hydrolyzing class A beta-lactamase
            blaKPC-2 - carbapenem-hydrolyzing class A beta-lactamase KPC-2 (allele)

Evidence used to annotate proteins relies on HMM and BLAST.  AMRFinderPlus will
assign the most specific name it can find that is justified by the evidence.
In the example shown above, the bottom level represents an allele. Assigning an
AMR protein to a specific allele requires a 100% identity BLAST match.  Each
higher level node may have an associated HMM, with defined cutoff scores that
make assignment to that family deterministic.  Not every (non-allele) node has
its own HMM, but every AMR protein is covered by at least one HMM somewhere
above it in the hierarchy.

Note that some nodes in the hierarchy group together child families that are
not necessarily related by homology.  Such nodes will always lack an HMM.
For example, the family bla ("beta-lactamase") contains child families bla-A
("class A beta-lactamase") and "metallo-beta-lactamase", which are similar in
function but lack any sequence homology to each other.


