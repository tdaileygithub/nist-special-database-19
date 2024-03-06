#pragma once
#ifndef GETNSET_H_
#define GETNSET_H_

/*
# proc: get_id - get the identity attribute from an IHead structure.
# proc:
# proc: set_id - set the identity attribute from an IHead structure.
# proc:
# proc: get_created - get the creation date attribute from an IHead structure.
# proc:
# proc: set_created - set the creation date attribute of an IHead structure.
# proc:
# proc: get_width - get the pixel width attribute from an IHead structure.
# proc:
# proc: set_width - set the pixel width attribute of an IHead structure.
# proc:
# proc: get_height - get the pixel height attribute from an IHead structure.
# proc:
# proc: set_height - set the pixel height attribute of an IHead structure.
# proc:
# proc: get_depth - get the pixel depth attribute from an IHead structure.
# proc:
# proc: set_depth - set the pixel depth attribute of an IHead structure.
# proc:
# proc: get_density - get the pixel density attribute from an IHead structure.
# proc:
# proc: set_density - set the pixel density attribute of an IHead structure.
# proc:
# proc: get_compression - get the compression code attribute from an
# proc:                   IHead structure.
# proc: set_compression - set the compression code  attribute of an
# proc:                   IHead structure.
# proc: get_complen - get the compressed data length attribute from
# proc:               an IHead structure.
# proc: set_complen - set the compressed data length attribute of
# proc:               an IHead structure.
# proc: get_align - get the scanline alignment attribute from an
# proc:             IHead structure.
# proc: set_align - set the scanline alignment attribute of an IHead structure.
# proc:
# proc: get_unitsize - get the memory unit size attribute from
# proc:                an IHead structure.
# proc: set_unitsize - set the memory unit size attribute of
# proc:                an IHead structure.
# proc: get_sigbit - get the bit order attribute from an IHead structure.
# proc:
# proc: set_sigbit - set the bit order attribute of an IHead structure.
# proc:
# proc: get_byte_order - get the byte order attribute from an IHead structure.
# proc:
# proc: set_byte_order - set the byte order attribute of an IHead structure.
# proc:
*/
/*
# proc: get_pix_offset - get the column pixel offset attribute from
# proc:                  an IHead structure.
# proc: set_pix_offset - set the column pixel offset attribute of
# proc:                  an IHead structure.
# proc: get_whitepix - get the white pixel attribute from an IHead structure.
# proc:
# proc: set_whitepix - set the white pixel attribute of an IHead structure.
# proc:
# proc: get_issigned - get the memory unit signed/unsigned attribute from
# proc:                an IHead structure.
# proc: set_issigned - set the memory unit signed/unsigned attribute of
# proc:                an IHead structure.
# proc: get_rm_cm - get the row/column major attribute from an IHead structure.
# proc:
# proc: set_rm_cm - set the row/column major attribute of an IHead structure.
# proc:
# proc: get_tb_bt - get the top-to-bottom/bottom-to-top attribute from
# proc:             an IHead structure.
# proc: set_tb_bt - set the top-to-bottom/bottom-to-top attribute of
# proc:             an IHead structure.
# proc: get_lr_rl - get the right-to-left/left-to-right attribute from
# proc:             an IHead structure.
# proc: set_lr_rl - set the right-to-left/left-to-right attribute of
# proc:             an IHead structure.
# proc: get_parent - get the parent image attribute from an IHead structure.
# proc:
# proc: set_parent - set the parent image attribute of an IHead structure.
# proc:
# proc: get_par_x - get the x-coordinate cut from parent attribute from
# proc:             an IHead structure.
# proc: set_par_x - set the x-coordinate cut from parent attribute of
# proc:             an IHead structure.
# proc: get_par_y - get the y-coordinate cut from parent attribute from
# proc:             an IHead structure.
# proc: set_par_y - set the y-coordinate cut from parent attribute of
# proc:             an IHead structure.
*/

#include <stdio.h>
#include <string.h>
#include "ihead.h"
#include "defs.h"

char* get_id(IHEAD* head);

void set_id(IHEAD* i, char* path);

char* get_created(IHEAD* head);

void set_created(IHEAD* head);

int get_width(IHEAD* head);

int set_width(IHEAD* head, int width);

int get_height(IHEAD* head);

int set_height(IHEAD* head, int height);

int get_depth(IHEAD* head);

int set_depth(IHEAD* head, int depth);

int get_density(IHEAD* head);

int set_density(IHEAD* head, int density);

int get_compression(IHEAD* head);

int set_compression(IHEAD* head, int compression_code);

int get_complen(IHEAD* head);

int set_complen(IHEAD* head, int complen);

int get_align(IHEAD* head);

int set_align(IHEAD* head, int align);

int get_unitsize(IHEAD* head);

int set_unitsize(IHEAD* head, int unitsize);

int get_sigbit(IHEAD* head);

int set_sigbit(IHEAD* head, int sigbit);

int get_byte_order(IHEAD* head);

int set_byte_order(IHEAD* head, int byte_order);

int get_pix_offset(IHEAD* head);

int set_pix_offset(IHEAD* head, int pix_offset);

int get_whitepix(IHEAD* head);

int set_whitepix(IHEAD* head, int whitepix);

int get_issigned(IHEAD* head);

int set_issigned(IHEAD*, int issigned);

int get_rm_cm(IHEAD* head);

int set_rm_cm(IHEAD* head, int rm_cm);

int get_tb_bt(IHEAD* head);

int set_tb_bt(IHEAD* head, int tb_bt);

int get_lr_rl(IHEAD* head);

int set_lr_rl(IHEAD* head, int lr_rl);

char* get_parent(IHEAD* head);

void set_parent(IHEAD* i, char* path);

int get_par_x(IHEAD* head);;

int set_par_x(IHEAD* head, int par_x);

int get_par_y(IHEAD* head);;

int set_par_y(IHEAD* head, int par_y);
#endif