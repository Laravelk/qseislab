#include "segyreader.h"

#include "data/seismcomponent.h"
#include "data/seismreceiver.h"

#include <memory>

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
  if (0 >= _sam_num) {
    throw std::runtime_error("segy_samples() -> negative");
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

std::unique_ptr<SeismComponent>
SegyReader::nextComponent(const std::unique_ptr<SeismReceiver> &receiver) {
  int err;
  char traceh[SEGY_TRACE_HEADER_SIZE];

  assert(0 == static_cast<unsigned>(_trace_bsize) % sizeof(float));
  uint32_t buffer_size = static_cast<uint32_t>(_trace_bsize) /
                         sizeof(float); // TODO: доставать размеры форматов и
                                        // размер данных из библиотеки

  std::unique_ptr<SeismComponent> component =
      std::make_unique<SeismComponent>(receiver->getUuid());
  int p_wave_arrival = 0;
  int s_wave_arrival = 0;

  for (int i = 0; i < receiver->getChannelNum(); ++i) {
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

    std::unique_ptr<SeismTrace> trace = std::make_unique<SeismTrace>();
    trace = std::make_unique<SeismTrace>();
    trace->setBuffer(buffer_size, buffer);
    delete[] buffer;

    component->addTrace(std::move(trace));

    ++_alreadyRead;
  }

  component->setSampleInterval(_sam_intr);
  component->addWavePick(
      Data::SeismWavePick(Data::SeismWavePick::Type::PWAVE, p_wave_arrival));
  component->addWavePick(
      Data::SeismWavePick(Data::SeismWavePick::Type::SWAVE, s_wave_arrival));

  return component;
}

void SegyReader::close() {
  _alreadyRead = 0;
  segy_close(_fp);
  _fp = nullptr;
}

} // namespace IO
} // namespace Data
