InterOp Format      {#binary_formats}
==============

This page describes the binary format for an InterOp file. The formats are broken down into two parts: (1) the
header, which is written first, a single time for the file and (2) n-Records, which are written next, many times
composing the rest of the file.

An InterOp file may have several versions of the format. This is important because certain values may not be
populated in the model. For example, for the CorrectedInt InterOp file, the signal to noise attribute is only
populated for version 2 of the format. This parser does not support all versions of the InterOp files, namely
versions that were supported by the GA platform.

In 2017, we introduced the concept of "by-cycle" InterOp files, where every InterOp has a separate file for
every cycle of data for which the file is produced. Each file has its own header and set of n-Records, but
is otherwise similar to before.

The documentation for the model notes when an attribute is only populated by a specific version of the format.

 - @subpage corrected_v2 "Corrected Intensity v2"
 - @subpage corrected_v3 "Corrected Intensity v3"
 - @subpage corrected_v4 "Corrected Intensity v4"
 - @subpage error_v3 "Error v3"
 - @subpage error_v4 "Error v4"
 - @subpage error_v5 "Error v5"
 - @subpage extended_tile_v2 "Extended Tile Version 2"
 - @subpage extended_tile_v3 "Extended Tile Version 3"
 - @subpage extraction_v2 "Extraction Version 2"
 - @subpage extraction_v3 "Extraction Version 3"
 - @subpage image_v1 "Image Version 1"
 - @subpage image_v2 "Image Version 2"
 - @subpage image_v3 "Image Version 3"
 - @subpage index_v1 "Index Version 1"
 - @subpage index_v2 "Index Version 2"
 - @subpage phasing_v1 "Phasing Version 1"
 - @subpage phasing_v2 "Phasing Version 2"
 - @subpage q_v4 "Q-Metrics Version 4"
 - @subpage q_v5 "Q-Metrics Version 5"
 - @subpage q_v6 "Q-Metrics Version 6"
 - @subpage q_v7 "Q-Metrics Version 7"
 - @subpage tile_v2 "Tile Version 2"
 - @subpage tile_v3 "Tile Version 3"
 - @subpage q_collapsed_v2 "Collapsed Q-Metrics Version 2 (BaseSpace)"
 - @subpage q_collapsed_v3 "Collapsed Q-Metrics Version 3 (BaseSpace)"
 - @subpage q_collapsed_v4 "Collapsed Q-Metrics Version 4 (BaseSpace)"
 - @subpage q_collapsed_v5 "Collapsed Q-Metrics Version 5 (BaseSpace)"
 - @subpage q_collapsed_v6 "Collapsed Q-Metrics Version 6 (BaseSpace)"
 - @subpage summary_run_v1 "Summary Run Version 1"
 
 The following are binary formats used only for testing purposes and are not officially supported:
 
 - @subpage index_summary_v1 "Index Summary v1"
 - @subpage summary_v1 "Run Summary v1"
 
