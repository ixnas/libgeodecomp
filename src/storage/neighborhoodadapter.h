#ifndef LIBGEODECOMP_STORAGE_NEIGHBORHOODADAPTER_H
#define LIBGEODECOMP_STORAGE_NEIGHBORHOODADAPTER_H

#include <libgeodecomp/geometry/coord.h>
#include <libgeodecomp/geometry/coordbox.h>
#include <libgeodecomp/storage/neighborhoodadapter.h>

namespace LibGeoDecomp {

/**
 * This class is most useful for interfacing meshless codes with
 * LibGeoDecomp. It can retrieve cells matching a certain id from the
 * ContainerCells in the current neighborhood.
 */
template<class NEIGHBORHOOD, typename KEY, typename CARGO, int DIM>
class NeighborhoodAdapter
{
public:
    typedef KEY Key;
    typedef CARGO Cargo;

    NeighborhoodAdapter(const NEIGHBORHOOD *neighbors) :
        neighbors(neighbors)
    {}

    /**
     * Will search neighboring containers for a Cargo which matches
     * the given ID.
     */
    const Cargo& operator[](const Key& id) const
    {
        const Cargo *res = (*neighbors)[Coord<DIM>()][id];

        if (res) {
            return *res;
        }

        CoordBox<DIM> box(Coord<DIM>::diagonal(-1), Coord<DIM>::diagonal(3));
        for (typename CoordBox<DIM>::Iterator i = box.begin(); i != box.end(); ++i) {
            if (*i != Coord<DIM>()) {
                res = (*neighbors)[*i][id];
                if (res) {
                    return *res;
                }
            }
        }

        LOG(FATAL, "could not find id " << id << " in neighborhood")

        throw std::logic_error("id not found");
    }

private:
    const NEIGHBORHOOD *neighbors;
};

}

#endif
