/* -*- c++ -*- */

#define CHANNELSOUNDER_API

/*%include "gnuradio.i"			// the common stuff */
%include "runtime_swig.i"			// the common stuff

//load generated python docstrings
%include "channelsounder_swig_doc.i"

%{
#include "channelsounder/mls_correlator.h"
#include "channelsounder/avg_m_over_n_cc.h"
%}


%include "channelsounder/mls_correlator.h"
GR_SWIG_BLOCK_MAGIC2(channelsounder,mls_correlator);
%include "channelsounder/avg_m_over_n_cc.h"
GR_SWIG_BLOCK_MAGIC2(channelsounder,avg_m_over_n_cc);
