# nist-special-database-19

NIST Special Database 19 Handprinted Forms and Characters Database

| category      | number files  |
| ------------- | ------------- |
| by_write      | 24318         |
| hsf_page      | 3669          |
| by_field      | 1586          |
| by_class      | 1426          |

# Field Information

The HSFs were scanned at 11.8 dots per millimeter (300 dots per inch) and contain 34 fields: 

- [Field 0] name occluded or left incomplete on all HSF images
- [Field 1-2] city/state field. Have never been processed by NIST
- [Field 3-30] 28 digit fields 
- [Field 31] upper-case field
- [Field 32] lower-case field
- [Field 33] and an unconstrained Constitution text paragraph

# Dr. Memory

https://drmemory.org/page_running.html

Excellent tool for memory leak detection.  Install it and update compile flags.

## MSVC Compile Flags

https://learn.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-170

```
/Zi /MT /EHsc /Oy- /Ob0
```

```cmd
"C:\Program Files (x86)\Dr. Memory\bin\drmemory.exe" -visual_studio -- C:\Users\tim\Documents\nist-special-database-19\out\build\x64-debug\sd19test.exe
```

# References

- [Cold Case: the Lost MNIST Digits](docs/NeurIPS-2019-cold-case-the-lost-mnist-digits-Paper.pdf)
- [EMNIST: an extension of MNIST to handwritten letters](docs/1702.05373v1.pdf)

## NIST User Guides
 
- [NIST Special Database 19 Handprinted Forms and Characters 2nd Edition](docs/sd19_users_guide_edition_2.pdf)
- [NIST Special Database 19 Handprinted Forms and Characters Database 2016](docs/1stEditionUserGuide.pdf)

## CCIT Group 4 Encoding [Black+White Faxs + .MIS Files]

- [Visualizing CCITT Group 3 and Group 4 TIFF Documents](docs/ccott_g3_g4_tiff.pdf)
- [FACSIMILE CODING SCHEMES AND CODING CONTROL FUNCTIONS FOR GROUP 4 FACSIMILE APPARATUS](docs/T-REC-T.6-198811-I!!PDF-E.pdf)

# Libraries + Tools Used

- [Sqlite3](https://www.sqlite.org/download.html) - Single file c++ db
- [toojpeg](https://create.stephan-brumme.com/toojpeg/) - Single file c++ file jpeg enocder 
- [doctest](https://github.com/doctest/doctest) - Single file c++ test framework
- [pugixml](https://github.com/zeux/pugixml) - Two file c++ xml library
- [miniz](https://github.com/richgel999/miniz) - Single file c++ zlib + png
- [sha256](https://github.com/System-Glitch/SHA256) - Single file c++ sha256 library
- [ulog](https://github.com/Carabasen/ulog) - Single file c++ logger