from PyQt4 import Qt
import PyQt4.Qwt5 as Qwt

import numpy as np
from channelsounder_qt_sink_c_scale_axes import time_scale, freq_scale
from channelsounder_qt_sink_c_scale_axes import xy_scaledQwtPlotPicker as QwtPlotPicker

class scatter_plot(Qwt.QwtPlot):
    """ QT widget which plots the scattering function. """

    def __init__(self, grblock, *args):
        Qwt.QwtPlot.__init__(self, *args)
        self.grblock = grblock
        self.scatter = np.zeros( (self.grblock.nt, self.grblock.ntau) )

        # set plot title
        self.setTitle('Scattering Function')

        # set plot layout
        self.plotLayout().setMargin(0)
        self.plotLayout().setCanvasMargin(0)
        self.plotLayout().setAlignCanvasToScales(True)

        # set legend
        legend = Qwt.QwtLegend()
        legend.setItemMode(Qwt.QwtLegend.ClickableItem)
        self.insertLegend(legend, Qwt.QwtPlot.RightLegend)

        # set axis titles
        self.setAxisTitle(Qwt.QwtPlot.xBottom, 'Delay (s)')
        self.setAxisTitle(Qwt.QwtPlot.yLeft, 'Doppler Spectrum (Hz)')

        # scale x-axis
        t_scale = time_scale(self.grblock.t_symbol)
        self.setAxisScaleDraw( Qwt.QwtPlot.xBottom, t_scale )

        # scale y-axis
        f_scale = freq_scale( self.grblock.prf, self.grblock.nt)
        self.setAxisScaleDraw( Qwt.QwtPlot.yLeft, f_scale )

        # create a QwtRasterData instance to store the numpy array
        # produced by the GNU Radio Python sink block
        self.scatter_data = ScatterData(self.scatter)

        # create a QwtPlotSpectrogram instance to plot the QwtRasterData
        self.plot_scatter = Qwt.QwtPlotSpectrogram()

        # create a greyscale colormap
        colorMap = Qwt.QwtLinearColorMap(Qt.Qt.black, Qt.Qt.white)
        self.plot_scatter.setColorMap(colorMap)

        # attach and pass the data
        self.plot_scatter.attach(self)
        self.plot_scatter.setData(self.scatter_data)

        # replot
        self.replot()

        # zoomer
        self.zoomer = Qwt.QwtPlotZoomer(Qwt.QwtPlot.xBottom,
                                        Qwt.QwtPlot.yLeft,
                                        Qwt.QwtPicker.DragSelection,
                                        Qwt.QwtPicker.AlwaysOff,
                                        self.canvas())
        self.zoomer.setRubberBandPen(Qt.QPen(Qt.Qt.green))

        # picker showing the coordinates at the mouse pointer
        self.picker = QwtPlotPicker(self.grblock.t_symbol,
                                          self.grblock.nt,
                                          self.grblock.prf,
                                          Qwt.QwtPlot.xBottom,
                                          Qwt.QwtPlot.yLeft,
                                          Qwt.QwtPicker.PointSelection,
                                          Qwt.QwtPlotPicker.CrossRubberBand,
                                          Qwt.QwtPicker.AlwaysOn,
                                          self.canvas())
        self.picker.setTrackerPen(Qt.QPen(Qt.Qt.cyan))

    def update_data(self):
        self.data = self.grblock.data
        scatter = np.fft.fft(self.data, axis = 0)
        scatter = np.fft.fftshift(scatter, axes = 0)
        scatter = np.abs(scatter)
        scatter = np.divide(scatter, scatter.max())
        # np.log10 cannot work with 0,
        # therefore we have to separately treat our data
        #idxg = np.where(scatter != 0)
        #idxb = np.where(scatter == 0)
        #scatter[idxg] = 20*np.log10(scatter[idxg])
        #scatter[idxb] = scatter[idxg].min()
        self.scatter_data.setData(scatter)
        self.replot()


class ScatterData(Qwt.QwtRasterData):

    def __init__(self, nparr):
        area = Qt.QRectF(0, 0, nparr.shape[1]-1, nparr.shape[0]-1)
        Qwt.QwtArrayData.__init__(self, area)
        self.scatter = nparr

    def setData(self, nparr):
        self.scatter = nparr

    def rasterHint(self, area):
        """ Set the appropriate resolution of the raster.

        The raster will have as many elements as the numpy matrix.

        """
        raster = Qt.QSize()
        raster.setHeight(area.height())
        raster.setWidth(area.width())
        return raster

    def copy(self):
        return self

    def range(self):
        return Qwt.QwtDoubleInterval(self.scatter.min(), self.scatter.max())

    def value(self, x, y):
        idx = int(x)
        idy = int(y)
        return self.scatter[idy, idx]

