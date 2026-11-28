LAB 05 - UART Communication and ADC Data Transfer
Student: Nguyen Cap Quoc Khanh - 2452500 - CC03

Upload the entire ZIP to Overleaf.
Main document: NguyenCapQuocKhanh_2452500_CC03_Lab05.tex
Compiler: pdfLaTeX. Compile twice for contents and cross-references.
Keep all PNG and PDF assets beside the main document.

lab5_schematic.png: supplied lab5_1.png
lab5_uart_results.png: supplied lab5_2.png
lab5_parser_fsm.pdf: vector parser diagram
lab5_communication_fsm.pdf: vector communication diagram
lab5_dataflow.pdf: vector module/data-flow diagram
hcmut_logo.png: university logo from the established report template

The firmware has not been modified. Code listings are selected excerpts
without source comments; the loop-back loop is an isolated optional test.
The supplied images show frame format and the 1.65-V / ADC=2048 result.
Additional timed retry/ACK traces, a changed-voltage transaction, and a
dedicated loop-back capture would complete the experimental evidence.

Updated diagrams: all three vector diagrams were redrawn with TikZ.
Editable TikZ sources are included in diagrams/.
The main report uses the supplied diagram PDFs directly; recompiling the
diagrams is optional. To edit one, compile its standalone TeX source and
copy the resulting PDF to the project root with the same name.
