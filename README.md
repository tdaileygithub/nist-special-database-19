# Overview

# nist-special-database-19

NIST Special Database 19

- 814,255 characters in the data set
- Source characters are 128x128

[MNIST](https://en.wikipedia.org/wiki/MNIST_database)

- 70,000 characters
- 60k test
- 10k train

<hr>

A 10X differnce in readily available data set size is worthy of a raw import for easier parsing in 2024.

<hr>

# Field Information

The HSFs were scanned at 11.8 dots per millimeter (300 dots per inch) and contain 34 fields: 

- [Field 0] name occluded or left incomplete on all HSF images
- [Field 1-2] city/state field. Have never been processed by NIST
- [Field 3-30] 28 digit fields 
- [Field 31] upper-case field
- [Field 32] lower-case field
- [Field 33] and an unconstrained Constitution text paragraph

# References

- [Cold Case: the Lost MNIST Digits](docs/NeurIPS-2019-cold-case-the-lost-mnist-digits-Paper.pdf)
- [EMNIST: an extension of MNIST to handwritten letters](docs/1702.05373v1.pdf)

## NIST Special Database 19 User Guides
 
- [NIST Special Database 19 Handprinted Forms and Characters Database 1995](docs/nistsd19.pdf)
- [NIST Special Database 19 Handprinted Forms and Characters 2nd Edition](docs/sd19_users_guide_edition_2.pdf)
- [NIST Special Database 19 Handprinted Forms and Characters Database 2016](docs/1stEditionUserGuide.pdf)

## CCIT Group 4 Encoding [aka Black+White Faxes & .MIS Files]

- [Visualizing CCITT Group 3 and Group 4 TIFF Documents](docs/ccott_g3_g4_tiff.pdf)
- [FACSIMILE CODING SCHEMES AND CODING CONTROL FUNCTIONS FOR GROUP 4 FACSIMILE APPARATUS](docs/T-REC-T.6-198811-I!!PDF-E.pdf)

## Neural Networks

## FANN

- [FANN manual](https://libfann.github.io/fann/docs/files/fann-h.html)
- [Implementation of a Fast Artificial Neural Network Library (fann)](docs/fann_doc_complete_1.0.pdf) Excellent 2003 NN paper with cpu considerations.  An early NN library before the industry exploded.
- [A C program which uses libfann to train a simple MLP on MNIST](https://github.com/tgflynn/mnist-fann)

# Libraries / Tools Used

## Databaes

- [Sqlite3](https://www.sqlite.org/download.html) - Single file c db

## Images

- [lodepng](https://github.com/lvandeve/lodepng) - Single file png decoder
- [toojpeg](https://create.stephan-brumme.com/toojpeg/) - Single file c++ file jpeg enocder 
- [sha256](https://github.com/System-Glitch/SHA256) - Single file c++ sha256 library
- [miniz](https://github.com/richgel999/miniz) - Single file c++ png with DEFLATE

## Testing

- [doctest](https://github.com/doctest/doctest) - Single file c++ test framework

## Utilities

- [pugixml](https://github.com/zeux/pugixml) - Two file c++ xml library
- [ulog](https://github.com/Carabasen/ulog) - Single file c++ logger
- [mongoose](https://github.com/cesanta/mongoose) - Single file c++ http server - embedded focus
- [Dr. Memory](https://drmemory.org/page_running.html) - Find Those malloc leaks!.  

## GUI

- [ImGui](https://github.com/ocornut/imgui) - Immediate Mode Gui - Cross Platform
  - [Implot](https://github.com/epezent/implot) - Chart library

## Neural Net

- [FANN](https://leenissen.dk/fann/wp/) - Fast Artifical Neural Network Library in C