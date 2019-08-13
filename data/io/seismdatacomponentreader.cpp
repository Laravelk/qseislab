#include "seismdatacomponentreader.h"

namespace Data {
namespace IO {
SeismDataComponentReader::SeismDataComponentReader(const QFileInfo &fileInfo)
    : _file(fileInfo.absoluteFilePath()), _instream(&_file) {
  if (!_file.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("File can not be opened");
  }

  _instream >> _componentNum >> _tracesInComponent;
}

std::vector<std::unique_ptr<float[]>> &SeismDataComponentReader::getData() {
  return _data;
}

void SeismDataComponentReader::next() {
  _data.clear();
  // TODO: проверять можно считывать из потока столько данных сколько нужно
  if (_componentNum < _readNum) {
    throw std::runtime_error("Data in the file ended");
  }

  unsigned num = 0;
  for (int i = 0; i < _tracesInComponent; ++i) {
    _instream >> num;
    std::unique_ptr<float[]> data = std::make_unique<float[]>(num);
    for (unsigned j = 0; j < num; ++j) {
      _instream >> data[j];
    }

    _data.push_back(std::move(data));
  }

  ++_readNum;
}

SeismDataComponentReader::~SeismDataComponentReader() { _file.close(); }

} // namespace IO
} // namespace Data
