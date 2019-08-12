#include "segyreader.h"

#include "data/seismcomponent.h"

#include <memory>


namespace Data {
namespace IO {
void SegyReader::setFilePath(const char* path)
{
    _fp = segy_open(path, "rb");
    if(!_fp) {
        throw std::runtime_error("segy_open()");
    }
}

void SegyReader::readBinHeader()
{
    char binheader[ SEGY_BINARY_HEADER_SIZE ];
    int err = segy_binheader( _fp, binheader );
    if(SEGY_OK != err) {
        throw std::runtime_error("segy_binheader()");
    }

    float fallback = -1.0;
    err = segy_sample_interval(_fp, fallback, &_sam_intr);
    if(SEGY_OK != err || fallback  == _sam_intr) {
        throw std::runtime_error("segy_sample_interval()");
    }

    _sam_num = segy_samples( binheader );
    if(0 >= _sam_num) {
       throw std::runtime_error("segy_samples() -> negative");
    }

    _format = segy_format( binheader );

    _trace0 = segy_trace0( binheader );

    _trace_bsize = segy_trsize( _format, _sam_num );
    if(0 >= _trace_bsize) {
       throw std::runtime_error("segy_trsize() -> negative");
    }

    err = segy_traces( _fp, &_trace_num, _trace0, _trace_bsize );
    if(SEGY_OK != err) {
        throw std::runtime_error("segy_traces()");
    }

    if(0 != _trace_num % traceInComponent()) {
        throw std::runtime_error("Number of routes is non-multiple to traceInComponent");
    }
}

int SegyReader::traceInComponent() const
{
    return SEGY_READER_TRACE_IN_COMPONENT;
}

bool SegyReader::hasNextComponent() const
{
    assert(0 == _alreadyRead % traceInComponent());

    return _trace_num > _alreadyRead;
}

std::unique_ptr<SeismTrace> SegyReader::nextTrace()
{
    int err;
    char traceh[ SEGY_TRACE_HEADER_SIZE ];

    assert(0 ==  static_cast<unsigned>(_trace_bsize) % sizeof(float) );
    unsigned buffer_size = static_cast<unsigned>(_trace_bsize) / sizeof(float); // TODO: доставать размеры форматов и размер данных из библиотеки

    err = segy_traceheader( _fp, _alreadyRead, traceh, _trace0, _trace_bsize );
    if(SEGY_OK != err) {
        throw std::runtime_error("segy_traceheader()");
    }

    int p_wave_arrivel;
    err = segy_get_field( traceh, SEGY_TR_CDP_X, &p_wave_arrivel );
    if(SEGY_OK != err) {
        throw std::runtime_error("segy_get_field(SEGY_TR_CDP_X)");
    }

    int s_wave_arrivel;
    err = segy_get_field( traceh, SEGY_TR_CDP_Y, &s_wave_arrivel );
    if(SEGY_OK != err) {
       throw std::runtime_error("segy_get_field(SEGY_TR_CDP_Y)");
    }

    float buffer[buffer_size];
    err = segy_readtrace( _fp, _alreadyRead, buffer, _trace0, _trace_bsize );
    if(SEGY_OK != err) {
        throw std::runtime_error("segy_readtrace()");
    }
    segy_to_native( _format, _sam_num, buffer );


    _trace = std::make_unique<SeismTrace>();
    _trace->setSampleInterval(_sam_intr);
    _trace->setPWaveArrival(p_wave_arrivel);
    _trace->setSWaveArrival(s_wave_arrivel);
    _trace->setBuffer(buffer_size, buffer);

    ++_alreadyRead;

    return std::move(_trace);
}

void SegyReader::close()
{
    segy_close(_fp);
}


} // namespace Reader
} // namespace Data
