Usage:

./merge_graphs number_of_graphs_N graph1 [ graph2 ... graphN ] graphml_file score_name1 [ score_name2 ... score_nameN ]


Specifically for the DREAM networks:

./merge_graphs 6 1_ppi_anonym_aligned_v2.txt 
                 2_ppi_anonym_aligned_v2.txt 
                 3_signal_anonym_aligned_directed_v3.txt
                 4_coexpr_anonym_aligned_v2.txt 
                 5_cancer_anonym_aligned_v2.txt
                 6_homology_anonym_aligned_v2.txt 
                 dream_merge.graphml 
                 ppi1_score ppi2_score signal_score coexpr_score cancer_score homology_score


(ingnores direction of signaling network)


You may want to install the lemon library:

https://lemon.cs.elte.hu/trac/lemon

Makefile should work on Linux once lemon is installed. Edit LEMON_HOME as necessary.
