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
#include <channelsounder_avg_m_over_n_cc.h>

#include <volk/volk.h>
#include <fftw3.h>


channelsounder_avg_m_over_n_cc_sptr
channelsounder_make_avg_m_over_n_cc (int m, int win_size)
{
	return channelsounder_avg_m_over_n_cc_sptr (new channelsounder_avg_m_over_n_cc (m, win_size));
}


channelsounder_avg_m_over_n_cc::channelsounder_avg_m_over_n_cc (int m, int win_size)
	: gr_block ("avg_m_over_n_cc",
		gr_make_io_signature (1, 1, sizeof (gr_complex)),
		gr_make_io_signature (1, 1, sizeof (gr_complex))),
	 d_m(m),
	 d_win_size(win_size),
	 d_counter(0)
{
	const int alignment_multiple = volk_get_alignment() / sizeof(gr_complex);
	d_alignment_multiple = alignment_multiple;
	set_alignment(std::max(1,alignment_multiple));
	d_avg = (gr_complex*)fftwf_malloc(sizeof(gr_complex)*d_win_size);
	memset(d_avg, 0, d_win_size*sizeof(gr_complex));
}


channelsounder_avg_m_over_n_cc::~channelsounder_avg_m_over_n_cc ()
{
}


void
channelsounder_avg_m_over_n_cc::forecast (int noutput_items,
					  gr_vector_int &ninput_items_required) 	
{
	ninput_items_required[0] = noutput_items;
}

int
channelsounder_avg_m_over_n_cc::general_work (int noutput_items,
			gr_vector_int &ninput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
	const gr_complex *in = (const gr_complex*) input_items[0];
	gr_complex *out = (gr_complex*) output_items[0];

	if ( (ninput_items[0] < d_win_size) || (noutput_items < d_win_size) )
	{
		return 0;
	}

	d_counter++;
	if( is_unaligned() )
		volk_32f_x2_add_32f_u((float*) d_avg, (float*) d_avg, (float*) in, 2*d_win_size);
	else
		volk_32f_x2_add_32f_a((float*) d_avg, (float*) d_avg, (float*) in, 2*d_win_size);
	consume_each(d_win_size);

	if (d_counter == d_m)
	{
		d_counter=0;
		for(int i=0; i<d_win_size; i++)
		{
			out[i] = d_avg[i]/gr_complex(d_m, 0);
		}
		memset(d_avg, 0, d_win_size*sizeof(gr_complex));
		return d_win_size;
	}
	else
	{
		return 0;
	}
}

