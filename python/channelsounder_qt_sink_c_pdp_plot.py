from PyQt4 import Qt
import PyQt4.Qwt5 as Qwt

import numpy as np
from channelsounder_qt_sink_c_scale_axes import time_scale, freq_scale
from channelsounder_qt_sink_c_scale_axes import x_scaledQwtPlotPicker as QwtPlotPicker

class pdp_plot(Qwt.QwtPlot):

    def __init__(self, grblock, *args):
        Qwt.QwtPlot.__init__(self, *args)

        self.setCanvasBackground(Qt.Qt.white)

        self.grblock = grblock

        # Initialize data
        self.t = np.arange(self.grblock.ntau)
        self.pdp = np.zeros(self.t.size)

        self.setTitle("Power Delay Profile")
        self.insertLegend(Qwt.QwtLegend(), Qwt.QwtPlot.BottomLegend);

        self.pdpcurve = Qwt.QwtPlotCurve("PDP")
        self.pdpcurve.attach(self)

        self.pdpcurve.setPen(Qt.QPen(Qt.Qt.red))

        mY = Qwt.QwtPlotMarker()
        mY.setLabelAlignment(Qt.Qt.AlignRight | Qt.Qt.AlignTop)
        mY.setLineStyle(Qwt.QwtPlotMarker.HLine)
        mY.setYValue(0.0)
        mY.attach(self)

        self.setAxisTitle(Qwt.QwtPlot.xBottom, "Time (seconds)")
        scale = time_scale(self.grblock.t_symbol)
        self.setAxisScaleDraw( Qwt.QwtPlot.xBottom, scale )

        self.setAxisTitle(Qwt.QwtPlot.yLeft, "Power")
        self.setAxisScale(Qwt.QwtPlot.yLeft, -100, 0)
        # for linear scale
        #self.setAxisScale(Qwt.QwtPlot.yLeft, 0, 2)

        # zoomer
        self.zoomer = Qwt.QwtPlotZoomer(Qwt.QwtPlot.xBottom,
                                        Qwt.QwtPlot.yLeft,
                                        Qwt.QwtPicker.DragSelection,
                                        Qwt.QwtPicker.AlwaysOff,
                                        self.canvas())
        self.zoomer.setRubberBandPen(Qt.QPen(Qt.Qt.green))

        # picker showing the coordinates at the mouse pointer
        self.picker = QwtPlotPicker(self.grblock.t_symbol,
                                        Qwt.QwtPlot.xBottom,
                                        Qwt.QwtPlot.yLeft,
                                        Qwt.QwtPicker.PointSelection,
                                        Qwt.QwtPlotPicker.CrossRubberBand,
                                        Qwt.QwtPicker.AlwaysOn,
                                        self.canvas())
        self.picker.setTrackerPen(Qt.QPen(Qt.Qt.cyan))

    def update_data(self):
        self.pdp = self.grblock.data
        self.pdp = np.sum(self.pdp, axis=0)
        self.pdp = np.abs(self.pdp)
        self.pdp = self.pdp**2
        # linear scale
        #self.pdp = np.divide(self.pdp, max(self.pdp) )
        gidx = np.where(self.pdp != 0 )
        bidx = np.where(self.pdp == 0 )
        self.pdp = np.divide(self.pdp, self.grblock.nt )
        self.pdp[gidx] = 10*np.log10(self.pdp[gidx])
        self.pdp[bidx] = -100
        self.pdpcurve.setData(self.t, self.pdp)
        self.replot()
