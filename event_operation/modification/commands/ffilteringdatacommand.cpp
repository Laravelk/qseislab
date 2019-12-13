#include "ffilteringdatacommand.h"

#include <unsupported/Eigen/FFT>
#include <math.h>
#include <iostream> // TODO: delete

FFilteringDataCommand::FFilteringDataCommand(const QUuid &shareUuid, Data::SeismEvent *event, const FFilteringDataCommand::Parameters &parameters)
    : CustomIndividualUndoCommand(shareUuid),
      _event(event), _parameters(parameters)
{}

void FFilteringDataCommand::undo() {
    uint traceNumberInVector = 0;
    for (auto &component : _event->getComponents()) {
        for (auto &trace : component->getTraces()) {
            auto oldBuffer = _oldTraces.at(traceNumberInVector);
            auto bufferInTrace = trace->getBuffer();
            for (uint i = 0; i < oldBuffer.size(); i++) {
                bufferInTrace[i] = oldBuffer[i];
            }
            traceNumberInVector++;
        }
    }
    _event->changeTrigger();
}

void FFilteringDataCommand::redo() {
    Eigen::FFT <float> fft;
    fillOldDataList();
//    std::cerr << "affter fill" << std::endl;
    uint traceNumberInVector = 0;
    for (auto &component : _event->getComponents()) {
        float sampleInterval = component->getSampleInterval();
        for (auto &trace : component->getTraces()) {
            std::vector<float> timevec = _oldTraces.at(traceNumberInVector);
            std::vector<std::complex<float>> freqvec;
            traceNumberInVector++;

            fft.fwd(freqvec, timevec);

//            std::cerr << "F " << _parameters.getF1() << " " << _parameters.getF2() << " " << _parameters.getF3() << " " << _parameters.getF4() << std::endl << std::endl;

            uint indexF1 = static_cast<uint>(ceil((_parameters.getF1() * static_cast<float>(timevec.size()) *
                                                sampleInterval) /
                                                    static_cast<float>(MICROSECONDS_IN_SECONDS)));
            uint indexF2 = static_cast<uint>(ceil((_parameters.getF2() * static_cast<float>(timevec.size()) *
                                              sampleInterval) /
                                                  static_cast<float>(MICROSECONDS_IN_SECONDS)));
            uint indexF3 =static_cast<uint>(ceil((_parameters.getF3() * static_cast<float>(timevec.size()) *
                                             sampleInterval) /
                                                 static_cast<float>(MICROSECONDS_IN_SECONDS)));
            uint indexF4 = static_cast<uint>(ceil((_parameters.getF4() * static_cast<float>(timevec.size()) *
                                              sampleInterval) /
                                                  static_cast<float>(MICROSECONDS_IN_SECONDS)));
//            std::cerr << "indexes: " << indexF1 << " " << indexF2 << " " << indexF3 << " " << indexF4 << std::endl << std::endl;

            // scalar f1 - f4 zone
            for (uint i = 0; i < indexF1; i++) {
                freqvec[i] = 0;
            }

            float h = indexF2 - indexF1;
            for (uint i = indexF1; i < indexF2; i++) {
                float scalar = (i - indexF1) / h;
                freqvec[i] *= scalar;
            }

            float d = indexF4 - indexF3;
            for (uint i = indexF3; i < indexF4; i++) {
                float scalar = (i - indexF3) / d;
                freqvec[i] *= scalar;
            }

            for (uint i = indexF4; i < timevec.size(); i++) {
                freqvec[i] = 0;
            }


            float *backArray = new float[indexF4 - indexF1];
            for (uint i = indexF4, j = 0; i >= indexF1; i--, j++) {
//                std::cerr << freqvec[i].real() << " " << i << " " << j << std::endl;
                backArray[j] = freqvec[i].real();
            }

            fft.inv(timevec, freqvec);

            auto bufferInTrace = trace->getBuffer();
            for (uint i = 0; i < timevec.size(); i++) {
                bufferInTrace[i] = timevec[i];
            }

            delete [] backArray;
        }
    }
    _event->changeTrigger();
}

bool FFilteringDataCommand::is(Data::SeismEvent::TransformOperation oper) const {
    return oper == Data::SeismEvent::TransformOperation::FFilteringData;
}

void FFilteringDataCommand::fillOldDataList()
{
    for (auto &component : _event->getComponents()) {
        for (auto &trace : component->getTraces()) {
            std::vector<float> buffer;
            for (int i = 0; i < trace->getBufferSize(); i++) {
                buffer.push_back(trace->getBuffer()[i]);
            }
            _oldTraces.push_back(buffer);
        }
    }
}

int FFilteringDataCommand::Parameters::getF1() const {
    return _F1;
}

void FFilteringDataCommand::Parameters::setF1(int f1) {
    _F1 = f1;
}

int FFilteringDataCommand::Parameters::getF2() const {
    return _F2;
}

void FFilteringDataCommand::Parameters::setF2(int f2) {
    _F2 = f2;
}

int FFilteringDataCommand::Parameters::getF3() const {
    return _F3;
}

void FFilteringDataCommand::Parameters::setF3(int f3) {
    _F3 = f3;
}

int FFilteringDataCommand::Parameters::getF4() const {
    return _F4;
}

void FFilteringDataCommand::Parameters::setF4(int f4) {
    _F4 = f4;
}
