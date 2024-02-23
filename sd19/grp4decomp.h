#pragma once

/*********************************************************************/
/* grp4decomp.h                                                      */
/* Originally decompression.h                                        */
/*********************************************************************/
#include <stdio.h>

#define True					1 
#define False					0 
#define Debug					False 

#define White					0
#define Black 					1
#define Black_byte				255 /* byte of all black bits: 11111111b */

#define Max_terminating_length	63		/* longest terminating code*/

#define Pixels_per_byte			8  
#define Bits_per_byte			8  
#define Last_bit_in_a_byte		7		/* assumes bits numbered from 0 - 7 */	
#define Last_bit_mask 			1		/* masks the last (low magnitude) bit */ 
#define Default_width_in_pixels	1728 	/* default width of a scan line */
#define Default_number_of_lines	2200 	/* default length of an image */

#define Invalid				   -1 
#define None					0 
#define Extra_positions			25		/* ensures extra room in allocations */
#define Not_done_yet			0

#define VL3 				   -3  		/* Vertical Left 3 mode */
#define VL2 				   -2  		/* Vertical Left 2 mode */
#define VL1 				   -1 		/* Vertical Left 1 mode */
#define V0						0		/* Vertical mode */	  
#define VR1						1 		/* Vertical Right 1 mode */  
#define VR2						2  		/* Vertical Right 2 mode */ 
#define VR3						3  		/* Vertical Right 3 mode */ 
#define P						4 		/* Pass mode */ 
#define H						5		/* Horizontal mode */
#define EOFB					6		/* End Of File Buffer */

#define No_offset				0		/* no offset during fseek() */
#define End_of_file				2		/* start at EOF during fseek() */
#define Start_of_file			0		/* start at BOF during fseek() */

//char* calloc();
//short* malloc();

struct parameters {
	short 	index;			/* indicates current position in "coding_line" */
	short	max_pixel;		/* the number of pixels in a scan line */
	short* reference_line;/* array of changing elements on reference line */
	short* coding_line;	/* array of changing elements on coding line */
};

struct decompressed_descriptor {
	char* data;				/* pointer to decompressed image */
	short  pixels_per_line;		/* the number of pixels in a scan line */
	short  number_of_lines;		/* the number of scan lines in the image */
};

struct compressed_descriptor {
	char* data;				/* pointer to compressed image */
	short  pixels_per_line;		/* the number of pixels in a scan line */
	short  number_of_lines;		/* the number of scan lines in the image */
	int    length_in_bytes;		/* length of the compressed image in bytes */
};

void vertical_mode_d(struct parameters* params, short offset);
void grp4decomp(unsigned char* indata, int inbytes, int width, int height, unsigned char* outdata, int* outbytes);
void control_decompression(struct compressed_descriptor* compressed, struct decompressed_descriptor* decompressed);
void read_compressed_file_into_memory(struct compressed_descriptor* compressed);
void prepare_to_decompress(struct compressed_descriptor* compressed, struct decompressed_descriptor* decompressed, struct parameters* params);
void set_up_first_line_d(struct parameters* params);
void set_up_first_and_last_changing_elements_d(struct parameters* params);
void prepare_to_decompress_next_line(struct parameters* params);
void swap_the_reference_and_coding_lines(struct parameters* params);
void crash_d();
short decompress_line(struct parameters* params);
short get_mode();
void pass_mode_d(struct parameters* params);
void horizontal_mode_d(struct parameters* params);
void prepare_to_write_bits_d(char* output_pointer, short bytes_per_line);
void write_bits_d(unsigned short length, unsigned short color);
void prepare_to_read_bits(char* input_pointer);
short read_bit();
short find_run_length_code(short unsigned color);