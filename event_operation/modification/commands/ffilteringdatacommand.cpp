#include "ffilteringdatacommand.h"

#include <unsupported/Eigen/FFT>
#include <iostream> // TODO: delete

FFilteringDataCommand::FFilteringDataCommand(const QUuid &shareUuid, Data::SeismEvent *event, const FFilteringDataCommand::Parameters &parameters)
    : CustomIndividualUndoCommand(shareUuid),
      _event(event), _parameters(parameters)
{}

void FFilteringDataCommand::undo() {
    // TODO: implement
}

void FFilteringDataCommand::redo() {
    Eigen::FFT <float> fft;
    fillOldDataList();
    std::cerr << "affter fill" << std::endl;
    for (auto &oldDataMapElement : _oldDataMap) {
        for (auto &trace : oldDataMapElement.second) {
        auto &traceOwner = _event->getComponents()[oldDataMapElement.first];
        std::vector<float> timevec = trace;
        std::vector<std::complex<float>> freqvec;
        fft.fwd(freqvec, timevec);

        int indexF1 = static_cast<int>(_parameters.getF1() * trace.size() *
                                      traceOwner->getSampleInterval() / MICROSECONDS_IN_SECONDS);
        int indexF2 = static_cast<int>(_parameters.getF2() * trace.size() *
                                                        traceOwner->getSampleInterval() / MICROSECONDS_IN_SECONDS);
        int indexF3 = static_cast<int>(_parameters.getF3() * trace.size() * traceOwner->getSampleInterval() / MICROSECONDS_IN_SECONDS);
        int indexF4 = static_cast<int>(_parameters.getF4() * trace.size() * traceOwner->getSampleInterval() / MICROSECONDS_IN_SECONDS);

        std::cerr << indexF1 << " " << indexF2 << " " << indexF3 << " " << indexF4 << std::endl;

        // scalar f1 - f4 zone
        for (int i = 0; i < indexF1; i++) {
            freqvec[i] = 0;
        }

        float h = indexF2 - indexF1;
        for (int i = indexF1; i < indexF2; i++) {
            float scalar = (i - indexF1) / h;
            freqvec[i] *= scalar;
        }

        float d = indexF4 - indexF3;
        for (int i = indexF3; i < indexF4; i++) {
            float scalar = (i - indexF3) / d;
            freqvec[i] *= scalar;
        }

        for (int i = indexF4; i < trace.size(); i++) {
            freqvec[i] = 0;
        }

        fft.inv(timevec, freqvec);


        // end zone

        // go to real part

//        fft.inv(); // go back

        }
    }
    _event->changeTrigger();
}

bool FFilteringDataCommand::is(Data::SeismEvent::TransformOperation oper) const {
    return oper == Data::SeismEvent::TransformOperation::FFilteringData;
}

void FFilteringDataCommand::fillOldDataList()
{
    int idx = 0;
    for (auto &component : _event->getComponents()) {
        std::vector<std::vector<float>> traces;
        for (auto &trace : component->getTraces()) {
            std::vector<float> buffer;
            for (int i = 0; i < trace->getBufferSize(); i++) {
                buffer.push_back(trace->getBuffer()[i]);
            }
            traces.push_back(buffer);
        }
        _oldDataMap[idx] = traces;
//        if (idx == 0) {
//            std::cerr << "HEREREEEE";
//            for (int i = 0; i < 20; i++) {
//            std::cerr << traces[0][i] << " ";
//            }
//            std::cerr << std::endl << std::endl << std::endl;
//        }
        idx++;
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
