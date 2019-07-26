/**
 * \file MPIProcessor.h
 *
 * \ingroup ThreadIO
 *
 * \brief Class def header for a class MPIProcessor
 *
 * @author kazuhiro
 */

/** \addtogroup ThreadIO

    @{*/
#ifndef __LARCV3THREADIO_MPIPROCESSOR_H
#define __LARCV3THREADIO_MPIPROCESSOR_H

#include "larcv3/core/processor/ProcessDriver.h"
#include "ThreadIOTypes.h"
#include <random>

namespace larcv3 {
  /**
     \class MPIProcessor
     User defined class MPIProcessor ... these comments are used to generate
     doxygen documentation!
  */
  class MPIProcessor : public larcv_base {

  public:

    /// Default constructor
    MPIProcessor(std::string name = "MPIProcessor");

    /// Default destructor
    ~MPIProcessor();

    bool batch_process(size_t nentries);

    void start_manager(size_t nentries);

    void stop_manager();

    inline bool manager_started() const { return _run_manager_thread; }

    void reset();

    void configure(const std::string config_file);

    void configure(const PSet& cfg);

    inline bool configured() const { return _configured;}

    void release_data(size_t storage_id);

    void release_data();

    inline const std::vector<larcv3::BatchDataState_t>& storage_status_array() const
    { return _batch_state_v; }

    inline size_t num_threads() const { return _num_threads; }

    void set_next_index(size_t index);

    void set_next_batch(const std::vector<size_t>& index_v);

    /// Sets the first entry the IOManager should look at
    void set_start_entry(size_t entry);

    /// Sets the number of entries that should be skipped between batches
    void set_entries_skip(size_t nskip);

    bool thread_running(size_t thread_id) const;

    bool thread_running() const;

    size_t num_thread_running() const;

    void status_dump() const;

    size_t process_counter(size_t thread_id) const;

    size_t process_counter() const;

    inline const std::vector<larcv3::ThreadFillerState_t>& thread_status() const
    { return _thread_state_v; }

    size_t get_n_entries() const;

    size_t batch_id(size_t storage_id) const;

    inline const std::vector<size_t>& thread_exec_counters() const
    { return _thread_exec_ctr_v; }

    const std::vector<size_t>& processed_entries(size_t storage_id=0) const;

    const std::vector<larcv3::EventID>& processed_events(size_t storage_id) const;

    const ProcessDriver* pd(size_t thread_id=0);

    const std::string& storage_name(size_t process_id) const;

    size_t process_id(const std::string& name) const;

    inline const std::vector<size_t>& batch_fillers() const
    { return _batch_filler_id_v; }

    inline const std::vector<larcv3::BatchDataType_t>& batch_types() const
    { return _batch_data_type_v; }

    inline size_t num_batch_storage() const
    { return _num_batch_storage; }

    void wait(std::chrono::microseconds duration);

  private:

    enum TPRandomAccessMode_t { kTPRandomNo, kTPRandomEntry, kTPRandomBatch, kTPRandomUnknown};

    int random_number(int range_min, int range_max);

    void terminate_threads();

    void manager_batch_process(size_t nentries);

    bool _batch_process_(size_t start_entry, size_t nentries, size_t thread_id);

    bool _processing;
    bool _configured;
    bool _enable_filter;

    // Hold the input files:
    std::vector<std::string> _input_fname_v;

    // Either set just the next index, or the next set of indexes:
    size_t _next_index;
    std::vector<size_t> _next_index_v;

    // Hold ID of each batch filler.  THis is used to index the BatchStorage as well.
    std::vector<size_t> _batch_filler_id_v;
    // List of datatypes
    std::vector<larcv3::BatchDataType_t> _batch_data_type_v;

    // main thread
    // thread-wise variables
    std::vector<std::thread> _thread_v;
    std::vector<size_t> _current_storage_id;
    std::vector<size_t> _thread_exec_ctr_v;
    std::vector<size_t> _batch_size_v;
    std::vector<size_t> _valid_counter_v;
    std::vector<size_t> _lifetime_valid_counter_v;
    // process-wise variables
    std::vector<std::string> _process_name_v;
    // storage-wise variables
    std::vector<larcv3::BatchDataState_t> _batch_state_v;
    std::vector<std::vector<size_t> > _batch_entries_v;
    std::vector<std::vector<larcv3::EventID> > _batch_events_v;
    std::vector<size_t> _batch_global_id;
  };

}

#endif
/** @} */ // end of doxygen group
