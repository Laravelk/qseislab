#include "segyreader.h"

#include "data/seismcomponent.h"
#include "data/seismtrace.h"

#include <memory>


namespace Data {
namespace IO {
SegyReader::SegyReader(const char* path)
{
    _fp = segy_open(path, "rb");
    if(!_fp) {
        errmsg(-1, "segy_open()");
        return;
    }

    if(0 != readBinHeader()) {
        return;
    }

    _isValid = true;
}

bool SegyReader::isValid() const
{
    return _isValid;
}

bool SegyReader::hasNextComponent() const
{
    assert(true == _isValid);
    assert(0 == _trace_num % 3);

    return (_trace_num / 3) > _alreadyRead;
}

SeismComponent* SegyReader::nextComponent()
{
    assert(true == _isValid);
    SeismComponent* component = new SeismComponent();

    int err;
    char traceh[ SEGY_TRACE_HEADER_SIZE ];

    assert(0 ==  static_cast<unsigned>(_trace_bsize) % sizeof(float) );
    unsigned buffer_size = static_cast<unsigned>(_trace_bsize) / sizeof(float); // TODO: доставать размеры форматов и размер данных из библиотеки


    for(int i = 0; i < 3; ++i) {
        err = segy_traceheader( _fp, _alreadyRead * 3 + i, traceh, _trace0, _trace_bsize );
        if(0 != err) {
            errmsg(err, "segy_traceheader()");
            delete component;
            return nullptr;
        }

        int cdp_x;
        err = segy_get_field( traceh, SEGY_TR_CDP_X, &cdp_x );
        if(0 != err) {
           errmsg(err, "segy_get_field(SEGY_TR_CDP_X)");
           delete component;
           return nullptr;
        }

        int cdp_y;
        err = segy_get_field( traceh, SEGY_TR_CDP_Y, &cdp_y );
        if(0 != err) {
           errmsg(err, "segy_get_field(SEGY_TR_CDP_Y)");
           delete component;
           return nullptr;
        }



        float* buffer = new float[buffer_size];
        err = segy_readtrace( _fp, _alreadyRead*3 + i, buffer, _trace0, _trace_bsize );
        if(0 != err) {
            delete[] buffer;
            errmsg(err, "segy_readtrace()");
            delete component;
            return nullptr;
        }
        segy_to_native( _format, _sam_num, buffer );

//        std::unique_ptr<float[]> data = std::make_unique<float[]>(buffer_size);
//        for(unsigned i = 0; i < buffer_size; ++i){
//            data[i] = buffer[i];
//        }
//        delete[] buffer;

        std::unique_ptr<SeismTrace> trace = std::make_unique<SeismTrace>();
//        trace->setSampleNum(_sam_num);
        trace->setSampleInterval(_sam_intr);
        trace->setPWaveArrival(cdp_x);
        trace->setSWaveArrival(cdp_y);
        trace->setBuffer(buffer_size, buffer);

        component->addTrace(trace);
    }

    ++_alreadyRead;
    return component;
}

const char* SegyReader::getErrMsg() const
{
    return _errmsg.c_str();
}

SegyReader::~SegyReader()
{
    segy_close(_fp);
}

int SegyReader::readBinHeader()
{
    char binheader[ SEGY_BINARY_HEADER_SIZE ];
    int err = segy_binheader( _fp, binheader );
    if(SEGY_OK != err) {
        return errmsg(err, "segy_binheader()");
    }

    float fallback = 0.0;
    err = segy_sample_interval(_fp, fallback, &_sam_intr);
    if(SEGY_OK != err || fallback  == _sam_intr) {
        return errmsg(err, "segy_sample_interval()");
    }

    _sam_num = segy_samples( binheader );
    if(0 >= _sam_num) {
       return errmsg(-1, "segy_samples() -> negative");
    }

    _format = segy_format( binheader );

    _trace0 = segy_trace0( binheader );

    _trace_bsize = segy_trsize( _format, _sam_num );
    if(0 >= _trace_bsize) {
       return errmsg(-1, "segy_trsize() -> negative");
    }

    err = segy_traces( _fp, &_trace_num, _trace0, _trace_bsize );
    if(SEGY_OK != err) {
        return errmsg(err, "segy_traces()");
    }

    if(0 != _trace_num % 3) {
        return errmsg(-1, "the number of routes is non-multiple to three");
    }

    return 0;
}

int SegyReader::errmsg(int errcode, const char* msg)
{
    if(!msg) {
        return errcode;
    }

    _errmsg = msg;
    return errcode;
}


} // namespace Reader
} // namespace Data
