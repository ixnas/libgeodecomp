#include <cxxtest/TestSuite.h>
#include <libgeodecomp/storage/linepointerassembly.h>
#include <libgeodecomp/storage/linepointerupdatefunctor.h>
#include <libgeodecomp/storage/updatefunctortestbase.h>

using namespace LibGeoDecomp;

namespace LibGeoDecomp {

class LinePointerUpdateFunctorTest : public CxxTest::TestSuite
{
public:

    template<class STENCIL>
    class UpdateFunctorTestHelper : public UpdateFunctorTestBase<STENCIL>
    {
    public:
        using UpdateFunctorTestBase<STENCIL>::DIM;
        typedef typename UpdateFunctorTestBase<STENCIL>::TestCellType TestCellType;
        typedef typename UpdateFunctorTestBase<STENCIL>::GridType GridType;
        typedef STENCIL Stencil;

        virtual void callFunctor(
            const Region<DIM>& region,
            const GridType& gridOld,
            GridType *gridNew,
            unsigned nanoStep)
        {
            CoordBox<DIM> gridBox = gridOld.boundingBox();
            const TestCellType *pointers[Stencil::VOLUME];
            for (typename Region<DIM>::StreakIterator i = region.beginStreak();
                 i != region.endStreak();
                 ++i) {
                Streak<DIM> streak = *i;
                LinePointerAssembly<Stencil>()(pointers, streak, gridOld);
                LinePointerUpdateFunctor<TestCellType>()(
                    streak, gridBox, pointers, &(*gridNew)[streak.origin], nanoStep);
            }
        }
    };

    void testMoore2D()
    {
        UpdateFunctorTestHelper<Stencils::Moore<2, 1> >().testSimple(3);
        UpdateFunctorTestHelper<Stencils::Moore<2, 1> >().testSplittedTraversal(3);
    }

    void testMoore3D()
    {
        UpdateFunctorTestHelper<Stencils::Moore<3, 1> >().testSimple(3);
        UpdateFunctorTestHelper<Stencils::Moore<3, 1> >().testSplittedTraversal(3);
    }

    void testVonNeumann2D()
    {
        UpdateFunctorTestHelper<Stencils::VonNeumann<2, 1> >().testSimple(3);
        UpdateFunctorTestHelper<Stencils::VonNeumann<2, 1> >().testSplittedTraversal(3);
    }

    void testVonNeumann3D()
    {
        UpdateFunctorTestHelper<Stencils::VonNeumann<3, 1> >().testSimple(3);
        UpdateFunctorTestHelper<Stencils::VonNeumann<3, 1> >().testSplittedTraversal(3);
    }
};

}
