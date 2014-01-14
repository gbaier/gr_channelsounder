/* -*- c++ -*- */
/* 
 * Copyright 2014 <+YOU OR YOUR COMPANY+>.
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

#include <channelsounder/api.h>
#include <gnuradio/sync_block.h>

namespace gr {
  namespace channelsounder {

     /*!
     *   Correlates sample-wise the input stream with the maximum length sequence defined by the parameters.
     *   This block is based on the gr-digital/pn_correlator_cc.
     *   
     *   Parameters:
     *   ----------
     *           degree : Degree of shift register must be in [1, 32]. If mask is 0, the degree determines a default mask (see digital_impl_glfsr.cc for the mapping).
     *           mask : Allows a user-defined bit mask for indexes of the shift register to feed back.
     *           seed : Initial setting for values in shift register.
     * \ingroup channelsounder
     *
     */
    class CHANNELSOUNDER_API mls_correlator : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<mls_correlator> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of channelsounder::mls_correlator.
       *
       * To avoid accidental use of raw pointers, channelsounder::mls_correlator's
       * constructor is in a private implementation
       * class. channelsounder::mls_correlator::make is the public interface for
       * creating new instances.
       */
      static sptr make(int degree,
                       int mask,
                       int seed);
    };

  } // namespace channelsounder
} // namespace gr

#endif /* INCLUDED_CHANNELSOUNDER_MLS_CORRELATOR_H */

