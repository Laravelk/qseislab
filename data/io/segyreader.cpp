#include "segyreader.h"

#include "data/seismcomponent.h"
#include "data/seismreceiver.h"

#include <QDateTime>

#include <assert.h>
#include <memory>

#include <iostream> // TODO: remove

namespace Data {
namespace IO {
void SegyReader::setFilePath(const char *path) {
  _fp = segy_open(path, "rb");
  if (!_fp) {
    throw std::runtime_error("segy_open()");
  }
}

void SegyReader::readBinHeader() {
  char binheader[SEGY_BINARY_HEADER_SIZE];
  int err = segy_binheader(_fp, binheader);
  if (SEGY_OK != err) {
    throw std::runtime_error("segy_binheader()");
  }

  float fallback = -1.0;
  err = segy_sample_interval(_fp, fallback, &_sam_intr);
  if (SEGY_OK != err || fallback == _sam_intr) {
    throw std::runtime_error("segy_sample_interval()");
  }

  _sam_num = segy_samples(binheader);
  if (0 > _sam_num) {
    throw std::runtime_error("segy_samples() -> negative");
  } else if (0 == _sam_num) {
    throw std::runtime_error("segy_samples() -> zero");
  }

  _format = segy_format(binheader);

  _trace0 = segy_trace0(binheader);

  _trace_bsize = segy_trsize(_format, _sam_num);
  if (0 >= _trace_bsize) {
    throw std::runtime_error("segy_trsize() -> negative");
  }

  err = segy_traces(_fp, &_trace_num, _trace0, _trace_bsize);
  if (SEGY_OK != err) {
    throw std::runtime_error("segy_traces()");
  }
}

bool SegyReader::hasNextComponent() const { return _trace_num > _alreadyRead; }

std::shared_ptr<SeismComponent>
SegyReader::nextComponent(const std::shared_ptr<SeismReceiver> &receiver) {
  int err;
  char traceh[SEGY_TRACE_HEADER_SIZE];

  assert(0 == static_cast<unsigned>(_trace_bsize) % sizeof(float));
  uint32_t buffer_size = static_cast<uint32_t>(_trace_bsize) /
                         sizeof(float); // TODO: доставать размеры форматов и
                                        // размер данных из библиотеки

  std::shared_ptr<SeismComponent> component =
      std::make_shared<SeismComponent>(receiver->getUuid());
  int p_wave_arrival = 0;
  int s_wave_arrival = 0;

  QDateTime date_time_stamp;

  //  for (int i = 0; i < receiver->getChannelAmount(); ++i) {
  int i = 0;
  for (auto &channel : receiver->getChannels()) {
    if (_trace_num == _alreadyRead) {
      throw std::runtime_error("No more traces in the segy-file");
    }

    err = segy_traceheader(_fp, _alreadyRead, traceh, _trace0, _trace_bsize);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_traceheader()");
    }

    int p_wave_arrival_trace;
    err = segy_get_field(traceh, SEGY_TR_CDP_X, &p_wave_arrival_trace);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_get_field(SEGY_TR_CDP_X)");
    }
    if (0 == i) {
      p_wave_arrival = p_wave_arrival_trace;
    }
    if (p_wave_arrival != p_wave_arrival_trace) {
      throw std::runtime_error(
          "Fields do not match in the component (p_wave_arrivel)");
    }

    // date-time-stamp reading
    int year_trace;
    int day_trace;
    int hour_trace;
    int minute_trace;
    int second_trace;
    int microsec_trace;
    err = segy_get_field(traceh, SEGY_TR_YEAR_DATA_REC, &year_trace);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_get_field(SEGY_TR_YEAR_DATA_REC)");
    }
    err = segy_get_field(traceh, SEGY_TR_DAY_OF_YEAR, &day_trace);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_get_field(SEGY_TR_DAY_OF_YEAR)");
    }
    err = segy_get_field(traceh, SEGY_TR_HOUR_OF_DAY, &hour_trace);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_get_field(SEGY_TR_HOUR_OF_DAY)");
    }
    err = segy_get_field(traceh, SEGY_TR_MIN_OF_HOUR, &minute_trace);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_get_field(SEGY_TR_MIN_OF_HOUR)");
    }
    err = segy_get_field(traceh, SEGY_TR_SEC_OF_MIN, &second_trace);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_get_field(SEGY_TR_SEC_OF_MIN)");
    }
    err = segy_get_field(traceh, SEGY_TR_LAG_A, &microsec_trace);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_get_field(SEGY_TR_LAG_A)");
    }
    QDateTime date_time_stamp_trace = QDateTime(
        QDate(year_trace, 1, 1).addDays(day_trace - 1),
        QTime(hour_trace, minute_trace, second_trace, microsec_trace));
    if (0 == i) {
      date_time_stamp = date_time_stamp_trace;
    }
    if (date_time_stamp != date_time_stamp_trace) {
      throw std::runtime_error(
          "Fields do not match in the component (date-time-stamp)");
    }
    // end date-time-stamp reading

    int s_wave_arrival_trace;
    err = segy_get_field(traceh, SEGY_TR_CDP_Y, &s_wave_arrival_trace);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_get_field(SEGY_TR_CDP_Y)");
    }
    if (0 == i) {
      s_wave_arrival = s_wave_arrival_trace;
    }
    if (s_wave_arrival != s_wave_arrival_trace) {
      throw std::runtime_error(
          "Fields do not match in the component (s_wave_arrivel)");
    }

    float *buffer = new float[buffer_size];
    err = segy_readtrace(_fp, _alreadyRead, buffer, _trace0, _trace_bsize);
    if (SEGY_OK != err) {
      throw std::runtime_error("segy_readtrace()");
    }
    segy_to_native(_format, _sam_num, buffer);

    std::shared_ptr<SeismTrace> trace = std::make_shared<SeismTrace>();
    trace->setOrientation(channel->getOrientation());

    trace->setBuffer(buffer_size, buffer);
    delete[] buffer;

    component->addTrace(std::move(trace));

    ++_alreadyRead;
    ++i;
  }

  auto &info = component->getInfo();

  info.setStampDateTime(date_time_stamp);
  info.setSampleInterval(_sam_intr);
  // TODO: ...
  if (0 != p_wave_arrival) {
    component->addWavePick(
        Data::SeismWavePick(Data::SeismWavePick::Type::PWAVE, p_wave_arrival));
  }
  if (0 != s_wave_arrival) {
    component->addWavePick(
        Data::SeismWavePick(Data::SeismWavePick::Type::SWAVE, s_wave_arrival));
  }

  return component;
}

void SegyReader::close() {
  _alreadyRead = 0;
  segy_close(_fp);
  _fp = nullptr;
}

} // namespace IO
} // namespace Data
