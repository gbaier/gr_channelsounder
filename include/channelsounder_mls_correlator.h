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

#ifndef INCLUDED_CHANNELSOUNDER_MLS_CORRELATOR_H
#define INCLUDED_CHANNELSOUNDER_MLS_CORRELATOR_H

#include <channelsounder_api.h>
#include <gr_sync_block.h>
#include <gri_glfsr.h>

#include <fftw3.h>
class channelsounder_mls_correlator;
typedef boost::shared_ptr<channelsounder_mls_correlator> channelsounder_mls_correlator_sptr;

CHANNELSOUNDER_API channelsounder_mls_correlator_sptr channelsounder_make_mls_correlator (int degree, int mask, int seed);

/*!
Correlates sample-wise the input stream with the maximum length sequence defined by the parameters.
This block is based on the gr-digital/pn_correlator_cc.

Parameters:
----------
        degree : Degree of shift register must be in [1, 32]. If mask is 0, the degree determines a default mask (see digital_impl_glfsr.cc for the mapping).
        mask : Allows a user-defined bit mask for indexes of the shift register to feed back.
        seed : Initial setting for values in shift register.
 */
class CHANNELSOUNDER_API channelsounder_mls_correlator : public gr_sync_block
{
	friend CHANNELSOUNDER_API channelsounder_mls_correlator_sptr channelsounder_make_mls_correlator (int degree, int mask, int seed);

	int        d_len;
	gri_glfsr* d_reference;
	float*	   d_pn;

	channelsounder_mls_correlator (int degree, int mask, int seed);

 public:
	~channelsounder_mls_correlator ();


	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_CHANNELSOUNDER_MLS_CORRELATOR_H */

