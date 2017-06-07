// © 2017 and later: Unicode, Inc. and others.
// License & terms of use: http://www.unicode.org/copyright.html

// norms.h
// created: 2017jun04 Markus W. Scherer
// (pulled out of n2builder.cpp)

// Storing & manipulating Normalizer2 builder data.

#ifndef __NORMS_H__
#define __NORMS_H__

#include "unicode/utypes.h"

#if !UCONFIG_NO_NORMALIZATION

#include "unicode/errorcode.h"
#include "unicode/unistr.h"
#include "unicode/utf16.h"
#include "normalizer2impl.h"
#include "toolutil.h"
#include "utrie2.h"
#include "uvectr32.h"

U_NAMESPACE_BEGIN

class BuilderReorderingBuffer {
public:
    BuilderReorderingBuffer() : fLength(0), fLastStarterIndex(-1), fDidReorder(FALSE) {}
    void reset() {
        fLength=0;
        fLastStarterIndex=-1;
        fDidReorder=FALSE;
    }
    int32_t length() const { return fLength; }
    UBool isEmpty() const { return fLength==0; }
    int32_t lastStarterIndex() const { return fLastStarterIndex; }
    UChar32 charAt(int32_t i) const { return fArray[i]>>8; }
    uint8_t ccAt(int32_t i) const { return (uint8_t)fArray[i]; }
    UBool didReorder() const { return fDidReorder; }

    void append(UChar32 c, uint8_t cc);
    void toString(UnicodeString &dest) const;

private:
    int32_t fArray[Normalizer2Impl::MAPPING_LENGTH_MASK];
    int32_t fLength;
    int32_t fLastStarterIndex;
    UBool fDidReorder;
};

struct CompositionPair {
    CompositionPair(UChar32 t, UChar32 c) : trail(t), composite(c) {}
    UChar32 trail, composite;
};

struct Norm {
    enum MappingType { NONE, REMOVED, ROUND_TRIP, ONE_WAY };

    UBool hasMapping() const { return mappingType>REMOVED; }

    // Requires hasMapping() and well-formed mapping.
    void setMappingCP() {
        UChar32 c;
        if(!mapping->isEmpty() && mapping->length()==U16_LENGTH(c=mapping->char32At(0))) {
            mappingCP=c;
        } else {
            mappingCP=U_SENTINEL;
        }
    }

    const CompositionPair *getCompositionPairs(int32_t &length) const {
        if(compositions==nullptr) {
            length=0;
            return nullptr;
        } else {
            length=compositions->size()/2;
            return reinterpret_cast<const CompositionPair *>(compositions->getBuffer());
        }
    }
    UChar32 combine(UChar32 trail) const;

    UnicodeString *mapping;
    UnicodeString *rawMapping;  // non-nullptr if the mapping is further decomposed
    UChar32 mappingCP;  // >=0 if mapping to 1 code point
    int32_t mappingPhase;
    MappingType mappingType;

    UVector32 *compositions;  // (trail, composite) pairs
    uint8_t cc, leadCC, trailCC;
    UBool combinesBack;
    UBool hasNoCompBoundaryAfter;

    /**
     * Overall type of normalization properties.
     * Set after most processing is done.
     *
     * Corresponds to the rows in the chart on
     * http://site.icu-project.org/design/normalization/custom
     * in numerical (but reverse visual) order.
     *
     * YES_NO means composition quick check=yes, decomposition QC=no -- etc.
     */
    enum Type {
        /** Initial value until most processing is done. */
        UNKNOWN,
        /** No mapping, does not combine, ccc=0. */
        INERT,
        /** Starter, no mapping, has compositions. */
        YES_YES_COMBINES_FWD,
        /** Starter with a round-trip mapping and compositions. */
        YES_NO_COMBINES_FWD,
        /** Starter with a round-trip mapping but no compositions. */
        YES_NO_MAPPING_ONLY,
        // TODO: minMappingNotCompYes, minMappingNoCompBoundaryBefore
        /** Has a one-way mapping. */
        NO_NO,
        /** Has an algorithmic one-way mapping to a single code point. */
        NO_NO_DELTA,
        /**
         * Combines both backward and forward, has compositions.
         * Allowed, but not normally used.
         */
        MAYBE_YES_COMBINES_FWD,
        /** Combines only backward. */
        MAYBE_YES_SIMPLE,
        /** Non-zero ccc but does not combine backward. */
        YES_YES_WITH_CC
    } type;
    /** Offset into the type's part of the extra data, or the algorithmic-mapping delta. */
    int32_t offset;

    /**
     * Error string set by processing functions that do not have access
     * to the code point, deferred for readable reporting.
     */
    const char *error;
};

class Norms {
public:
    Norms(UErrorCode &errorCode);
    ~Norms();

    int32_t length() const { return utm_countItems(normMem); }
    const Norm &getNormRefByIndex(int32_t i) const { return norms[i]; }
    Norm &getNormRefByIndex(int32_t i) { return norms[i]; }

    Norm *allocNorm();
    /** Returns an existing Norm unit, or nullptr if c has no data. */
    Norm *getNorm(UChar32 c);
    /** Returns a Norm unit, creating a new one if necessary. */
    Norm *createNorm(UChar32 c);
    /** Returns an existing Norm unit, or an immutable empty object if c has no data. */
    const Norm &getNormRef(UChar32 c) const;
    uint8_t getCC(UChar32 c) const { return getNormRef(c).cc; }

    void reorder(UnicodeString &mapping, BuilderReorderingBuffer &buffer) const;
    UBool combinesWithCCBetween(const Norm &norm, uint8_t lowCC, uint8_t highCC) const;

    class Enumerator {
    public:
        Enumerator(Norms &n) : norms(n) {}
        virtual ~Enumerator();
        /** Called for enumerated value!=0. */
        virtual void rangeHandler(UChar32 start, UChar32 end, Norm &norm) = 0;
        /** @internal Public only for C callback. */
        UBool rangeHandler(UChar32 start, UChar32 end, uint32_t value);
    protected:
        Norms &norms;
    };

    void enumRanges(Enumerator &e);

private:
    Norms(const Norms &other) = delete;
    Norms &operator=(const Norms &other) = delete;

    UTrie2 *normTrie;
    UToolMemory *normMem;
    Norm *norms;
};

class CompositionBuilder : public Norms::Enumerator {
public:
    CompositionBuilder(Norms &n) : Norms::Enumerator(n) {}
    /** Adds a composition mapping for the first character in a round-trip mapping. */
    void rangeHandler(UChar32 start, UChar32 end, Norm &norm) override;
};

class Decomposer : public Norms::Enumerator {
public:
    Decomposer(Norms &n) : Norms::Enumerator(n), didDecompose(FALSE) {}
    /** Decomposes each character of the current mapping. Sets didDecompose if any. */
    void rangeHandler(UChar32 start, UChar32 end, Norm &norm) override;
    UBool didDecompose;
};

U_NAMESPACE_END

#endif // #if !UCONFIG_NO_NORMALIZATION

#endif  // __NORMS_H__