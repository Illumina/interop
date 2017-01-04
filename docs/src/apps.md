# Applications                          {#apps}

The InterOp package includes several command line tools to extraction information from InterOp files.

| Content                                 | Description                                                                |
| --------------------------------------- | ---------------------------------------------------------------------------|
| @subpage dumptext "dumptext"            | Convert binary InterOp data into a CSV formatted txt file                  |
| @subpage imaging_table "imaging_table"  | Generate the SAV Imaging Tab table as a CSV text file                      |
| @subpage summary_table "summary"        | Generate the SAV Summary Tab summary table as a text file                  |
| @subpage plot_by_cycle "plot_by_cycle"  | Generate the SAV Analysis Tab By Cycle Plot as a GNUPlot text file         |
| @subpage plot_by_lane "plot_by_lane"    | Generate the SAV Analysis Tab By Lane Plot as a GNUPlot text file          |
| @subpage plot_flowcell "plot_flowcell"  | Generate the SAV Analysis Tab Flowcell heat map as a GNUPlot text file     |
| @subpage q_hist "plot_qscore_histogram" | Generate the SAV Analysis Tab Q-score histogram as a GNUPlot text file     |
| @subpage q_hmap "plot_qscore_heatmap"   | Generate the SAV Analysis Tab Q-score heat map as a GNUPlot text file      |
| @subpage plot_sampleqc "plot_sample_qc" | Generate the SAV Indexing Tab index graph as a GNUPlot text file           |
| @subpage index_summary "index-summary"  | Generate the SAV Indexing Tab summary table as a CSV text file             |
| @subpage cyclesim "cyclesim"            | Simulate the InterOps of a run folder at a specific cycle                  |
| @subpage dumpbin "dumpbin"              | Developer app to help create unit tests by dumping the binary format       |

Note: interop2csv has been deprecated in favor of dumptext
