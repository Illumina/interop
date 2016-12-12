InterOp Format      {#binary_formats}
==============

This page describes the binary format for an InterOp file. The formats are broken down into two parts: (1) the
header, which is written first, a single time for the file and (2) n-Records, which are written next, many times
composing the rest of the file.

An InterOp file may have several versions of the format. This is important because certain values may not be
populated in the model. For example, for the CorrectedInt InterOp file, the signal to noise attribute is only
populated for version 2 of the format. This parser does not support all versions of the InterOp files, namely
versions that were supported by the GA platform.

The documentation for the model notes when an attribute is only populated by a specific version of the format.

 - @subpage corrected_v2 "Corrected Intensity v2"
 - @subpage corrected_v3 "Corrected Intensity v3"
 - @subpage error_v3 "Error v3"
 - @subpage extraction_v2 "Extraction Version 2"
 - @subpage image_v1 "Image Version 1"
 - @subpage image_v2 "Image Version 2"
 - @subpage index_v1 "Index Version 1"
 - @subpage q_v4 "Q-Metrics Version 4"
 - @subpage q_v5 "Q-Metrics Version 5"
 - @subpage q_v6 "Q-Metrics Version 6"
 - @subpage tile_v2 "Tile Version 2"
 - @subpage q_collapsed_v2 "Collapsed Q-Metrics Version 2"
 - @subpage q_collapsed_v3 "Collapsed Q-Metrics Version 3"
 - @subpage q_collapsed_v4 "Collapsed Q-Metrics Version 4"
 - @subpage q_collapsed_v5 "Collapsed Q-Metrics Version 5"
 - @subpage q_collapsed_v6 "Collapsed Q-Metrics Version 6"
 
 The following are binary formats used only for testing purposes and are not officially supported:
 
 - @subpage index_summary_v1 "Index Summary v1"
 - @subpage summary_v1 "Run Summary v1"
 
