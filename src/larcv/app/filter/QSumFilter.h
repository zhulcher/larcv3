/**
 * \file QSumFilter.h
 *
 * \ingroup Package_Name
 * 
 * \brief Class def header for a class QSumFilter
 *
 * @author kazuhiro
 */

/** \addtogroup Package_Name

    @{*/
#ifndef __QSUMFILTER_H__
#define __QSUMFILTER_H__

#include "core/processor/ProcessBase.h"
#include "core/processor/ProcessFactory.h"
namespace larcv {

  /**
     \class ProcessBase
     User defined class QSumFilter ... these comments are used to generate
     doxygen documentation!
  */
  class QSumFilter : public ProcessBase {

  public:
    
    /// Default constructor
    QSumFilter(const std::string name="QSumFilter");
    
    /// Default destructor
    ~QSumFilter(){}

    void configure(const larcv::PSet&);

    void initialize();

    bool process(larcv::IOManager& mgr);

    void finalize();

  private:

    std::string _image_producer;
    std::vector<double> _min_qsum_v;
    std::vector<int>    _min_pixel_v;
    std::vector<double> _min_adc_v;

  };

  /**
     \class larcv::QSumFilterFactory
     \brief A concrete factory class for larcv::QSumFilter
  */
  class QSumFilterProcessFactory : public ProcessFactoryBase {
  public:
    /// ctor
    QSumFilterProcessFactory() { ProcessFactory::get().add_factory("QSumFilter",this); }
    /// dtor
    ~QSumFilterProcessFactory() {}
    /// creation method
    ProcessBase* create(const std::string instance_name) { return new QSumFilter(instance_name); }
  };

}

#endif
/** @} */ // end of doxygen group 

