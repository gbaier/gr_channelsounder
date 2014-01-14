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

#ifndef INCLUDED_CHANNELSOUNDER_AVG_M_OVER_N_CC_H_IMPL
#define INCLUDED_CHANNELSOUNDER_AVG_M_OVER_N_CC_H_IMPL

#include <channelsounder/avg_m_over_n_cc.h>
#include <fftw3.h>


namespace gr {
  namespace channelsounder {

        class avg_m_over_n_cc_impl : public avg_m_over_n_cc
        {
        private:
            int d_m;
            int d_win_size;
            int d_counter;
            int d_alignment_multiple;
            gr_complex *d_avg;

         public:
            avg_m_over_n_cc_impl (int m, int win_size);
            ~avg_m_over_n_cc_impl ();
            void forecast (int noutput_items,
                       gr_vector_int &ninput_items_required);

            int general_work (int noutput_items,
                    gr_vector_int &ninput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items);
        };
  } // namespace channelsounder
} // namespace gr

#endif /* INCLUDED_CHANNELSOUNDER_AVG_M_OVER_N_CC_H_IMPL */

