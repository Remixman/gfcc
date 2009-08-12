#include "tl-omp-nanox.hpp"

using namespace TL;
using namespace TL::Nanox;


OMPTransform::OMPTransform()
{
    set_phase_name("OpenMP for nanox");
    set_phase_description("This phase implements OpenMP targeting nanox runtime");

    on_directive_post["task"].connect(functor(&OMPTransform::task_postorder, *this));
}

EXPORT_PHASE(TL::Nanox::OMPTransform)
