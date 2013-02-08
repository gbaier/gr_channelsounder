import numpy as np

from gnuradio import gr
import gnuradio.extras

# necessary for data exchange between GNU Radio and Qt
import threading

# stuff needed to build a Qt GUI with tabs
from PyQt4 import Qt
import sys

# import plotting widgets
from channelsounder_qt_sink_c_scatter_plot import *
from channelsounder_qt_sink_c_cir_plot import *
from channelsounder_qt_sink_c_pdp_plot import *

class channelsounder_qt_sink_c(gr.block):
    """ GNU Radio channelsounder QT sink """

    def __init__(self, samp_rate, prf, ntau, nt):
        """
        Parameters

        samp_rate : int, float
             Sample rate of the GNU radio flowgraph
        nt : int
             Number of measurements that are used to compute the power delay
             profile and the scattering function.
        ntau : int
             Length of the transmitted pulse

        """
        gr.block.__init__(
            self,
            name = "channelsounder sink c",
            in_sig = [np.complex64],
            out_sig = None,
        )
        self.samp_rate = samp_rate
        self.t_symbol = 1./samp_rate
        self.prf = prf
        self.ntau = ntau
        self.nt = nt
        self.cir = np.zeros(ntau)
        # numpy array which will contain nt measurements of the CIR
        self.data = np.zeros((nt, ntau), dtype = complex)
        # the condition manages the data exchange between this block and the qt
        # GUI
        self.condition = threading.Condition()

    def forecast(self,  noutput_items, ninput_items_required):
        for i in range(len(ninput_items_required)):
            ninput_items_required[i] = self.ntau

    def work(self, input_items, output_items):
        # if we do not have enough input samples do not do anything
        if len(input_items[0]) < self.ntau:
            return 0
        else:
            self.condition.acquire()
            self.cir = input_items[0][:self.ntau]
            # rotate the measurements
            self.data = np.roll(self.data, -1, axis=0)
            # overwrite the oldest measurment
            self.data[-1,:] = input_items[0][:self.ntau]
            self.condition.notify()
            self.condition.release()
            return self.ntau

    def pyqwidget(self):
        """ returns a QT Widget

        This method mimics the behaviour of the official gr-qtgui sink

        Notes:
        The GNU Radio block passes itself to QT Widget so that the QT Widget
        can access the input samples of the GNU Radio block.

        """
        return main_window(self)


class main_window(Qt.QTabWidget):
    """Main window of the channelsounder QT sink.

    Three plots are created:

    1) power delay profile
    2) channel impulse response
    3) scattering function

    """

    def __init__(self, grblock, *args):
        """
        Parameters:

        grblock : GNU Radio block of the QT sink


        Notes:

        Via the parameter grblock the QT Widget has access to the all of the
        GNU radio blocks data.

        """
        Qt.QTabWidget.__init__(self, *args)
        self.grblock = grblock

        # create tabs for the power delay profile, channel impulse response and
        # the scattering function
        # The GNU Radio block is passed as an argument to gain access to the
        # measurements as well as getting knowledge how to scale the axes of
        # the plots (pulse repetition frequency and sample rate).
        self.pdp_tab = pdp_plot(grblock)
        self.cir_tab = cir_plot(grblock)
        self.scatter_tab = scatter_plot(grblock)

        # add those tabs to the main window
        self.addTab(self.pdp_tab, "PDP")
        self.addTab(self.cir_tab, "CIR")
        self.addTab(self.scatter_tab, "Scatter")

        # execute timerEvent every 100ms
        self.startTimer(100)
        self.setMinimumSize(500, 300)

    def timerEvent(self, e):
        """ updates the data of all plots

        Notes:
        The threading condition stops the GNU Radio flowgraph until the data
        has been extracted.

        """

        self.grblock.condition.acquire()
        self.pdp_tab.update_data()
        self.cir_tab.update_data()
        self.scatter_tab.update_data()
        self.grblock.condition.notify()
        self.grblock.condition.release()

