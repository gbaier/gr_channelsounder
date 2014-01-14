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

#ifndef INCLUDED_CHANNELSOUNDER_MLS_CORRELATOR_H_IMPL
#define INCLUDED_CHANNELSOUNDER_MLS_CORRELATOR_H_IMPL

#include <channelsounder/mls_correlator.h>
#include <gnuradio/digital/glfsr.h>
#include <fftw3.h>

namespace gr {
  namespace channelsounder {


    class mls_correlator_impl : public mls_correlator
    {
    private:
        int                     d_len;
        gr::digital::glfsr*     d_reference;
        float*	                d_pn;

     public:
        mls_correlator_impl (int degree, int mask, int seed);
        ~mls_correlator_impl ();

        int work (int noutput_items,
            gr_vector_const_void_star &input_items,
            gr_vector_void_star &output_items);
    };
  } // namespace channelsounder
} // namespace gr

#endif /* INCLUDED_CHANNELSOUNDER_MLS_CORRELATOR_H_IMPL */

