/* -*- c++ -*- */
/* 
 * Copyright 2012 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include <channelsounder_mls_correlator.h>
#include <iostream>

#include <volk/volk.h>
#include <fftw3.h>

channelsounder_mls_correlator_sptr
channelsounder_make_mls_correlator (int degree, int mask, int seed)
{
	return channelsounder_mls_correlator_sptr (new channelsounder_mls_correlator (degree, mask, seed));
}


channelsounder_mls_correlator::channelsounder_mls_correlator (int degree, int mask, int seed)
	: gr_sync_block ("mls_correlator",
		gr_make_io_signature (1, 1, sizeof (gr_complex)),
		gr_make_io_signature (1, 1, sizeof (gr_complex)))
{
	d_len = (unsigned int)((1ULL << degree)-1);
	// Since a correlation is computed at least d_len input items are required.
	set_history(d_len);

	const int alignment_multiple = volk_get_alignment() / sizeof(gr_complex);
	set_alignment(std::max(1,alignment_multiple));
	
	if (mask == 0)
		mask = gri_glfsr::glfsr_mask(degree);
	d_reference = new gri_glfsr(mask, seed);

	// d_pn will hold the complete maximum length sequence.
	// Since we are dealing with complex numbers each chip has
	// to be repeated twice.
	d_pn = (float*)fftwf_malloc(sizeof(float)*2*d_len);
	for (int i = 0; i < 2*d_len; i += 2){
		d_pn[i] = 2.0*d_reference->next_bit()-1.0;
		d_pn[i+1] = d_pn[i];
	}
}

channelsounder_mls_correlator::~channelsounder_mls_correlator ()
{
  delete d_reference;
}

int
channelsounder_mls_correlator::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const gr_complex *in = (const gr_complex *) input_items[0];
	gr_complex *out = (gr_complex *) output_items[0];
	gr_complex sum;

	gr_complex* temp = (gr_complex*)fftwf_malloc(sizeof(gr_complex)*d_len);

	for (int i = 0; i < noutput_items; i++) {
		// Since the pointer is incremented with each loop iteration, and is_unaligned() only
		// checks the start of the input buffer, the alignment might change.
		if( ( is_unaligned() && (i%2 == 0) ) || ( !is_unaligned() && (i%2 == 1) ) )
			volk_32f_x2_multiply_32f_u((float*) temp, (float*) d_pn, (float*) in, 2*d_len);
		else
			volk_32f_x2_multiply_32f_a((float*) temp, (float*) d_pn, (float*) in, 2*d_len);

		sum = 0.0;
		for (int j = 0; j < d_len; j++) {
			sum += temp[j];
		}	
		in++;
		*out++ = sum*gr_complex(1.0/d_len, 0.0);
	}
	return noutput_items;
}

