import PyQt4.Qwt5 as Qwt

class time_scale(Qwt.QwtScaleDraw):
    """ Changes the scale of the delay axis """
    def __init__(self, t_symbol, *args):
        Qwt.QwtScaleDraw.__init__(self, *args)
        self.t_symbol = t_symbol

    def label(self, value):
        return Qwt.QwtText(str(value*self.t_symbol))

class freq_scale(Qwt.QwtScaleDraw):
    """ Changes the scale of the Doppler Spectrum axis """
    def __init__(self, prf, nt, *args):
        Qwt.QwtScaleDraw.__init__(self, *args)
        self.prf = prf
        self.nt = nt

    def label(self, value):
        value = value - self.nt/2.
        value = value/self.nt
        return Qwt.QwtText("%5.2f" % (value*self.prf))

class xy_scaledQwtPlotPicker(Qwt.QwtPlotPicker):
    """ Changes the scale of the picker axes

    This modifies the text next to the mouse cursor. The text will contain
    the scaled x- and y-coordinates so that they represent the delay and the
    Doppler frequency.

    """

    def __init__(self, t_symbol, nt, prf, *args):
        Qwt.QwtPlotPicker.__init__(self, *args)
        self.t_symbol = t_symbol
        self.nt = nt
        self.prf = prf

    def trackerText(self, pos):
        plot_coords = self.invTransform(pos)
        x = plot_coords.x()*self.t_symbol
        y = ( plot_coords.y()/self.nt - 1/2. )*self.prf
        return Qwt.QwtText( '{x:f}, {y:f}'.format(x=x, y=y) )

class x_scaledQwtPlotPicker(Qwt.QwtPlotPicker):
    """ Changes the scale of the picker axes

    This modifies the text next to the mouse cursor. The text will contain
    the scaled x-coordinates so that it represents the delay

    """
    def __init__(self, t_symbol, *args):
        Qwt.QwtPlotPicker.__init__(self, *args)
        self.t_symbol = t_symbol

    def trackerText(self, pos):
        plot_coords = self.invTransform(pos)
        x = plot_coords.x()*self.t_symbol
        y = plot_coords.y()
        return Qwt.QwtText( '{x:f}, {y:f}'.format(x=x, y=y) )
