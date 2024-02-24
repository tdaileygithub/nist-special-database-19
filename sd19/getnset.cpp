
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <string.h>

#include "defs.h"
#include "fatalerr.h"
#include "getnset.h"
#include "ihead.h"
#include "syserr.h"
#include "validcmp.h"

char* get_id(IHEAD* head)
{
	if (head == NULL)
		fatalerr("get_id", "pointer head is NULL", NULL);
	return(strdup(head->id));
}

void set_id(IHEAD* i, char* path)
{
	if (i == (IHEAD*)NULL)
		fatalerr("set_id", "null ihead pointer", (char*)NULL);
	if (path == (char*)NULL)
		fatalerr("set_id", "null filename pointer", (char*)NULL);

	while ((path != (char*)NULL) && (strlen(path) >= sizeof((*i).id)))
		path = strchr(path + 1, '/');

	memset(i->id, NULL, sizeof((*i).id));
	if (path != (char*)NULL)
		(void)strcpy(i->id, path);
}

char* get_created(IHEAD* head)
{
	if (head == NULL)
		fatalerr("get_created", "pointer head is NULL", NULL);
	return(strdup(head->created));
}

void set_created(IHEAD* head)
{
	char* current_time();
	char* cur_time;

	if (head == NULL)
		fatalerr("set_created", "pointer head is NULL", NULL);
	if ((cur_time = current_time()) == NULL)
		memset(head->created, NULL, DATELEN);
	else
		strcpy(head->created, cur_time);
}

int get_width(IHEAD* head)
{
	int width;

	if (head == NULL)
		fatalerr("get_width", "pointer head is NULL", NULL);
	sscanf(head->width, "%d", &width);
	return(width);
}

int set_width(IHEAD* head, int width)
{
	if (head == NULL)
		fatalerr("set_width", "pointer head is NULL", NULL);
	memset(head->width, NULL, SHORT_CHARS);
	sprintf(head->width, "%d", width);
	return(True);
}

int get_height(IHEAD* head)
{
	int height;

	if (head == NULL)
		fatalerr("get_height", "pointer head is NULL", NULL);
	sscanf(head->height, "%d", &height);
	return(height);
}

int set_height(IHEAD* head, int height)
{
	if (head == NULL)
		fatalerr("set_height", "pointer head is NULL", NULL);
	memset(head->height, NULL, SHORT_CHARS);
	sprintf(head->height, "%d", height);
	return(True);
}

int get_depth(IHEAD* head)
{
	int depth;

	if (head == NULL)
		fatalerr("get_depth", "pointer head is NULL", NULL);
	sscanf(head->depth, "%d", &depth);
	return(depth);
}

int set_depth(IHEAD* head, int depth)
{
	if (head == NULL)
		fatalerr("set_depth", "pointer head is NULL", NULL);
	memset(head->depth, NULL, SHORT_CHARS);
	sprintf(head->depth, "%d", depth);
	return(True);
}

int get_density(IHEAD* head)
{
	int density;

	if (head == NULL)
		fatalerr("get_density", "pointer head is NULL", NULL);
	sscanf(head->density, "%d", &density);
	return(density);
}

int set_density(IHEAD* head, int density)
{
	if (head == NULL)
		fatalerr("set_density", "pointer head is NULL", NULL);
	memset(head->density, NULL, SHORT_CHARS);
	sprintf(head->density, "%d", density);
	return(True);
}

int get_compression(IHEAD* head)
{
	int compression_code;

	if (head == NULL)
		fatalerr("get_compression", "pointer head is NULL", NULL);
	sscanf(head->compress, "%d", &compression_code);
	return(compression_code);
}

int set_compression(IHEAD* head, int compression_code)
{
	if (head == NULL)
		fatalerr("set_compression", "pointer head is NULL", NULL);
	if (valid_compression(compression_code)) {
		memset(head->compress, NULL, SHORT_CHARS);
		sprintf(head->compress, "%d", compression_code);
	}
	else
		return(False);
	return(True);
}

int get_complen(IHEAD* head)
{
	int complen;

	if (head == NULL)
		fatalerr("get_complen", "pointer head is NULL", NULL);
	sscanf(head->complen, "%d", &complen);
	return(complen);
}

int set_complen(IHEAD* head, int complen)
{
	if (head == NULL)
		fatalerr("set_complen", "pointer head is NULL", NULL);
	memset(head->complen, NULL, SHORT_CHARS);
	sprintf(head->complen, "%d", complen);
	return(True);
}

int get_align(IHEAD* head)
{
	int align;

	if (head == NULL)
		fatalerr("get_align", "pointer head is NULL", NULL);
	sscanf(head->align, "%d", &align);
	return(align);
}

int set_align(IHEAD* head, int align)
{
	if (head == NULL)
		fatalerr("set_align", "pointer head is NULL", NULL);
	memset(head->align, NULL, SHORT_CHARS);
	sprintf(head->align, "%d", align);
	return(True);
}

int get_unitsize(IHEAD* head)
{
	int unitsize;

	if (head == NULL)
		fatalerr("get_unitsize", "pointer head is NULL", NULL);
	sscanf(head->unitsize, "%d", &unitsize);
	return(unitsize);
}

int set_unitsize(IHEAD* head, int unitsize)
{
	if (head == NULL)
		fatalerr("set_unitsize", "pointer head is NULL", NULL);
	memset(head->unitsize, NULL, SHORT_CHARS);
	sprintf(head->unitsize, "%d", unitsize);
	return(True);
}

int get_sigbit(IHEAD* head)
{
	int sigbit;

	if (head == NULL)
		fatalerr("get_sigbit", "pointer head is NULL", NULL);
	sigbit = atoi(&(head->sigbit));
	return(sigbit);
}

int set_sigbit(IHEAD* head, int sigbit)
{
	if (head == NULL)
		fatalerr("set_sigbit", "pointer head is NULL", NULL);

	if ((sigbit == 0) || (sigbit == '0'))
		head->sigbit = '0';
	else if ((sigbit == 1) || (sigbit == '1'))
		head->sigbit = '1';
	else
		fatalerr("set_sigbit", "Sigbit must equal 1 (MSB Last) or 0 (MSB First).", NULL);

	return(True);
}

int get_byte_order(IHEAD* head)
{
	int byte_order;

	if (head == NULL)
		fatalerr("get_byte_order", "pointer head is NULL", NULL);
	byte_order = atoi(&(head->byte_order));
	return(byte_order);
}

int set_byte_order(IHEAD* head, int byte_order)
{
	if (head == NULL)
		fatalerr("set_byte_order", "pointer head is NULL", NULL);
	head->byte_order = (char)byte_order;

	if ((byte_order == 0) || (byte_order == '0'))
		head->byte_order = '0';
	else if ((byte_order == 1) || (byte_order == '1'))
		head->byte_order = '1';
	else
		fatalerr("set_byte_order", "Byte_order must equal 1 (Low-High) or 0 (High-Low).", NULL);

	return(True);
}

int get_pix_offset(IHEAD* head)
{
	int pix_offset;

	if (head == NULL)
		fatalerr("get_pix_offset", "pointer head is NULL", NULL);
	sscanf(head->pix_offset, "%d", &pix_offset);
	return(pix_offset);
}

int set_pix_offset(IHEAD* head, int pix_offset)
{
	if (head == NULL)
		fatalerr("set_pix_offset", "pointer head is NULL", NULL);
	memset(head->pix_offset, NULL, SHORT_CHARS);
	sprintf(head->pix_offset, "%d", pix_offset);
	return(True);
}

int get_whitepix(IHEAD* head)
{
	int whitepix;

	if (head == NULL)
		fatalerr("get_whitepix", "pointer head is NULL", NULL);
	sscanf(head->whitepix, "%d", &whitepix);
	return(whitepix);
}

int set_whitepix(IHEAD* head, int whitepix)
{
	if (head == NULL)
		fatalerr("set_whitepix", "pointer head is NULL", NULL);
	memset(head->whitepix, NULL, SHORT_CHARS);
	sprintf(head->whitepix, "%d", whitepix);
	return(True);
}

int get_issigned(IHEAD* head)
{
	int issigned;

	if (head == NULL)
		fatalerr("get_issigned", "pointer head is NULL", NULL);
	issigned = atoi(&(head->issigned));
	return(issigned);
}

int set_issigned(IHEAD* head, int issigned)
{
	if (head == NULL)
		fatalerr("set_issigned", "pointer head is NULL", NULL);

	if ((issigned == 0) || (issigned == '0'))
		head->issigned = '0';
	else if ((issigned == 1) || (issigned == '1'))
		head->issigned = '1';
	else
		fatalerr("set_issigned", "Issigned must equal 1 (signed) or 0 (unsigned).", NULL);

	return(True);
}

int get_rm_cm(IHEAD* head)
{
	int rm_cm;

	if (head == NULL)
		fatalerr("get_rm_cm", "pointer head is NULL", NULL);
	rm_cm = atoi(&(head->rm_cm));
	return(rm_cm);
}

int set_rm_cm(IHEAD* head, int rm_cm)
{
	if (head == NULL)
		fatalerr("set_rm_cm", "pointer head is NULL", NULL);

	if ((rm_cm == 0) || (rm_cm == '0'))
		head->rm_cm = '0';
	else if ((rm_cm == 1) || (rm_cm == '1'))
		head->rm_cm = '1';
	else
		fatalerr("set_rm_cm", "Rm_cm must equal 1 (Column Major) or 0 (Row Major).", NULL);

	return(True);
}

int get_tb_bt(IHEAD* head)
{
	int tb_bt;

	if (head == NULL)
		fatalerr("get_tb_bt", "pointer head is NULL", NULL);
	tb_bt = atoi(&(head->tb_bt));
	return(tb_bt);
}

int set_tb_bt(IHEAD* head, int tb_bt)
{
	if (head == NULL)
		fatalerr("set_tb_bt", "pointer head is NULL", NULL);

	if ((tb_bt == 0) || (tb_bt == '0'))
		head->tb_bt = '0';
	else if ((tb_bt == 1) || (tb_bt == '1'))
		head->tb_bt = '1';
	else
		fatalerr("set_tb_bt", "Tb_bt must equal 1 (bottom2top) or 0 (top2bottom).", NULL);

	return(True);
}

int get_lr_rl(IHEAD* head)
{
	int lr_rl;

	if (head == NULL)
		fatalerr("get_lr_rl", "pointer head is NULL", NULL);
	lr_rl = atoi(&(head->lr_rl));
	return(lr_rl);
}

int set_lr_rl(IHEAD* head, int lr_rl)
{
	if (head == NULL)
		fatalerr("set_lr_rl", "pointer head is NULL", NULL);

	if ((lr_rl == 0) || (lr_rl == '0'))
		head->lr_rl = '0';
	else if ((lr_rl == 1) || (lr_rl == '1'))
		head->lr_rl = '1';
	else
		fatalerr("set_lr_rl", "Lr_rl must equal 1 (right2left) or 0 (left2right).", NULL);

	return(True);
}

char* get_parent(IHEAD* head)
{
	if (head == NULL)
		fatalerr("get_parent", "pointer head is NULL", NULL);
	return(strdup(head->parent));
}

/* LINTLIBRARY */
/*
 * Stan Janet
 * 12/14/90
 *
 * Clears all bytes of parent field of iheader,
 * then sets the field to the longest subpath
 * that will fit in the space provided in the structure,
 * if any.
 */

void set_parent(IHEAD* i, char* path)
{
	if (i == (IHEAD*)NULL)
		fatalerr("set_parent", "null ihead pointer", (char*)NULL);
	if (path == (char*)NULL)
		fatalerr("set_parent", "null filename pointer", (char*)NULL);

	while ((path != (char*)NULL) && (strlen(path) >= sizeof((*i).parent)))
		path = strchr(path + 1, '/');

	memset(i->parent, NULL, sizeof((*i).parent));
	if (path != (char*)NULL)
		(void)strcpy(i->parent, path);
}

int get_par_x(IHEAD* head)
{
	int par_x;

	if (head == NULL)
		fatalerr("get_par_x", "pointer head is NULL", NULL);
	sscanf(head->par_x, "%d", &par_x);
	return(par_x);
}

int set_par_x(IHEAD* head, int par_x)
{
	if (head == NULL)
		fatalerr("set_par_x", "pointer head is NULL", NULL);
	memset(head->par_x, NULL, SHORT_CHARS);
	sprintf(head->par_x, "%d", par_x);
	return(True);
}

int get_par_y(IHEAD* head)
{
	int par_y;

	if (head == NULL)
		fatalerr("get_par_y", "pointer head is NULL", NULL);
	sscanf(head->par_y, "%d", &par_y);
	return(par_y);
}

int set_par_y(IHEAD* head, int par_y)
{
	if (head == NULL)
		fatalerr("set_par_y", "pointer head is NULL", NULL);
	memset(head->par_y, NULL, SHORT_CHARS);
	sprintf(head->par_y, "%d", par_y);
	return(True);
}