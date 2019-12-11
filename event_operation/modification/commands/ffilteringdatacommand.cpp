#include "ffilteringdatacommand.h"

#include <unsupported/Eigen/FFT>
#include <iostream> // TODO: delete

FFilteringDataCommand::FFilteringDataCommand(const QUuid &shareUuid, Data::SeismEvent *event, const FFilteringDataCommand::Parameters &parameters)
    : CustomIndividualUndoCommand(shareUuid),
      _event(event)
{}

void FFilteringDataCommand::undo() {
    // TODO: implement
}

void FFilteringDataCommand::redo() {
    Eigen::FFT <float> fft;
    fillOldDataList();
    int idx = 0;
    for (auto &oldDataMapElement : _oldDataMap) {
        for (auto &trace : oldDataMapElement.second) {
        std::vector<float> timevec = trace;
        std::vector<std::complex<float>> freqvec;
        fft.fwd(freqvec, timevec);

        // scalar f1 - f4 zone

        // end zone

        // go to real part

//        fft.inv(); // go back

        }
    }
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

int FFilteringDataCommand::Parameters::getSampleInterval() const
{
    return _sampleInterval;
}

void FFilteringDataCommand::Parameters::setSampleInterval(int value)
{
    _sampleInterval = value;
}
