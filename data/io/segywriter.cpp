#include "segywriter.h"

#include "data/seismevent.h"

namespace Data {
namespace IO {

void SegyWriter::save(const QFileInfo& fileInfo, SeismEvent * const event) {
    _fp = segy_open(fileInfo.absoluteFilePath().toLocal8Bit().data(), "wb");
    if (!_fp) {
      throw std::runtime_error("segy_open()");
    }

    auto* _component = event->getComponents().front();
    auto* _trace = _component->getTraces().front();

    int err = SEGY_OK;
    char binheader[SEGY_BINARY_HEADER_SIZE] = {0};

    _sam_num = _trace->getBufferSize();
    _sam_intr = _component->getInfo().getSampleInterval();
    _format = _component->getInfo().getFormat();
    _trace_bsize = segy_trace_bsize(_sam_num);

    err = segy_set_bfield(binheader, SEGY_BIN_INTERVAL, static_cast<int32_t>(_sam_intr));
    if(SEGY_OK != err) {
        throw std::runtime_error("segy_set_bfield(SEGY_BIN_INTERVAL)");
    }

    err = segy_set_bfield(binheader, SEGY_BIN_SAMPLES, _sam_num);
    if(SEGY_OK != err) {
        throw std::runtime_error("segy_set_bfield(SEGY_BIN_SAMPLES)");
    }

    err = segy_set_bfield(binheader, SEGY_BIN_FORMAT, _format);
    if(SEGY_OK != err) {
        throw std::runtime_error("segy_set_bfield(SEGY_BIN_FORMAT)");
    }

    err = segy_write_binheader(_fp, binheader);
    if(SEGY_OK != err) {
        throw std::runtime_error("segy_write_binheader()");
    }

    _trace0 = segy_trace0(binheader);


    int traceno = 0;
    for (auto& component : event->getComponents()) {
        char trace_header[SEGY_TRACE_HEADER_SIZE] = {0};
        err = segy_set_field(trace_header, SEGY_TR_SAMPLE_INTER, static_cast<int32_t>(_sam_intr));
        if(SEGY_OK != err) {
            throw std::runtime_error("segy_set_field(SEGY_TR_SAMPLE_INTER)");
        }

        if(component->containsWavePickBy(SeismWavePick::Type::PWAVE)){
            err = segy_set_field(trace_header, SEGY_TR_CDP_X, component->getWavePicks().at(SeismWavePick::Type::PWAVE).getArrival());
            if(SEGY_OK != err) {
                throw std::runtime_error("segy_set_field(SEGY_TR_CDP_X)");
            }
        }
        if(component->containsWavePickBy(SeismWavePick::Type::SWAVE)){
            err = segy_set_field(trace_header, SEGY_TR_CDP_Y, component->getWavePicks().at(SeismWavePick::Type::SWAVE).getArrival());
            if(SEGY_OK != err) {
                throw std::runtime_error("segy_set_field(SEGY_TR_CDP_Y)");
            }
        }


        auto& date_time_stamp = component->getStampDateTime();
        err = segy_set_field(trace_header, SEGY_TR_YEAR_DATA_REC, date_time_stamp.date().year());
        if (SEGY_OK != err) {
          throw std::runtime_error("segy_set_field(SEGY_TR_YEAR_DATA_REC)");
        }
        err = segy_set_field(trace_header, SEGY_TR_DAY_OF_YEAR, date_time_stamp.date().day());
        if (SEGY_OK != err) {
          throw std::runtime_error("segy_set_field(SEGY_TR_DAY_OF_YEAR)");
        }
        err = segy_set_field(trace_header, SEGY_TR_HOUR_OF_DAY, date_time_stamp.time().hour());
        if (SEGY_OK != err) {
          throw std::runtime_error("segy_set_field(SEGY_TR_HOUR_OF_DAY)");
        }
        err = segy_set_field(trace_header, SEGY_TR_MIN_OF_HOUR, date_time_stamp.time().minute());
        if (SEGY_OK != err) {
          throw std::runtime_error("segy_set_field(SEGY_TR_MIN_OF_HOUR)");
        }
        err = segy_set_field(trace_header, SEGY_TR_SEC_OF_MIN, date_time_stamp.time().second());
        if (SEGY_OK != err) {
          throw std::runtime_error("segy_set_field(SEGY_TR_SEC_OF_MIN)");
        }
        err = segy_set_field(trace_header, SEGY_TR_LAG_A, date_time_stamp.time().msec());
        if (SEGY_OK != err) {
          throw std::runtime_error("segy_set_field(SEGY_TR_LAG_A)");
        }

        for(auto trace : component->getTraces()) {
            err = segy_write_traceheader(_fp, traceno, trace_header, _trace0, _trace_bsize);
            if(SEGY_OK != err) {
                 throw std::runtime_error("segy_write_traceheader()");
            }

            segy_from_native(_format, _sam_num, trace->getBuffer());
            err = segy_writetrace(_fp, traceno, trace->getBuffer(), _trace0, _trace_bsize);
            segy_to_native(_format, _sam_num, trace->getBuffer());
            if(SEGY_OK != err) {
                 throw std::runtime_error("segy_writetrace()");
            }

            ++traceno;
        }
    }

    segy_close(_fp);
}

}
}
