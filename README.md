# Overview

# NIST Special Database 19 - History + Background Information

https://www.nist.gov/srd/nist-special-database-19

- First edition 1995
- Second edition 2016
- 814,255 processed characters in the data set
- Raw Source MIS Characters are 128x128 at 1bpp encoded with GROUP 4 FACSIMILE [Fax Machine]
  -	[see sd19/grp4decomp.cpp](sd19/grp4decomp.cpp)
- 62 character classes 
  - [a-z]
  - [A-Z]
  - [0-9]

Compare the original SD19 dataset to its children:

https://en.wikipedia.org/wiki/MNIST_database
- 60k train + 10k tesst

https://www.itl.nist.gov/iaui/vip/cs_links/EMNIST/Readme.txt
- 814,255 characters. 62 unbalanced classes.
- EMNIST Balanced:  131,600 characters. 47 balanced classes.
- Same IHEAD and MIS format - just another repackage

## Acronymns

- HSF - Handwriting Sample Form
- MIS - Multiple Isolated Character Images
- PCT - File with an IHEAD that contains one complete HSF form
- CLS - Manually checked list of characters prsent inside the MIS file

# Handwriting Sample Form [HSF] 

![HSF Form Example](assets/images/hsf.png)

**Example**: 
- Sample Form #8
- Writer # 4164
- Template #10

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
