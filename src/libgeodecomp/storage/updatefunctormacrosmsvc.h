#ifndef LIBGEODECOMP_STORAGE_UPDATEFUNCTORMACROSMSVC_H
#define LIBGEODECOMP_STORAGE_UPDATEFUNCTORMACROSMSVC_H

// MSVC doesn't have a _Pragma(), but requires __pragma(), hence here
// is basically a copy of updatefunctormacros.h. Oh, but of course we
// have to shun all std::size_t loop counters as MSVC wouldn't accept
// it as loop counter for OpenMP.
#ifdef _MSC_BUILD

#ifdef LIBGEODECOMP_WITH_THREADS
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_1                         \
    if (concurrencySpec.enableOpenMP() &&                               \
        !modelThreadingSpec.hasOpenMP()) {                              \
        if (concurrencySpec.preferStaticScheduling()) {                 \
            __pragma(omp parallel for schedule(static))                 \
            for (int c = 0; c < int(region.numPlanes()); ++c) {         \
                typename Region<DIM>::StreakIterator e =                \
                    region.planeStreakIterator(std::size_t(c + 1));     \
                typedef typename Region<DIM>::StreakIterator Iter;      \
                for (Iter i = region.planeStreakIterator(std::size_t(c + 0)); \
                     i != e;                                            \
                     ++i) {                                             \
                    LGD_UPDATE_FUNCTOR_BODY;                            \
                }                                                       \
            }                                                           \
    /**/
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_2                         \
        } else {                                                        \
            if (!concurrencySpec.preferFineGrainedParallelism()) {      \
                __pragma(omp parallel for schedule(dynamic))            \
                for (int c = 0; c < int(region.numPlanes()); ++c) {     \
                    typename Region<DIM>::StreakIterator e =            \
                        region.planeStreakIterator(std::size_t(c + 1)); \
                    typedef typename Region<DIM>::StreakIterator Iter;  \
                    for (Iter i = region.planeStreakIterator(std::size_t(c + 0)); \
                         i != e;                                        \
                         ++i) {                                         \
                        LGD_UPDATE_FUNCTOR_BODY;                        \
                    }                                                   \
                }                                                       \
    /**/
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_3                         \
            } else {                                                    \
                typedef typename Region<DIM>::StreakIterator Iter;      \
                std::vector<Streak<DIM> > streaks;                      \
                streaks.reserve(region.numStreaks());                   \
                for (Iter i = region.beginStreak();                     \
                     i != region.endStreak();                           \
                     ++i) {                                             \
                    Streak<DIM> s = *i;                                 \
                    auto granularity = modelThreadingSpec.granularity(); \
                    while (s.length() > granularity) {                  \
                        Streak<DIM> tranche = s;                        \
                        tranche.endX = s.origin.x() + granularity -     \
                            (s.origin.x() % granularity);               \
                        streaks.push_back(tranche);                     \
                        s.origin.x() = tranche.endX;                    \
                    }                                                   \
                    streaks.push_back(s);                               \
                }                                                       \
                __pragma(omp parallel for schedule(dynamic))            \
                for (int j = 0; j < int(streaks.size()); ++j) {         \
                    Streak<DIM> *i = &streaks[std::size_t(j)];          \
                    LGD_UPDATE_FUNCTOR_BODY;                            \
                }                                                       \
            }                                                           \
    /**/
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_4                         \
    /**/
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_5                         \
    /**/
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_6                         \
        }                                                               \
        return;                                                         \
    }
#else
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_1
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_2
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_3
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_4
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_5
#define LGD_UPDATE_FUNCTOR_THREADING_SELECTOR_6
#endif

#endif

#endif
