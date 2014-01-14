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

#ifndef INCLUDED_CHANNELSOUNDER_AVG_M_OVER_N_CC_H
#define INCLUDED_CHANNELSOUNDER_AVG_M_OVER_N_CC_H

#include <channelsounder/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace channelsounder {

    /*!
     * \brief Takes m-times the average over a vector of size win_size
     * \ingroup channelsounder
     */
    class CHANNELSOUNDER_API avg_m_over_n_cc : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<avg_m_over_n_cc> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of channelsounder::avg_m_over_n_cc.
       *
       * To avoid accidental use of raw pointers, channelsounder::avg_m_over_n_cc's
       * constructor is in a private implementation
       * class. channelsounder::avg_m_over_n_cc::make is the public interface for
       * creating new instances.
       */
      static sptr make(int m,
                       int win_size);
    };

  } // namespace channelsounder
} // namespace gr

#endif /* INCLUDED_CHANNELSOUNDER_AVG_M_OVER_N_CC_H */

