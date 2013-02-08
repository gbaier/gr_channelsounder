from PyQt4 import Qt
import PyQt4.Qwt5 as Qwt

import numpy as np
from channelsounder_qt_sink_c_scale_axes import time_scale, freq_scale
from channelsounder_qt_sink_c_scale_axes import x_scaledQwtPlotPicker as QwtPlotPicker

class cir_plot(Qwt.QwtPlot):

    def __init__(self, grblock, *args):
        Qwt.QwtPlot.__init__(self, *args)

        self.setCanvasBackground(Qt.Qt.white)

        self.grblock = grblock

        # Initialize data
        self.t = np.arange(self.grblock.ntau)
        self.cir_real = np.zeros(self.t.size)
        self.cir_imag = np.zeros(self.t.size)

        self.setTitle("Channel Impulse Response")
        self.insertLegend(Qwt.QwtLegend(), Qwt.QwtPlot.BottomLegend);

        self.cir_real_curve = Qwt.QwtPlotCurve("CIR real")
        self.cir_real_curve.attach(self)
        self.cir_real_curve.setPen(Qt.QPen(Qt.Qt.blue))

        self.cir_imag_curve = Qwt.QwtPlotCurve("CIR imag")
        self.cir_imag_curve.attach(self)
        self.cir_imag_curve.setPen(Qt.QPen(Qt.Qt.red))

        # draw a black line at Y = 0
        mY = Qwt.QwtPlotMarker()
        mY.setLabelAlignment(Qt.Qt.AlignRight | Qt.Qt.AlignTop)
        mY.setLineStyle(Qwt.QwtPlotMarker.HLine)
        mY.setYValue(0.0)
        mY.attach(self)

        self.setAxisTitle(Qwt.QwtPlot.xBottom, "Time (seconds)")

        scale = time_scale(self.grblock.t_symbol)
        self.setAxisScaleDraw(Qwt.QwtPlot.xBottom, scale)

        self.setAxisTitle(Qwt.QwtPlot.yLeft, "Amplitude")
        self.setAxisScale(Qwt.QwtPlot.yLeft, -1, 1)

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
        self.cir_real = self.grblock.cir.real
        self.cir_imag = self.grblock.cir.imag
        self.cir_real_curve.setData(self.t, self.cir_real)
        self.cir_imag_curve.setData(self.t, self.cir_imag)
        self.replot()

