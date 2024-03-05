# Overview

# NIST Special Database 19 - History + Background Information

https://www.nist.gov/srd/nist-special-database-19

- First edition 1995
- Second edition 2016
- 814,255 processed characters in the data set
- Raw Source MIS Characters are 128x128 at 1bpp encoded in 
- 62 character classes 
  - [a-z]
  - [A-Z]
  - [0-9]

## Acronymns

- HSF - Handwriting Sample Form
- MIS - Multiple Isolated Character Images
- PCT - File with an IHEAD that contains one complete HSF form
- CLS - Manually checked list of characters prsent inside the MIS file

# Handwriting Sample Form [HSF] 

![HSF Form Example](assets/images/hsf.png)

**Example**: Sample Form #8, Writer # 4164, Template #10, HSF Page SHA256 f259c09024d451a3e73146f49603bf0af122e649ab116c89f06d91241b51cad8

The HSFs were scanned at 11.8 dots per millimeter (300 dots per inch) and contain 34 fields: 

- [Field 0] name occluded or left incomplete on all HSF images
- [Field 1-2] city/state field. Have never been processed by NIST
- [Field 3-30] 28 digit fields 
- [Field 31] upper-case field
- [Field 32] lower-case field
- [Field 33] and an unconstrained Constitution text paragraph

## Processed Sqlite sd19.db3

- sd19.db3 - 988,839,936 bytes @ __128x128__ MIS characters and all HSF Pages
- sd19.db3 - 849,248,256 bytes @ __32x23 MIS__ characters scaled with norm_2nd_gen2\(\) and all HSF Pages
- 3,668 hsf_page rows [Same As the stated count from NIST]
- 814,255 mis rows [Same As the stated count from NIST]
- db creation around 2 minutes on my desktop - 8 minutes on my laptop
