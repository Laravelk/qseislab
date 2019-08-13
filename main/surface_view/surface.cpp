#include "surface.h"

namespace Main {
Surface::Surface(Q3DSurface *surface)
                : _surface(surface)
{

}

Surface::~Surface()
{
    delete _surface;
}

} // namespace Mains
