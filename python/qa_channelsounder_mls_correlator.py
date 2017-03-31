#!/usr/bin/env python
# 
# Copyright 2012 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 
#

from gnuradio import gr, gr_unittest, digital, blocks

import channelsounder

class qa_mls_correlator (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        self.degree = 5
        self.length = 2**self.degree - 1
        expected_result = (self.length - 1)*[-1.0/self.length]+[1]

        # send a maximum length sequence with period 31
        src_real = digital.glfsr_source_f(self.degree, True, 0, 1)
        src_imag = blocks.null_source(gr.sizeof_float*1)
        f2c = blocks.float_to_complex(1)

        mls_correlator = channelsounder.mls_correlator(self.degree, mask = 0, seed = 1)

        # keep only the samples of the first two autocorrelation periods
        head = blocks.head(gr.sizeof_gr_complex, 2*self.length)
        dest = blocks.vector_sink_c(1)

        self.tb.connect( src_real, (f2c, 0))
        self.tb.connect( src_imag, (f2c, 1))

        self.tb.connect( f2c, mls_correlator )
        self.tb.connect( mls_correlator, head, dest )
        # set up fg
        self.tb.run ()

        # check data

        result_data = dest.data()
        # discard the first period, since it is tainted by the zeros inserted
        # by set_history()
        result_data = result_data[self.length:]
        self.assertFloatTuplesAlmostEqual (expected_result, result_data, 6)

if __name__ == '__main__':
    gr_unittest.main ()
