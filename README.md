# nist-special-database-19

NIST Special Database 19 Handprinted Forms and Characters Database


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
- [sqlite_orm_](https://github.com/fnc12/sqlite_orm/tree/v1.8.2) - Single file c++ wrapper for sqlite3