#pragma once

#include <memory>

namespace Data {
class SeismEvent;
}

namespace EventOperation {
namespace EventTools {

bool dataToEBasis(std::unique_ptr<Data::SeismEvent> &);

} // namespace EventTools
} // namespace EventOperation
