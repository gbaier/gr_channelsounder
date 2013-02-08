/* -*- c++ -*- */

#define CHANNELSOUNDER_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "channelsounder_swig_doc.i"


%{
#include "channelsounder_mls_correlator.h"
#include "channelsounder_avg_m_over_n_cc.h"
%}
GR_SWIG_BLOCK_MAGIC(channelsounder,mls_correlator);
%include "channelsounder_mls_correlator.h"

GR_SWIG_BLOCK_MAGIC(channelsounder,avg_m_over_n_cc);
%include "channelsounder_avg_m_over_n_cc.h"
