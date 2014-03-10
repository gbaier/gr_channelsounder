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

#include <gnuradio/io_signature.h>
#include "avg_m_over_n_cc_impl.h"

#include <volk/volk.h>
#include <fftw3.h>

namespace gr {
  namespace channelsounder {

    avg_m_over_n_cc::sptr avg_m_over_n_cc::make (int m,
                                                 int win_size)
    {
        return gnuradio::get_initial_sptr (new avg_m_over_n_cc_impl (m, win_size));
    }

    avg_m_over_n_cc_impl::avg_m_over_n_cc_impl (int m, int win_size)
        : gr::block ("avg_m_over_n_cc",
            gr::io_signature::make (1, 1, sizeof (gr_complex)),
            gr::io_signature::make (1, 1, sizeof (gr_complex))),
         d_m(m),
         d_win_size(win_size),
         d_counter(0)

    {
        const int alignment_multiple = volk_get_alignment() / sizeof(gr_complex);
        d_alignment_multiple = alignment_multiple;
        set_alignment(std::max(1,alignment_multiple));
        d_avg = (gr_complex*)fftwf_malloc(sizeof(gr_complex)*d_win_size);
        memset(d_avg, 0, d_win_size*sizeof(gr_complex));
        set_min_noutput_items(d_win_size);
    }


    avg_m_over_n_cc_impl::~avg_m_over_n_cc_impl ()
    {
    }


    void
    avg_m_over_n_cc_impl::forecast (int noutput_items,
                          gr_vector_int &ninput_items_required) 	
    {
        if (noutput_items < d_win_size) {
            ninput_items_required[0] = d_m*d_win_size;
        } else {
            ninput_items_required[0] = d_m*noutput_items;
        }
    }

    int
    avg_m_over_n_cc_impl::general_work (int noutput_items,
                gr_vector_int &ninput_items,
                gr_vector_const_void_star &input_items,
                gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex*) input_items[0];
        gr_complex *out = (gr_complex*) output_items[0];

        // if we do not have enough input samples wait for more
        if (ninput_items[0] < d_win_size) {
            return 0;
        }

        // add to average
        d_counter++;
        if( is_unaligned() ) {
            volk_32f_x2_add_32f_u((float*) d_avg, (float*) d_avg, (float*) in, 2*d_win_size);
        } else {
            volk_32f_x2_add_32f_a((float*) d_avg, (float*) d_avg, (float*) in, 2*d_win_size);
        }
        consume_each(d_win_size);

        if (d_counter == d_m) {
            d_counter=0;
            for(int i=0; i<d_win_size; i++) {
                out[i] = d_avg[i]/gr_complex(d_m, 0);
            }
            memset(d_avg, 0, d_win_size*sizeof(gr_complex));
            return d_win_size;
        } else {
            return 0;
        }
    }

  } // namespace channelsounder
} // namespace gr
