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

from gnuradio import gr, gr_unittest, blocks
import channelsounder_swig

class qa_avg_m_over_n_cc (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        data = [0,1,2,3,4,5,6,7,8,9] + [9,8,7,6,5,4,3,2,1,0]
        data = data + 10*[1] + [-10,-10,0,0,0,5,10,0,0,30]
        m = 4
        win_size = 10
        expected_result = [0, 0, 10, 10, 10, 15, 20, 10, 10, 40]
        expected_result = [ float(el)/4 for el in expected_result ]

        src = blocks.vector_source_c(data, True)
        avg_4_over_10 = channelsounder_swig.avg_m_over_n_cc(m = 4,
                                                            win_size = win_size)
        head = blocks.head(gr.sizeof_gr_complex, win_size)
        dest = blocks.vector_sink_c(1)

        self.tb.connect(src, avg_4_over_10, head, dest)
        # set up fg
        self.tb.run ()

        # check data
        result_data = dest.data()
        self.assertFloatTuplesAlmostEqual (expected_result, result_data, 6)


if __name__ == '__main__':
    gr_unittest.main ()
