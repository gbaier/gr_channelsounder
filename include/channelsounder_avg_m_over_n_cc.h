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

#ifndef INCLUDED_CHANNELSOUNDER_AVG_M_OVER_N_CC_H
#define INCLUDED_CHANNELSOUNDER_AVG_M_OVER_N_CC_H

#include <channelsounder_api.h>
#include <gr_block.h>

#include <fftw3.h>

class channelsounder_avg_m_over_n_cc;
typedef boost::shared_ptr<channelsounder_avg_m_over_n_cc> channelsounder_avg_m_over_n_cc_sptr;

CHANNELSOUNDER_API channelsounder_avg_m_over_n_cc_sptr channelsounder_make_avg_m_over_n_cc (int m, int win_size);

/*!
Takes m-times the average over a vector of size win_size
 */
class CHANNELSOUNDER_API channelsounder_avg_m_over_n_cc : public gr_block
{
	friend CHANNELSOUNDER_API channelsounder_avg_m_over_n_cc_sptr channelsounder_make_avg_m_over_n_cc (int m, int win_size);

	channelsounder_avg_m_over_n_cc (int m, int win_size);
	int d_m;
	int d_win_size;
	int d_counter;
	int d_alignment_multiple;
	gr_complex *d_avg;

 public:
	~channelsounder_avg_m_over_n_cc ();
	void forecast (int noutput_items,
		       gr_vector_int &ninput_items_required);

	int general_work (int noutput_items,
			gr_vector_int &ninput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items);
};

#endif /* INCLUDED_CHANNELSOUNDER_AVG_M_OVER_N_CC_H */

